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
#include <string.h>
#include <stdio.h>
#include "cl_entity.h"
#include "pm_shared.h"
#include "crosshair_defs.h"

const int iMaxFrames = 24;
// We get the players speed and the frametime from pm_shared.c
extern "C" 
{
	vec3_t export_speed; 
	float export_frametime;
}

HSPRITE hCrosshair;

int CHudCrosshair::Init(void)
{
	gHUD.AddHudElem(this);

	m_iFlags |= HUD_ACTIVE;

	return 1;
}

int CHudCrosshair::VidInit(void)
{
	// dynamic xhairs
	m_HUD_Crosshair1 = gHUD.GetSpriteIndex( "crosshair1" );
	m_HUD_Crosshair2 = gHUD.GetSpriteIndex( "crosshair2" );
	m_HUD_Crosshair3 = gHUD.GetSpriteIndex( "crosshair3" );
	m_HUD_Crosshair4 = gHUD.GetSpriteIndex( "crosshair4" );
	m_HUD_Crosshair5 = gHUD.GetSpriteIndex( "crosshair5" );
	m_HUD_Crosshair6 = gHUD.GetSpriteIndex( "crosshair6" );
	
	// static xhairs
	m_HUD_CrosshairStatic1 = gHUD.GetSpriteIndex( "statxhair1" );
	m_HUD_CrosshairStatic2 = gHUD.GetSpriteIndex( "statxhair2" );
	m_HUD_CrosshairStatic3 = gHUD.GetSpriteIndex( "statxhair3" );
	m_HUD_CrosshairStatic4 = gHUD.GetSpriteIndex( "statxhair4" );
	m_HUD_CrosshairStatic5 = gHUD.GetSpriteIndex( "statxhair5" );
	m_HUD_CrosshairStatic6 = gHUD.GetSpriteIndex( "statxhair6" );

	// custom xhairs
	m_HUD_CrosshairCustom1 = gHUD.GetSpriteIndex( "custxhair1" );
	m_HUD_CrosshairCustom2 = gHUD.GetSpriteIndex( "custxhair2" );
	m_HUD_CrosshairCustom3 = gHUD.GetSpriteIndex( "custxhair3" );

	// dynamic xhairs
	m_hSprite1 = gHUD.GetSprite( m_HUD_Crosshair1 );
	m_hSprite2 = gHUD.GetSprite( m_HUD_Crosshair2 );
	m_hSprite3 = gHUD.GetSprite( m_HUD_Crosshair3 );
	m_hSprite4 = gHUD.GetSprite( m_HUD_Crosshair4 );
	m_hSprite5 = gHUD.GetSprite( m_HUD_Crosshair5 );
	m_hSprite6 = gHUD.GetSprite( m_HUD_Crosshair6 );
	m_SpriteAreaDyn1 = &gHUD.GetSpriteRect( m_HUD_Crosshair1 );

	// static xhairs
	m_hSpriteStatic1 = gHUD.GetSprite( m_HUD_CrosshairStatic1 );
	m_hSpriteStatic2 = gHUD.GetSprite( m_HUD_CrosshairStatic2 );
	m_hSpriteStatic3 = gHUD.GetSprite( m_HUD_CrosshairStatic3 );
	m_hSpriteStatic4 = gHUD.GetSprite( m_HUD_CrosshairStatic4 );
	m_hSpriteStatic5 = gHUD.GetSprite( m_HUD_CrosshairStatic5 );
	m_hSpriteStatic6 = gHUD.GetSprite( m_HUD_CrosshairStatic6 );

	m_SpriteAreaStat1 = &gHUD.GetSpriteRect( m_HUD_CrosshairStatic1 );
	m_SpriteAreaStat2 = &gHUD.GetSpriteRect( m_HUD_CrosshairStatic2 );
	m_SpriteAreaStat3 = &gHUD.GetSpriteRect( m_HUD_CrosshairStatic3 );
	m_SpriteAreaStat4 = &gHUD.GetSpriteRect( m_HUD_CrosshairStatic4 );
	m_SpriteAreaStat5 = &gHUD.GetSpriteRect( m_HUD_CrosshairStatic5 );
	m_SpriteAreaStat6 = &gHUD.GetSpriteRect( m_HUD_CrosshairStatic6 );

	// custom xhairs
	m_hSpriteCustom1 = gHUD.GetSprite( m_HUD_CrosshairCustom1 );
	m_hSpriteCustom2 = gHUD.GetSprite( m_HUD_CrosshairCustom2 );
	m_hSpriteCustom3 = gHUD.GetSprite( m_HUD_CrosshairCustom3 );

	m_SpriteAreaCustom1 = &gHUD.GetSpriteRect( m_HUD_CrosshairCustom1 );
	m_SpriteAreaCustom2 = &gHUD.GetSpriteRect( m_HUD_CrosshairCustom2 );
	m_SpriteAreaCustom3 = &gHUD.GetSpriteRect( m_HUD_CrosshairCustom3 );

	m_flLastTime = 0;
	m_flMoving = 0;
	m_iFlags |= HUD_ACTIVE;
	
	return 1;
}



int CHudCrosshair::Draw(float flTime)
{
	if ( gHUD.m_iHideHUDDisplay & (HIDEHUD_ALL | HIDEHUD_WEAPONS) && g_iTeamNumber != SPECS)
		return 1;

	if(g_iUser1)
		return 1;


	static wrect_t nullrc;
	SetCrosshair( 0, nullrc, 0, 0, 0 );

	int r, g, b, x, y, play_frame;
	int x_length, y_length;

	UnpackRGB(r, g, b, 255);
	
	// init red
	r = 255;
	g = 0;
	b = 0;


	if (CVAR_GET_FLOAT("cl_dynamicxhair") == iDynHealthCrosshair)
	{
		float pulsespeed = 3 + (10 * (1 - (g_flHealth/100)));
		float helper = 10*sin(flTime*pulsespeed);
		if (helper > 8)
			r = 60;
		else r= 255;
		g = 0;
		b = 0;
	}
	if (CVAR_GET_FLOAT("cl_dynamicxhair") == iDynStaminaCrosshair)
	{
		float pulsespeed = 3 + (10 * (1 - (g_flStamina/100)));
		float helper = 10*sin(flTime*pulsespeed);
		if (helper > 7)
		{
			r = 0;
			b = 255;
		}
		else
		{
			r = 255;
			b = 0;
		}
		g = 0;
	}
	if (CVAR_GET_FLOAT("cl_dynamicxhair") == iDynRedCrosshair)
	{
		r = 255;
		g = 0;
		b = 0;
	}
	if (CVAR_GET_FLOAT("cl_dynamicxhair") == iDynBlueCrosshair)
	{
		r = 0;
		g = 0;
		b = 255;
	}
	if (CVAR_GET_FLOAT("cl_dynamicxhair") == iDynYellowCrosshair)
	{
		r = 255;
		g = 255;
		b = 0;
	}
	if (CVAR_GET_FLOAT("cl_dynamicxhair") == iDynBlackCrosshair)
	{
		r = 0;
		g = 0;
		b = 0;
	}
	if (CVAR_GET_FLOAT("cl_dynamicxhair") == iDynWhiteCrosshair)
	{
		r = 255;
		g = 255;
		b = 255;
	}

	if (CVAR_GET_FLOAT("cl_dynamicxhair") == iDynDiscoCrosshair)
	{
		r = 255*(cos(flTime*4)+1);
		g = 255*(sin(flTime*3)+1);
		b = 255*(sin(flTime*5)+1);
	}
	float flScale_Add, flScale_Sub, flTreshhold;

	if(g_iTeamNumber == BRITS)
	{
		switch (g_iPlayerClass)
		{
		case 1:
			// british commander
			flScale_Add = 5;//0.6;
			flScale_Sub = 2;//1.2;
			flTreshhold = 6;
			break;
		case 2:
			// british medium inf
			flScale_Add = 5;//0.4;
			flScale_Sub = 2;//0.85;
			flTreshhold = 6;
			break;
		case 3:
			// british heavy inf
			flScale_Add = 5;//0.8;
			flScale_Sub = 2;//0.65;
			flTreshhold = 6;
			break;
		default:
			return 1;
			break;
		}
	}
	else if(g_iTeamNumber == AMERS)
	{
		switch (g_iPlayerClass)
		{
		case 1:
			// american commander
			flScale_Add = 5;//0.55;
			flScale_Sub = 2;//1.1;
			flTreshhold = 6;
		break;
		case 2:
			// american medium inf
			flScale_Add = 5;//0.65;
			flScale_Sub = 2;//0.8;
			flTreshhold = 6;
			break;
		case 3:
			// american heavy inf
			flScale_Add = 5;//0.9;
			flScale_Sub = 2;//0.6;
			flTreshhold = 6;
			break;
		default:
			return 1;
			break;
		}
	}
	else
		return 1;

	// We scale the Treshhold so we have nice numbers above
	flTreshhold = 0.1f * flTreshhold;

	float flAverage = 0;

	if (export_speed[0] < 0)
		export_speed[0] = - export_speed[0];
	if (export_speed[1] < 0)
		export_speed[1] = - export_speed[1];
	if (export_speed[2] < 0)
		export_speed[2] = - export_speed[2];

	flAverage = Length(export_speed);
	
	// debug player movement
	//char text[255];
	//sprintf(text, "%.4g %.4g %.4g\n", export_speed[0], export_speed[1], export_speed[2]);
	//gEngfuncs.pfnConsolePrint(text);

	if ((flAverage*export_frametime) > flTreshhold )
	m_flMoving += export_frametime * flAverage * flScale_Add;

	else
	m_flMoving -= export_frametime * 300.0f * flScale_Sub;

	// m_flScale_add = crosshair "blur" factor , bigger = gets blurry faster 
	// m_flScale_sub = crosshair "sharpen" factor , bigger = gets sharp faster
	// m_flTreshhold = if the movement of the player is below this value the crosshair wont react at all
	// We want a totally clean crosshair when certain classes are crouching
	if(g_iTeamNumber == BRITS)
	{
		switch (g_iPlayerClass)
		{
		case 1: 
			if (!(g_bCrouched) && (m_flMoving < 12))
			m_flMoving =12;
			break;
		case 3: 
			if (!(g_bCrouched) && (m_flMoving < 12))
			m_flMoving =12;
			break;
		default:
			break;
		
		}
	}
	else if(g_iTeamNumber == AMERS)
	{
		switch (g_iPlayerClass)
		{
		case 1: 
			if (!(g_bCrouched) && (m_flMoving < 12))
			m_flMoving =12;
			break;
		case 2: 
			if (!(g_bCrouched) && (m_flMoving < 12))
			m_flMoving =12;
			break;
		default:
			break;
		}
	}
	if (m_flMoving < 0) 
		m_flMoving = 0;
	if (m_flMoving > 100) 
		m_flMoving = 100;
	

	play_frame = (int)(m_flMoving / 4);

	if (play_frame > iMaxFrames)
		play_frame = iMaxFrames;

	// Different crosshairs for different classes

	if (CVAR_GET_FLOAT("cl_dynamicxhair") < 10 )
	{
		y_length = gHUD.GetSpriteRect(m_HUD_Crosshair1).bottom - gHUD.GetSpriteRect(m_HUD_Crosshair1).top;
		x_length = gHUD.GetSpriteRect(m_HUD_Crosshair1).right - gHUD.GetSpriteRect(m_HUD_Crosshair1).left;
		if(g_iTeamNumber == BRITS)
		{
			switch (g_iPlayerClass)
			{
				case 1:
					// british commander
					SPR_Set(m_hSprite5, r, g, b);
					break;
				case 2:
					// british medium inf
					SPR_Set(m_hSprite3, r, g, b);
					break;
				case 3:
					// british heavy inf
					SPR_Set(m_hSprite1, r, g, b);
					break;
				default:
					return 1;
					break;
			}
		}
		else if(g_iTeamNumber == AMERS)
		{
			switch (g_iPlayerClass)
			{
				case 1:
					// american commander
					SPR_Set(m_hSprite6, r, g, b);
					break;
				case 2:
					// american medium inf
					SPR_Set(m_hSprite2, r, g, b);
					break;
				case 3:
					// american heavy inf
					SPR_Set(m_hSprite4, r, g, b);
					break;
				default:
					return 1;
					break;
			}
		}
		else
			return 1;
		// calculate crosshair position
		x = ScreenWidth / 2 - x_length / 2;
		y = ScreenHeight / 2 - y_length /2;
	
		SPR_DrawHoles(play_frame, x, y, m_SpriteAreaDyn1);
	}
	if (CVAR_GET_FLOAT("cl_dynamicxhair") == iStaticCrosshair )
	{
		if(g_iTeamNumber == BRITS)
		{
			switch (g_iPlayerClass)
			{
				case 1:
					// british commander
					SPR_Set(m_hSpriteStatic5, r, g, b);
					y_length = gHUD.GetSpriteRect(m_HUD_CrosshairStatic5).bottom - gHUD.GetSpriteRect(m_HUD_CrosshairStatic5).top;
					x_length = gHUD.GetSpriteRect(m_HUD_CrosshairStatic5).right - gHUD.GetSpriteRect(m_HUD_CrosshairStatic5).left;
					x = ScreenWidth / 2 - x_length / 2;
					y = ScreenHeight / 2 - y_length /2;
	
					SPR_DrawHoles(play_frame, x, y, m_SpriteAreaStat5);
					break;
				case 2:
					// british medium inf
					SPR_Set(m_hSpriteStatic3, r, g, b);
					y_length = gHUD.GetSpriteRect(m_HUD_CrosshairStatic3).bottom - gHUD.GetSpriteRect(m_HUD_CrosshairStatic3).top;
					x_length = gHUD.GetSpriteRect(m_HUD_CrosshairStatic3).right - gHUD.GetSpriteRect(m_HUD_CrosshairStatic3).left;
					x = ScreenWidth / 2 - x_length / 2;
					y = ScreenHeight / 2 - y_length /2;
	
					SPR_DrawHoles(play_frame, x, y, m_SpriteAreaStat3);
					break;
				case 3:
					// british heavy inf
					SPR_Set(m_hSpriteStatic1, r, g, b);
					y_length = gHUD.GetSpriteRect(m_HUD_CrosshairStatic1).bottom - gHUD.GetSpriteRect(m_HUD_CrosshairStatic1).top;
					x_length = gHUD.GetSpriteRect(m_HUD_CrosshairStatic1).right - gHUD.GetSpriteRect(m_HUD_CrosshairStatic1).left;
					x = ScreenWidth / 2 - x_length / 2;
					y = ScreenHeight / 2 - y_length /2;
	
					SPR_DrawHoles(play_frame, x, y, m_SpriteAreaStat1);
					break;
				default:
					return 1;
					break;
			}
		}
		else if(g_iTeamNumber == AMERS)
		{
			switch (g_iPlayerClass)
			{
				case 1:
					// american commander
					SPR_Set(m_hSpriteStatic6, r, g, b);
					y_length = gHUD.GetSpriteRect(m_HUD_CrosshairStatic6).bottom - gHUD.GetSpriteRect(m_HUD_CrosshairStatic6).top;
					x_length = gHUD.GetSpriteRect(m_HUD_CrosshairStatic6).right - gHUD.GetSpriteRect(m_HUD_CrosshairStatic6).left;
					x = ScreenWidth / 2 - x_length / 2;
					y = ScreenHeight / 2 - y_length /2;
	
					SPR_DrawHoles(play_frame, x, y, m_SpriteAreaStat6);
					break;
				case 2:
					// american medium inf
					SPR_Set(m_hSpriteStatic2, r, g, b);
					y_length = gHUD.GetSpriteRect(m_HUD_CrosshairStatic2).bottom - gHUD.GetSpriteRect(m_HUD_CrosshairStatic2).top;
					x_length = gHUD.GetSpriteRect(m_HUD_CrosshairStatic2).right - gHUD.GetSpriteRect(m_HUD_CrosshairStatic2).left;
					x = ScreenWidth / 2 - x_length / 2;
					y = ScreenHeight / 2 - y_length /2;
	
					SPR_DrawHoles(play_frame, x, y, m_SpriteAreaStat2);
					break;
				case 3:
					// american heavy inf
					SPR_Set(m_hSpriteStatic4, r, g, b);
					y_length = gHUD.GetSpriteRect(m_HUD_CrosshairStatic4).bottom - gHUD.GetSpriteRect(m_HUD_CrosshairStatic4).top;
					x_length = gHUD.GetSpriteRect(m_HUD_CrosshairStatic4).right - gHUD.GetSpriteRect(m_HUD_CrosshairStatic4).left;
					x = ScreenWidth / 2 - x_length / 2;
					y = ScreenHeight / 2 - y_length /2;
	
					SPR_DrawHoles(play_frame, x, y, m_SpriteAreaStat4);
					break;
				default:
					return 1;
					break;
			}
		}
		else
			return 1;
	}
	if (CVAR_GET_FLOAT("cl_dynamicxhair") == iCustom1Crosshair )
	{
		SPR_Set(m_hSpriteCustom1, r, g, b);

		y_length = gHUD.GetSpriteRect(m_HUD_CrosshairCustom1).bottom - gHUD.GetSpriteRect(m_HUD_CrosshairCustom1).top;
		x_length = gHUD.GetSpriteRect(m_HUD_CrosshairCustom1).right - gHUD.GetSpriteRect(m_HUD_CrosshairCustom1).left;
		x = ScreenWidth / 2 - x_length / 2;
		y = ScreenHeight / 2 - y_length /2;
	
		SPR_DrawHoles(play_frame, x, y, m_SpriteAreaCustom1);
	}
	if (CVAR_GET_FLOAT("cl_dynamicxhair") == iCustom2Crosshair )
	{
		SPR_Set(m_hSpriteCustom2, r, g, b);

		y_length = gHUD.GetSpriteRect(m_HUD_CrosshairCustom2).bottom - gHUD.GetSpriteRect(m_HUD_CrosshairCustom2).top;
		x_length = gHUD.GetSpriteRect(m_HUD_CrosshairCustom2).right - gHUD.GetSpriteRect(m_HUD_CrosshairCustom2).left;
		x = ScreenWidth / 2 - x_length / 2;
		y = ScreenHeight / 2 - y_length /2;
	
		SPR_DrawHoles(play_frame, x, y, m_SpriteAreaCustom2);

	}
	if (CVAR_GET_FLOAT("cl_dynamicxhair") == iCustom3Crosshair )
	{
		SPR_Set(m_hSpriteCustom3, r, g, b);
		
		y_length = gHUD.GetSpriteRect(m_HUD_CrosshairCustom3).bottom - gHUD.GetSpriteRect(m_HUD_CrosshairCustom3).top;
		x_length = gHUD.GetSpriteRect(m_HUD_CrosshairCustom3).right - gHUD.GetSpriteRect(m_HUD_CrosshairCustom3).left;
		x = ScreenWidth / 2 - x_length / 2;
		y = ScreenHeight / 2 - y_length /2;
	
		SPR_DrawHoles(play_frame, x, y, m_SpriteAreaCustom3);

	}
	return 1;
}