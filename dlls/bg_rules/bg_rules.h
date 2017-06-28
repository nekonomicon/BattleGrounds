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

//This file was created on 11/9/2k+1 at 21:00 by Ben Banfield for the Battlegrounds modification of Half-Life
//This file's purpose to declare the Battlegrounds roundbased gamerules

const int TEAMPLAY_TEAMLISTLENGTH = NUM_TEAMS * NAME_LENGTH;

class CRoundRulesContainer;

class CBaseBgRules : public CHalfLifeMultiplay
{
public:
	CBaseBgRules();
	~CBaseBgRules(){}

	//Ben - New Bg variables go here
	CRoundRulesContainer *pRoundRules;
	bool	m_iSayText;
	unsigned short m_usRoundEnd;

	//Ben - New teampaly functions go here
	void InitSpawns( void );
	void GiveTeamPoints( int iTeam, int iPoints );
	void SetTeamPoints( int iTeam, int iPoints );

	virtual void HandleTeams( CBasePlayer *pPlayer, int m_iTeamNum);
	virtual void HandleClasses( CBasePlayer *pPlayer, int m_iClassNum);
	virtual void UpdateMyScores( CBasePlayer *pPlayer);
	virtual void Respawn(CBasePlayer *pPlayer, int iTeamRespawningAs, int iClassRespawningAs);
	virtual void HandleVoiceMenu1(int iSlot, CBasePlayer *pPlayer);
	virtual void HandleVoiceMenu2(int iSlot, CBasePlayer *pPlayer);

	virtual bool ClientConnected( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ] );
	virtual bool ClientCommand( CBasePlayer *pPlayer, const char *pcmd );
	virtual void ClientUserInfoChanged( CBasePlayer *pPlayer, char *infobuffer );
	virtual bool IsTeamplay( void );
	virtual bool FPlayerCanTakeDamage( CBasePlayer *pPlayer, CBaseEntity *pAttacker );
	virtual int PlayerRelationship( CBaseEntity *pPlayer, CBaseEntity *pTarget );
	virtual const char *GetTeamID( CBaseEntity *pEntity );
	virtual bool ShouldAutoAim( CBasePlayer *pPlayer, edict_t *target );
	virtual int IPointsForKill( CBasePlayer *pAttacker, CBasePlayer *pKilled );
	virtual void InitHUD( CBasePlayer *pl );
	virtual void DeathNotice( CBasePlayer *pVictim, entvars_t *pKiller, entvars_t *pevInflictor );
	virtual const char *GetGameDescription( void ) { return "The Battle Grounds"; }  // this is the game name that gets seen in the server browser
	virtual void UpdateGameMode( CBasePlayer *pPlayer );  // the client needs to be informed of the current game mode
	virtual void PlayerKilled( CBasePlayer *pVictim, entvars_t *pKiller, entvars_t *pInflictor );
	virtual void Think ( void );
	virtual int GetTeamIndex( const char *pTeamName );
	virtual bool IsValidTeam( const char *pTeamName );
	const char *SetDefaultPlayerTeam( CBasePlayer *pPlayer );
	virtual void ChangePlayerTeam( CBasePlayer *pPlayer, const char *pTeamName, bool bKill, bool bGib );

	int iBritishScore;
	int iAmericanScore;
private:

	void RecountTeams(int &iBrits, int &iAmers);
	int TeamWithFewestPlayers( void );

	bool m_DisableDeathMessages;
	bool m_DisableDeathPenalty;
	bool m_teamLimit;				// This means the server set only some teams as valid
	char m_szTeamList[TEAMPLAY_TEAMLISTLENGTH];
};

extern DLL_GLOBAL CBaseBgRules* g_pBGRules;
