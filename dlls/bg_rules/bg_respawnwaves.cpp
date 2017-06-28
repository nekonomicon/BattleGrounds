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

//This file was created on the 8/3/03 at 15:30 by Ben Banfield for the Battle Grounds modification of Half-Life
//Its purpose is to define the respawn wave class

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"player.h"

#include	"game.h"
#include	"bg_round.h"

// setup all variables for the round
CRespawnWaveRules::CRespawnWaveRules()
{
	SetRespawnStyle(WAVE_RESPAWN);
	SetRoundTimeLimit(RESPAWN_WAVE_ROUNDTIME_LIMIT);
	SetRestartTimeLimit(ROUND_RULES_RESTARTTIME_LIMIT);
	bFullRestart = false;
}

// kill everything for the round
CRespawnWaveRules::~CRespawnWaveRules()
{
}

//updates the round's progress
void CRespawnWaveRules::RoundThink()
{
	// the round rules container handles full round restarts and mode changes

	switch(GetRoundState())
	{
		// round preperation
		case ROUND_NOTSTARTED:
		{
			if(GetRestartTime() <= gpGlobals->time) {
				// make a full restart if it was called for
				if(bFullRestart == true) {
					FullRestart(false, false);
					bFullRestart = false;
					break; // lets give everyone a frame to spawn
				}

				// actually start a round
				if(CheckForRoundStart())
				{
					StartRound();
				}
				else
				{
					StartDummyRound();
				}
			}
			break;
		}
		// players on both teams
		case ROUND_RUNNING:
		{
			CheckForRoundEnd();
			break;
		}
		// only players on one team
		case ROUND_WAITING_FOR_PLAYERS:
		{
			if(!CheckForRoundStart())
			{
				CheckForRoundEnd();
			}
			break;
		}
		// someone won
		case ROUND_RESTARTING:
		{
			AwardWinners();
			SetRestartLength(RESTART_TIME);
			RestartRound();
			ResetWorld(true, true);
			bFullRestart = true;

			if( (int)CVAR_GET_FLOAT("mp_respawnstyle") == ROUND_RESPAWN)
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
			}
			break;
		}
		// normal respawn EOR (end of round)
		case ROUND_ENDING:
		{
			RespawnPlayers();
			RestartRound();
			SetRestartLength(0);
			break;
		}
		default:
		{
			int iRoundState = GetRoundState();
			ALERT(at_console, "Bad Round State in RespawnWave (%i)\n", iRoundState);
			UTIL_LogPrintf("World triggered \"Bad Round State in Respawn Wave\" (round_state \"%i\") (respawn_style \"%i\")\n", iRoundState, (int)GetRespawnStyle());
			break;
		}
	}
}

// - checks for win conditions
bool CRespawnWaveRules::CheckForRoundEnd()
{
	// - checks for time up
	if(GetRoundTime() <= gpGlobals->time)
	{
		// -- move to restarting state if time up
		SetRoundState(ROUND_ENDING, false);
		return true;
	}
	// - check for other win conditions like all points captured
	bool bOtherConditions = HandleMiscWinConditions();

	if(bOtherConditions == true)
	{
		// - reset the world on other win conditions
		// ben notes : the true here means SetRoundState won't call our think function
		// ben notes : effectively this means a delay between the capturing of all points
		// ben notes : and the round restarting, which is a good thing
		SetRoundState(ROUND_RESTARTING);
		return true;
	}
	return false;
}

// - respawns all dea
void CRespawnWaveRules::RespawnPlayers()
{
	for(iCount = 0; iCount <= gpGlobals->maxClients; iCount++)
	{
		CBasePlayer *pPlayer = (CBasePlayer*)UTIL_PlayerByIndex(iCount);

		if(pPlayer)
		{
			if(pPlayer->m_iDead == DEAD)
			{
				if(pPlayer->m_iTeamRespawningAs != UNDEFINED && pPlayer->m_iClassRespawningAs != UNDEFINED)
				{
					if(pPlayer->m_iTeamRespawningAs != SPECS)
						RespawnPlayer(pPlayer);
				}
			}
		}
	}
}

// -Prepares players for the forcoming round
void CRespawnWaveRules::RestartRound()
{
	// - unfreeze all players
	for(iCount = 0; iCount <= gpGlobals->maxClients; iCount++)
	{
		CBasePlayer *pPlayer = (CBasePlayer*)UTIL_PlayerByIndex(iCount);

		if(pPlayer)
		{
			pPlayer->ResetSpeed();
		}
	}

	// - reset the round time clock ready for the next round
	SetRoundTimeOver();

	// - move to round start
	SetRoundState(ROUND_NOTSTARTED, false);
}

// Ben - EOF
