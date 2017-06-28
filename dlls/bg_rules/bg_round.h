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
//Its purpose is to declare the classes for round and respawn rules in BG

#ifndef BG_RULES_H
#define BG_RULES_H

//Ben - Define Timelimits
#define MAP_TIME CVAR_GET_FLOAT( "mp_timelimit" )
#define ROUND_TIME CVAR_GET_FLOAT( "mp_roundtime" )
#define RESTART_TIME CVAR_GET_FLOAT( "mp_restarttime" )
#define ROUNDS_LEFT CVAR_GET_FLOAT( "mp_roundsleft" )
#define ROUND_PAUSETIME CVAR_GET_FLOAT( "mp_pausetime" )
#define RESPAWNING_STYLE CVAR_GET_FLOAT( "mp_respawnstyle" )

#define BRITISH_WIN_SOUND "misc/win_british.wav"
#define AMERICAN_WIN_SOUND "misc/win_american.wav"

#define iWinBonus CVAR_GET_FLOAT( "mp_winbonus" )

// round win conditions
typedef enum
{
	NONE = 0,					//no reason yet
	TIME_UP = 1,				// time up--draw
	ALL_BRITISH_KILLED = 2,		// team 1 win
	ALL_AMERICANS_KILLED = 3,	// team 2 win
	DRAW = 4,					// draw (ie both teams empty)
	POINTS_CAPTURED = 5,		// all points have been captured
	NO_BRITISH_LEFT = 6,		//no british players left
	NO_AMERICANS_LEFT = 7,		//no american players left
	// include any misc conditions here
}ROUND_WIN;

// respawn style types
typedef enum
{
	NULL_RESPAWN = 0,
	WAVE_RESPAWN = 1,
	ROUND_RESPAWN = 2
}RESPAWN_STYLE;

// round state enums
typedef enum
{
	ROUND_NOTSTARTED = 1,	// nobody in-game yet or a 1 frame session while restarting
	ROUND_PAUSED = 2,		// pause time before running round
	ROUND_RUNNING = 3,		// round in session
	ROUND_ENDING = 4,		// an end condition has been meet
	ROUND_WAITING_FOR_PLAYERS = 5,	// only players on 1 team
	ROUND_NOROUNDS = 6,		// no round rules on this type - not used atm
	ROUND_RESTARTING = 7,	// in the middle of restarting the round
	ROUND_NOPLAYERS = 8,	// no players at all
} ROUND_STATE;

const float ROUND_UPDATE_TIME = 0.5;
const float RESPAWN_WAVE_ROUNDTIME_LIMIT = 30.0;
const float ROUND_RULES_ROUNDTIME_LIMIT = 420.0;
const float ROUND_RULES_PAUSETIME_LIMIT = 7.0;
const float ROUND_RULES_RESTARTTIME_LIMIT = 4.0;


// The basic round rules from which all will be derived from
class CBaseRoundRules
{
public:
	friend class CRoundRulesContainer;

	CBaseRoundRules();
	~CBaseRoundRules();

	virtual void RoundThink() = 0;
	
	ROUND_STATE GetRoundState();
	RESPAWN_STYLE GetRespawnStyle();
	float GetRoundTime() { return m_flRoundTime; }
	float GetRestartTime() { return m_flRestartTime; }

	int GetNumOfBritishPlayers() { return m_iBritish; }
	int GetNumOfAmericanPlayers() { return m_iAmericans; }
	int GetNumOfSpectatingPlayers() { return m_iSpectators; }
	int GetTotalNumOfPlayers() { return (m_iBritish + m_iAmericans + m_iSpectators); }

	void CountPlayersOnTeam(bool bIncludeDead);
	void CountPlayersOnTeamFuture(bool bIncludeDead);

	void SetNoRound() { SetRoundState(ROUND_NOPLAYERS, false); }
	virtual void SetRoundBegin() { bFullRestart = true; SetRoundState(ROUND_RESTARTING, false); }

protected:
	void SetRoundState(ROUND_STATE iRoundState, bool bSkipAutoStateUpdate = true);	

	bool UpdateRound();
	void FullRestart(bool bResetScores = false, bool bResetCustom = true);

	virtual bool CheckForRoundStart();
	virtual bool CheckForRoundEnd() = 0;
	virtual void RespawnPlayers() = 0;
	virtual void StartRound() ;
	virtual void StartDummyRound();
	virtual void RestartRound() = 0;
	virtual void AwardWinners();
	virtual void ResetWorld(bool bResetCustom = true, bool bResetOnlyCustom = false);
	virtual void RespawnPlayer(CBasePlayer *pPlayer);
	virtual bool HandleMiscWinConditions();

	float GetRoundTimeLimit() { return m_flRoundTimeLimit; }
	float GetPauseTimeLimit() { return m_flPauseTimeLimit; }
	float GetRestartTimeLimit() { return m_flRestartTimeLimit; }

	void SetRoundLength(float flRoundTime, bool bPauseTime = false);
	void SetRestartLength(float flRestartTime);
	void SetRoundTimeLimit(float flRoundTimeLimit);
	void SetPauseTimeLimit(float flPauseTimeLimit);
	void SetRestartTimeLimit(float flRestartTimeLimit);
	void SetRoundTimeOver() { m_flRoundTime = 0.0; }

	int iCount;
	int m_iWinningTeam;
	bool bFullRestart;

	void SetRespawnStyle(RESPAWN_STYLE iRespawnStyle);
private:
	void ClearTeamsCount() { m_iBritish = 0;  m_iAmericans = 0;  m_iSpectators = 0; }
	void SendRoundTimeToClients(bool bPaused = false);
	void StopWinSounds();

	RESPAWN_STYLE m_iRespawnStyle;
	ROUND_STATE m_iRoundState;
	float m_flPauseTimeLimit;
	float m_flRoundTimeLimit;
	float m_flRestartTimeLimit;
	float m_flRoundTime;
	float m_flRestartTime;

	int m_iBritish;
	int m_iAmericans;
	int m_iSpectators;
};

// Respawning only those who are dead at a certain interval
class CRespawnWaveRules : public CBaseRoundRules
{
public:
	CRespawnWaveRules();
	~CRespawnWaveRules();
	virtual void RoundThink();

	virtual void SetRoundBegin() { bFullRestart = true; SetRoundState(ROUND_ENDING, false); }
private:
	virtual bool CheckForRoundEnd();
	virtual void RespawnPlayers();
	virtual void RestartRound();
};

// Repsawning all players at a certain interval
class CRoundRules : public CBaseRoundRules
{
public:
	CRoundRules();
	~CRoundRules();
	virtual void RoundThink();

private:
	virtual bool CheckForRoundEnd();
	virtual void RespawnPlayers() {}
	virtual void RestartRound();
	void PauseTime();
	void PrepareRound();
};

// The Round Rules Container, allowing us to switch round rule styles easily
class CRoundRulesContainer
{
public:
	CRoundRulesContainer();
	~CRoundRulesContainer();

	void RoundThink();

	CBaseBgRules *GetGameRules() { return pBgRules; }
	ROUND_STATE GetRoundState() { return pRoundRules->GetRoundState(); }

private:
	void StartRoundRules();
	CBaseRoundRules *pRoundRules;
	CBaseBgRules *pBgRules;

	float m_flRestartRoundTime;
	float m_flLastRoundUpdate;

	bool bSwitchingRespawnStyle;
};

#endif

// Ben - EOF
