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

//This file was created on the 5/5/03 at 12:05 by Ben Banfield for the Battle Grounds modification for Half-Life
//Its purpose is to define class which will store player origins
//The reason behind this is that I think the player origin isn't updated straight away.
//We have to await the engine deciding it wants to update the mod copy before our info is accurate

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "saverestore.h"
#include "player.h"
#include "client.h"

COriginTracker *pOriginTracker = new COriginTracker;


COriginTracker::COriginTracker()
{
	for(int i = 0; i <= 32; i++)
	{
		m_pPlayerOrigins[i] = new Vector(0, 0, 0);
	}
}

COriginTracker::~COriginTracker()
{
	for(int i = 0; i <= 32; i++)
	{
		delete m_pPlayerOrigins[i];
		m_pPlayerOrigins[i] = NULL;
	}
}

void COriginTracker::UpdatePlayer(vec3_t origin, int iPlayerIndex)
{
	if(iPlayerIndex > 0 && iPlayerIndex < 33)
	{
		delete m_pPlayerOrigins[iPlayerIndex];
		m_pPlayerOrigins[iPlayerIndex] = new Vector(origin.x, origin.y, origin.z);
	}
}

vec3_t *COriginTracker::GetPlayerOrigin(int iPlayerIndex)
{
	if(m_pPlayerOrigins[iPlayerIndex])
	{
		return m_pPlayerOrigins[iPlayerIndex];
	}
	return NULL;
}
