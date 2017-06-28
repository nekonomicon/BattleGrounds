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
	
//This file was created on the 23/3/03 at 18:49 by Ben Banfield for the Battle Grounds modification of Half-Life
//Its purpose is to define the round rules class

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"player.h"

#include	"game.h"
#include	"bg_round.h"

// - create the round rules
CRoundRules::CRoundRules()
{
	SetRespawnStyle(ROUND_RESPAWN);
	SetRoundTimeLimit(ROUND_RULES_ROUNDTIME_LIMIT);
	SetPauseTimeLimit(ROUND_RULES_PAUSETIME_LIMIT);
	SetRestartTimeLimit(ROUND_RULES_RESTARTTIME_LIMIT);

	bFullRestart = false;
}

// update the round's state
void CRoundRules::RoundThink()
{
	// the round rules container handles full round restarts and mode changes

	switch(GetRoundState())
	{
		// round preperation
		case ROUND_NOTSTARTED:
		{
			if(GetRestartTime() <= gpGlobals->time) {
				if(bFullRestart == true) {
					FullRestart(false, false);
					bFullRestart = false;
				}
				PauseTime();
			}
			break;
		}
		// start the round
		case ROUND_PAUSED:
		{
			// wait until the pause time is over before starting the rounds
			if(GetRoundTime() <= gpGlobals->time)
			{
				if(CheckForRoundStart())
				{
					PrepareRound();				
					StartRound();
				}
				else 
				{
					PrepareRound();
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
		// someone one or sv_resartround
		case ROUND_RESTARTING:
		{
			AwardWinners();
			RestartRound();
			ResetWorld(true, true);
			break;
		}
		default:
		{
			int iRoundState = GetRoundState();
			ALERT(at_console, "Bad Round State in RoundRules (%i)\n", iRoundState);
			UTIL_LogPrintf("World triggered \"Bad Round State in RoundRules\" (round_state \"%i\") (respawn_style \"%i\")\n", iRoundState, (int)GetRespawnStyle());
			break;
		}
	}
}

// - pauses everyone
void CRoundRules::PauseTime()
{
	// - unfreeze all players
	for(iCount = 0; iCount <= gpGlobals->maxClients; iCount++)
	{
		CBasePlayer *pPlayer = (CBasePlayer*)UTIL_PlayerByIndex(iCount);

		if(pPlayer)
		{
			//prevent spectators from being frozen
			if(pPlayer->m_iTeamRespawningAs == SPECS)
				continue;

			g_engfuncs.pfnSetClientMaxspeed( ENT(pPlayer->pev), 1);
		}
	}
	// - send the pause time to the clients
	// - Ben notes: check round restart won't return true until we have passed this time
	SetRoundLength(ROUND_PAUSETIME, true);
	SetRoundState(ROUND_PAUSED);
}

// - makes the round ready to play
void CRoundRules::PrepareRound()
{
	// - unfreeze all players
	for(iCount = 0; iCount <= gpGlobals->maxClients; iCount++)
	{
		CBasePlayer *pPlayer = (CBasePlayer*)UTIL_PlayerByIndex(iCount);

		if(pPlayer)
		{
			//prevent spectators from being frozen
			if(pPlayer->m_iTeamRespawningAs == SPECS)
				continue;

			pPlayer->ResetSpeed();
		}
	}
}

// - checks for win conditions
bool CRoundRules::CheckForRoundEnd()
{
	// - check whether all players on one team have died
	// - (counting only those players who are alive)
	CountPlayersOnTeam(false);

	if(GetRoundState() != ROUND_WAITING_FOR_PLAYERS)
	{
		if(GetNumOfBritishPlayers() <= UNDEFINED)
		{
			m_iWinningTeam = AMERS;
			// -- move to restarting state if time up
			SetRoundState(ROUND_RESTARTING);
			return true;
		}

		else if(GetNumOfAmericanPlayers() <= UNDEFINED)
		{
			m_iWinningTeam = BRITS;
			// -- move to restarting state if time up
			SetRoundState(ROUND_RESTARTING);
			return true;
		}
	}

	// - all dead and we are waiting for players
	if(GetRoundState() == ROUND_WAITING_FOR_PLAYERS)
	{
		CountPlayersOnTeam(true);
		int iBritish = GetNumOfBritishPlayers();
		int iAmericans = GetNumOfAmericanPlayers();

		CountPlayersOnTeam(false);
		int iBritishDead = iBritish - GetNumOfBritishPlayers();
		int iAmericansDead = iAmericans - GetNumOfAmericanPlayers();

		if(iBritish > UNDEFINED && iBritishDead == iBritish)
		{
			m_iWinningTeam = AMERS;
			// -- move to restarting state if time up
			SetRoundState(ROUND_RESTARTING);
			return true;
		}
		else if(iAmericans > UNDEFINED  && iAmericansDead == iAmericans)
		{
			m_iWinningTeam = BRITS	;
			// -- move to restarting state if time up
			SetRoundState(ROUND_RESTARTING);
			return true;
		}
	}

	// - checks for time up
	if(GetRoundTime() <= gpGlobals->time)
	{
		// -- find the winning team
		CountPlayersOnTeam(true);
		int iBritish = GetNumOfBritishPlayers();
		int iAmericans = GetNumOfAmericanPlayers();

		CountPlayersOnTeam(false);
		int iBritishDead = iBritish - GetNumOfBritishPlayers();
		int iAmericansDead = iAmericans - GetNumOfAmericanPlayers();

		// we will award the team who has killed the most players the win first
		if(iBritishDead > iAmericansDead)
		{
			m_iWinningTeam = AMERS;
			// -- move to restarting state if time up
			SetRoundState(ROUND_RESTARTING);
			return true;
		}
		else if(iAmericansDead > iBritishDead)
		{
			m_iWinningTeam = BRITS;
			// -- move to restarting state if time up
			SetRoundState(ROUND_RESTARTING);
			return true;
		}
		// if dead are tied, we will award the team with the fewest players ( if they have any )
		else
		{
			if(iBritish > iAmericans && iAmericans > UNDEFINED)
			{
				m_iWinningTeam = AMERS;
				// -- move to restarting state if time up
				SetRoundState(ROUND_RESTARTING);
				return true;
			}
			else if(iBritish > iAmericans && iAmericans == UNDEFINED)
			{
				m_iWinningTeam = BRITS;
				// -- move to restarting state if time up
				SetRoundState(ROUND_RESTARTING);
				return true;
			}
			else if(iAmericans > iBritish && iBritish > UNDEFINED)
			{
				m_iWinningTeam = BRITS;
				// -- move to restarting state if time up
				SetRoundState(ROUND_RESTARTING);
				return true;
			}
			else if(iAmericans > iBritish && iBritish == UNDEFINED)
			{
				m_iWinningTeam = AMERS;
				// -- move to restarting state if time up
				SetRoundState(ROUND_RESTARTING);
				return true;
			}
			// teams are equally balanced and have killed the same no. so it is a tie
			else
			{
				// --log a draw
				UTIL_LogPrintf(ROUND_LOGGING, "World triggered \"Round Draw\" (respawn_style \"%i\")\n", (int)GetRespawnStyle());

				m_iWinningTeam = UNDEFINED;
				SetRoundState(ROUND_RESTARTING);
				return true;
			}
		}
	}

	// - check for other win conditions like all points captured
	bool bOtherConditions = HandleMiscWinConditions();

	if(bOtherConditions == true)
	{
		// - reset the world on other win conditions
		SetRoundState(ROUND_RESTARTING);
		return true;
	}
	return false;
}


// -Prepares players for the forcoming round
void CRoundRules::RestartRound()
{
	// - set the restart to the pause the mapper wants
	SetRestartLength(RESTART_TIME);
	bFullRestart = true;
	
	// - reset the round time clock ready for the next round
	SetRoundTimeOver();

	// - move to round start
	SetRoundState(ROUND_NOTSTARTED);
}

// Ben - EOF
