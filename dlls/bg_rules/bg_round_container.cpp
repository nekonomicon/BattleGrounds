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

//This file was created on the 10/3/03 at 21:27 by Ben Banfield for the Battle Grounds modification of Half-Life
//Its purpose is to define the round rules container class

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"player.h"

#include	"game.h"
#include	"bg_round.h"

// we want a bg rules pointer for some of the classes, but not all which is why we keep a member copy, rather than usuing the global
class CBaseBgRules;
extern CBaseBgRules *g_pBGRules;

extern float flOurTimeDifference;

void exec_map_config()
{
	char map_config[512];
	sprintf(map_config, "exec maps/%s.cfg\n", STRING(gpGlobals->mapname));
	ALERT(at_logged, "EXECUTED maps/%s.cfg\n", STRING(gpGlobals->mapname));
	SERVER_COMMAND(map_config);
}

// setup all variables up to default state
CRoundRulesContainer::CRoundRulesContainer()
{
	m_flLastRoundUpdate = gpGlobals->time;
	m_flRestartRoundTime = 0.0;

	// make sure we have a valid bg rules pointer for our round rules to force certain events
	if(g_pBGRules)
		pBgRules = g_pBGRules;
	else
		pBgRules = NULL;
	
	// - turn the round rules on
	pRoundRules = NULL;
	exec_map_config();
	StartRoundRules();

	bSwitchingRespawnStyle = false;
}

// clean up all variables
CRoundRulesContainer::~CRoundRulesContainer()
{
	m_flLastRoundUpdate = 0.0;
	m_flRestartRoundTime = 0.0;
	bSwitchingRespawnStyle = false;

	pBgRules = NULL;

	delete pRoundRules;
	pRoundRules = NULL;
}

// Ben - Start of Round Helper Functions

// do low level round updating, like chekcing for mode changes and full restarts
void CRoundRulesContainer::RoundThink()
{
	// - check bg rules point validity and set it up again if need be
	if(pBgRules == NULL)
	{
		if(g_pBGRules)
			pBgRules = g_pBGRules;
	}

	// - check round rules validity and set it up again if need be
	if(pRoundRules == NULL)
		StartRoundRules();

	// mode change check
	if(((int)CVAR_GET_FLOAT("mp_respawnstyle") != (int)pRoundRules->GetRespawnStyle()) &&
		bSwitchingRespawnStyle == false)
	{
		// - force RR if it has changed
		m_flRestartRoundTime = gpGlobals->time + 1.0;

		// - alert clients of RR (we know the time till RR this time)
		char szRR[256];
		sprintf(szRR, "The Game will restart in 1 second\n\0");
		UTIL_ClientPrintAll(HUD_PRINTCENTER, szRR);
		UTIL_LogPrintf(ROUND_LOGGING, "World triggered \"Round Restart\" (time \"1\")\n");

		bSwitchingRespawnStyle = true;
		// - no need to return as that will be done soon
	}
	else {}

	// restart time check
	// - check for any qued rr
	if(m_flRestartRoundTime > 0.0)
	{
		if(m_flRestartRoundTime < gpGlobals->time)
		{
			// -- switch to the new respawn style
			if(bSwitchingRespawnStyle == true)
				StartRoundRules();

			// -- round restart please
			pRoundRules->FullRestart(true);
			m_flRestartRoundTime = 0.0;

			// -- reset the time limit
			flOurTimeDifference += (CVAR_GET_FLOAT("mp_timelimit") * 60) - CVAR_GET_FLOAT("mp_timeleft");

			// -- ben notes: we will return after this function, so an abort is not needed
		}
		// - already has rr qued up so abort
		return;
	}

	// - sv_restartround check
	if(CVAR_GET_FLOAT("sv_restartround") > 0.0)
	{
		// -- save rr time
		m_flRestartRoundTime = gpGlobals->time + CVAR_GET_FLOAT("sv_restartround");

		// -- clear sv_restartround now that it has been saved
		CVAR_SET_FLOAT("sv_restartround", 0);

		// -- alert clients of RR
		char szRR[256];
		sprintf(szRR, "The Game will restart in %1.f seconds\n\0", (m_flRestartRoundTime - gpGlobals->time));
		UTIL_ClientPrintAll(HUD_PRINTCENTER, szRR);

		UTIL_LogPrintf(ROUND_LOGGING, "World triggered \"Round Restart\" (time \"%1.f\")\n", (m_flRestartRoundTime - gpGlobals->time));
		
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
		
		return;
	}

	// - check there are players
	pRoundRules->CountPlayersOnTeamFuture(true);
	if((pRoundRules->GetNumOfBritishPlayers() + pRoundRules->GetNumOfAmericanPlayers()) <= UNDEFINED && (pRoundRules->GetRoundState() != ROUND_NOPLAYERS))
	{
		pRoundRules->SetNoRound();
		return;
	}
	if(pRoundRules->GetRoundState() == ROUND_NOPLAYERS)
	{
		if((pRoundRules->GetNumOfBritishPlayers() + pRoundRules->GetNumOfAmericanPlayers()) <= UNDEFINED)
		{
			return;
		}
		else
		{
			pRoundRules->SetRoundBegin();
			return;
		}
	}

	// check time since last update, so that we don't kill the cpu
	// - abort the update if we are too close to the last update
	if(m_flLastRoundUpdate + ROUND_UPDATE_TIME > gpGlobals->time)
		return;

	// - update round update time if needed
	else
		m_flLastRoundUpdate = gpGlobals->time;

	// - no rr so business as usual
	pRoundRules->RoundThink();
}

// - creates the round rules
void CRoundRulesContainer::StartRoundRules()
{
	// - kill current round rules
	if(pRoundRules)
		delete pRoundRules;

	pRoundRules = NULL;

	// - start up a round rule
	int iRespawnStyle = (int)CVAR_GET_FLOAT("mp_respawnstyle");
	switch(iRespawnStyle)
	{
		case (int)WAVE_RESPAWN:
		{
			pRoundRules = new CRespawnWaveRules;
			break;
		}
		case (int)ROUND_RESPAWN:
		{
			pRoundRules = new CRoundRules;
			break;
		}
		default:
		{
			pRoundRules = new CRespawnWaveRules;

			// - set the cvar
			CVAR_SET_FLOAT("mp_respawnstyle", WAVE_RESPAWN);
			break;
		}
	}
	// - set a preliminary state
	pRoundRules->SetRoundState(ROUND_NOTSTARTED);

	// - stops us from killing the respawn style next full restart
	bSwitchingRespawnStyle = false;
}
