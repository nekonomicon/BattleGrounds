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

//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============

#ifndef VOICE_BANMGR_H
#define VOICE_BANMGR_H
#ifdef _WIN32
#pragma once
#endif


// This class manages the (persistent) list of squelched players.
class CVoiceBanMgr
{
public:

				CVoiceBanMgr();
				~CVoiceBanMgr();	

	// Init loads the list of squelched players from disk.
	bool		Init(char const *pGameDir);
	void		Term();

	// Saves the state into voice_squelch.dt.
	void		SaveState(char const *pGameDir);

	bool		GetPlayerBan(char const playerID[16]);
	void		SetPlayerBan(char const playerID[16], bool bSquelch);

	// Call your callback for each banned player.
	void		ForEachBannedPlayer(void (*callback)(char id[16]));


protected:

	class BannedPlayer
	{
	public:
		char			m_PlayerID[16];
		BannedPlayer	*m_pPrev, *m_pNext;
	};

	void				Clear();
	BannedPlayer*	InternalFindPlayerSquelch(char const playerID[16]);
	BannedPlayer*	AddBannedPlayer(char const playerID[16]);


protected:

	BannedPlayer	m_PlayerHash[256];
};


#endif // VOICE_BANMGR_H
