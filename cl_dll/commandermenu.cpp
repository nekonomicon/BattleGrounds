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

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "MATH.H"
#include "STDLIB.H"
#include "com_model.h"
#include "studio.h"
#include "vgui_int.h"
#include "studio_util.h"
#include "r_studioint.h"
#include <math.h>
#include "cl_entity.h"
#include <string.h>
#include <stdio.h>
#include "bg_defs.h"


int CHudCommanderMenu::Init(void)
{
	gHUD.AddHudElem(this);
	m_iFlags |= HUD_ACTIVE;

	m_iPointsPerSecond = 0;
	m_flTotalPoints = 0;
	m_flSpendablePoints = 0;
	m_iRush = 0;
	m_iArtillery = 0;
	m_iRush_Max = RUSH_COST;
	m_iArtillery_Max = 500;
	m_bResetTime = true;
	return 1;
}

int CHudCommanderMenu::Die(void)
{
	m_bArtilleryMessage = false;
	m_bRushMessage = false;
	m_iPointsPerSecond = 0;
	m_flTotalPoints = 0;
	m_flSpendablePoints = 0;
	m_iRush = 0;
	m_iArtillery = 0;
	m_bResetTime = true;
	return 1;
}

int CHudCommanderMenu::VidInit(void)
{
	return 1;
}

//Pre sends execution request to server
//Post is called once the server returned that the execution was sucessful

int CHudCommanderMenu::PostExecute(int special)
{
	if (special == iRush)
	{
		m_iRush = 0;
		m_bRushMessage = false;
		return 1;
	}
	
	if (special == iArtillery)
	{
		m_iArtillery = 0;
		m_bArtilleryMessage = false;
		return 1;
	}

	return 0;
}

int CHudCommanderMenu::PreExecute(int special)
{
	if (g_iPlayerClass != 1)
		return 0;

	//BP commanders menu special abilities
	
	//const int iRush = 1;
	//const int iAccuracy = 2;
	//const int iDamage = 3;
	//const int iArtillery = 4;
	//const int iMarkEnemy = 5;
	//const int iReinforcements = 6;

	if ((special == iRush) && (m_iRush == m_iRush_Max))
	{
		//send clientcommand
		return 1;
	}
	
	if ((special == iArtillery) && (m_iArtillery == m_iArtillery_Max))
	{
		
		//send clientcommand
		return 1;
	}	
	return 0;
}

int CHudCommanderMenu::Fill(int special)
{
	if (special == iRushFull)
		m_bRushMessage = true;
	
	if (special == iArtilleryFull)
		m_bArtilleryMessage = true;

	return 1;
}

int CHudCommanderMenu::Draw(float flTime)
{
	if ( gHUD.m_iHideHUDDisplay & (HIDEHUD_ALL | HIDEHUD_WEAPONS) && g_iTeamNumber != SPECS)
		return 1;

	if(g_iUser1)
		return 1;

	if (g_iPlayerClass != 1)
		return 1;

	if (m_bResetTime)
	{
		m_flLastTime = flTime;
		m_bResetTime = false;
	}

	int captflagcount = 3;
	int totalflagcount = 5;
	
	char captflags[255];
	sprintf (captflags,"captured flags: %d of %d", captflagcount, totalflagcount);
	
	if (totalflagcount < 1)	// to prevent "divided by zero" error
		totalflagcount = 1;
	
	m_iPointsPerSecond = 5 + (int)((float)captflagcount/(float)totalflagcount * 15.0f);
	m_flTotalPoints += m_iPointsPerSecond * (flTime - m_flLastTime);
	m_flSpendablePoints += m_iPointsPerSecond * (flTime - m_flLastTime); 
	m_flLastTime = flTime;
	
	char pointsadded[255];
	sprintf (pointsadded,"points / second: %d", m_iPointsPerSecond);
	
	char totalpoints[255];
	sprintf (totalpoints,"total points: %d", (int)m_flTotalPoints);
	
	char healpoints[255];
	int healingpoints = (int)((float)captflagcount/(float)totalflagcount * 5.0f);
	sprintf (healpoints,"you can heal: %d points/second", healingpoints);
	
	bool allfull = false;

	while (((int)m_flSpendablePoints > 0) && (!allfull))
	{
		if ((int)m_flSpendablePoints > 0)
		{
			if (!m_bAddedRush)
			{
				m_iRush++;
				if (m_iRush > (m_iRush_Max-1))
					if (m_bRushMessage)
						m_iRush = m_iRush_Max;
					else
					{
						m_flSpendablePoints--;
						m_iRush = m_iRush_Max-1;
					}
				else 
					m_flSpendablePoints--;
				m_bAddedRush = true;
			}
		}
		
		if ((int)m_flSpendablePoints > 0)
		{
			if (!m_bAddedArtillery)
			{
				m_iArtillery++;
				if (m_iArtillery > (m_iArtillery_Max-1))
					if (m_bArtilleryMessage)
						m_iArtillery = m_iArtillery_Max;
					else 
					{
						m_flSpendablePoints--;
						m_iArtillery = (m_iArtillery_Max-1);
					}
				else 
					m_flSpendablePoints--;
				m_bAddedArtillery = true;
			}
		}	
		
		if(m_bAddedArtillery && m_bAddedRush)
		{		
				m_bAddedRush = false;
				m_bAddedArtillery = false;
		}

		if ((m_iRush >= (m_iRush_Max-1)) && (m_iArtillery >=	 (m_iArtillery_Max-1)))
		{
			allfull = true;
			m_flSpendablePoints = 0;
		}
	}

	char rushpoints[255];
	sprintf (rushpoints,"Rush: %d / %d", m_iRush, m_iRush_Max);
	
	char artillerypoints[255];
	sprintf (artillerypoints,"Artillery: %d / %d", m_iArtillery, m_iArtillery_Max);
	
	int x = ScreenWidth - 200;
	int y = 20;
	
	gEngfuncs.pfnDrawSetTextColor( 255, 255, 255 );
	DrawConsoleString( x, y, "Commanders Menu:" );
	y += 20;
	gEngfuncs.pfnDrawSetTextColor( 255, 255, 255 );
	DrawConsoleString( x, y, captflags );
	y += 13;
	gEngfuncs.pfnDrawSetTextColor( 255, 255, 255 );
	DrawConsoleString( x, y, pointsadded );
	y += 13;
	gEngfuncs.pfnDrawSetTextColor( 255, 255, 255 );
	DrawConsoleString( x, y, totalpoints );
	y += 20;
	gEngfuncs.pfnDrawSetTextColor( 255, 255, 255 );
	DrawConsoleString( x, y, healpoints );
	y += 13;
	if (m_iRush == m_iRush_Max)
		gEngfuncs.pfnDrawSetTextColor( 255, 0, 0 );
	else
		gEngfuncs.pfnDrawSetTextColor( 255, 255, 255 );
	DrawConsoleString( x, y, rushpoints );
	y += 13;
	if (m_iArtillery == m_iArtillery_Max)
		gEngfuncs.pfnDrawSetTextColor( 255, 0, 0 );
	else 
		gEngfuncs.pfnDrawSetTextColor( 255, 255, 255 );
	DrawConsoleString( x, y, artillerypoints );

	return 1;
}



/*
Notes:

todo:

get_total_flags();
get_captured_flags( team );
buttons
commander client -> server message
server -> all clients message
server healing checks

*/