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

//This file was created on the 22/6/2K+2 at 21:44 by Ben Banfield for the Battle Grounds modification of half-Life
//its purpose is to define the class which will display the current round time on the hud

#include "STDIO.H"
#include "STDLIB.H"

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include <string.h>

#include "vgui_int.h"
#include "vgui_TeamFortressViewport.h"
#include "vgui_ServerBrowser.h"
#include "vgui_SpectatorPanel.h"
#include "cvardef.h"
#include "usercmd.h"
#include "const.h"

bool g_bUpdateLastClockTime = false;

const char szSize = 5;

//DECLARE_MESSAGE(m_Clock, PlayClock )
DECLARE_MESSAGE(m_Clock, ClockTime )

int CHudClock::Init(void)
{
	g_ShowClock = gEngfuncs.pfnRegisterVariable("cl_showclock", "1", FCVAR_ARCHIVE | FCVAR_USERINFO );

//	HOOK_MESSAGE( PlayClock );
	HOOK_MESSAGE( ClockTime );

	m_iFlags |= HUD_ACTIVE;

	gHUD.AddHudElem( this );

	m_iClockState = 1;
	m_iCurrentClockTime = 0;
	m_iClockTime = 0;
	m_iLastClockUpdate = 0;
	return 1;
}

int CHudClock::VidInit(void)
{
	m_HUD_Sprite = gHUD.GetSpriteIndex("number_0");

	m_HUD_Colon = gHUD.GetSpriteIndex("colon_5");
	m_hColonSprite = gHUD.GetSprite(m_HUD_Colon);
	m_ColonArea = &gHUD.GetSpriteRect(m_HUD_Colon);

	return 1;
}

void CHudClock::Reset(){}

// PAT: not the best way to do this, but the variables are only
// accessible this way. i really recommend throwing the var in ONCE
// and counting down in the draw function (flTime will help).

// Trust me, we learned the hard way in TOD.
/* ben - removed we are doing this via a client cvar now
int CHudClock::MsgFunc_PlayClock(const char *pszName,  int iSize, void *pbuf )
{
    BEGIN_READ( pbuf, iSize );
    m_iShowClock = READ_BYTE();

    if (m_iShowClock == 1)
	m_iFlags |= HUD_ACTIVE;
    else
	m_iFlags &= ~HUD_ACTIVE;

    return 1;
}
*/

int CHudClock::MsgFunc_ClockTime(const char *pszName,  int iSize, void *pbuf )
{
    BEGIN_READ( pbuf, iSize );

	//time on the clock
	m_iClockTime = READ_SHORT();
	//pause time or round time?
	m_iClockState = READ_SHORT();

	m_iFlags |= HUD_ACTIVE;

	g_bUpdateLastClockTime = true;

	return 1;
}

int CHudClock::Draw(float flTime)
{
	if(g_bUpdateLastClockTime)
	{
		g_bUpdateLastClockTime = false;
		m_iLastClockUpdate = flTime;
	}

	//get how much time has elapsed since we were given a time to count down from
	int iTemp = flTime - m_iLastClockUpdate;
	// remove that amount of time from the amount we were given
	m_iCurrentClockTime = m_iClockTime - iTemp;

	if(m_iCurrentClockTime < 0)
		m_iCurrentClockTime = 0;
	
	if ( gHUD.m_iHideHUDDisplay & (HIDEHUD_HEALTH | HIDEHUD_ALL) )
		return 1;

	if(g_ShowClock->value < 1)
		return 1;

	if(g_iUser1 != 0)
		return 1;

	int x = 0, y = 0;	// screen position
	int r = 0, g = 0, b = 0;// colours

	// ben - 0 is round time, should add a constant in bg_defs.h
	if(m_iClockState == CLOCK_ROUND)
		UnpackRGB(r,g,b, RGB_WHITEISH);
	//pausetime
	else
		UnpackRGB(r,g,b, RGB_REDISH);

	y = ScreenHeight - 41;

	x = 96;
	x = gHUD.DrawHudNumber(x, y, DHN_2DIGITS | DHN_DRAWZERO, m_iCurrentClockTime/60, r, g, b, 2);

	// Draw the ":" between the MM and SS
	x = gHUD.DrawHudString(x, y-3, ScreenWidth, ":", r, g, b);

	x = gHUD.DrawHudNumber(x, y, DHN_2DIGITS | DHN_PREFIXZERO, m_iCurrentClockTime%60, r, g, b, 2);
	
	return 1;
}
