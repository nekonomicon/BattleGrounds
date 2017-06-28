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

//This file was created some time ago by Ben Banfield for the Battle Grounds modification of Half-Life
//Its purpose is to declare all classes and variables which will be used in point based play

#ifndef POINT_H
#define POINT_H

// this structure will hold the trigger's target
struct trigger_s
{
	char szTarget[128];
};

const int SF_DONT_SHOW_IN_HUD = 1;
const int SF_SUPRESS_MESSAGES = 2;
const int SF_NOT_NEEDED_FOR_WIN = 4;
const int SF_STAY_CAPTURED_ON_DEAD = 8;
const int SF_PREVENT_POINT_OVERLOADING = 16;

class CPointManager;

// The basic bg point, from which capture, escape and destruction points inherit off
class CBasePoint : public CBaseEntity
{
public:
	// Base entity functions
	virtual void Spawn( void );
	virtual void Precache( void ) {}
	virtual void KeyValue( KeyValueData *pkvd );

	// Accessor functions
	POINT_STATE GetPointStatus( void );
	TARGET_TEAM GetTargetTeam( void );
	TARGET_TEAM GetEngagingTeam( void );
	POINT_TYPE GetPointType( void );
	int GetPointID( void );

	virtual void UpdateClientHUD( CBasePlayer *pPlayer = NULL ) {}
	virtual void ResetPoint() {}
	virtual void ClearMemory() {}
	virtual void PlayerDied( CBasePlayer *pPlayer ) {}

protected:
	void SetPointStatus( POINT_STATE iPointState );
	void SetTargetTeam( TARGET_TEAM iTargetTeam );
	void SetEngagingTeam( TARGET_TEAM iEngagingTeam );
	void SetPointType( POINT_TYPE iPointType );

	void UpdateAllFlags( void );

	bool CanPlayerUsePoint( CBasePlayer *pPlayer );

	void SendTextMessageToAllClients( int iPlayer, int iPlayers, int iPointEvent );

	int m_iPosition; // position in the hud
private:
	void SetID( int iID );

	POINT_STATE m_iPointStatus;	// the status of the point
	TARGET_TEAM m_iTargetTeam;	// the team who can engage in the point
	TARGET_TEAM m_iEngagingTeam; // the team who is currently engaged in the point

	int m_iID;
	POINT_TYPE m_iPointType;
};

struct point_s
{
	CBasePoint *pPoint;
	float flTimeAdded;
	bool bWinConditionsSatisfied;
	TARGET_TEAM iTeam;
};

// The class which will handle all interactions with points
class CPointManager
{
public:
	CPointManager( void );
	~CPointManager( void );
private:
	CPointManager(const CPointManager&) {}
public:
	void UpdatePlayersHUD( CBasePlayer *pPlayer = NULL, bool bClearHud = false ); // update either a single player or everyone's hud with the latest point info
	bool EnoughPointsReadyForWin( void ); // determines if we have a point based win
	int  GetWinningTeam( void ); // returns the winning team

	void PlayerDied( CBasePlayer *pPlayer ); // handles a player dieing
	void PlayerDisconnected( CBasePlayer *pPlayer ); // handles a player disconnecting
	
	point_s *ReturnPointByID( int iId ); // returns a point
	void ResetAllPoints( void ); // returns all points to a default state
	void UpdateAllFlags( int iId ); // updates all flags for a point
	void DestroyAllPoints( void ); // cleans up memory for all points
protected:
	friend class CBasePoint;
	int AddPoint( CBasePoint *pPoint ); // adds in a point
	void UpdatePointState( int iId ); // updates a point's state
private:
	inline void SetHasAnyTeamWon( bool bWon );
	inline void SetTeamWon( TARGET_TEAM iTeam );

	point_s *m_apPoints[MAX_POINTS+1];
	bool m_bHasAnyTeamWon; // has any team won yet?
	TARGET_TEAM m_iTeamWon; // which team has won
};

class CPointModel : public CBaseEntity
{
public:
	virtual void Spawn( void );
	virtual void Precache( void );
	virtual void KeyValue( KeyValueData *pkvd );
	void EXPORT FlagThink( void );
	void SetModel( int iTeam );
	char *ReturnTargetPoint( void );

	void	Animate( float frames );

	inline int		ShouldAnimate( void ) { return m_iAnimate && m_flMaxFrame > 1.0; }
private:
	char szBritishFlag[256];
	char szAmericanFlag[256];
	char szDefaultFlag[256];

	int			m_iAnimate;
	float		m_flLastTime;
	float		m_flMaxFrame;
};
#endif
