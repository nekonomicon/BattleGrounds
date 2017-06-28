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

//This file was created on the 17/5/03 at 14:00 by Ben Banfield for the Battle Grounds modification for Half-Life
//Its purpose is to replace Point.cpp as the capture point class.  Hopefully this will resolve ambiguity when we add different point types

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "soundent.h"
#include "animation.h"
#include "particle_defs.h"

#include "gamerules.h"
#include "bg_rules.h"

#include "point.h"
#include "capture_point.h"

#include "../../cl_dll/message_defs.h"

extern int gmsgHintbox;

LINK_ENTITY_TO_CLASS(func_capturepoint, CCapturePoint);

//retain as much 1.x compatibility as possible
LINK_ENTITY_TO_CLASS(func_point, CCapturePoint);
LINK_ENTITY_TO_CLASS(func_possesion, CCapturePoint);
LINK_ENTITY_TO_CLASS(func_ctb, CCapturePoint);

// I'm paranoid and I want to give each value a default state
void CCapturePoint::InitialiseValues( void )
{
	if(m_bInitialisedValues == true)
		return;

	// -set default values to help improve 1.x compatibility
	// ben : undone default score compatibility as then you have invisible points giving scores
	m_flCaptureStartTime = 0.0;
	m_flLastTeamBonus = 0.0;
	m_flTimeHeld = 0.0;
	m_iCaptureTime = 0;
	m_iPlayerAlwaysInPoint = NO;
	m_iPlayerBonus = 0;
	m_iPlayerBonusDelay = 10;
	m_iPlayersNeeded = 1;
	m_iTeamBonus = 0;
	m_iTeamBonusInterval = 20;
	SetTargetTeam(EITHER);
	SetEngagingTeam(EITHER);
	m_iPosition = 0;
	
	// change the player we last worked with to the current player
	m_pPlayerLastUpdated = NULL;

	int i = 0;

	for(i = 0; i <= MAX_PLAYERS_CAPTURING; i++)
	{
		m_apPlayers[i] = NULL;
	}

	for(i = 0; i <= iTotalTriggers; i++)
	{
		m_apTriggers[i] = NULL;
	}

	m_bInitialisedValues = true;
}

// set the entity up
void CCapturePoint::Spawn() 
{
	// -make sure all values have a default value
	if(m_bInitialisedValues == false)
		InitialiseValues();

	// -team is mapped to pev->team - HACK HACK
	if((int)GetTargetTeam() != pev->team && pev->team != 0)
		SetTargetTeam((TARGET_TEAM)pev->team);

	CBasePoint::Spawn();
	
	SetPointType(CAPTURE_POINT);
	SetPointStatus(FREE);

	// -setup our think and touch functions
	SetTouch( &CCapturePoint::CapturePointTouch );
	SetThink( &CCapturePoint::CapturePointThink );
	pev->nextthink = gpGlobals->time + 0.1;
}

// input settings from the map
void CCapturePoint::KeyValue(KeyValueData *pKeyValueData)
{
	// -make sure all values have a default value
	if(m_bInitialisedValues == false)
		InitialiseValues();

	// -base point values
	if(FStrEq(pKeyValueData->szKeyName, "hold_forever"))
	{
		m_iPlayerAlwaysInPoint = (SHOULD_HOLD)atoi(pKeyValueData->szValue);
	}
	// -1.x compatibility
	else if(FStrEq(pKeyValueData->szKeyName, "should_hold"))
	{
		m_iPlayerAlwaysInPoint = (SHOULD_HOLD)atoi(pKeyValueData->szValue);
	}

	else if(FStrEq(pKeyValueData->szKeyName, "capture_time"))
	{
		m_iCaptureTime = atoi(pKeyValueData->szValue);
	}
	// -1.x compatibility
	else if(FStrEq(pKeyValueData->szKeyName, "hold_time"))
	{
		m_iCaptureTime = atoi(pKeyValueData->szValue);
	}

	else if(FStrEq(pKeyValueData->szKeyName, "team_bonus"))
	{
		m_iTeamBonus = atoi(pKeyValueData->szValue);
	}
	else if(FStrEq(pKeyValueData->szKeyName, "team_bonus_interval"))
	{
		m_iTeamBonusInterval = atoi(pKeyValueData->szValue);
	}

	else if(FStrEq(pKeyValueData->szKeyName, "player_bonus"))
	{
		m_iPlayerBonus = atoi(pKeyValueData->szValue);
	}
	// -1.x compatibility
	else if(FStrEq(pKeyValueData->szKeyName, "points"))
	{
		m_iPlayerBonus = atoi(pKeyValueData->szValue);
	}

	else if(FStrEq(pKeyValueData->szKeyName, "player_bonus_delay"))
	{
		m_iPlayerBonusDelay = atoi(pKeyValueData->szValue);
	}
	// -1.x compatibility
	else if(FStrEq(pKeyValueData->szKeyName, "next_points"))
	{
		m_iPlayerBonusDelay = atoi(pKeyValueData->szValue);
	}
	else if(FStrEq(pKeyValueData->szKeyName, "players_needed"))
	{
		m_iPlayersNeeded = atoi(pKeyValueData->szValue);

		if(m_iPlayersNeeded > MAX_PLAYERS_CAPTURING)
			m_iPlayersNeeded = MAX_PLAYERS_CAPTURING;
	}

	// -allowing for point ordering in the menu
	else if(FStrEq(pKeyValueData->szKeyName, "point_position"))
	{
		m_iPosition = atoi(pKeyValueData->szValue);
	}

	// -triggers: capturing
	else if(FStrEq(pKeyValueData->szKeyName, "trigger_brit_capturing"))
	{
		m_apTriggers[brit_capturing] = new trigger_s;
		sprintf(m_apTriggers[brit_capturing]->szTarget, "%s\0", pKeyValueData->szValue);
	}
	else if(FStrEq(pKeyValueData->szKeyName, "trigger_amer_capturing"))
	{
		m_apTriggers[amer_capturing] = new trigger_s;
		sprintf(m_apTriggers[amer_capturing]->szTarget, "%s\0", pKeyValueData->szValue);
	}
	else if(FStrEq(pKeyValueData->szKeyName, "trigger_capturing"))
	{
		m_apTriggers[capturing] = new trigger_s;
		sprintf(m_apTriggers[capturing]->szTarget, "%s\0", pKeyValueData->szValue);
	}

	// -triggers: captured
	else if(FStrEq(pKeyValueData->szKeyName, "trigger_brit_captured"))
	{
		m_apTriggers[brit_captured] = new trigger_s;
		sprintf(m_apTriggers[brit_captured]->szTarget, "%s\0", pKeyValueData->szValue);
	}
	else if(FStrEq(pKeyValueData->szKeyName, "trigger_amer_captured"))
	{
		m_apTriggers[amer_captured] = new trigger_s;
		sprintf(m_apTriggers[amer_captured]->szTarget, "%s\0", pKeyValueData->szValue);
	}
	else if(FStrEq(pKeyValueData->szKeyName, "trigger_captured"))
	{
		m_apTriggers[captured] = new trigger_s;
		sprintf(m_apTriggers[captured]->szTarget, "%s\0", pKeyValueData->szValue);
	}

	// -triggers:abandoned
	else if(FStrEq(pKeyValueData->szKeyName, "trigger_brit_abandoned"))
	{
		m_apTriggers[brit_abandoned] = new trigger_s;
		sprintf(m_apTriggers[brit_abandoned]->szTarget, "%s\0", pKeyValueData->szValue);
	}
	else if(FStrEq(pKeyValueData->szKeyName, "trigger_amer_abandoned"))
	{
		m_apTriggers[amer_abandoned] = new trigger_s;
		sprintf(m_apTriggers[amer_abandoned]->szTarget, "%s\0", pKeyValueData->szValue);
	}
	else if(FStrEq(pKeyValueData->szKeyName, "trigger_abandoned"))
	{
		m_apTriggers[abandoned] = new trigger_s;
		sprintf(m_apTriggers[abandoned]->szTarget, "%s\0", pKeyValueData->szValue);
	}

	// -triggers: lost
	else if(FStrEq(pKeyValueData->szKeyName, "trigger_brit_lost"))
	{
		m_apTriggers[brit_lost] = new trigger_s;
		sprintf(m_apTriggers[brit_lost]->szTarget, "%s\0", pKeyValueData->szValue);
	}
	else if(FStrEq(pKeyValueData->szKeyName, "trigger_amer_lost"))
	{
		m_apTriggers[amer_lost] = new trigger_s;
		sprintf(m_apTriggers[amer_lost]->szTarget, "%s\0", pKeyValueData->szValue);
	}
	else if(FStrEq(pKeyValueData->szKeyName, "trigger_lost"))
	{
		m_apTriggers[lost] = new trigger_s;
		sprintf(m_apTriggers[lost]->szTarget, "%s\0", pKeyValueData->szValue);
	}

	else if(FStrEq(pKeyValueData->szKeyName, "trigger_player_entered"))
	{
		m_apTriggers[player_entering] = new trigger_s;
		sprintf(m_apTriggers[player_entering]->szTarget, "%s\0", pKeyValueData->szValue);
	}
	else if(FStrEq(pKeyValueData->szKeyName, "trigger_brit_player_entered"))
	{
		m_apTriggers[brit_player_entering] = new trigger_s;
		sprintf(m_apTriggers[brit_player_entering]->szTarget, "%s\0", pKeyValueData->szValue);
	}
	else if(FStrEq(pKeyValueData->szKeyName, "trigger_amer_player_entered"))
	{
		m_apTriggers[amer_player_entering] = new trigger_s;
		sprintf(m_apTriggers[amer_player_entering]->szTarget, "%s\0", pKeyValueData->szValue);
	}

	else if(FStrEq(pKeyValueData->szKeyName, "trigger_player_exited"))
	{
		m_apTriggers[player_exiting] = new trigger_s;
		sprintf(m_apTriggers[player_exiting]->szTarget, "%s\0", pKeyValueData->szValue);
	}
	else if(FStrEq(pKeyValueData->szKeyName, "trigger_brit_player_exited"))
	{
		m_apTriggers[brit_player_exiting] = new trigger_s;
		sprintf(m_apTriggers[brit_player_exiting]->szTarget, "%s\0", pKeyValueData->szValue);
	}
	else if(FStrEq(pKeyValueData->szKeyName, "trigger_amer_player_exited"))
	{
		m_apTriggers[amer_player_exiting] = new trigger_s;
		sprintf(m_apTriggers[amer_player_exiting]->szTarget, "%s\0", pKeyValueData->szValue);
	}
	// -pass all other values off to the base point
	else
	{
		CBasePoint::KeyValue(pKeyValueData);
	}
}

// our think function. this will handle deletion of players from a point
void CCapturePoint::CapturePointThink( void )
{
	int i = 0;
	m_flLastThinkTime = gpGlobals->time;

	int iPointState = GetPointStatus();

	// -loop through all players checking that they are doing what they are meant to
	for(i = 0; i <= MAX_PLAYERS_CAPTURING; i++ )
	{
		player_s *player = PlayerInPointByIndex(i);

		if(player == NULL)
			continue;

		player->iNumOfThinksSinceLastTouch++;

		// -make sure the player has valid teams and classes
		if(player->pPlayer->pev->team >= SPECS || player->pPlayer->pev->team <= UNDEFINED)
		{
			HandlePlayerExited(player->pPlayer);
			continue;
		}
		if(player->pPlayer->pev->playerclass > NUM_CLASSES || player->pPlayer->pev->playerclass <= UNDEFINED)
		{
			HandlePlayerExited(player->pPlayer);
			continue;
		}

		// --for points where the player always has to be in the point,
		// --we will remove them if they have been flagged absent previously
		if(m_iPlayerAlwaysInPoint == YES)
		{
			if(HaveEnoughThinksOccured(player) == true)
			{
				HandlePlayerExited(player->pPlayer);
				continue;
			}
		}

		// --if the player has died, we will remove them from the point
		if(player->pPlayer->m_iDead == DEAD && !(pev->spawnflags & SF_STAY_CAPTURED_ON_DEAD))
		{
			// --when func_capturepoint supports it check whether the point can
			// --continue held/capturing with dead players
			HandlePlayerExited(player->pPlayer);
			continue;
		}

		// --remove all players, who haven't been unflagged by the touch function
		// --only if the point isn't held.
		// --player who aren't in point on held and have to be in point for capture have been removed earlier
		if(HaveEnoughThinksOccured(player) == true && iPointState != HELD)
		{
			HandlePlayerExited(player->pPlayer);
			continue;
		}

		// --if the player is still involved and the point is capturing		
		if(player && player->pPlayer && iPointState == HELD)
		{
			// --send points to player if points haven't already been sent and the delay has passed
			if(player->bSentPoints == false && (m_flCaptureStartTime + m_iCaptureTime + m_iPlayerBonusDelay) < gpGlobals->time)
			{
				player->bSentPoints = true;
				if(m_iPlayerBonus > 0)
					player->pPlayer->AddPoints(0, m_iPlayerBonus);
			}
		}

		// --remove the player on change team
		if(player && player->pPlayer) {
			int iTeam = (int) GetEngagingTeam();

			if(player->pPlayer->pev->team != iTeam) {
				HandlePlayerExited(player->pPlayer);
				continue;
			}
		}

		// --change the player we last worked with to the current player
		m_pPlayerLastUpdated = player->pPlayer;
	}

	// -the point state needs to be updated incase in removing anyone we've messed it up
	iPointState = GetPointStatus();
	bool bEnoughPlayers = EnoughPlayersInvolvedInPoint();

	if(bEnoughPlayers == false)
	{
		// -not enough players, so change the point's status
		if(iPointState == HELD) {
			HandlePointLost();
		}

		else if(iPointState == CAPTURING)
			HandlePointAborted();
	}

	// -update the time capturing so far
	if(iPointState == CAPTURING)
		m_flTimeHeld = gpGlobals->time - m_flCaptureStartTime;

	// -handle any point relate functions
	if(iPointState == HELD)
	{
		// --give points to team if the interval has gone
		if(m_flLastTeamBonus + m_iTeamBonusInterval <= gpGlobals->time)
		{
			m_flLastTeamBonus = gpGlobals->time;
			if(m_iTeamBonus > 0)
				g_pBGRules->GiveTeamPoints((int)GetEngagingTeam(), m_iTeamBonus);
		}
	}

	pev->nextthink = gpGlobals->time + 0.1;
}

// our touch function. this will handle addition of players to capturing a point
void CCapturePoint::CapturePointTouch( CBaseEntity *pEnt )
{
	// -filter out any non-players
	if(pEnt->IsPlayer() == false)
		return;

	CBasePlayer *pPlayer = GetClassPtr((CBasePlayer*)pEnt->pev);

	// -remove invalid players
	if(!pPlayer)
	{
		pev->nextthink = gpGlobals->time + 0.1;
		return;
	}

	// thinking doesn't occur when there is a touch so lets have this
	if(m_flLastThinkTime + TIME_NEEDED <= gpGlobals->time)
		CapturePointThink();

	// -we are only going to spend processor time on players who can't do things to this point
	if(CanPlayerUsePoint(pPlayer) == false)
	{
		// --if they are involved but can't do anything, then we should remove them
		if(IsPlayerInvolved(pPlayer))
		{
			int iTeam = (int)GetTargetTeam();

			// ---we can allow the player to be dead, but not changing teams
			if(iTeam != pPlayer->pev->team && iTeam != UNDEFINED)
				HandlePlayerExited(pPlayer);

			// ---don't allow team changes
			if(iTeam != pPlayer->m_iTeamRespawningAs)
				HandlePlayerExited(pPlayer);

			// ---if the point allows dead players to stay captured then don't remove the player from the point
			if(!((pev->spawnflags & SF_STAY_CAPTURED_ON_DEAD) && GetPointStatus() == HELD))
				HandlePlayerExited(pPlayer);
		}

		pev->nextthink = gpGlobals->time + 0.1;
		return;
	}

	POINT_STATE iPointState = GetPointStatus();
	bool bOthers = OtherPlayersInPoint(pPlayer);

	if(iPointState == FREE)
	{
		// --if there are any other players in the point
		// --check that they are on our team
		if(bOthers == true && GetEngagingTeam() == (TARGET_TEAM)pPlayer->pev->team)
			HandlePlayerCapturing(pPlayer);

		// --allow the player to capture if no-one is in the point
		else if(bOthers == false)
			HandlePlayerCapturing(pPlayer);

		// --players who aren't on the capturing team are attempting to capture with us in the point.
		// --don't let them do anything to the point
		else  {
			pev->nextthink = gpGlobals->time + 0.1;
		}
	}

	else if(iPointState == CAPTURING)
	{
		// --don't let the other team interfere with our capture
		// --this is important for maps like bh where the capture point is practically
		// --the spawn point
		if(bOthers == true && GetEngagingTeam() != (TARGET_TEAM)pPlayer->pev->team)
		{
			pev->nextthink = gpGlobals->time + 0.1;
		}
		// --add more players in
		else if(GetEngagingTeam() == (TARGET_TEAM)pPlayer->pev->team)
		{
			if(!(pev->spawnflags & SF_PREVENT_POINT_OVERLOADING))
				HandlePlayerEntering(pPlayer);
		}

		// --check for point capture done
		bool bEnoughPlayers = EnoughPlayersInvolvedInPoint();

		// --we have enougzh players
		if(bEnoughPlayers == true)
		{
			m_flTimeHeld = gpGlobals->time - m_flCaptureStartTime;
			if((int)m_flTimeHeld >= m_iCaptureTime) {
				HandlePointCaptured();
			}
		}
		// --someone has left
		// -- we'll let think deal with this
		else {
//			HandlePointAborted();
		}
	}

	else if(iPointState == HELD)
	{
		// --give points to team if the interval has gone
		if(m_flLastTeamBonus + m_iTeamBonusInterval <= gpGlobals->time)
		{
			m_flLastTeamBonus = gpGlobals->time;
			if(m_iTeamBonus > 0)
				g_pBGRules->GiveTeamPoints((int)GetEngagingTeam(), m_iTeamBonus);
		}

		// --allow the other team to capture the point
		if(bOthers == false && GetEngagingTeam() != (TARGET_TEAM)pPlayer->pev->team)
		{
			HandlePointLost();
			HandlePlayerCapturing(pPlayer);
		}
		// --add more players in
		else if(GetEngagingTeam() == (TARGET_TEAM)pPlayer->pev->team && !IsPlayerInvolved(pPlayer))
		{
			if(!(pev->spawnflags & SF_PREVENT_POINT_OVERLOADING))
			{
				HandlePlayerEntering(pPlayer);
				int iIndex = IndexByPlayer(pPlayer);

				// ---no points for being added to the list after the capture
				if(iIndex > UNDEFINED)
					PlayerInPointByIndex(iIndex)->bSentPoints = true;
			}
		}
	}

	// -mark this player as involved
	if(IsPlayerInvolved(pPlayer) == true)
	{
		player_s * player = UpdatePlayerInPoint(pPlayer);

		// --if the player is still involved and the point is capturing		
		if(player && player->pPlayer && iPointState == HELD)
		{
			// --send points to player if points haven't already been sent and the delay has passed
			if(player->bSentPoints == false && (m_flCaptureStartTime + m_iCaptureTime + m_iPlayerBonusDelay) < gpGlobals->time)
			{
				player->bSentPoints = true;
				if(m_iPlayerBonus > 0)
					player->pPlayer->AddPoints(0, m_iPlayerBonus);
			}
		}
		UpdatePlayerInPoint(pPlayer);
	}

	// change the player we last worked with to the current player
	m_pPlayerLastUpdated = pPlayer;

	if(m_flLastThinkTime + TIME_NEEDED <= gpGlobals->time)
		CapturePointThink();

	pev->nextthink = gpGlobals->time + 0.1;
}

// POINT HELPER FUNCTIONS

// determines whether we have all the required players in the point
bool CCapturePoint::EnoughPlayersInvolvedInPoint( void )
{
	int i = 0;
	int iPlayers = 0;
	for(i = 0; i <= MAX_PLAYERS_CAPTURING; i++)
	{
		player_s *m_pPlayer = PlayerInPointByIndex(i);
		if(m_pPlayer != NULL)
			iPlayers++;
	}

	if(iPlayers < m_iPlayersNeeded)
		return false;

	else
		return true;
}

// returns the number of players involved in the point atm
int CCapturePoint::GetNumOfPlayersInvolvedInPoint( void )
{
	int i = 0;
	int iPlayers = 0;
	for(i = 0; i <= MAX_PLAYERS_CAPTURING; i++)
	{
		player_s *player = PlayerInPointByIndex(i);

		if(player != NULL)
			iPlayers++;
	}
	return iPlayers;
}

// checks whether a player is involved or not in this point
bool CCapturePoint::IsPlayerInvolved( CBasePlayer *pPlayer )
{
	int i = 0;
	for(i = 0; i <= MAX_PLAYERS_CAPTURING; i++)
	{
		player_s *player = PlayerInPointByIndex(i);

		if(player && player->pPlayer == pPlayer)
			return true;
	}
	return false;
}

// checks whether anyone else is in this point at the moment
bool CCapturePoint::OtherPlayersInPoint( CBasePlayer *pPlayerToIgnore )
{
	int i = 0;
	for(i = 0; i <= MAX_PLAYERS_CAPTURING; i++)
	{
		player_s *player = PlayerInPointByIndex(i);

		if(player != NULL) {
			if(pPlayerToIgnore != NULL) {
				if(pPlayerToIgnore != player->pPlayer) {
					if(HaveEnoughThinksOccured(player) == false)
						return true;
				}
			} else {
				if(HaveEnoughThinksOccured(player) == false)
					return true;
			}
		}
	}
	return false;

}

// checks whether anyone else is involved with this point at the moment
bool CCapturePoint::OtherPlayersInvolvedInPoint( CBasePlayer *pPlayerToIgnore )
{
	int i = 0;
	for(; i <= MAX_PLAYERS_CAPTURING; i++)
	{
		player_s *player = PlayerInPointByIndex(i);

		if(player != NULL) {
			if(pPlayerToIgnore != NULL) {
				if(pPlayerToIgnore != player->pPlayer) {
					return true;
				}
			} else {
				return true;
			}
		}
	}
	return false;
}

// returns the players by the index given us
player_s *CCapturePoint::PlayerInPointByIndex( int iPlayer )
{
	// -check the number is valid
	if(iPlayer < 0 || iPlayer > (MAX_PLAYERS_CAPTURING + 1))
	{
		return NULL;
	}

	player_s *player = m_apPlayers[iPlayer];

	if(!player)
		return NULL;

	if(!player->pPlayer)
	{
		delete m_apPlayers[iPlayer];
		m_apPlayers[iPlayer] = NULL;
		return NULL;
	}
	return m_apPlayers[iPlayer];
}

// returns the index of a player by the pointer sent to the function
int CCapturePoint::IndexByPlayer( CBasePlayer *pPlayer )
{
	if(!pPlayer)
		return -1;

	int i = 0;
	for(i = 0; i <= MAX_PLAYERS_CAPTURING; i++)
	{
		player_s *player = PlayerInPointByIndex(i);

		if(player && player->pPlayer == pPlayer)
		{
			return i;
		}
	}
	return -1;
}

// updates the players status in the point
player_s *CCapturePoint::UpdatePlayerInPoint( CBasePlayer *pPlayer, bool bRemove)
{
	if(!pPlayer)
		return NULL;

	int i = 0;
	for(i = 0; i <= MAX_PLAYERS_CAPTURING; i++)
	{
		if(m_apPlayers[i] && m_apPlayers[i]->pPlayer == pPlayer)
		{
			// we've been updated, lets rejoice
			m_apPlayers[i]->iNumOfThinksSinceLastTouch = 0;

			// -remove the player from the list if requested
			if(bRemove == true)
			{
				m_apPlayers[i]->pPlayer = NULL;
				delete m_apPlayers[i];
				m_apPlayers[i] = NULL;
				return NULL;
			}
			return m_apPlayers[i];
		}
	}

	// -search for an empty slot to inset this player in
	for(i = 0; i <= MAX_PLAYERS_CAPTURING; i++)
	{
		if(!m_apPlayers[i])
		{
			m_apPlayers[i] = new player_s;
			m_apPlayers[i]->pPlayer = pPlayer;
			m_apPlayers[i]->bSentPoints = false;
			m_apPlayers[i]->iNumOfThinksSinceLastTouch = 0;
			return m_apPlayers[i];
		}
	}
	return NULL;
}

// updates the clients hud with the latest info
void CCapturePoint::UpdateClientHUD( CBasePlayer *pPlayer )
{
	if(pev->spawnflags & SF_DONT_SHOW_IN_HUD)
		return;

	extern int gmsgPoints;

	if(pPlayer != NULL)
		MESSAGE_BEGIN(MSG_ONE, gmsgPoints, NULL, pPlayer->pev);
	else
		MESSAGE_BEGIN(MSG_ALL, gmsgPoints);

	WRITE_SHORT((int)GetPointID());
	WRITE_SHORT((int)m_iPosition);
	WRITE_SHORT((int)GetPointStatus());
	WRITE_STRING(STRING(pev->targetname));
	WRITE_SHORT((int)GetPointType());
	WRITE_SHORT((int)GetTargetTeam());
	WRITE_SHORT((int)GetEngagingTeam());
	WRITE_SHORT(m_iCaptureTime);
	WRITE_SHORT(GetNumOfPlayersInvolvedInPoint());
	WRITE_SHORT(m_iPlayersNeeded);

	MESSAGE_END();
}

// fires the correct triggers
void CCapturePoint::FireTriggers( int iTriggerId, int iTeam )
{
	if(iTriggerId < 0 || iTriggerId > iTotalTriggers)
		return;

	if(m_apTriggers[iTriggerId] && m_apTriggers[iTriggerId]->szTarget[0] != '\0')
	{
		FireTargets(m_apTriggers[iTriggerId]->szTarget, m_pPlayerLastUpdated, this, USE_TOGGLE, 0);
	}

	if(iTeam != 0)
	{
		int iTrigger2 = iTriggerId + iTeam;
		FireTriggers(iTrigger2, 0);
	}
}

// resets the point to the default state
void CCapturePoint::ResetPoint( void )
{
	if(GetPointStatus() == HELD)
	{
		// -make sure points have been awarded
		for(int i = 0; i <= MAX_PLAYERS_CAPTURING; i++)
		{
			player_s *m_pPlayer = PlayerInPointByIndex(i);

			if(m_pPlayer && m_pPlayer->pPlayer && m_pPlayer->bSentPoints == false)
			{
				m_pPlayer->bSentPoints = true;
				if(m_iPlayerBonus > 0)
					m_pPlayer->pPlayer->AddPoints(0, m_iPlayerBonus);
			}
		}
	}

	m_flCaptureStartTime = 0.0;
	m_flLastTeamBonus = 0.0;
	m_flTimeHeld = 0.0;
	m_pPlayerLastUpdated = NULL;
	SetEngagingTeam(EITHER);
	SetPointStatus(FREE);

	int i = 0;

	for(i = 0; i <= MAX_PLAYERS_CAPTURING; i++)
	{
		if(m_apPlayers[i])
			delete m_apPlayers[i];
		m_apPlayers[i] = NULL;
	}

	UpdateAllFlags();
	UpdateClientHUD();
}

void CCapturePoint::ClearMemory()
{
	int i = 0;
	for(i = 0; i <= iTotalTriggers; i++) {
		if(m_apTriggers) {
			delete m_apTriggers[i];
			m_apTriggers[i] = NULL;
		}
	}
	for(i = 0; i <= MAX_PLAYERS_CAPTURING; i++) {
		if(m_apPlayers) {
			delete m_apPlayers[i];
			m_apTriggers[i];
		}
	}
}
// checks whether any player hasn't been touched since X number of thinks
bool CCapturePoint::HaveEnoughThinksOccured( player_s *player )
{
	// get rid of them if they are gone
	if( !player ) {
		return true;
	}

	// we've been here a long time
	if(player->iNumOfThinksSinceLastTouch >= NUM_THINKS_NEEDED) {
		return true;
	}

	// we've had a small number of thinks since last touch
	return false;
}

// EVENT FUNCTIONS

// handles a player attempting to capture a point
void CCapturePoint::HandlePlayerCapturing( CBasePlayer *pPlayer )
{
	if(IsPlayerInvolved(pPlayer) == true)
		return;

	// -set the team, if no other
	if(GetEngagingTeam() == EITHER)
		SetEngagingTeam((TARGET_TEAM)pPlayer->pev->team);

	// -add the player to the capturing list
	UpdatePlayerInPoint(pPlayer);

	// change the player we last worked with to the current player
	m_pPlayerLastUpdated = pPlayer;

	int iTeam = (int)GetEngagingTeam();

	// -fire any triggers which don't matter if the point has moved to capturing or not
	FireTriggers(player_entering, iTeam);

	// -update the status, if we have enough players
	if(EnoughPlayersInvolvedInPoint() == true)
	{
		m_flTimeHeld = 0.0;
		m_flCaptureStartTime = gpGlobals->time;
		SetPointStatus(CAPTURING);

		// -give clients a text message
		if(GetNumOfPlayersInvolvedInPoint() > 1) {
			SendTextMessageToAllClients( pPlayer->entindex(), GetNumOfPlayersInvolvedInPoint(), iPointEvent_Capturing);
		} else {
			SendTextMessageToAllClients( pPlayer->entindex(), GetNumOfPlayersInvolvedInPoint(), iPointEvent_Player_Capturing);
		}
		// -fire triggers
		FireTriggers(capturing, iTeam);

		// -log team capturing point
		UTIL_LogPrintf( POINT_LOGGING, "Team \"%s\" triggered \"point_captured_started\" (point \"%s\")\n",  
			szTeamNames[iTeam],
			STRING(pev->targetname));
	}
	else
	{
		// -give clients a text message
		SendTextMessageToAllClients( pPlayer->entindex(), GetNumOfPlayersInvolvedInPoint(), iPointEvent_Player_Entered);
	}

	// -update any model entities
	UpdateAllFlags();

	// -update clients hud's
	UpdateClientHUD();

	// -log player entering
	UTIL_LogPrintf( POINT_LOGGING, "\"%s<%i><%s><%s>\" triggered \"player_capturing_point\" (point \"%s\")\n",  
		STRING( pPlayer->pev->netname ), 
		GETPLAYERUSERID( pPlayer->edict() ),
		GETPLAYERAUTHID( pPlayer->edict() ),
		szTeamNames[pPlayer->pev->team],
		STRING(pev->targetname));

	// -show flag capturing hintbox
	if (GetPointStatus() == CAPTURING)
	{
		MESSAGE_BEGIN( MSG_ONE, gmsgHintbox, NULL, pPlayer->pev );
		WRITE_SHORT(HINTBOX_FLAGCAP);
		MESSAGE_END();
	}
}

// handles this point being captured
void CCapturePoint::HandlePointCaptured( void )
{
	SetPointStatus(HELD);

	int iTeam = (int)GetEngagingTeam();
	// -fire triggers
	FireTriggers(captured, iTeam);

	// -message clients
	if(GetNumOfPlayersInvolvedInPoint() > 1)
		SendTextMessageToAllClients( 0, GetNumOfPlayersInvolvedInPoint(), iPointEvent_Captured);
	else
		SendTextMessageToAllClients( m_pPlayerLastUpdated->entindex(), GetNumOfPlayersInvolvedInPoint(), iPointEvent_Player_Captured);

	// -update any model entities
	UpdateAllFlags();

	// -update clients hud's
	UpdateClientHUD();

	// -give points to team
	m_flLastTeamBonus = gpGlobals->time;
	if(m_iTeamBonus > 0)
		g_pBGRules->GiveTeamPoints(iTeam, m_iTeamBonus);

	// -log team capturing point
	UTIL_LogPrintf( POINT_LOGGING, "Team \"%s\" triggered \"point_captured\" (point \"%s\")\n",  
		szTeamNames[iTeam],
		STRING(pev->targetname));

	CPointModel *pFlag = NULL;
	pFlag = (CPointModel*)UTIL_FindEntityByClassname( pFlag, "env_flag" );

	// loop through all flags for this point displaying a particle effect at them
	while (pFlag) { 
		if(pFlag) {
			if(FStrEq(STRING(pFlag->pev->target),STRING(pev->targetname))) {
				// create particle effect
				extern int gmsgParticles;
				MESSAGE_BEGIN(MSG_ALL, gmsgParticles);
					WRITE_SHORT(0);
					WRITE_BYTE(0);
					WRITE_COORD( pFlag->pev->origin.x );
					WRITE_COORD( pFlag->pev->origin.y );
					WRITE_COORD( pFlag->pev->origin.z );
					WRITE_COORD( 0 );
					WRITE_COORD( 0 );
					WRITE_COORD( 0 );
					if(iTeam == BRITS)
						WRITE_SHORT(iDefaultRedSmoke);
					else
						WRITE_SHORT(iDefaultBlueSmoke);
				MESSAGE_END();
			}
		}
		pFlag = (CPointModel*)UTIL_FindEntityByClassname( pFlag, "env_flag" );
	}	
}

// handles this point's capture being aborted
void CCapturePoint::HandlePointAborted( int iTeam )
{
	SetPointStatus(FREE);

	if(iTeam == 0)
		iTeam = (int)GetEngagingTeam();

	// -fire triggers
	FireTriggers(abandoned, iTeam);

	// -message clients
	SendTextMessageToAllClients( 0, GetNumOfPlayersInvolvedInPoint(), iPointEvent_Abandoned);

	// -reset the point vars
	m_flCaptureStartTime = 0.0;
	m_flTimeHeld = 0.0;

	// -we're still engaged in the point if we have players there
	if((m_iPlayersNeeded - GetNumOfPlayersInvolvedInPoint()) > 0 )
		SetEngagingTeam(EITHER);

	// -update any model entities
	UpdateAllFlags();

	// -update clients hud's
	UpdateClientHUD();

	// -log team abandoning capture
	UTIL_LogPrintf( POINT_LOGGING, "Team \"%s\" triggered \"point_abandoned\" (point \"%s\")\n",  
		szTeamNames[iTeam],
		STRING(pev->targetname));
}

// handles this point's capture being aborted
void CCapturePoint::HandlePointLost( int iTeam )
{
	SetPointStatus(FREE);

	if(iTeam == 0)
		iTeam = (int)GetEngagingTeam();

	// -fire triggers
	FireTriggers(lost, iTeam);

	// -message clients
	SendTextMessageToAllClients( 0, GetNumOfPlayersInvolvedInPoint(), iPointEvent_Lost);

	// -reset any needed vars
	m_flCaptureStartTime = 0.0;
	m_flTimeHeld = 0.0;

	// -remove all players who are no longer physically in the point
	for(int i = 0; i <= MAX_PLAYERS_CAPTURING; i++)
	{
		player_s *player = PlayerInPointByIndex(i);
		if(player && HaveEnoughThinksOccured(player)) {
			HandlePlayerExited(player->pPlayer, false);
		}
	}

	// -if there are no players physically in the point, its open to everyone
	if(GetNumOfPlayersInvolvedInPoint() <= 0)
		SetEngagingTeam(EITHER);

	// -update any model entities
	UpdateAllFlags();

	// -update clients hud's
	UpdateClientHUD();

	// -log team losing point
	UTIL_LogPrintf( POINT_LOGGING, "Team \"%s\" triggered \"point_lost\" (point \"%s\")\n",  
		szTeamNames[iTeam],
		STRING(pev->targetname));	
}

// controls a player leaving the point
// player only leaves the point if the point isn't held
// or the point is held and stay captured on dead isn't set
void CCapturePoint::HandlePlayerExited( CBasePlayer *pPlayer, bool bSendTextMessage )
{
	if(IsPlayerInvolved(pPlayer) == false)
		return;

	int iTeam = (int)GetEngagingTeam();
	int iStatus = GetPointStatus();

	if(iStatus != HELD ||(iStatus == HELD && !(pev->spawnflags & SF_STAY_CAPTURED_ON_DEAD))) {
		UpdatePlayerInPoint(pPlayer, true);
	} else {
		if(pPlayer->m_iTeamRespawningAs != iTeam)
			UpdatePlayerInPoint(pPlayer, true);
	}

	FireTriggers(player_exiting, iTeam);	

	int iPlayers = GetNumOfPlayersInvolvedInPoint();

	if(bSendTextMessage)
		SendTextMessageToAllClients(pPlayer->entindex(), iPlayers, iPointEvent_Player_Exited);

	// -log player exiting
	if(pPlayer && pPlayer->pev)
	{
		if(iStatus != HELD ||(iStatus == HELD && !(pev->spawnflags & SF_STAY_CAPTURED_ON_DEAD))) {
			UTIL_LogPrintf( POINT_LOGGING, "\"%s<%i><%s><%s>\" triggered \"player_abandoning_point\" (point \"%s\")\n",  
				STRING( pPlayer->pev->netname ), 
				GETPLAYERUSERID( pPlayer->edict() ),
				GETPLAYERAUTHID( pPlayer->edict() ),
				szTeamNames[pPlayer->pev->team],
				STRING(pev->targetname));
		} else {
			if(pPlayer->m_iTeamRespawningAs != iTeam) {
				UTIL_LogPrintf( POINT_LOGGING, "\"%s<%i><%s><%s>\" triggered \"player_abandoning_point\" (point \"%s\")\n",  
					STRING( pPlayer->pev->netname ), 
					GETPLAYERUSERID( pPlayer->edict() ),
					GETPLAYERAUTHID( pPlayer->edict() ),
					szTeamNames[pPlayer->pev->team],
					STRING(pev->targetname));
			}
		}
	}

	// -update the team if its obvious we can't be involved anymore
	if(iPlayers <= 0)
		SetEngagingTeam(EITHER);

	// -trigger the appropriate point handler if we don't have enough people involved
	if((m_iPlayersNeeded - iPlayers) > 0) {
		if(iStatus == CAPTURING) {
			HandlePointAborted(iTeam);
		} else if(iStatus == HELD && !(pev->spawnflags & SF_STAY_CAPTURED_ON_DEAD)) {
			HandlePointLost(iTeam);
		} else if(pPlayer->m_iTeamRespawningAs != iTeam) {
			HandlePointLost(iTeam);
		} else {}
	}

	// -update saved status
	iStatus = GetPointStatus();

	// -update any model entities
	UpdateAllFlags();

	// -update the clients hud
	UpdateClientHUD();
}

// handles a player entering a point after the capture has begun
void CCapturePoint::HandlePlayerEntering( CBasePlayer *pPlayer )
{
	if(IsPlayerInvolved(pPlayer) == true)
		return;

	// -double check the player is on the right team
	if((int)GetEngagingTeam() != pPlayer->pev->team)
		return;

	// -limit the maximuim number of people overloading a point
	if(GetNumOfPlayersInvolvedInPoint() >= (MAX_PLAYERS_CAPTURING) - 1)
		return;

	// -add the player to the capturing list
	UpdatePlayerInPoint(pPlayer);

	// change the player we last worked with to the current player
	m_pPlayerLastUpdated = pPlayer;

	// -fire any triggers which don't matter if the point has moved to capturing or not
	FireTriggers(player_entering, (int)GetEngagingTeam());

	// -give clients a text message
	SendTextMessageToAllClients( pPlayer->entindex(), GetNumOfPlayersInvolvedInPoint(), iPointEvent_Player_Entered);

	// -update any model entities
	UpdateAllFlags();

	// -update clients hud's
	UpdateClientHUD();

	// -log player entering
	UTIL_LogPrintf( POINT_LOGGING, "\"%s<%i><%s><%s>\" triggered \"player_entering_point\" (point \"%s\")\n",  
		STRING( pPlayer->pev->netname ), 
		GETPLAYERUSERID( pPlayer->edict() ),
		GETPLAYERAUTHID( pPlayer->edict() ),
		szTeamNames[pPlayer->pev->team],
		STRING(pev->targetname));	

	// -show flag overloading hintbox
	if (GetPointStatus() == HELD)
	{
		MESSAGE_BEGIN( MSG_ONE, gmsgHintbox, NULL, pPlayer->pev );
		WRITE_SHORT(HINTBOX_FLAGOVERLOAD);
		MESSAGE_END();
	}
}
