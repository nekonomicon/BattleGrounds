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

//This file was created on the 17/5/03 at 14:04 by Ben Banfield for the Battle Grounds modification for Half-Life
//The purpose of this file is to declare the CCapturePoint class

#ifndef CAPTURE_POINT_H
#define CAPTURE_POINT_H

const int NUM_THINKS_NEEDED = 4;
const float TIME_NEEDED = 0.1;

// trigger constants
const int capturing = 0;
const int brit_capturing = 1;
const int amer_capturing = 2;

const int captured = 3;
const int brit_captured = 4;
const int amer_captured = 5;

const int abandoned = 6;
const int brit_abandoned = 7;
const int amer_abandoned = 8;

const int lost = 9;
const int brit_lost = 10;
const int amer_lost = 11;

const int player_entering = 12;
const int brit_player_entering = 13;
const int amer_player_entering = 14;

const int player_exiting = 15;
const int brit_player_exiting = 16;
const int amer_player_exiting = 17;

const int iTotalTriggers = 17;

// this structure will hold all the players who are in the capture sequence
struct player_s
{
	CBasePlayer *pPlayer;
	bool bSentPoints;
	int iNumOfThinksSinceLastTouch;
};

const int MAX_PLAYERS_CAPTURING = 20;

class CCapturePoint : public CBasePoint
{
public:
	// Basic Entity Setup functions
	virtual void Spawn( void );
	virtual void Precache( void ) {}
	virtual void KeyValue( KeyValueData *pkvd );

	void EXPORT CapturePointThink( void );
	void EXPORT CapturePointTouch( CBaseEntity *pEntity );

	TARGET_TEAM GetHoldingTeam( void );
	int GetNumOfPlayersInvolvedInPoint( void );

	void UpdateClientHUD( CBasePlayer *pPlayer = NULL );
	void ResetPoint();
	void ClearMemory();
	virtual void PlayerDied( CBasePlayer *pPlayer ) { HandlePlayerExited(pPlayer, true); }
protected:
	// Capture Point Events
	void HandlePointCaptured( void );
	void HandlePlayerCapturing( CBasePlayer *pPlayer );
	void HandlePlayerEntering( CBasePlayer *pPlayer );
	void HandlePointLost( int iTeam = 0 );
	void HandlePointAborted( int iTeam = 0 );
	void HandlePlayerExited( CBasePlayer *pPlayer, bool bSendTextMessage = true );

private:
	void InitialiseValues( void );
	void FireTriggers( int iTriggerId, int iTeam = 0);

	bool OtherPlayersInPoint( CBasePlayer *pPlayerToIgnore = NULL );
	bool OtherPlayersInvolvedInPoint( CBasePlayer *pPlayerToIgnore = NULL );
	bool EnoughPlayersInvolvedInPoint( void );
	bool IsPlayerInvolved( CBasePlayer *pPlayer );
	bool HaveEnoughThinksOccured( player_s *player );
	player_s *PlayerInPointByIndex( int iPlayer );
	int	IndexByPlayer( CBasePlayer *pPlayer );

	player_s *UpdatePlayerInPoint( CBasePlayer *pPlayer, bool bRemove = false );

	player_s *m_apPlayers[MAX_PLAYERS_CAPTURING + 1]; // array of players who are contributing to the capture of the point
	CBasePlayer *m_pPlayerLastUpdated; // a pointer to the player who was last updated

	float m_flTimeHeld; // time the player has held so far
	float m_flCaptureStartTime; // time the capture started;

	// data passed to us from the mapper
	trigger_s *m_apTriggers[iTotalTriggers + 1]; // array of all triggers and their targets
	SHOULD_HOLD m_iPlayerAlwaysInPoint; // whether a player always has to be in the point for it to be considered held

	int m_iCaptureTime;	// time needed to capture the point
	int m_iPlayersNeeded; // number players needed to capture the point	

	int m_iPlayerBonus;	// how big a point bonus will the capturing player get
	int m_iTeamBonus; // how big a point bonus the team gets for capturing a point
	int m_iPlayerBonusDelay; // how long do they have to hold the point for, before they can get this bonus
	int m_iTeamBonusInterval; // how often the team gets this bonus

	float m_flLastTeamBonus; // the time of our last team bonus

	bool m_bInitialisedValues; // have all values been given a default state?

	float m_flLastThinkTime; //the time the think function was last called
};

#endif
