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

//This file was created on the 22/6/2k+2 at 14:02 by Ben Banfield for the Battle Grounds modification for Half-Life.
//Its purpose is to define the class which draws the hud background to the screen

#include "STDIO.H"
#include "STDLIB.H"

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include <string.h>

#include "vgui_int.h"
#include "vgui_TeamFortressViewport.h"
#include "vgui_ServerBrowser.h"

extern int g_iWaterLevel;

int CHudBackground::Init()
{
	// ben - this shouldn't be a hud element
//	gHUD.AddHudElem(this);

	return 1;
}

int CHudBackground::VidInit()
{
	m_HUD_back = gHUD.GetSpriteIndex( "hudback" );
	m_HUD_water = gHUD.GetSpriteIndex( "water" );
	m_hSprite = gHUD.GetSprite( m_HUD_back );
	m_SpriteArea = &gHUD.GetSpriteRect( m_HUD_back );

	return 1;
}

int CHudBackground::Draw(float flTime)
{
	int r = 0, g = 0, b = 0, x = 0, y = 0;
	int x_length = 0, y_length = 0;      // positions of the spots

	y_length = gHUD.GetSpriteRect(m_HUD_back).bottom - gHUD.GetSpriteRect(m_HUD_back).top;
	x_length = gHUD.GetSpriteRect(m_HUD_back).right - gHUD.GetSpriteRect(m_HUD_back).left;

	UnpackRGB(r, g, b, RGB_WHITEISH);
	ScaleColors(r, g, b, 255);
	// position the sprite in the upper right corner of display...
	x = XRES(0);

	y = (YRES(480) - y_length);

	if (g_iWaterLevel != 3)
	{
		y_length = gHUD.GetSpriteRect(m_HUD_back).bottom - gHUD.GetSpriteRect(m_HUD_back).top;
		x_length = gHUD.GetSpriteRect(m_HUD_back).right - gHUD.GetSpriteRect(m_HUD_back).left;

		SPR_Set(m_hSprite, r, g, b);
		SPR_DrawHoles(0, x, y, m_SpriteArea);
		return 1;
	}
	else
	{
		y_length = gHUD.GetSpriteRect(m_HUD_water).bottom - gHUD.GetSpriteRect(m_HUD_water).top;
		x_length = gHUD.GetSpriteRect(m_HUD_water).right - gHUD.GetSpriteRect(m_HUD_water).left;

		SPR_Set(gHUD.GetSprite(m_HUD_water), r, g, b);
		SPR_DrawHoles((((int)(flTime*14))%24), x, y, m_SpriteArea);
		return 0;
	}
}