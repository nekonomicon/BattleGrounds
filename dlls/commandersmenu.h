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

/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

#ifndef COMMANDERSMENU_H
#define COMMANDERSMENU_H

struct autharray 
{
    char ID[32][255];
};

class CCommander
{
public:
	CCommander(CBasePlayer *pPlayer);
	~CCommander();
	void Init();
	void Think(entvars_t *pev);
	void Rush(CBasePlayer *pCommanderPlayer);
	void GetPlayersInCommanderRange(CBasePlayer *pPlayer, float Range);
	int Execute(int special, entvars_t* pevToBeNotified);
private:
	float m_flRushEndTime;
	bool m_bRushActive;
	autharray m_RushPlayerIndex;
	
	bool m_bArtilleryFull;
	bool m_bRushFull;
	int m_iPointsPerSecond;
	float m_flTotalPoints;
	float m_flSpendablePoints;
	float m_flLastTime;
	int m_iRush;
	int m_iRush_Max;
	bool m_bAddedRush;
	int m_iArtillery;
	int m_iArtillery_Max;
	bool m_bAddedArtillery;
};


#endif