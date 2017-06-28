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

#include <string.h>
#include <stdio.h>
#include "voice_banmgr.h"
#pragma warning (disable : 4244)

#define BANMGR_FILEVERSION	1
char const *g_pBanMgrFilename = "voice_ban.dt";



// Hash a player ID to a byte.
unsigned char HashPlayerID(char const playerID[16])
{
	unsigned char curHash = 0;

	for(int i=0; i < 16; i++)
		curHash += (unsigned char)playerID[i];

	return curHash;
}



CVoiceBanMgr::CVoiceBanMgr()
{
	Clear();
}


CVoiceBanMgr::~CVoiceBanMgr()
{
	Term();
}


bool CVoiceBanMgr::Init(char const *pGameDir)
{
	Term();

	char filename[512];
	_snprintf(filename, sizeof(filename), "%s/%s", pGameDir, g_pBanMgrFilename);

	// Load in the squelch file.
	FILE *fp = fopen(filename, "rb");
	if(fp)
	{
		int version;
		fread(&version, 1, sizeof(version), fp);
		if(version == BANMGR_FILEVERSION)
		{
			fseek(fp, 0, SEEK_END);
			int nIDs = (ftell(fp) - sizeof(version)) / 16;
			fseek(fp, sizeof(version), SEEK_SET);

			for(int i=0; i < nIDs; i++)
			{
				char playerID[16];
				fread(playerID, 1, 16, fp);
				AddBannedPlayer(playerID);
			}			
		}

		fclose(fp);
	}

	return true;
}


void CVoiceBanMgr::Term()
{
	// Free all the player structures.
	for(int i=0; i < 256; i++)
	{
		BannedPlayer *pListHead = &m_PlayerHash[i];
		BannedPlayer *pNext;
		for(BannedPlayer *pCur=pListHead->m_pNext; pCur != pListHead; pCur=pNext)
		{
			pNext = pCur->m_pNext;
			delete pCur;
		}
	}

	Clear();
}


void CVoiceBanMgr::SaveState(char const *pGameDir)
{
	// Save the file out.
	char filename[512];
	_snprintf(filename, sizeof(filename), "%s/%s", pGameDir, g_pBanMgrFilename);

	FILE *fp = fopen(filename, "wb");
	if(fp)
	{
		int version = BANMGR_FILEVERSION;
		fwrite(&version, 1, sizeof(version), fp);

		for(int i=0; i < 256; i++)
		{
			BannedPlayer *pListHead = &m_PlayerHash[i];
			for(BannedPlayer *pCur=pListHead->m_pNext; pCur != pListHead; pCur=pCur->m_pNext)
			{
				fwrite(pCur->m_PlayerID, 1, 16, fp);
			}
		}

		fclose(fp);
	}
}


bool CVoiceBanMgr::GetPlayerBan(char const playerID[16])
{
	return !!InternalFindPlayerSquelch(playerID);
}


void CVoiceBanMgr::SetPlayerBan(char const playerID[16], bool bSquelch)
{
	if(bSquelch)
	{
		// Is this guy already squelched?
		if(GetPlayerBan(playerID))
			return;
	
		AddBannedPlayer(playerID);
	}
	else
	{
		BannedPlayer *pPlayer = InternalFindPlayerSquelch(playerID);
		if(pPlayer)
		{
			pPlayer->m_pPrev->m_pNext = pPlayer->m_pNext;
			pPlayer->m_pNext->m_pPrev = pPlayer->m_pPrev;
			delete pPlayer;
		}
	}
}


void CVoiceBanMgr::ForEachBannedPlayer(void (*callback)(char id[16]))
{
	for(int i=0; i < 256; i++)
	{
		for(BannedPlayer *pCur=m_PlayerHash[i].m_pNext; pCur != &m_PlayerHash[i]; pCur=pCur->m_pNext)
		{
			callback(pCur->m_PlayerID);
		}
	}
}


void CVoiceBanMgr::Clear()
{
	// Tie off the hash table entries.
	for(int i=0; i < 256; i++)
		m_PlayerHash[i].m_pNext = m_PlayerHash[i].m_pPrev = &m_PlayerHash[i];
}


CVoiceBanMgr::BannedPlayer* CVoiceBanMgr::InternalFindPlayerSquelch(char const playerID[16])
{
	int index = HashPlayerID(playerID);
	BannedPlayer *pListHead = &m_PlayerHash[index];
	for(BannedPlayer *pCur=pListHead->m_pNext; pCur != pListHead; pCur=pCur->m_pNext)
	{
		if(memcmp(playerID, pCur->m_PlayerID, 16) == 0)
			return pCur;
	}

	return NULL;
}


CVoiceBanMgr::BannedPlayer* CVoiceBanMgr::AddBannedPlayer(char const playerID[16])
{
	BannedPlayer *pNew = new BannedPlayer;
	if(!pNew)
		return NULL;

	int index = HashPlayerID(playerID);
	memcpy(pNew->m_PlayerID, playerID, 16);
	pNew->m_pNext = &m_PlayerHash[index];
	pNew->m_pPrev = m_PlayerHash[index].m_pPrev;
	pNew->m_pPrev->m_pNext = pNew->m_pNext->m_pPrev = pNew;
	return pNew;
}

