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

// This file was created on the 21/9/2K+2 at 22:05 by Ben Banfield for the Battle Grounds modification of Half-Life
// Its purpose is to draw the middle section of the hud

#include "STDIO.H"
#include "STDLIB.H"

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include <string.h>

#include "vgui_int.h"
#include "vgui_TeamFortressViewport.h"
#include "vgui_ServerBrowser.h"
#include "vgui_ScorePanel.h"

int CHudMiddle::Init()
{
	// ben - this shouldn't be a hud element
//	gHUD.AddHudElem(this);

	return 1;
}

int CHudMiddle::VidInit()
{
	m_HUD_Middle = gHUD.GetSpriteIndex( "ctb" );
	m_hSprite = gHUD.GetSprite( m_HUD_Middle );
	m_SpriteArea = &gHUD.GetSpriteRect( m_HUD_Middle );

	return 1;
}

int CHudMiddle::Draw(float flTime)
{
	int r = 0, g = 0, b = 0, x = 0, y = 0;

	int iBritish = 0, iBritish_1 = 0, iBritish_10 = 0, iBritish_100 = 0, iBritish_1000 = 0;
	int iAmericans = 0, iAmericans_1 = 0, iAmericans_10 = 0, iAmericans_100 = 0, iAmericans_1000 = 0;

	int x_length = 0, y_length = 0;      // positions of the spots

	y_length = gHUD.GetSpriteRect(m_HUD_Middle).bottom - gHUD.GetSpriteRect(m_HUD_Middle).top;
	x_length = gHUD.GetSpriteRect(m_HUD_Middle).right - gHUD.GetSpriteRect(m_HUD_Middle).left;

	UnpackRGB(r, g, b, RGB_WHITEISH);

	// position the sprite in the upper right corner of display...
	x = 83;

	y = (YRES(480) - (y_length + (y_length /4) + 3));

	SPR_Set(m_hSprite, r, g, b);
	SPR_DrawHoles(0, x, y, m_SpriteArea);

	// BP - Score Points start

	gViewPort->m_pScoreBoard->ExportTeamScores(iBritish, iAmericans);

	y = ScreenHeight - 60;
	
	x = 97;
	
	if (iBritish > 9)
		x -= 5;

	if (iBritish > 99)
		x -= 2;

	if (iBritish > 999)
		x += 0;

	iBritish_1000 = (int) iBritish / 1000;
	iBritish -= iBritish_1000 * 1000;

	iBritish_100 = (int) iBritish / 100;
	iBritish -= iBritish_100 * 100;

	iBritish_10 = (int) iBritish / 10;
	iBritish -= iBritish_10 * 10;

	iBritish_1 = (int) iBritish;

	int h = 0;
	if (iBritish_1000)
	{
		h = gHUD.DrawHudNumber(x, y, DHN_DRAWZERO, iBritish_1000, r, g, b, 2);
	}

	if (iBritish_100 || iBritish_1000)
	{
		x += 8;
		h = gHUD.DrawHudNumber(x, y, DHN_DRAWZERO, iBritish_100, r, g, b, 2);
	}

	if (iBritish_10 || iBritish_100  || iBritish_1000)
	{
		x += 8;
		h = gHUD.DrawHudNumber(x, y, DHN_DRAWZERO, iBritish_10, r, g, b, 2);
	}

	x += 8;
	h = gHUD.DrawHudNumber(x, y, DHN_DRAWZERO, iBritish_1, r, g, b, 2);

	x = 130;

	if (iAmericans > 9)
		x -= 5;

	if (iAmericans > 99)
		x -= 3;

	if (iAmericans > 999)
		x += 3;

	iAmericans_1000 = (int) iAmericans / 1000;
	iAmericans -= iAmericans_1000 * 1000;

	iAmericans_100 = (int) iAmericans / 100;
	iAmericans -= iAmericans_100 * 100;

	iAmericans_10 = (int) iAmericans / 10;
	iAmericans -= iAmericans_10 * 10;

	iAmericans_1 = (int) iAmericans;

	if (iAmericans_1000) 
	{
		h = gHUD.DrawHudNumber(x, y, DHN_DRAWZERO, iAmericans_1000, r, g, b, 2);
	}

	if (iAmericans_100 || iAmericans_1000)
	{
		x += 8;
		h = gHUD.DrawHudNumber(x, y, DHN_DRAWZERO, iAmericans_100, r, g, b, 2);
	}
	if (iAmericans_10 || iAmericans_100  || iAmericans_1000)
	{
		x += 8;
		h = gHUD.DrawHudNumber(x, y, DHN_DRAWZERO, iAmericans_10, r, g, b, 2);
	}

	x += 8;
	h = gHUD.DrawHudNumber(x, y, DHN_DRAWZERO, iAmericans_1, r, g, b, 2);

	// BP - Score Points end
	return 1;
}