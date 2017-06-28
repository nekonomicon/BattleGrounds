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
//
//  hud_msg.cpp

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "r_efx.h"
#include "vgui_TeamFortressViewport.h"
#include "vgui_ScorePanel.h"

extern "C" int gHullsFixed;

//LRC - the fogging fog
float g_fFogColor[3];
float g_fStartDist = 0;
float g_fEndDist = 0;
//int g_iFinalStartDist; //for fading
int g_iFinalEndDist = 0;   //for fading
float g_fFadeDuration = 0; //negative = fading out

#define MAX_CLIENTS 32


/// USER-DEFINED SERVER MESSAGE HANDLERS

int CHud :: MsgFunc_ResetHUD(const char *pszName, int iSize, void *pbuf )
{
	ASSERT( iSize == 0 );

	// clear all hud data
	HUDLIST *pList = m_pHudList;

	while ( pList )
	{
		if ( pList->p )
			pList->p->Reset();
		pList = pList->pNext;
	}

	// reset sensitivity
	m_flMouseSensitivity = 0;

	// reset concussion effect
	m_iConcussionEffect = 0;

	// reset hulls
	gHullsFixed = 0;

	// reset fog
	g_fStartDist = 0;
	g_fEndDist = 0;
	g_iFinalEndDist = 0;
	g_fFadeDuration = 0;

	return 1;
}

void CAM_ToFirstPerson(void);

void CHud :: MsgFunc_ViewMode( const char *pszName, int iSize, void *pbuf )
{
	CAM_ToFirstPerson();
}

void CHud :: MsgFunc_InitHUD( const char *pszName, int iSize, void *pbuf )
{
	// prepare all hud data
	HUDLIST *pList = m_pHudList;

	while (pList)
	{
		if ( pList->p )
			pList->p->InitHUDData();
		pList = pList->pNext;
	}

	//Probably not a good place to put this.
//	pBeam = pBeam2 = NULL;
}

//LRC
int CHud::MsgFunc_SetFog( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );

	for ( int i = 0; i < 3; i++ )
		 g_fFogColor[ i ] = READ_BYTE();

	g_fFadeDuration = READ_SHORT();
	g_fStartDist = READ_SHORT();

	if (g_fFadeDuration > 0)
	{
//		// fading in
		g_iFinalEndDist = READ_SHORT();
		g_fEndDist = FOG_LIMIT;
	}
	else if (g_fFadeDuration < 0)
	{
//		// fading out
		g_iFinalEndDist = g_fEndDist = READ_SHORT();
	}
	else
	{
		g_fEndDist = READ_SHORT();
	}
	return 1;
}


int CHud :: MsgFunc_GameMode(const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );
	m_Teamplay = READ_BYTE();

	return 1;
}


int CHud :: MsgFunc_Damage(const char *pszName, int iSize, void *pbuf )
{
	int		armor, blood;
	Vector	from;
	int		i;
	float	count;
	
	BEGIN_READ( pbuf, iSize );
	armor = READ_BYTE();
	blood = READ_BYTE();

	for (i=0 ; i<3 ; i++)
		from[i] = READ_COORD();

	count = (blood * 0.5) + (armor * 0.5);

	if (count < 10)
		count = 10;

	// TODO: kick viewangles,  show damage visually

	return 1;
}

int CHud :: MsgFunc_Concuss( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );
	m_iConcussionEffect = READ_BYTE();
	if (m_iConcussionEffect)
		this->m_StatusIcons.EnableIcon("dmg_concuss",255,160,0);
	else
		this->m_StatusIcons.DisableIcon("dmg_concuss");
	return 1;
}

//Ben
void CHud::MsgFunc_Dead(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ( pbuf, iSize);

	short client = READ_BYTE();
	short dead_state = READ_BYTE();

	if(dead_state == 2)
	{
		g_PlayerExtraInfo[client].dead = true;
	}
	else
	{
		g_PlayerExtraInfo[client].dead = false;
	}
	gViewPort->m_pScoreBoard->Update();
//	m_CommanderMenu.Die();
}

// Ben Addition to Raiko Contrib - Melee Only
void CHud::MsgFunc_MeleeOnly( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize);

	g_iMeleeOnly = READ_BYTE();
}
// Ben End

// Raiko Contrib - Stationary Reload
void CHud::MsgFunc_SReload(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);

	g_iStationaryReload = READ_BYTE();
}
// Raiko End

// BP: Laser Tag
void CHud::MsgFunc_LaserTag( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize);

	g_iLaserTag = READ_BYTE();
}
// BP End