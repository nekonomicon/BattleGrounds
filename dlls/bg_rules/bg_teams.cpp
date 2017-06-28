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

//This file was created on the 1/12/02 at 16:23 by Ben Banfield for the Battle Grounds modification of Half-life
//Its purpose is to contain all functions from the CBaseBgRules class relating to teams and classes

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"player.h"
#include	"bg_class.h"
#include	"weapons.h"
#include	"gamerules.h"
#include	"game.h"
#include	"bg_rules.h"
#include	"voice_gamemgr.h"
#include	"func_break.h"
#include	"particle_emitter.h"
#include	"func_grass.h"
#include	"pm_shared.h"
//#include	"commandersmenu.h"

#include	"../../cl_dll/message_defs.h"

extern void CopyToBodyQue(entvars_t *pev);
extern int gmsgPredefinedMessage;
extern int gmsgHintbox;

//Funcs to handle the players class.
void HandleBritishClasses(CBasePlayer *pPlayer, int iClassNum);
void HandleAmericanClasses(CBasePlayer *pPlayer, int iClassNum);

//funcs to handle the players team
void HandleBritishTeam(CBasePlayer *pPlayer);
void HandleAmericanTeam(CBasePlayer *pPlayer);
void HandleSpectating(CBasePlayer *pPlayer, CBaseBgRules *pBg);

//checks whether the player has changed sides/classes or not
enum
{
	SAME = 0,
	CHANGED
};

enum
{
	FIRST_SPAWN = 0,
	ANOTHER_SPAWN
};

//checking functions
int CheckHasChangedTeam(CBasePlayer *pPlayer, int iTeamSpawningAs);
int CheckHasSwitchedOrIsFirstSpawn(CBasePlayer *pPlayer);
int CheckHasChangedClasses(CBasePlayer *pPlayer, int iClassSpawningAs);
bool CheckCanJoinTeam(CBasePlayer *pPlayer, int iTeamSpawningAs);
bool CheckCanJoinClass(CBasePlayer *pPlayer, int iClassSpawningAs);

//functions which deal with the consequences of the playing changing teams/class
void ResetTeamVariables(CBasePlayer *pPlayer);
void ResetClassVariables(CBasePlayer *pPlayer);
void Kill(CBasePlayer *pPlayer, int iDamage);

const int iKill = 101;
const int iGib = 180;

//reset that team variables to "clean" values
void ResetTeamVariables(CBasePlayer *pPlayer)
{
	pPlayer->m_iTeamRespawningAs = pPlayer->pev->team = UNDEFINED;
	sprintf(pPlayer->m_szTeamName, "\0");
}

//reset the class variables to "clean" values
void ResetClassVariables(CBasePlayer *pPlayer)
{
	pPlayer->m_iClassRespawningAs = pPlayer->pev->playerclass = UNDEFINED;
	sprintf(pPlayer->m_szTeamName, "\0");
}

//kill the player
void Kill(CBasePlayer *pPlayer, int iDamage)
{
	pPlayer->TakeDamage(VARS(ENTINDEX(0)), pPlayer->pev, iDamage, 0);
}

//check whether this is the players first spawn or not. this will also check whether the player has already been killed for his actions
int CheckHasSwitchedOrIsFirstSpawn(CBasePlayer *pPlayer )
{
	if(pPlayer->m_bHasSpawnedBefore == true)
	{
		return ANOTHER_SPAWN;
	}
	return FIRST_SPAWN;
}

//check whether we have changed teams
int CheckHasChangedTeam(CBasePlayer *pPlayer, int iTeamSpawningAs)
{
	if(CheckHasSwitchedOrIsFirstSpawn(pPlayer) == ANOTHER_SPAWN)
	{
		if(pPlayer->pev->team != iTeamSpawningAs)
		{
			return CHANGED;
		}
		return SAME;
	}
	//has never spawned
	else
	{
		return SAME;
	}
}

//check whether we have changed classes between respawns
int CheckHasChangedClasses(CBasePlayer *pPlayer, int iClassSpawningAs)
{
	//if we have spawned before
	if(CheckHasSwitchedOrIsFirstSpawn(pPlayer) == ANOTHER_SPAWN)
	{
		//& the class isn't the same as last spawn
		if(pPlayer->pev->playerclass != iClassSpawningAs)
		{
			//we have changed :(
			return CHANGED;
		}
		//otherwise we are in the same old boring job
		return SAME;
	}
	//however if this is our first spawn
	else
	{
		return SAME;
	}
}

//checks the number of players on a team and makes sure a team change won't unbalance them
bool CheckCanJoinTeam(CBasePlayer *pPlayer, int iTeam)
{
	int iBritish = 0;
	int iAmerican = 0;
	int iSpectator = 0;
	int i = 0;

	for(; i <= gpGlobals->maxClients; i++)
	{
		CBasePlayer *player = (CBasePlayer*)UTIL_PlayerByIndex(i);
		if(player)
		{
			if(player->pev->team == BRITS)
			{
				iBritish++;
			}
			else if(player->pev->team == AMERS)
			{
				iAmerican++;
			}
			else
			{
				if(player->m_iTeamRespawningAs == BRITS)
				{
					iBritish++;
				}
				else if(player->m_iTeamRespawningAs == AMERS)
				{
					iAmerican++;
				}
				else
				{
					iSpectator++;
				}
			}
		}
	}

	// don't include ourself in the counting
	if(pPlayer->pev->team == BRITS)
		iBritish -= 1;

	else if(pPlayer->pev->team == AMERS)
		iAmerican -= 1;

	else{}

	//if the team the player is trying to join already has more players
	//then there will be 2 more if the player joins and we can't allow that
	if(iTeam == BRITS)
	{
		if(iBritish >= iAmerican + (int)CVAR_GET_FLOAT("mp_limitteams"))
		{
			ShowVGUI(pPlayer, MENU_TEAM);
			ClientPrint(pPlayer->pev, HUD_PRINTCENTER, "Too many players on this team");
			return false;
		}
		else
			return true;
	}
	else if(iTeam == AMERS)
	{
		if(iAmerican >= iBritish + (int)CVAR_GET_FLOAT("mp_limitteams"))
		{
			ShowVGUI(pPlayer, MENU_TEAM);
			UTIL_SayText("Too many players on this team", pPlayer);
			return false;
		}
		else
			return true;
	}

	//let the server admins decide whether we allow spectators
	else
	{
		if(CVAR_GET_FLOAT("allow_spectators") < 1.0)
			return false;
		else
			return true;
	}
}

// determines whether a player can join this class
bool CheckCanJoinClass(CBasePlayer *pPlayer, int iClassNum)
{
	// spectators joining classes? what will they think of next?
	if(pPlayer->m_iTeamRespawningAs == SPECS)
		return false;

	char szCvar[256];
	sprintf(szCvar, "mp_limit_%s", szClassCodes[pPlayer->m_iTeamRespawningAs][iClassNum]);

	int iCvar = (int)CVAR_GET_FLOAT(szCvar);
	if(iCvar == -1)
		return true;

	int i = 0;
	int iPlayers = 0;

	// -count players who are taking up this limit
	for(; i <= gpGlobals->maxClients; i++)
	{
		CBasePlayer *player = (CBasePlayer*)UTIL_PlayerByIndex(i);
		if(player && player != pPlayer)
		{
			// -count players who already have slots as this class or 
			if(player->m_iTeamRespawningAs == pPlayer->m_iTeamRespawningAs)
			{
				if(player->m_iClassRespawningAs == iClassNum)
				{
					iPlayers++;
					continue;
				}
			}
			// -counts players who are this class already
			if(player->pev->team == pPlayer->m_iTeamRespawningAs)
			{
				if(player->pev->playerclass == iClassNum)
				{
					iPlayers++;
					continue;
				}
			}
		}
	}
	// -more or the same number of players as the cvar allows so say no
	if(iCvar <= iPlayers)
	{
		// -resend vgui menu
		if(pPlayer->m_iTeamRespawningAs == BRITS)
			ShowVGUI(pPlayer, MENU_BRITISH_CLASS);
		else if(pPlayer->m_iTeamRespawningAs == AMERS)
			ShowVGUI(pPlayer, MENU_AMERICAN_CLASS);
		else
			ShowVGUI(pPlayer, MENU_TEAM);

		// -tell the player this class is out of bounds
		if(iCvar == 0)
			UTIL_SayText("This class is disabled at present", pPlayer);
		else
			UTIL_SayText("Too many players in this class", pPlayer);
		

		// -return from this disallowing the handling of this class to continue
		return false;
	}
	return true;
}

//Ben - After a team has been joined handle the next stage
void CBaseBgRules::HandleTeams(CBasePlayer *pPlayer, int iTeamNum)
{
	//check whether the team number is possible
	if(iTeamNum <= UNDEFINED || iTeamNum > NUM_TEAMS)
	{
		//todo: tell the user this
		ALERT(at_console, "Bad user input on team choice\n");
		return;
	}

	//exit the function if we are a spec and we are trying to join it again
	if(pPlayer->pev->team == SPECS && iTeamNum == SPECS)
		return;

	//check whether we want team balancing
	if(CVAR_GET_FLOAT("mp_autoteambalance") > 0)
	{
		if(CheckCanJoinTeam(pPlayer, iTeamNum) == false)
			return;
	}

	// don't save to team respawning as until we can be sure about the class
	pPlayer->pev->iuser4 = iTeamNum;

	// kill anyone changing team
	if(CheckHasChangedTeam(pPlayer, iTeamNum) == CHANGED)
	{
		// reset the class if the player was just a spectator so that they can't get around our checks on classes
		if(pPlayer->pev->team == SPECS)
			pPlayer->m_iClassRespawningAs = UNDEFINED;

		// dispaly the appropriate team meny
		if(iTeamNum == BRITS)
		{
			ShowVGUI(pPlayer, MENU_BRITISH_CLASS);
		}
		else if(iTeamNum == AMERS)
		{
			ShowVGUI(pPlayer, MENU_AMERICAN_CLASS);
		}
		else
		{
			MESSAGE_BEGIN(MSG_ALL, gmsgPredefinedMessage);

			WRITE_SHORT(iTeamClass);
			WRITE_SHORT(pPlayer->entindex());
			WRITE_SHORT(SPECS);
			WRITE_SHORT(0);
			
			MESSAGE_END();

			HandleSpectating(pPlayer, this);
		}
		return;
	}

	//deal with the british
	if(iTeamNum == BRITS)
	{
		ShowVGUI(pPlayer, MENU_BRITISH_CLASS);
	}
	//deal with the americans
	else if(iTeamNum == AMERS)
	{
		ShowVGUI(pPlayer, MENU_AMERICAN_CLASS);
	}
	//deal with the spectators
	else
	{
		MESSAGE_BEGIN(MSG_ALL, gmsgPredefinedMessage);

		WRITE_SHORT(iTeamClass);
		WRITE_SHORT(pPlayer->entindex());
		WRITE_SHORT(SPECS);
		WRITE_SHORT(0);
		
		MESSAGE_END();

		HandleSpectating(pPlayer, this);
	}
}

// slightly deciving name i think. It actually handles what class we will respawn as
void CBaseBgRules::HandleClasses(CBasePlayer *pPlayer, int iClassNum)
{
	//check whether the number given to us is out of the realms of possibility
	if(iClassNum <= UNDEFINED || iClassNum > NUM_CLASSES)
	{
		ALERT(at_console, "Bad Class Num, trying to be handled\n");
		// maybe we should tell user we have an invalid class
		return;
	}

	int iTemp = pPlayer->m_iTeamRespawningAs;
	pPlayer->m_iTeamRespawningAs = pPlayer->pev->iuser4;
	//checks whether we can join this class. if we can't then this func will send the menus again
	if(CheckCanJoinClass(pPlayer, iClassNum) == false) {
		pPlayer->m_iTeamRespawningAs = iTemp;
		return;
	}

	bool bChanged = false;
	bool bKill = false;

	// we must have changed something if we have changed team
	if(CheckHasChangedTeam(pPlayer, pPlayer->m_iTeamRespawningAs) == CHANGED)
	{
		bChanged = true;
		bKill = true;
	}
	else
	{
		// changed classes on the same team
		if(CheckHasChangedClasses(pPlayer, iClassNum) == CHANGED)
		{
			bChanged = true;
			pPlayer->m_iClassRespawningAs = iClassNum;
		}
		// people spawning for the first time will get this
		else
		{
			pPlayer->m_iClassRespawningAs = iClassNum;
		}
	}

	// we are to kill them
	if(bChanged == true)
	{
		// tell the player what they will repsawn as
		MESSAGE_BEGIN(MSG_ONE, gmsgPredefinedMessage, NULL, pPlayer->pev);

		WRITE_SHORT(iTeamClassMe);
		WRITE_SHORT(pPlayer->m_iTeamRespawningAs);
		WRITE_SHORT(iClassNum);
		
		MESSAGE_END();

		if(bKill == true)
		{
			pPlayer->m_iClassRespawningAs = iClassNum;
			Kill(pPlayer, iKill);
		}
	}

	if(CheckHasSwitchedOrIsFirstSpawn(pPlayer) == FIRST_SPAWN || bKill == true)
	{
		MESSAGE_BEGIN(MSG_ALL, gmsgPredefinedMessage);

		WRITE_SHORT(iTeamClass);
		WRITE_SHORT(pPlayer->entindex());
		WRITE_SHORT(pPlayer->m_iTeamRespawningAs);
		WRITE_SHORT(pPlayer->m_iClassRespawningAs);
		
		MESSAGE_END();
	}

	UpdateMyScores(pPlayer);
	
	if(pPlayer->pev->team == UNDEFINED)
	{
		pPlayer->StartObserver(pPlayer->pev->origin, pPlayer->pev->angles);
	}
}

// probably the most important routine in gamerules section
// deals with players spawning
void CBaseBgRules::Respawn(CBasePlayer *pPlayer, int iTeamRespawningAs, int iClassRespawningAs)
{
	if(!pPlayer->m_bIsAllowedToRespawn)
		return;

	if((iTeamRespawningAs <= UNDEFINED) || (iTeamRespawningAs > NUM_TEAMS)) {
		ALERT(at_console, "Serious mess up. TeamRespawningAs is invalid\n");
		return;
	}

	//check whether the number given to us is out of the realms of possibility
	if(iClassRespawningAs <= UNDEFINED || iClassRespawningAs > NUM_CLASSES) {
		ALERT(at_console, "Bad Class Num, trying to be handled\n");
		// maybe we should tell user we have an invalid class
		return;
	}

	char szTeamLog[256];
	char szClassLog[256];
	
	
	// changed team and/or class
	if(CheckHasChangedTeam(pPlayer, iTeamRespawningAs) == CHANGED)
	{
		// format "Name<unique id><auth/wonid><team>" joined team "x"
		sprintf(szTeamLog, "\"%s<%i><%s><%s>\" joined team \"%s\"\n", 
				STRING( pPlayer->pev->netname ), 
				GETPLAYERUSERID( pPlayer->edict() ),
				GETPLAYERAUTHID( pPlayer->edict() ),
				szTeamNames[pPlayer->pev->team],
				szTeamNames[iTeamRespawningAs]);

		// format "Name<unique id><auth/wonid><team>" changed role to "x"
		sprintf(szClassLog, "\"%s<%i><%s><%s>\" changed role to \"%s\"\n", 
				STRING( pPlayer->pev->netname ), 
				GETPLAYERUSERID( pPlayer->edict() ),
				GETPLAYERAUTHID( pPlayer->edict() ),
				szTeamNames[iTeamRespawningAs],
				szClassNames[iTeamRespawningAs][iClassRespawningAs]);

		UTIL_LogPrintf(szTeamLog);
		UTIL_LogPrintf(szClassLog);
	}
	// stayed on same team
	else
	{
		// changed class though
		if(CheckHasChangedClasses(pPlayer, iClassRespawningAs) == CHANGED)
		{
			// format "Name<unique id><auth/wonid><team>" changed role to "x"
			sprintf(szClassLog, "\"%s<%i><%s><%s>\" changed role to \"%s\"\n", 
					STRING( pPlayer->pev->netname ), 
					GETPLAYERUSERID( pPlayer->edict() ),
					GETPLAYERAUTHID( pPlayer->edict() ),
					szTeamNames[iTeamRespawningAs],
					szClassNames[iTeamRespawningAs][iClassRespawningAs]);

			UTIL_LogPrintf(szClassLog);
		}
	}

	// first spawn
	if(pPlayer->m_bHasSpawnedBefore == false)
	{
		// format "Name<unique id><auth/wonid><team>" joined team "x"
		sprintf(szTeamLog, "\"%s<%i><%s><-1>\" joined team \"%s\"\n", 
				STRING( pPlayer->pev->netname ), 
				GETPLAYERUSERID( pPlayer->edict() ),
				GETPLAYERAUTHID( pPlayer->edict() ),
				szTeamNames[iTeamRespawningAs]);

		// format "Name<unique id><auth/wonid><team>" changed role to "x"
		sprintf(szClassLog, "\"%s<%i><%s><%s>\" changed role to \"%s\"\n", 
				STRING( pPlayer->pev->netname ), 
				GETPLAYERUSERID( pPlayer->edict() ),
				GETPLAYERAUTHID( pPlayer->edict() ),
				szTeamNames[iTeamRespawningAs],
				szClassNames[iTeamRespawningAs][iClassRespawningAs]);

		UTIL_LogPrintf(szTeamLog);
		UTIL_LogPrintf(szClassLog);
	}

	if(iTeamRespawningAs == UNDEFINED)
	{
		iTeamRespawningAs = pPlayer->pev->team;
		iClassRespawningAs = pPlayer->pev->playerclass;
	}
	if(iTeamRespawningAs == BRITS)
	{
		HandleBritishTeam(pPlayer);
		HandleBritishClasses(pPlayer, iClassRespawningAs);
	}
	else if(iTeamRespawningAs == AMERS)
	{
		HandleAmericanTeam(pPlayer);
		HandleAmericanClasses(pPlayer, iClassRespawningAs);
	}
	else
	{
		HandleSpectating(pPlayer, this);
	}

	UpdateMyScores(pPlayer);

	// particles can only happen after the player has spawned
	if(pPlayer->m_bHasSpawnedBefore == false) {
		pPlayer->m_bSpawnPS = true;
		pPlayer->m_bSpawnGrass = true;

	}

	// we have now spawned
	pPlayer->m_bHasSpawnedBefore = true;
	pPlayer->pev->fuser3 = pPlayer->pev->health = MAX_HEALTH;
	
	// BP Display Class statistics Hintbox
	MESSAGE_BEGIN( MSG_ONE, gmsgHintbox, NULL, pPlayer->pev );
        WRITE_SHORT(HINTBOX_CLASS_LIGHTA); // generic class msg
		WRITE_SHORT(iTeamRespawningAs);
		WRITE_SHORT(iClassRespawningAs);	
    MESSAGE_END();
}

// sets all variables in relation to joining the british team
void HandleBritishTeam(CBasePlayer *pPlayer)
{
	//Our Team Name
	sprintf(pPlayer->m_szTeamName, "%s\n", szTeamNames[BRITS]);

	//Sends the team to the client dll
	pPlayer->pev->team = pPlayer->m_iTeamRespawningAs = BRITS;

	pPlayer->pev->iuser1 = 0;
	pPlayer->m_iSpectatorMode = 0;
	pPlayer->pev->iuser2 = 0;
	pPlayer->pev->iuser3 = 0;
	pPlayer->m_hObserverTarget = NULL;

	g_pBGRules->GetPlayerSpawnSpot(pPlayer);
}

//handles becomming an american
void HandleAmericanTeam(CBasePlayer *pPlayer)
{
	//Our team name
	sprintf(pPlayer->m_szTeamName, "%s\n", szTeamNames[AMERS]);

	//give our team the correct numbers
	pPlayer->pev->team = pPlayer->m_iTeamRespawningAs = AMERS;

	pPlayer->pev->iuser1 = 0;
	pPlayer->m_iSpectatorMode = 0;
	pPlayer->pev->iuser2 = 0;
	pPlayer->pev->iuser3 = 0;
	pPlayer->m_hObserverTarget = NULL;

	g_pBGRules->GetPlayerSpawnSpot(pPlayer);
}

// handles becoming a spectator
void HandleSpectating(CBasePlayer *pPlayer, CBaseBgRules *pBg)
{
	// log this
	char szTeamLog[512];

	// format "Name<unique id><auth/wonid><team>" joined team "x"
	sprintf(szTeamLog, "\"%s<%i><%s><-1>\" joined team \"%s\"\n", 
			STRING( pPlayer->pev->netname ), 
			GETPLAYERUSERID( pPlayer->edict() ),
			GETPLAYERAUTHID( pPlayer->edict() ),
			szTeamNames[SPECS]
			);

	UTIL_LogPrintf(szTeamLog);

	// reset the varaibles, so we can have a smooth respawn next time
	ResetTeamVariables(pPlayer);
	ResetClassVariables(pPlayer);

	//Our Team Name
	sprintf(pPlayer->m_szTeamName, "%s\n", szTeamNames[SPECS]);

	//Sends the team to the client dll
	pPlayer->pev->team = pPlayer->m_iTeamRespawningAs = SPECS;

	pPlayer->TakeDamage(VARS(INDEXENT(0)), VARS(INDEXENT(0)), iKill, DMG_GENERIC);
	pPlayer->StopObserver();

	g_pBGRules->GetPlayerSpawnSpot(pPlayer);
	pPlayer->Observer_FindNextPlayer(false);
	pPlayer->StartObserver(pPlayer->pev->origin, pPlayer->pev->v_angle);
	pPlayer->Observer_SetMode(OBS_CHASE_LOCKED);

	pBg->UpdateMyScores(pPlayer);
}

//Giving the player a class
void HandleBritishClasses(CBasePlayer *pPlayer, int iClassNum)
{
	if(iClassNum == LIGHT) 
		pPlayer->pClass = new CLight_B(pPlayer);
	else if(iClassNum == MEDIUM)
		pPlayer->pClass = new CMedium_B(pPlayer);
	else if(iClassNum == HEAVY)
		pPlayer->pClass = new CHeavy_B(pPlayer);
	else {
		ShowVGUI(pPlayer, MENU_BRITISH_CLASS);
		return;
	}
}

void HandleAmericanClasses(CBasePlayer *pPlayer, int iClassNum)
{
	if(iClassNum == LIGHT)
		pPlayer->pClass = new CLight_A(pPlayer);
	else if(iClassNum == MEDIUM)
		pPlayer->pClass = new CMedium_A(pPlayer);
	else if(iClassNum == HEAVY)
		pPlayer->pClass = new CHeavy_A(pPlayer);
	else {
		ShowVGUI(pPlayer, MENU_AMERICAN_CLASS);
		return;
	}
}
