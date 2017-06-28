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

//This file was created on the 22/6/2k+2 at 18:15 by Ben Banfield for the Battle Grounds modification of Half-Life
//Its purpose is to display the player scurrent stamina

#include "STDIO.H"
#include "STDLIB.H"

#include "hud.h"
#include "cl_util.h"
#include "com_model.h"
#include "studio.h"
#include "parsemsg.h"
#include <string.h>

#include "vgui_int.h"
#include "vgui_TeamFortressViewport.h"
#include "vgui_ServerBrowser.h"

#include "studio_util.h"
#include "r_studioint.h"

extern engine_studio_api_t IEngineStudio;
extern CHudHintbox		gHintBox;

int CHudStamina::Init()
{
	return 1;
}

int CHudStamina::VidInit()
{
	m_HUD_stamina = gHUD.GetSpriteIndex( "stamina" );
	m_hSprite = gHUD.GetSprite( m_HUD_stamina );
	m_SpriteArea = &gHUD.GetSpriteRect( m_HUD_stamina );

	return 1;
}

int CHudStamina::Draw(float flTime)
{
	int r = 0, g = 0, b = 0, x = 0, y = 0;
	int x_length = 0, y_length = 0;      // positions of the spots

	y_length = gHUD.GetSpriteRect(m_HUD_stamina).bottom - gHUD.GetSpriteRect(m_HUD_stamina).top;
	x_length = gHUD.GetSpriteRect(m_HUD_stamina).right - gHUD.GetSpriteRect(m_HUD_stamina).left;

	UnpackRGB(r, g, b, RGB_WHITEISH);

	if(ScreenWidth >= 640)
		x = 35;
	else
		x = 17;

	if ((g_flStamina / MAX_STAMINA) < 0.2)
		gHintBox.Set(HINTBOX_LOWSTAMINA);
	
	y = (YRES(480) - (y_length + ((ScreenWidth >= 640) ? 5 : 3)));

	if(!IEngineStudio.IsHardware())
	{
		y = (YRES(480) - (20 + ((ScreenWidth >= 640) ? 5 : 3)));
		x += 50;

		gHUD.DrawHudNumber(x, y, DHN_2DIGITS | DHN_DRAWZERO, g_flStamina, r, g, b);
		return 1;
	}

	SPR_Set(m_hSprite, r, g, b);

	int iClipLevel = (y_length - ((g_flStamina * y_length) / MAX_STAMINA));
	//clipper starts at the clipping level. its height is the full amount of health take away the mamount clipped
	SPR_EnableScissor(x, y+iClipLevel, 24 , y_length-iClipLevel);
	SPR_DrawHoles(0, x, y, m_SpriteArea);
	SPR_DisableScissor();
	return 1;
}