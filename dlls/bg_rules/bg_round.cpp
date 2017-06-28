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

//This file was created on the 8/3/03 at 16:26 by Ben Banfield for the Battle Grounds modification of Half-Life
//Its purpose is to define the base round rules class

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"player.h"

#include	"game.h"
#include	"bg_round.h"

#include	"gamerules.h"
#include	"bg_rules.h"

#include	"func_break.h"
#include	"doors.h"

#include	"point.h"
#include	"../../cl_dll/message_defs.h"

#include	"func_grass.h"
#include	"particle_emitter.h"

#include "point_win.h" // Saint Greg Contribution


extern CPointManager *pPointManager;
extern CBaseBgRules *g_pBGRules;



// setup all variables up to default state
CBaseRoundRules::CBaseRoundRules()
{
	m_flRoundTimeLimit = 0.0;
	m_flRoundTime = 0.0;

	m_iRespawnStyle = NULL_RESPAWN;
	SetRespawnStyle((RESPAWN_STYLE)((int)CVAR_GET_FLOAT("mp_respawnstyle")));

	m_iRoundState = ROUND_NOTSTARTED;

	iCount = 0;
	ClearTeamsCount();
	m_iWinningTeam = 0;

	ALERT(at_console, "New Round Rules Created\n");
	UTIL_LogPrintf(ROUND_LOGGING, "World triggered \"Round Rules Created\"\n");
}

// clean up all variables
CBaseRoundRules::~CBaseRoundRules()
{
	m_flRoundTimeLimit = 0.0;
	m_flRoundTime = 0.0;

	m_iRespawnStyle = NULL_RESPAWN;
	m_iRoundState = ROUND_NOTSTARTED;

	iCount = 0;
	ClearTeamsCount();
	m_iWinningTeam = 0;

	ALERT(at_console, "Old Round Rules Destroyed\n");
	UTIL_LogPrintf(ROUND_LOGGING, "World triggered \"Round Rules Destroyed\"\n");
}


// returns the current respawn style
RESPAWN_STYLE CBaseRoundRules::GetRespawnStyle()
{
	if(!m_iRespawnStyle)
	{
		// - report the error
		ALERT(at_console, "Null Respawn Style\n");
		UTIL_LogPrintf(ROUND_LOGGING, "World triggered \"Null Respawn Style\"\n");

		SetRespawnStyle(WAVE_RESPAWN);
		return m_iRespawnStyle;
	}

	return m_iRespawnStyle;
}

// returns the current round state
ROUND_STATE CBaseRoundRules::GetRoundState()
{
	// check validity of current round state
	if(!m_iRoundState)
	{
		// - report the error
		ALERT(at_console, "Null Round State\n");
		UTIL_LogPrintf(ROUND_LOGGING, "World triggered \"Null Round State\"\n");

		// - set a default round state to help the game recover
		SetRoundState(ROUND_NOTSTARTED, false);
		return m_iRoundState;
	}

	return m_iRoundState;
}

// sets the round state
// ben notes : bSkipAutoRoundThink will avoid calling the round think function
// ben notes : after updating the status
void CBaseRoundRules::SetRoundState(ROUND_STATE iRoundState, bool bSkipAutoRoundThink)
{
	// check validity of what we have been sent
	if((iRoundState < ROUND_NOTSTARTED) || (iRoundState > ROUND_NOPLAYERS))
	{
		// - report the error
		ALERT(at_console, "Bad Round State being set - (%i)\n", (int)iRoundState);
		UTIL_LogPrintf(ROUND_LOGGING, "World triggered \"Bad Round State\" (round_state \"%i\") (old_round_state \"%i\")\n", (int)iRoundState, (int)m_iRoundState);

		// - give the round state a dummy value to keep it going
		iRoundState = ROUND_WAITING_FOR_PLAYERS;
	}
	// - log the change
	UTIL_LogPrintf(ALL_LOGGING, "World triggered \"Round State Change\" (round_state \"%i\") (old_round_state \"%i\")\n", (int)iRoundState, (int)m_iRoundState);

	// - set the round state
	m_iRoundState = iRoundState;

	// - call round think to update the round progress.
	// ben notes : after set round state is called we should leave the round think
	// ben notes : function or wierd bugs could creep in
	if(GetRoundState() != ROUND_NOPLAYERS && bSkipAutoRoundThink == false)
		this->RoundThink();
}

// sets the respawn style
void CBaseRoundRules::SetRespawnStyle(RESPAWN_STYLE iRespawnStyle)
{
	// check validity of what we have been sent
	if((iRespawnStyle < WAVE_RESPAWN) || (iRespawnStyle > ROUND_RESPAWN))
	{
		// - report the error
		ALERT(at_console, "Bad Respawn Style being set - (%i)\n", (int)iRespawnStyle);
		UTIL_LogPrintf(ROUND_LOGGING, "World triggered \"Bad Respawn Style\" (respawn_style \"%i\") (old_respawn_style \"%i\")\n", (int)iRespawnStyle, (int)m_iRespawnStyle);

		// - give the round state a dummy value to keep it going
		iRespawnStyle = WAVE_RESPAWN;
	}
	// - log the change
	UTIL_LogPrintf(ALL_LOGGING, "World triggered \"Respawn Style Change\" (respawn_style \"%i\") (old_respawn_style \"%i\")\n", (int)iRespawnStyle, (int)m_iRespawnStyle);

	// - set the round state
	m_iRespawnStyle = iRespawnStyle;
}

// Counts the number of players who are on a team
void CBaseRoundRules::CountPlayersOnTeam(bool bIncludeDead)
{
	// - init vars by reseting them
	ClearTeamsCount();
	// - loop through players recording their team
	for(iCount = 0; iCount <= gpGlobals->maxClients; iCount++)
	{
		CBasePlayer *pPlayer = (CBasePlayer*)UTIL_PlayerByIndex(iCount);

		if(pPlayer)
		{
			if((bIncludeDead == false) && (pPlayer->m_iDead == DEAD))
				continue;

			if(pPlayer->pev->team == BRITS)
			{
				m_iBritish++;
			}
			else if(pPlayer->pev->team == AMERS)
			{
				m_iAmericans++;
			}
			else
			{
				m_iSpectators++;
			}
		}
	}
}

// Counts players on each team, including future teams
void CBaseRoundRules::CountPlayersOnTeamFuture(bool bIncludeDead)
{
	// - init vars by reseting them
	ClearTeamsCount();
	// - loop through players recording their team
	for(iCount = 0; iCount <= gpGlobals->maxClients; iCount++)
	{
		CBasePlayer *pPlayer = (CBasePlayer*)UTIL_PlayerByIndex(iCount);

		if(pPlayer)
		{
			if((bIncludeDead == false) && (pPlayer->m_iDead == DEAD))
				continue;

			if(pPlayer->m_iClassRespawningAs <= UNDEFINED)
				continue;

			if((pPlayer->pev->team == BRITS) || (pPlayer->m_iTeamRespawningAs == BRITS))
			{
				m_iBritish++;
			}
			else if((pPlayer->pev->team == AMERS) || (pPlayer->m_iTeamRespawningAs == AMERS))
			{
				m_iAmericans++;
			}
			else
			{
				m_iSpectators++;
			}
		}
	}
}

// sets the maximum length the round pause can be
void CBaseRoundRules::SetPauseTimeLimit(float flPauseTimeLimit)
{
	// - non-zero value sets to the limit
	if(flPauseTimeLimit)
	{
		m_flPauseTimeLimit = flPauseTimeLimit;
	}
	// - zero value sets infinite round time limit (well practically)
	else
	{
		// ben - prolly won't use this, but it makes sure there is a value at all times
		m_flPauseTimeLimit = 0.0;
	}
}

// sets the maximum length a round can be
void CBaseRoundRules::SetRoundTimeLimit(float flRoundTimeLimit)
{
	// - non-zero value sets to the limit
	if(flRoundTimeLimit)
	{
		m_flRoundTimeLimit = flRoundTimeLimit;
	}
	// - zero value sets infinite round time limit (well practically)
	else
	{
		// ben - :D thats a lot of seconds
		// ben - prolly won't use this, but it makes sure there is a value at all times
		m_flRoundTimeLimit = 999999.9;
	}
}

// sets the maximum length the restart pause time can be
void CBaseRoundRules::SetRestartTimeLimit(float flRestartTimeLimit)
{
	// - non-zero value sets to the limit
	if(flRestartTimeLimit)
	{
		m_flRestartTimeLimit = flRestartTimeLimit;
	}
	// - zero value sets infinite restart time limit (well practically)
	else
	{
		// ben - :D thats a lot of seconds
		// ben - prolly won't use this, but it makes sure there is a value at all times
		m_flRestartTimeLimit = 999999.9;
	}
}

// sets the length of the round
void CBaseRoundRules::SetRoundLength(float flRoundTime, bool bPaused)
{
	// - cap the round length to the round time limit or pause time limit depending on state
	if(!bPaused)
	{
		if(flRoundTime > GetRoundTimeLimit())
			flRoundTime =  GetRoundTimeLimit();
	}
	else
	{
		if(flRoundTime > GetPauseTimeLimit())
			flRoundTime = GetPauseTimeLimit();
	}

	// - check for greater than zero value and set round length if so
	if(flRoundTime > 0.0)
	{
		m_flRoundTime = gpGlobals->time + flRoundTime;
	}
	// - sets to the limit if it is less than zero
	else
	{
		// -- this is wierd so log it
		ALERT(at_console, "Bad Round Length being set - (%1.f)\n", flRoundTime);
		UTIL_LogPrintf(ROUND_LOGGING, "World triggered \"Bad Round Length\" (round_length \"%1.f\") (old_round_length \"1.f\") (respawn_style \"%i\") (round_state \"%i\")\n", flRoundTime, GetRoundTime(), (int)m_iRespawnStyle, (int)m_iRoundState);

		if(!bPaused)
			m_flRoundTime = gpGlobals->time + GetRoundTimeLimit();
		else
			m_flRoundTime = gpGlobals->time + GetPauseTimeLimit();
	}

	// - send client the round time
	SendRoundTimeToClients(bPaused);
}

// sets the length of the pause between round end and respawning
void CBaseRoundRules::SetRestartLength(float flRestartTime)
{
	if(flRestartTime > GetRestartTimeLimit()) {
		flRestartTime = GetRestartTimeLimit();
	}

	if(flRestartTime >= 0) {
		m_flRestartTime = gpGlobals->time + flRestartTime;
	} else {
		// -- this is wierd to log it
		ALERT(at_console, "Bad Restart Length being set - (%1.f)\n", flRestartTime);
		UTIL_LogPrintf(ROUND_LOGGING, "World triggered \"Bad Restart Length\" (restart_length \"%1.f\") (old_restart_length \"1.f\") (respawn_style \"%i\") (round_state \"%i\")\n", flRestartTime, GetRestartTime(), (int)m_iRespawnStyle, (int)m_iRoundState);
		
		m_flRestartTime = gpGlobals->time + GetRestartTimeLimit();
	}
}

// sents the round time and state to all clients
void CBaseRoundRules::SendRoundTimeToClients(bool bPaused)
{
	extern int gmsgClockTime;

	MESSAGE_BEGIN(MSG_ALL, gmsgClockTime);
	WRITE_SHORT((int)(GetRoundTime() - gpGlobals->time));
	WRITE_SHORT((bPaused ? CLOCK_PAUSE : CLOCK_ROUND));
	MESSAGE_END();
}

// - stops the win sounds early
void CBaseRoundRules::StopWinSounds()
{
}

// Ben - End of Helper functions
// Ben - Start of Round State Functions

// determines whether we have enough players for a round
bool CBaseRoundRules::CheckForRoundStart()
{
	// - determine number of players present and future
	CountPlayersOnTeamFuture(true);

	// - assess whether we have enough players for a round
	// - return depending on situation
	// ben notes : m_iTeamRespawning as must be set only once m_iClassRespawning as is set
	// ben notes : or we will get players who aren't ready counted here
	if((GetNumOfBritishPlayers() > UNDEFINED) && (GetNumOfAmericanPlayers() > UNDEFINED))
	{
		// -- force a full restart if we are waiting for players
		if(GetRoundState() == ROUND_WAITING_FOR_PLAYERS)
		{
			// --- set restart round
			CVAR_SET_FLOAT("sv_restartround", 1.0);
			// ---message clients tellin them this
			// TODO: Add in messaging function
		}
		return true;
	}
	else
	{
		return false;
	}
}

// starts the round
void CBaseRoundRules::StartRound()
{
	// - set the round time on the servers
	if(ROUND_TIME > GetRoundTimeLimit())
	{
		SetRoundLength(GetRoundTimeLimit());
	}
	else
	{
		SetRoundLength(ROUND_TIME);
	}
	// - set the round time on the clients
	// ben notes : moved into the set round length function so it is always upto date on the clients

	// - update the round status
	SetRoundState(ROUND_RUNNING, false);
}

// starts a dummy round, which will just be forgotten when enough players join
void CBaseRoundRules::StartDummyRound()
{
	// - set the round time on the servers
	if(ROUND_TIME > GetRoundTimeLimit())
	{
		SetRoundLength(GetRoundTimeLimit());
	}
	else
	{
		SetRoundLength(ROUND_TIME);
	}
	// - set the round time on the clients
	// ben notes : moved into the set round length function so it is always upto date on the clients

	// - update the round status
	SetRoundState(ROUND_WAITING_FOR_PLAYERS, false);
}

// - checks whether any other win conditions are triggered - for example all points captured
bool CBaseRoundRules::HandleMiscWinConditions()
{
extern CPointManager *pPointManager;
// - query the status of the points
if(pPointManager->EnoughPointsReadyForWin())
{
 m_iWinningTeam = pPointManager->GetWinningTeam();
 StopWinSounds();
 return true;
}
   CPointWin *pPointWin = NULL;
   do{
      pPointWin = (CPointWin *) UTIL_FindEntityByClassname(pPointWin,"point_win");
       if(pPointWin && pPointWin->IsActivated())
       {
          m_iWinningTeam = pPointWin->GetTargetTeam();
          StopWinSounds();
          pPointWin->ResetEnt();
          return true;
       }
   }while(pPointWin);

return false;
}

// - allows a player to respawn
void CBaseRoundRules::RespawnPlayer(CBasePlayer *pPlayer)
{
	pPlayer->m_bIsAllowedToRespawn = true;
	if(pPlayer->pev->deadflag == DEAD_RESPAWNABLE)
	{
		pPlayer->PlayerDeathThink();
	}
}

// rewards the winning team (scores, winning sound, etc)
void CBaseRoundRules::AwardWinners()
{
	// - check team num is always a valid team or null (tie)
	if((m_iWinningTeam < UNDEFINED) || (m_iWinningTeam >= SPECS))
	{
		ALERT(at_console, "Bad Winning Team - (%i)\n", (int)m_iWinningTeam);
		UTIL_LogPrintf(ROUND_LOGGING, "World triggered \"Bad Winning Team\" (respawn_style \"%i\") (round_state \"%i\") (winning_team \"%i\")\n", (int)GetRespawnStyle(), (int)GetRoundState(), m_iWinningTeam);
		return;
	}
	
	// - no sound if tie
	if(m_iWinningTeam == UNDEFINED)
		return;

	// - send updated team score to client
	g_pBGRules->GiveTeamPoints(m_iWinningTeam, (int)iWinBonus);

	// - play winning sound
	for(iCount = 0; iCount <= gpGlobals->maxClients; iCount++) 
	{ 
		CBasePlayer *pPlayer = (CBasePlayer*)UTIL_PlayerByIndex(iCount);
		if(pPlayer)
		{
			PLAYBACK_EVENT_FULL( FEV_HOSTONLY, pPlayer->edict(), g_pBGRules->m_usRoundEnd, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, 0, 0, m_iWinningTeam, 0, 0, 0 );
		}
	}

	// - lets tell the player's who won
	extern int gmsgPredefinedMessage;
	MESSAGE_BEGIN(MSG_ALL, gmsgPredefinedMessage);
		WRITE_SHORT(iRoundWon);
		WRITE_SHORT(m_iWinningTeam);
	MESSAGE_END();

	// Ben Contrib - Logging Winners (Demo Contrib)
	// -log the wins
	UTIL_LogPrintf(ROUND_LOGGING, "Team \"%s\" triggered \"Round Win\" (respawn_style \"%i\")\n", szTeamNames[m_iWinningTeam], (int)GetRespawnStyle());
	// Ben Contrib End
}

// restarts everyone and the world
void CBaseRoundRules::FullRestart(bool bResetScores, bool bResetCustom)
{
	// - returns world to default settings
	ResetWorld(bResetCustom);

	// - respawns all players
	for(iCount = 0; iCount <= gpGlobals->maxClients; iCount++)
	{
		CBasePlayer *pPlayer = (CBasePlayer*)UTIL_PlayerByIndex(iCount);

		if(pPlayer)
		{
			if(pPlayer->m_iTeamRespawningAs != UNDEFINED && pPlayer->m_iClassRespawningAs != UNDEFINED)
			{
				if(pPlayer->m_iTeamRespawningAs != SPECS)
				{
					pPlayer->m_bHasDied = true;
					pPlayer->m_bIsAllowedToRespawn = true;
					// ben note : override the check in respawn player as we do want to respawn everyone
					pPlayer->PlayerDeathThink();
					pPlayer->pev->sequence = pPlayer->LookupActivity( ACT_WALK );
					pPlayer->m_fSequenceFinished = true;

					// restart all particles for the player
					pPlayer->m_bSpawnPS = true;
					pPlayer->m_bSpawnGrass = true;
					pPlayer->m_flLastPSSpawn = 0.0;
					pPlayer->m_flLastGrassSpawn = 0.0;
					pPlayer->pLastGrassSpawned = NULL;
					pPlayer->pLastPSSpawned = NULL;
				}
			}
		}
	}

	// - resets the scores of all to 0 damage, 0 kills and 0 deaths
	if(bResetScores)
	{
		for(iCount = 0; iCount  <= gpGlobals->maxClients; iCount++)
		{
			CBasePlayer *pPlayer = (CBasePlayer*)UTIL_PlayerByIndex(iCount);
			if(pPlayer)
			{
				// -- reset any other variables to default here
				pPlayer->m_flCapturePoints = 0.0;
				pPlayer->m_flDamageDone = 0.0;
				pPlayer->pev->frags = 0;
				pPlayer->m_iDeaths = 0;
			}
		}
		
		// -- get the hud and scoreboard to reset
		extern int gmsgResetScores;
		MESSAGE_BEGIN(MSG_ALL, gmsgResetScores);
		WRITE_BYTE(1);
		MESSAGE_END();

		// -- reset the server side scores
		g_pBGRules->iBritishScore = 0;
		g_pBGRules->iAmericanScore = 0;
		CVAR_SET_FLOAT("mp_britishscore", g_pBGRules->iBritishScore);
		CVAR_SET_FLOAT("mp_americanscore", g_pBGRules->iAmericanScore);
	}

	// - set a new round state
	SetRoundState(ROUND_NOTSTARTED);
}

// resets all entities that need reseting
void CBaseRoundRules::ResetWorld(bool bResetCustom, bool bResetOnlyCustom)
{
	// clear all particlesystems with this hijacked message
	extern int gmsgParticles;
	MESSAGE_BEGIN(MSG_ALL, gmsgParticles);
		WRITE_SHORT(0);
		WRITE_BYTE(0);
		WRITE_COORD(0);
		WRITE_COORD(0);
		WRITE_COORD(0);
		WRITE_COORD(0);
		WRITE_COORD(0);
		WRITE_COORD(0);
		WRITE_SHORT(9999);
		WRITE_STRING("");
	MESSAGE_END();

	if(bResetOnlyCustom == false) {
		// - breakables
		CBreakable * pBreakable = NULL;
		pBreakable = (CBreakable*)UTIL_FindEntityByClassname( pBreakable, "func_breakable" );

		while (pBreakable) { 
			pBreakable->Reset();
			pBreakable = (CBreakable*)UTIL_FindEntityByClassname( pBreakable, "func_breakable" );
		}

		// - doors
		CBaseDoor *pDoor = NULL;
		pDoor = (CBaseDoor*)UTIL_FindEntityByClassname( pBreakable, "func_door" );

		while (pBreakable) { 
			pDoor->Spawn();
			pDoor->SUB_UseTargets(pDoor, USE_OFF, 0.0);
			pDoor = (CBaseDoor*)UTIL_FindEntityByClassname( pDoor, "func_door" );
			
		}



		// - points
		pPointManager->ResetAllPoints();
		pPointManager->UpdatePlayersHUD();

		// - particles
		CParticleEmitter *pEmitter = NULL;
		pEmitter = (CParticleEmitter*)UTIL_FindEntityByClassname( pEmitter, "env_particleemitter" );
		while(pEmitter) {
			pEmitter->Use(NULL, NULL, USE_OFF, 0.0);
			if(pEmitter->pev->spawnflags & SF_START_ON) {
				pEmitter->bIsOn = true;
			} else {
				pEmitter->bIsOn = false;
			}
			pEmitter = (CParticleEmitter*)UTIL_FindEntityByClassname( pEmitter, "env_particleemitter" );
		}

		// - grass
		CGrass *pGrass = NULL;
		pGrass = (CGrass*)UTIL_FindEntityByClassname( pGrass, "func_grass" );
		while(pGrass) {
			pGrass->Use(NULL, NULL, USE_OFF, 0.0);
			pGrass->bIsOn = true;
			pGrass = (CGrass*)UTIL_FindEntityByClassname( pGrass, "func_grass" );
		}

		// - spawn containers
		g_pBGRules->InitSpawns();
	}

	if(bResetCustom == true) {
		g_pBGRules->InitSpawns();

		// -reseter ent
		CBaseEntity *pResetEnt = NULL;
		pResetEnt = (CBaseEntity*)UTIL_FindEntityByClassname( pResetEnt, "info_reset" );

		// -this allows us to reset the ents the mapper wants
		// -ben : I didn't want to do this but this could solve some bh probs
		CountPlayersOnTeam(true);
		if(GetNumOfBritishPlayers() <= 0 || GetNumOfAmericanPlayers() <= 0)
			return;

		while (pResetEnt) { 
			pResetEnt->ResetEnt();
			pResetEnt = (CBaseEntity*)UTIL_FindEntityByClassname( pResetEnt, "info_reset" );
		}
	}
}

// Ben - EOF
