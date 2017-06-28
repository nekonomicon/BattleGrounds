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

//This file was created on the 20th of April 2003 at 23:00 by Ryan Walkowski for the Battle Grounds modification for Half-Life.
//Its purpose is to display the arm and leg shot to the screen and also controls the water hud when under water

#include "STDIO.H"
#include "STDLIB.H"
#include "MATH.H"

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include <string.h>

#include "vgui_int.h"
#include "vgui_TeamFortressViewport.h"
#include "vgui_ServerBrowser.h"

DECLARE_MESSAGE(m_ArmLegShot, ArmLegShot)


int CHudArmLegShot::Init(void)
{
	HOOK_MESSAGE(ArmLegShot);
	m_iFlags |= HUD_ACTIVE;

	for(m_iShotCount = 0; m_iShotCount < 10; m_iShotCount++)
	{
		m_iShots[m_iShotCount] = SHOT_NONE;
	}
	m_iShotCount = 0;

	memset( &m_iShots, 0, sizeof(m_iShots));
	
	gHUD.AddHudElem(this);
	
	return 1;
};

int CHudArmLegShot::VidInit(void)
{
	m_HUD_armlegshot = gHUD.GetSpriteIndex("armlegshot");

	return 1;
};

int CHudArmLegShot::MsgFunc_ArmLegShot(const char *pszName, int iSize, void *pbuf )
{ 
	BEGIN_READ( pbuf, iSize ); 
	int x = READ_BYTE(); 
	
	if(x != SHOT_CLEAR)
	{
		if(m_iShotCount < 10)
		{
			m_iShots[m_iShotCount] = x;
			m_iShotCount++;
			return 1;
		}
	}
	for(m_iShotCount = 0; m_iShotCount < 10; m_iShotCount++)
	{
		m_iShots[m_iShotCount] = SHOT_NONE;
	}
	m_iShotCount = 0;

	return 1;
}

int CHudArmLegShot::Draw(float flTime)
{
	return 1;

/*	if ( gHUD.m_iHideHUDDisplay & ( HIDEHUD_ALL ) )
		return 1;
	if (!(gHUD.m_iWeaponBits & (1<<(WEAPON_SUIT)) ))
		return 1;
	
	int r, g, b, x, y; 
	UnpackRGB(r, g, b, RGB_WHITEISH);
	ScaleColors(r, g, b, 255);

	y = ScreenHeight-250;
	x = ScreenWidth/48;


	for(m_iShotCount = 0; m_iShotCount < 10; m_iShotCount++)
	{
		if(m_iShots[m_iShotCount] != SHOT_NONE)
		{
			SPR_Set(gHUD.GetSprite(m_HUD_armlegshot), 255, 255, 255);
			SPR_DrawHoles( m_iShots[m_iShotCount] + 1, x, y,NULL);
			y -= ((gHUD.GetSpriteRect(m_HUD_armlegshot).top - gHUD.GetSpriteRect(m_HUD_armlegshot).bottom) + 5);
		}
	}
	
	return 1;
	*/
}