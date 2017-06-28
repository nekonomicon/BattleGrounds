/*
    Copyright 2001 to 2004. The Battle Grounds Team and Contributors

    This file is part of the Battle Grounds Modification for Half-Life.

    The Battle Grounds Modification for Half-Life is free software;
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser General Public License as published by the Free
    Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    The Battle Grounds Modification for Half-Life is distributed in
    the hope that it will be useful, but WITHOUT ANY WARRANTY; without
    even the implied warranty of MERCHANTABILITY or FITNESS FOR A
    PARTICULAR PURPOSE.  See the GNU Lesser General Public License
    for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with The Battle Grounds Modification for Half-Life;
    if not, write to the Free Software Foundation, Inc., 59 Temple Place,
    Suite 330, Boston, MA  02111-1307  USA

    You must obey the GNU Lesser General Public License in all respects for
    all of the code used other than code distributed with the Half-Life
    SDK developed by Valve.  If you modify this file, you may extend this
    exception to your version of the file, but you are not obligated to do so.
    If you do not wish to do so, delete this exception statement from your
    version.
*/

//This fiel was created by Ben Banfield on the 24/10/2k+1 at 18:31 for the Battle Grounds Modification of Half-Life
//Its purpose is to handle the teamplay aspects of the Battle Grounds Mod

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"player.h"
#include	"weapons.h"
#include	"gamerules.h"
#include	"game.h"
#include	"bg_rules.h"
#include	"voice_gamemgr.h"
#include	"point.h"
#include	"bg_round.h"
#include	"particle_emitter.h"

#include	"../../cl_dll/message_defs.h"

extern void SendFogData ( Vector col, int iFadeTime, int iStartDist, int iEndDist );
extern Vector g_col;
extern int g_iFadeOut;
extern int g_iStartDist;
extern int g_iEndDist;
static int team_scores[NUM_TEAMS];

extern DLL_GLOBAL bool		g_fGameOver;
extern DLL_GLOBAL ULONG		g_ulModelIndexPlayer;
DLL_GLOBAL CBaseBgRules*	g_pBGRules = NULL;

float flOurTimeDifference = 0.0;

//Ben - Gameplays
//extern CLineManager	*pLineManager;
CPointManager *pPointManager = NULL;

CVoiceGameMgr	g_VoiceGameMgr;

class CMultiplayGameMgrHelper : public IVoiceGameMgrHelper
{
public:
	virtual bool CanPlayerHearPlayer(CBasePlayer *pListener, CBasePlayer *pTalker)
	{
		if ( g_pGameRules->PlayerRelationship( pListener, pTalker ) != GR_TEAMMATE )
		{
				return false;
		}

		return true;
	}
};
static CMultiplayGameMgrHelper g_GameMgrHelper;

CBaseBgRules::CBaseBgRules()
{
	g_VoiceGameMgr.Init(&g_GameMgrHelper, gpGlobals->maxClients);

	m_DisableDeathMessages = false;
	m_DisableDeathPenalty = false;

	memset( team_scores, 0, sizeof(team_scores) );

	m_usRoundEnd = PRECACHE_EVENT( 1, "events/round_end.sc");

	iBritishScore = 0;
	iAmericanScore = 0;

	// reset our ps id generator per map
	// this is liekly to be called after all the spawns
	// this then resets for the next map
	iParticleIDCount = 0;

	// reset cvars before the rr container is made or the map settings will got overwritten
	// Raiko Contrib
	CVAR_SET_FLOAT("mp_stationaryreload", 0.0);
	// Raiko Contrib End
	CVAR_SET_FLOAT("mp_meleeonly", 0.0);
	CVAR_SET_FLOAT("mp_britishscore", 0.0);
	CVAR_SET_FLOAT("mp_americanscore", 0.0);
	CVAR_SET_FLOAT("mp_limit_light_a", -1);
	CVAR_SET_FLOAT("mp_limit_light_b", -1);
	CVAR_SET_FLOAT("mp_limit_medium_a", 2); // default limit if none are set in cfg files
	CVAR_SET_FLOAT("mp_limit_medium_b", -1);
	CVAR_SET_FLOAT("mp_limit_heavy_a", -1);
	CVAR_SET_FLOAT("mp_limit_heavy_b", 2); // default limit if none are set in cfg files
	CVAR_SET_FLOAT("mp_winbonus", 200);
	CVAR_SET_FLOAT("mp_roundsleft", 0);
	CVAR_SET_FLOAT("mp_respawnstyle", 1);
	CVAR_SET_FLOAT("mp_pausetime", 2);
	CVAR_SET_FLOAT("mp_roundtime", 15);
	CVAR_SET_FLOAT("mp_restarttime", 1);
	CVAR_SET_FLOAT("allow_spectators", 1);
	CVAR_SET_FLOAT("mp_autoteambalance", 1);
	CVAR_SET_FLOAT("mp_limitteams", 2);
	CVAR_SET_FLOAT("sv_maxspeed", 235);

	// exec server.cfg before the map cfg
	if ( IS_DEDICATED_SERVER() )
	{
		// dedicated server
		char *servercfgfile = (char *)CVAR_GET_STRING( "servercfgfile" );

		if ( servercfgfile && servercfgfile[0] )
		{
			char szCommand[256];
			
			ALERT( at_console, "Executing dedicated server config file\n" );
			sprintf( szCommand, "exec %s\n", servercfgfile );
			SERVER_COMMAND( szCommand );
		}
	}
	else
	{
		// listen server
		char *lservercfgfile = (char *)CVAR_GET_STRING( "lservercfgfile" );

		if ( lservercfgfile && lservercfgfile[0] )
		{
			char szCommand[256];
			
			ALERT( at_console, "Executing listen server config file\n" );
			sprintf( szCommand, "exec %s\n", lservercfgfile );
			SERVER_COMMAND( szCommand );
		}
	}

	pRoundRules = new CRoundRulesContainer;
	pPointManager = new CPointManager;
}

extern cvar_t timeleft, fragsleft, version;

#include "voice_gamemgr.h"
extern CVoiceGameMgr	g_VoiceGameMgr;

void CBaseBgRules :: Think ( void )
{
	///// Check game rules /////
	static int last_frags;
	static int last_time;

	int frags_remaining = 0;
	int time_remaining = 0;

	g_VoiceGameMgr.Update(gpGlobals->frametime);

	if ( g_fGameOver )   // someone else quit the game already
	{
		CHalfLifeMultiplay::Think();
		return;
	}

	float flTimeLimit = CVAR_GET_FLOAT("mp_timelimit") * 60;
	
	time_remaining = (int)(flTimeLimit ? ( flTimeLimit - (gpGlobals->time - flOurTimeDifference) ) : 0);

	if ( flTimeLimit != 0 && (gpGlobals->time - flOurTimeDifference) >= flTimeLimit )
	{
		GoToIntermission();
		return;
	}

	float flFragLimit = fraglimit.value;
	if ( flFragLimit )
	{
		int bestfrags = 9999;
		int remain = 0;

		// check if any team is over the frag limit
		for ( int i = 0; i < NUM_TEAMS; i++ )
		{
			if ( team_scores[i] >= flFragLimit )
			{
				GoToIntermission();
				return;
			}

			remain = (int)flFragLimit - team_scores[i];
			if ( remain < bestfrags )
			{
				bestfrags = remain;
			}
		}
		frags_remaining = bestfrags;
	}

	// Updates when frags change
	if ( frags_remaining != last_frags )
	{
		g_engfuncs.pfnCvar_DirectSet( &fragsleft, UTIL_VarArgs( "%i", frags_remaining ) );
	}

	// Updates once per second
	if ( timeleft.value != last_time )
	{
		g_engfuncs.pfnCvar_DirectSet( &timeleft, UTIL_VarArgs( "%i", time_remaining ) );
	}

	last_frags = frags_remaining;
	last_time  = time_remaining;

	if(CVAR_GET_STRING("mp_version") != sBgVersion)
		g_engfuncs.pfnCvar_DirectSet( &version, const_cast<char*>(sBgVersion) );

	// Ben Addition to Raiko Contrib - Melee Only
	extern DLL_GLOBAL float g_flMeleeOnly;
	
	// determine whether the melee mode has changed and send to client if it has
	float flMelee = CVAR_GET_FLOAT("mp_meleeonly");
	if(g_flMeleeOnly != flMelee)
	{
		g_flMeleeOnly = flMelee;

		extern int gmsgMeleeOnly;

		MESSAGE_BEGIN(MSG_ALL, gmsgMeleeOnly);
		WRITE_BYTE((int)flMelee);
		MESSAGE_END();

		if(flMelee > 0.0)
			UTIL_ClientPrintAll(HUD_PRINTCENTER, UTIL_VarArgs("Melee Only Mode is now on"));
		else
			UTIL_ClientPrintAll(HUD_PRINTCENTER, UTIL_VarArgs("Melee Only Mode is now off"));
	}
	// Ben End

	// Ben Addition to Raiko Contrib - LB Score
	float flBritishScore = CVAR_GET_FLOAT("mp_britishscore");
	float flAmericanScore = CVAR_GET_FLOAT("mp_americanscore");

	if(iBritishScore != (int)flBritishScore)
	{
		SetTeamPoints(BRITS, (int)flBritishScore);
		UTIL_ClientPrintAll(HUD_PRINTCONSOLE, UTIL_VarArgs("British Score has been overriden to be %i\0", iBritishScore));
	}

	if(iAmericanScore != (int)flAmericanScore)
	{
		SetTeamPoints(AMERS, (int)flAmericanScore);
		UTIL_ClientPrintAll(HUD_PRINTCONSOLE, UTIL_VarArgs("American Score has been overriden to be %i\0", iAmericanScore));

	}
	// Ben End

	// Raiko Contrib - Stationary Reload
	extern DLL_GLOBAL float g_flStationaryReload;

	float flStationaryReload = CVAR_GET_FLOAT("mp_stationaryreload");
	if(g_flStationaryReload != flStationaryReload)
	{
		g_flStationaryReload = flStationaryReload;

		extern int gmsgStationaryReload;

		MESSAGE_BEGIN(MSG_ALL, gmsgStationaryReload);
		WRITE_BYTE((int)flStationaryReload);
		MESSAGE_END();

		if(flStationaryReload > 0.0)
			UTIL_ClientPrintAll(HUD_PRINTCENTER, UTIL_VarArgs("Stationary Reload Mode is now on"));
		else
			UTIL_ClientPrintAll(HUD_PRINTCENTER, UTIL_VarArgs("Stationary Reload Mode is now off"));
	}
	// Raiko Contrib Stationary Reload End

	if(pRoundRules != NULL)
		pRoundRules->RoundThink();
}

bool CBaseBgRules::ClientConnected(edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[])
{
	g_VoiceGameMgr.ClientConnected(pEntity);
	return true;
}

//=========================================================
// ClientCommand
// the user has typed a command which is unrecognized by everything else;
// this check to see if the gamerules knows anything about the command
//=========================================================
bool CBaseBgRules :: ClientCommand( CBasePlayer *pPlayer, const char *pcmd )
{
	if ( FStrEq( pcmd, "menuselect" ) )
	{
		if ( CMD_ARGC() < 2 )
			return true;

		int slot = atoi( CMD_ARGV(1) );

		switch (pPlayer->m_eCurrentMenu)
		{
			case VOICE_MENU1:
				HandleVoiceMenu1(slot, pPlayer);
				break;
			case VOICE_MENU2:
				HandleVoiceMenu2(slot, pPlayer);
			default:
				break;
		}
		pPlayer->m_eCurrentMenu = NO_MENU;
		return true;
	}
	else if ( FStrEq( pcmd, "jointeam" ) )
	{
		int slot = atoi( CMD_ARGV(1) );//we need to re-identify the slot

		if(slot > NUM_TEAMS || slot < UNDEFINED)
		{
			// Team with fewest players
			HandleTeams(pPlayer, TeamWithFewestPlayers());
		}
		else
		{
			//class vgui menus are now handled inside the team code
			HandleTeams(pPlayer, slot);
		}

		return true;
	}

	else if ( FStrEq( pcmd, "joinclass" ) )
	{
		int slot = atoi( CMD_ARGV(1) );

		//if the class number is out of the range of the number of classes do a random class
		if(slot > NUM_CLASSES || slot < UNDEFINED)
		{
			int random = RANDOM_LONG(UNDEFINED+1, NUM_CLASSES);
			HandleClasses(pPlayer, random);
		}
		//if it is a valid class number
		else
		{
			HandleClasses(pPlayer, slot);
		}

		return true;
	}

	return g_VoiceGameMgr.ClientCommand(pPlayer, pcmd);
}

extern int gmsgGameMode;
extern int gmsgSayText;
extern int gmsgTeamInfo;

void CBaseBgRules :: UpdateGameMode( CBasePlayer *pPlayer )
{
	MESSAGE_BEGIN( MSG_ONE, gmsgGameMode, NULL, pPlayer->pev );
		WRITE_BYTE( 1 );  // game mode teamplay
	MESSAGE_END();
}

const char *CBaseBgRules::SetDefaultPlayerTeam( CBasePlayer *pPlayer )
{
	//Ben - All gone for the new team system
	return NULL;
}


//=========================================================
// InitHUD
//=========================================================
void CBaseBgRules::InitHUD( CBasePlayer *pPlayer )
{
	//Ben
	extern int gmsgTeams;
	//Ben

	pPlayer->pev->team = 0;

	//Ben - Sends our team names to the client
	MESSAGE_BEGIN(MSG_ONE, gmsgTeams, NULL, pPlayer->pev);
	WRITE_BYTE(3);
	WRITE_STRING(British);
	WRITE_STRING(Americans);
	WRITE_STRING(Spectators);
	MESSAGE_END();

    CHalfLifeMultiplay::InitHUD( pPlayer ); 

    // update this player with all the other players team info
    // loop through all active players and send their team info to the new client
    for ( int i = 0; i <= gpGlobals->maxClients; i++ )
    {
        CBasePlayer *plr = (CBasePlayer*)UTIL_PlayerByIndex( i );
        if ( plr)
        {
            MESSAGE_BEGIN( MSG_ONE, gmsgTeamInfo, NULL, pPlayer->edict() );
                WRITE_BYTE( plr->entindex() );
                WRITE_STRING( szTeamNames[plr->m_iTeamRespawningAs]);
            MESSAGE_END();

			extern int gmsgScoreInfo;

			MESSAGE_BEGIN(MSG_ONE, gmsgScoreInfo, NULL, pPlayer->edict() );
				WRITE_BYTE( plr->entindex());
				WRITE_LONG( (long)plr->m_flDamageDone );
				WRITE_LONG( (long)plr->m_flCapturePoints );
				WRITE_SHORT( plr->m_iDeaths );
				WRITE_SHORT( plr->m_iClassRespawningAs );
				WRITE_SHORT( plr->m_iTeamRespawningAs );
			MESSAGE_END();

			extern int gmsgDead;

			MESSAGE_BEGIN(MSG_ONE, gmsgDead, NULL, pPlayer->edict());
				WRITE_BYTE(plr->entindex());
				WRITE_BYTE(plr->m_iDead);
			MESSAGE_END();
        }
    }

	pPlayer->m_bIsAllowedToRespawn = false;

	ShowVGUI(pPlayer, MENU_TEAM);

	extern int gmsgFullTeamScore;

	MESSAGE_BEGIN(MSG_ONE, gmsgFullTeamScore, NULL, pPlayer->pev);
	WRITE_SHORT( iBritishScore );
	WRITE_SHORT( iAmericanScore );
	MESSAGE_END();

	float flMelee = CVAR_GET_FLOAT("mp_meleeonly");
	extern int gmsgMeleeOnly;

	MESSAGE_BEGIN(MSG_ONE, gmsgMeleeOnly, NULL, pPlayer->pev);
	WRITE_BYTE((int)flMelee);
	MESSAGE_END();

	if(pPointManager != NULL) {
		pPointManager->UpdatePlayersHUD(pPlayer, true);
		pPointManager->UpdatePlayersHUD(pPlayer);
	}

	CEnvFog *pFog = NULL;
	pFog = (CEnvFog*)UTIL_FindEntityByClassname( pFog, "env_fog" );

	// -only send fog if we have a fog ent
	if(pFog)
	{ 
		SendFogData ( g_col, g_iFadeOut, g_iStartDist, g_iEndDist );
	}

	pPlayer->m_bSpawnPS			= false;
	pPlayer->m_bSpawnGrass		= false;
	pPlayer->m_flLastPSSpawn		= 0.0;
	pPlayer->m_flLastGrassSpawn	= 0.0;
	pPlayer->pLastGrassSpawned = NULL;
	pPlayer->pLastPSSpawned = NULL;
}


void CBaseBgRules::ChangePlayerTeam( CBasePlayer *pPlayer, const char *pTeamName, bool bKill, bool bGib )
{
	int damageFlags = DMG_GENERIC;
	int clientIndex = pPlayer->entindex();

	if ( !bGib )
	{
		damageFlags |= DMG_NEVERGIB;
	}
	else
	{
		damageFlags |= DMG_ALWAYSGIB;
	}

	if ( bKill )
	{
		// kill the player,  remove a death,  and let them start on the new team
		m_DisableDeathMessages = true;
		m_DisableDeathPenalty = true;

		entvars_t *pevWorld = VARS( INDEXENT(0) );
		pPlayer->TakeDamage( pevWorld, pevWorld, 900, damageFlags );

		m_DisableDeathMessages = false;
		m_DisableDeathPenalty = false;
	}

	pPlayer->RemoveAllItems( true );


	MESSAGE_BEGIN( MSG_ONE, gmsgTeamInfo, NULL, pPlayer->edict() );
		WRITE_BYTE( clientIndex );
		WRITE_STRING( pPlayer->m_szTeamName );
	MESSAGE_END();
}


//=========================================================
// ClientUserInfoChanged
//=========================================================
void CBaseBgRules::ClientUserInfoChanged( CBasePlayer *pPlayer, char *infobuffer ) {} 

extern int gmsgDeathMsg;

//=========================================================
// Deathnotice. 
//=========================================================
void CBaseBgRules::DeathNotice( CBasePlayer *pVictim, entvars_t *pKiller, entvars_t *pevInflictor )
{

	CHalfLifeMultiplay::DeathNotice( pVictim, pKiller, pevInflictor );
}

//=========================================================
//=========================================================
void CBaseBgRules :: PlayerKilled( CBasePlayer *pVictim, entvars_t *pKiller, entvars_t *pInflictor )
{
	if ( !m_DisableDeathPenalty )
	{
		CHalfLifeMultiplay::PlayerKilled( pVictim, pKiller, pInflictor );
	}
}


//=========================================================
// IsTeamplay
//=========================================================
bool CBaseBgRules::IsTeamplay( void )
{
	return true;
}

bool CBaseBgRules::FPlayerCanTakeDamage( CBasePlayer *pPlayer, CBaseEntity *pAttacker )
{
	if ( pAttacker && PlayerRelationship( pPlayer, pAttacker ) == GR_TEAMMATE )
	{
		// my teammate hit me.
		if ( (friendlyfire.value == 0) && (pAttacker != pPlayer) )
		{
			// friendly fire is off, and this hit came from someone other than myself,  then don't get hurt
			return false;
		}
	}

	return CHalfLifeMultiplay::FPlayerCanTakeDamage( pPlayer, pAttacker );
}

//=========================================================
//=========================================================
int CBaseBgRules::PlayerRelationship( CBaseEntity *pPlayer, CBaseEntity *pTarget )
{
	// half life multiplay has a simple concept of Player Relationships.
	// you are either on another player's team, or you are not.
	if ( !pPlayer || !pTarget || !pTarget->IsPlayer() )
		return GR_NOTTEAMMATE;

	if (pPlayer->pev->team == pTarget->pev->team)
	{
		return GR_TEAMMATE;
	}

	return GR_NOTTEAMMATE;
}

//=========================================================
//=========================================================
bool CBaseBgRules::ShouldAutoAim( CBasePlayer *pPlayer, edict_t *target )
{
	// always autoaim, unless target is a teammate
	CBaseEntity *pTgt = CBaseEntity::Instance( target );
	if ( pTgt && pTgt->IsPlayer() )
	{
		if ( PlayerRelationship( pPlayer, pTgt ) == GR_TEAMMATE )
			return false; // don't autoaim at teammates
	}

	return CHalfLifeMultiplay::ShouldAutoAim( pPlayer, target );
}

//=========================================================
//=========================================================
int CBaseBgRules::IPointsForKill( CBasePlayer *pAttacker, CBasePlayer *pKilled )
{
	if ( !pKilled )
		return 0;

	if ( !pAttacker )
		return 1;

	if ( pAttacker != pKilled && PlayerRelationship( pAttacker, pKilled ) == GR_TEAMMATE )
		return -1;

	return 1;
}

//=========================================================
//=========================================================
const char *CBaseBgRules::GetTeamID( CBaseEntity *pEntity )
{
	if ( pEntity == NULL || pEntity->pev == NULL )
		return "";

	// return their team name
	return pEntity->TeamID();
}


int CBaseBgRules::GetTeamIndex( const char *pTeamName )
{
	if ( pTeamName && *pTeamName != 0 )
	{
		// try to find existing team
		for ( int tm = 0; tm < NUM_TEAMS; tm++ )
		{
			if ( !stricmp( szTeamNames[tm], pTeamName ) )
				return tm;
		}
	}
	
	return -1;	// No match
}


bool CBaseBgRules::IsValidTeam( const char *pTeamName ) 
{
	if ( !m_teamLimit )	// Any team is valid if the teamlist isn't set
		return true;

	return ( GetTeamIndex( pTeamName ) != -1 ) ? true : false;
}

int CBaseBgRules::TeamWithFewestPlayers( void )
{
	int iBritish = 0;
	int iAmerican = 0;

	RecountTeams(iBritish, iAmerican);

	//less brits
	if(iBritish < iAmerican)
	{
		return BRITS;
	}
	//less americans
	else if(iBritish > iAmerican)
	{
		return AMERS;
	}
	else
	{
		return (int)RANDOM_LONG(BRITS, AMERS);
	}
}

void CBaseBgRules::RecountTeams(int &iBrits, int &iAmers)
{
	int i = 0;
	int iBritish = 0;
	int iAmerican = 0;

	for(; i <= gpGlobals->maxClients; i++)
	{
		CBasePlayer *pPlayer = (CBasePlayer*)UTIL_PlayerByIndex(i);

		if(pPlayer)
		{
			if(pPlayer->pev->team == BRITS)
			{
				iBritish++;
			}
			else if(pPlayer->pev->team == AMERS)
			{
				iAmerican++;
			}
			else
			{
				if(pPlayer->m_iTeamRespawningAs == BRITS)
				{
					iBritish++;
				}
				else if(pPlayer->m_iTeamRespawningAs == AMERS)
				{
					iAmerican++;
				}
			}
		}
	}
	iBrits = iBritish;
	iAmers = iAmerican;
}

void CBaseBgRules::HandleVoiceMenu1(int iSlot, CBasePlayer *pPlayer)
{
	if(pPlayer->m_flTimeTillNextVoiceCommand > gpGlobals->time)
		return;

	if(iSlot < 1 || iSlot > 8)
		return;

	if((pPlayer->pev->team < BRITS) || (pPlayer->pev->team > AMERS))
		return;

	if(pPlayer->m_iDead == DEAD)
		return;

	if((int)CVAR_GET_FLOAT("mp_voice_commands") == 0)
		return;

	char file[64];
	int rand = 0;
	bool bSayEveryone = false;
	int iMsg = 0;

	switch(iSlot)
	{
	//yes
	case 1:
		rand = RANDOM_LONG(1, 3);
		switch(rand)
		{
		case 1:
			sprintf(file, "aye"); break;
		case 2:
			sprintf(file, "ok"); break;
		case 3:
			sprintf(file, "roger"); break;
		default:
			sprintf(file, "aye"); break;
		}
		iMsg = iCmd_Yes;
		break;
	//no
	case 2:
		rand = RANDOM_LONG(1, 2);
		switch(rand)
		{
		case 1:
			sprintf(file, "cant"); break;
		case 2:
			sprintf(file, "impossible"); break;
		default:
			sprintf(file, "cant"); break;
		}
		iMsg = iCmd_No;
		break;
	//Follow me
	case 3:
		sprintf(file, "follow_me");
		iMsg = iCmd_Follow;
		break;
	//Leave it to me
	case 4:
		sprintf(file, "leave_to_me");
		iMsg = iCmd_Leave_To_Me;
		break;
	//Under attack
	case 5:
		sprintf(file, "under_attack");
		iMsg = iCmd_Need_Help;
		break;
	//Spread out
	case 6:
		sprintf(file, "spread");
		iMsg = iCmd_Spread_Out;
		break;
	//battle cry
	case 7:
		if(pPlayer->m_flTimeTillNextBattleCry > gpGlobals->time)
		{
			return;
		}
		else
		{
			pPlayer->m_flTimeTillNextBattleCry = gpGlobals->time + 30;
		}

		if(pPlayer->pev->team == BRITS)
		{
			sprintf(file, "for_king");
			iMsg = iCmd_Brit_Battlecry;
			bSayEveryone = true;
		}
		else
		{
			sprintf(file, "freedom");
			iMsg = iCmd_Amer_Battlecry;
			bSayEveryone = true;
		}
		break;
	//medic!!!
	case 8:
		sprintf(file, "medical_aid");
		iMsg = iCmd_Medical_Aid;
		break;
	default:
		return;
	}
	char ext[6] = "wav\0";
	char folder[24] = "commands\0";

	char command[256];
	sprintf(command, "%s/%s.%s\0", folder, file, ext);

	pPlayer->m_flTimeTillNextVoiceCommand = gpGlobals->time + 2.0;

	for(int i = 0; i <= gpGlobals->maxClients; i++)
	{
		CBasePlayer *player = (CBasePlayer*)UTIL_PlayerByIndex(i);
		if(player)
		{
			if((player->pev->team == pPlayer->pev->team) || (bSayEveryone == true))
			{
				if(g_VoiceGameMgr.PlayerHasBlockedPlayer(player, pPlayer) == false)
				{
					extern int gmsgPredefinedMessage;
					MESSAGE_BEGIN(MSG_ONE, gmsgPredefinedMessage, NULL, player->pev);

					WRITE_SHORT(iVoiceCommands);
					WRITE_SHORT(iMsg);
					WRITE_SHORT(pPlayer->entindex());

					MESSAGE_END();
				}
			}
		}
	}

	EMIT_SOUND_DYN(ENT(pPlayer->pev), CHAN_STATIC, command, VOL_NORM, ATTN_NORM, 0, PITCH_NORM);
}

void CBaseBgRules::HandleVoiceMenu2(int iSlot, CBasePlayer *pPlayer)
{
	if(pPlayer->m_flTimeTillNextVoiceCommand > gpGlobals->time)
		return;

	if(iSlot < 1 || iSlot > 9)
		return;

	if((pPlayer->pev->team < BRITS) || (pPlayer->pev->team > AMERS))
		return;

	if(pPlayer->m_iDead == DEAD)
		return;

	if((int)CVAR_GET_FLOAT("mp_voice_commands") == 0)
		return;

	char file[64];
	int rand = 0;
	bool bSayEveryone = false;
	int iMsg = 0;

	switch(iSlot)
	{
	//advance
	case 1:
		rand = RANDOM_LONG(1, 2);
		switch(rand)
		{
		case 1:
			sprintf(file, "move"); break;
		case 2:
			sprintf(file, "advance"); break;
		default:
			sprintf(file, "advance"); break;
		}
		iMsg = iCmd_Move_On;
		break;
	//retreat
	case 2:
		sprintf(file, "retreat");
		iMsg = iCmd_Retreat;
		break;
	//rally
	case 3:
		sprintf(file, "rally");
		iMsg = iCmd_Rally;
		break;
	//halt
	case 4:
		sprintf(file, "halt");
		iMsg = iCmd_Halt;
		break;
	//line up
	case 5:
		sprintf(file, "line_up");
		iMsg = iCmd_Line_Up;
		break;
	//Make Ready
	case 6:
		rand = RANDOM_LONG(1, 2);
		switch(rand)
		{
		case 1:
			sprintf(file, "make_ready"); break;
		case 2:
			sprintf(file, "make_ready2"); break;
		default:
			sprintf(file, "make_ready"); break;
		}
		iMsg = iCmd_Make_Ready;
		break;
	//Make Ready
	case 7:
		rand = RANDOM_LONG(1, 2);
		switch(rand)
		{
		case 1:
			sprintf(file, "present"); break;
		case 2:
			sprintf(file, "present2"); break;
		default:
			sprintf(file, "present"); break;
		}
		iMsg = iCmd_Present_Arms;
		break;
	//fire
	case 8:
		sprintf(file, "fire");
		iMsg = iCmd_Fire;
		break;
	//line up
	case 9:
		sprintf(file, "cease");
		iMsg = iCmd_Cease_Fire;
		break;
	default:
		return;
	}
	char ext[6] = "wav\0";
	char folder[24] = "commands\0";

	char command[256];
	sprintf(command, "%s/%s.%s\0", folder, file, ext);

	pPlayer->m_flTimeTillNextVoiceCommand = gpGlobals->time + 2.0;

	for(int i = 0; i <= gpGlobals->maxClients; i++)
	{
		CBasePlayer *player = (CBasePlayer*)UTIL_PlayerByIndex(i);
		if(player)
		{
			if((player->pev->team == pPlayer->pev->team) || (bSayEveryone == true))
			{
				if(g_VoiceGameMgr.PlayerHasBlockedPlayer(player, pPlayer) == false)
				{
					extern int gmsgPredefinedMessage;
					MESSAGE_BEGIN(MSG_ONE, gmsgPredefinedMessage, NULL, player->pev);

					WRITE_SHORT(iVoiceCommands);
					WRITE_SHORT(iMsg);
					WRITE_SHORT(pPlayer->entindex());

					MESSAGE_END();
				}
			}
		}
	}

	EMIT_SOUND_DYN(ENT(pPlayer->pev), CHAN_STATIC, command, VOL_NORM, ATTN_NORM, 0, PITCH_NORM);
}

//update our scores & send them to everyone
void CBaseBgRules::UpdateMyScores(CBasePlayer *pPlayer)
{
	extern int gmsgTeamInfo;
	extern int gmsgScoreInfo;
	extern int gmsgDead;

	if(pPlayer == NULL)
		return;

	int iClassNum = 0;

	if(pPlayer->pev->playerclass == UNDEFINED ||
		(pPlayer->pev->playerclass != pPlayer->m_iClassRespawningAs &&
		pPlayer->m_iDead == DEAD))
	{
		iClassNum = pPlayer->m_iClassRespawningAs;
		
	}
	else
	{
		iClassNum = pPlayer->pev->playerclass;
	}

	MESSAGE_BEGIN(MSG_ALL, gmsgTeamInfo);
	WRITE_BYTE(pPlayer->entindex());
	WRITE_STRING(szTeamNames[pPlayer->m_iTeamRespawningAs]);
	MESSAGE_END();

	MESSAGE_BEGIN(MSG_ALL, gmsgScoreInfo);
	WRITE_BYTE( pPlayer->entindex() );
	WRITE_LONG( (long)pPlayer->m_flDamageDone );
	WRITE_LONG( (long)pPlayer->m_flCapturePoints );
	WRITE_SHORT( pPlayer->m_iDeaths );
	WRITE_SHORT( iClassNum );
	WRITE_SHORT( pPlayer->m_iTeamRespawningAs );
	MESSAGE_END();

	MESSAGE_BEGIN(MSG_ALL, gmsgDead);
	WRITE_BYTE( pPlayer->entindex() );
	WRITE_BYTE( pPlayer->m_iDead );
	MESSAGE_END();
}

// gives a team extra points
void CBaseBgRules::GiveTeamPoints( int iTeam, int iPoints )
{
	if( g_fGameOver )
		return;

	// -save the total points, so we can send to new clients on connect
	int iTotalPoints = 0;
	if(iTeam == BRITS)
	{
		iBritishScore += iPoints;
		iTotalPoints = iBritishScore;
	}

	else if(iTeam == AMERS)
	{
		iAmericanScore += iPoints;
		iTotalPoints = iAmericanScore;
	}

	extern int gmsgFullTeamScore;
	MESSAGE_BEGIN(MSG_ALL, gmsgFullTeamScore);
	WRITE_LONG( iBritishScore );
	WRITE_LONG( iAmericanScore );
	MESSAGE_END();

	// Ben Addition to Raiko Contrib - LB Score
	CVAR_SET_FLOAT("mp_britishscore", iBritishScore);
	CVAR_SET_FLOAT("mp_americanscore", iAmericanScore);
	// Ben End

	// -log the score changes
	UTIL_LogPrintf(BASIC_LOGGING, "Team \"%s\" triggered \"recieve_points\" (points_added \"%i\") (total_score \"%i\")\n",
			szTeamNames[iTeam], iPoints, iTotalPoints);
}

// this should set points to an exact value
void CBaseBgRules::SetTeamPoints( int iTeam, int iPoints )
{
	int iPointsFinal = 0;

	// disallow negative scores
	if(iPoints < 0)
		iPoints = 0;

	if(iTeam == BRITS)
	{
		iPointsFinal = iPoints - iBritishScore;
		iBritishScore = iPoints;
		CVAR_SET_FLOAT("mp_britishscore", iBritishScore);
	}

	else if(iTeam == AMERS)
	{
		iPointsFinal = iPoints - iAmericanScore;
		iAmericanScore = iPoints;

		CVAR_SET_FLOAT("mp_americanscore", iAmericanScore);
	}
	else{}

	extern int gmsgFullTeamScore;
	MESSAGE_BEGIN(MSG_ALL, gmsgFullTeamScore);
	WRITE_LONG( iBritishScore );
	WRITE_LONG( iAmericanScore );
	MESSAGE_END();

	// -log the score changes
	UTIL_LogPrintf(BASIC_LOGGING, "World triggered \"override_points\" (team \"%s\") (new_total_score \"%i\")\n",
			szTeamNames[iTeam], iPointsFinal);
}

void CBaseBgRules::InitSpawns( void ) {
	CSpawnContainer *pSpawnContainer = NULL;
	pSpawnContainer = (CSpawnContainer*)UTIL_FindEntityByClassname( pSpawnContainer, "info_spawn_container" );

	while (pSpawnContainer)
	{ 
		bool bOn = false;
		// 1 is start on
		if(pSpawnContainer->pev->spawnflags & 1) 
			bOn = true;

		pSpawnContainer->SetOurSpawnPoints(bOn);
		pSpawnContainer->bActive = bOn;
		pSpawnContainer->bTriggeredOn = bOn;
		pSpawnContainer->ResetEnt();

		pSpawnContainer = (CSpawnContainer*)UTIL_FindEntityByClassname( pSpawnContainer, "info_spawn_container" );
	}
}

//=========================================================
// Ben - Map detection
//========================================================= 
class CInfoBgDetect : public CBaseEntity
{
    void Spawn( void )
    {
        UTIL_SetOrigin( pev, pev->origin );
        pev->solid = SOLID_NOT;
        pev->effects = EF_NODRAW;        
    }
}; 
LINK_ENTITY_TO_CLASS( info_bg_detect, CInfoBgDetect ); 
//Ben
