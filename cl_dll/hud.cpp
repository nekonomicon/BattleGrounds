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
// hud.cpp
//
// implementation of CHud class
//

#include "hud.h"
#include "cl_util.h"
#include <string.h>
#include <stdio.h>
#include "parsemsg.h"
#include "hud_servers.h"
#include "vgui_int.h"
#include "vgui_TeamFortressViewport.h"

#include "demo.h"
#include "demo_api.h"
#include "vgui_scorepanel.h"
#include "particle_header.h"

extern cvar_t *g_ShowFog;
extern cvar_t *g_HintBoxLevel;
extern cvar_t *g_ShowPoints;
extern cvar_t *g_Text_ypos;

// SLUGGO: messaging is borked if this isn't a member var of 
// gHUD (or CHud class for you C++ dudes) - leave in CHUD def'n

// ben 5/9/2k+2
// ben - the stamina and the background don't use hud messeging and they won't get called by draw if no msg has been sent to them, so i had to make this hack.

// ben2 - maybe you are right. I have been debuging baer's crash and some wierd things are happening
// if i put m_Clock.Init() as the first element to be init, it crashes when it tries to reset the hud,
// however if i put it elsewhere it crashes on init. Now I don't know why m_Points didn't have this problem
// but i will try and make m_BackGround and m_Stamina seperate from anything but the health.

// ben3 - problem still occurs after doing that. The vftable is not valid for some reason. I don't see how hud elements can create access violations as the hud keeps track of adding and removing them
// and this element looks fine and like any other

// ben4 - well i seem to have fixed it. I moved CHudClock m_Clock to being the first hud element defined in the CHud class definition and things worked fine.
// I have no idea why the heck it should do this any answers on this matter would be helpful
// ben 5/9/2K+2 end

// ben 23/4/03
// ben - Finally solved this problem.  It was a buffer overrun in m_Points.Init(), dastardly thing, everything should be fine now, about to test it

class CHLVoiceStatusHelper : public IVoiceStatusHelper
{
public:
	virtual void GetPlayerTextColor(int entindex, int color[3])
	{
		color[0] = color[1] = color[2] = 255;

		if( entindex >= 0 && entindex < sizeof(g_PlayerExtraInfo)/sizeof(g_PlayerExtraInfo[0]) )
		{
			int iTeam = g_PlayerExtraInfo[entindex].teamnumber;

			if ( iTeam < 0 )
			{
				iTeam = 0;
			}

			iTeam = iTeam % iNumberOfTeamColors;

			color[0] = iTeamColors[iTeam][0];
			color[1] = iTeamColors[iTeam][1];
			color[2] = iTeamColors[iTeam][2];
		}
	}

	virtual void UpdateCursorState()
	{
		gViewPort->UpdateCursorState();
	}

	virtual int	GetAckIconHeight()
	{
		return ScreenHeight - gHUD.m_iFontHeight*3 - 6;
	}

	virtual bool			CanShowSpeakerLabels()
	{
		if( gViewPort && gViewPort->m_pScoreBoard )
			return !gViewPort->m_pScoreBoard->isVisible();
		else
			return false;
	}
};
static CHLVoiceStatusHelper g_VoiceStatusHelper;


extern client_sprite_t *GetSpriteList(client_sprite_t *pList, const char *psz, int iRes, int iCount);

extern cvar_t *sensitivity;
cvar_t *cl_lw = NULL;

void ShutdownInput (void);

//DECLARE_MESSAGE(m_Logo, Logo)
int __MsgFunc_Logo(const char *pszName, int iSize, void *pbuf)
{
	return gHUD.MsgFunc_Logo(pszName, iSize, pbuf );
}

//DECLARE_MESSAGE(m_Logo, Logo)
int __MsgFunc_ResetHUD(const char *pszName, int iSize, void *pbuf)
{
	return gHUD.MsgFunc_ResetHUD(pszName, iSize, pbuf );
}

int __MsgFunc_InitHUD(const char *pszName, int iSize, void *pbuf)
{
	if(gEngfuncs.pDemoAPI->IsPlayingback() )
	{
		extern void HUD_InitClientWeapons( void );
		HUD_InitClientWeapons();
	}
	gHUD.MsgFunc_InitHUD( pszName, iSize, pbuf );
	return 1;
}

int __MsgFunc_ViewMode(const char *pszName, int iSize, void *pbuf)
{
	gHUD.MsgFunc_ViewMode( pszName, iSize, pbuf );
	return 1;
}

//LRC
int __MsgFunc_SetFog(const char *pszName, int iSize, void *pbuf)
{
	gHUD.MsgFunc_SetFog( pszName, iSize, pbuf );
	return 1;
}

//Ben
int __MsgFunc_Dead(const char *pszName, int iSize, void *pbuf)
{
	gHUD.MsgFunc_Dead( pszName, iSize, pbuf );
	return 1;
}

int __MsgFunc_SetFOV(const char *pszName, int iSize, void *pbuf)
{
	return gHUD.MsgFunc_SetFOV( pszName, iSize, pbuf );
}

int __MsgFunc_Concuss(const char *pszName, int iSize, void *pbuf)
{
	return gHUD.MsgFunc_Concuss( pszName, iSize, pbuf );
}

int __MsgFunc_GameMode(const char *pszName, int iSize, void *pbuf )
{
	return gHUD.MsgFunc_GameMode( pszName, iSize, pbuf );
}

// Ben Addition to Raiko Contrib - Melee Only
int __MsgFunc_MeleeOnly(const char *pszName, int iSize, void *pbuf )
{
	gHUD.MsgFunc_MeleeOnly( pszName, iSize, pbuf );
	return 1;
}
// Ben End

// Raiko Contrib - Stationary Reload
int __MsgFunc_SReload(const char *pszName, int iSize, void *pbuf)
{
	gHUD.MsgFunc_SReload(pszName, iSize, pbuf);
	return 1;
}
// Raiko End

// BP LaserTag
int __MsgFunc_LaserTag(const char *pszName, int iSize, void *pbuf )
{
	gHUD.MsgFunc_LaserTag( pszName, iSize, pbuf );
	return 1;
}
// BP End

// TFFree Command Menu
void __CmdFunc_OpenCommandMenu(void)
{
	if ( gViewPort )
	{
		gViewPort->ShowCommandMenu( gViewPort->m_StandardMenu );
	}
}

// TFC "special" command
void __CmdFunc_InputPlayerSpecial(void)
{
	if ( gViewPort )
	{
		gViewPort->InputPlayerSpecial();
	}
}

void __CmdFunc_CloseCommandMenu(void)
{
	if ( gViewPort )
	{
		gViewPort->InputSignalHideCommandMenu();
	}
}

void __CmdFunc_ForceCloseCommandMenu( void )
{
	if ( gViewPort )
	{
		gViewPort->HideCommandMenu();
	}
}

void __CmdFunc_ToggleServerBrowser( void )
{
	if ( gViewPort )
	{
		gViewPort->ToggleServerBrowser();
	}
}

// TFFree Command Menu Message Handlers
int __MsgFunc_ValClass(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_ValClass( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_TeamNames(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_TeamNames( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_VGUIMenu(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_VGUIMenu( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_MOTD(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_MOTD( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_ServerName(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_ServerName( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_ScoreInfo(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_ScoreInfo( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_TeamScore(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_TeamScore( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_FullScore(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_FullScore( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_TeamInfo(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_TeamInfo( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_Spectator(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_Spectator( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_AllowSpec(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_AllowSpec( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_ResetScores(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_ResetScores( pszName, iSize, pbuf );
	return 0;
}

//BP - ParticleEmitter
int __MsgFunc_Particles(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_Particles( pszName, iSize, pbuf );
	return 0;
}

//BP - Grass
int __MsgFunc_Grass(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_Grass( pszName, iSize, pbuf );
	return 0;
}

//BP - Class Statistics Hintbox
int __MsgFunc_HintBox(const char *pszName, int iSize, void *pbuf)
{
    if (gViewPort)
        return gViewPort->MsgFunc_HintBox( pszName, iSize, pbuf );
    return 0;
}

//BP - Commandermenu
//int __MsgFunc_ResComPoint( const char *pszName, int iSize, void *pbuf )
//{
//    if (gViewPort)
//       return gViewPort->MsgFunc_ResComPoint( pszName, iSize, pbuf );
//    return 0;
//}

// This is called every time the DLL is loaded
void CHud :: Init( void )
{
	//BP ParticleEmitter
	HOOK_MESSAGE( Particles );
	HOOK_MESSAGE( Grass );
	HOOK_MESSAGE( HintBox );

	//BP CommanderMenu
//	HOOK_MESSAGE( ResComPoint );

	HOOK_MESSAGE( Logo );
	HOOK_MESSAGE( ResetHUD );
	HOOK_MESSAGE( GameMode );
	HOOK_MESSAGE( InitHUD );
	HOOK_MESSAGE( ViewMode );
	HOOK_MESSAGE( SetFog );
	//Ben
	HOOK_MESSAGE( Dead );
	HOOK_MESSAGE( SetFOV );
	HOOK_MESSAGE( Concuss );

	// TFFree CommandMenu
	HOOK_COMMAND( "+commandmenu", OpenCommandMenu );
	HOOK_COMMAND( "-commandmenu", CloseCommandMenu );
	HOOK_COMMAND( "ForceCloseCommandMenu", ForceCloseCommandMenu );
	HOOK_COMMAND( "special", InputPlayerSpecial );
	HOOK_COMMAND( "togglebrowser", ToggleServerBrowser );

	HOOK_MESSAGE( ValClass );
	HOOK_MESSAGE( TeamNames );
	HOOK_MESSAGE( MOTD );
	HOOK_MESSAGE( ServerName );
	HOOK_MESSAGE( ScoreInfo );
	HOOK_MESSAGE( TeamScore );
	HOOK_MESSAGE( TeamInfo );

	HOOK_MESSAGE( Spectator );
	HOOK_MESSAGE( AllowSpec );

	// VGUI Menus
	HOOK_MESSAGE( VGUIMenu );
	HOOK_MESSAGE( ResetScores );
	HOOK_MESSAGE( FullScore );

	// Ben Addition to Raiko Contrib - Melee Only
	HOOK_MESSAGE( MeleeOnly );
	// Ben End
	
	// BP LaserTag
	HOOK_MESSAGE( LaserTag );
	// BP End

	// Raiko Contrib - Stationary Reload
	HOOK_MESSAGE(SReload);
	// Raiko End

	CVAR_CREATE( "hud_takesshots", "0", FCVAR_ARCHIVE );		// controls whether or not to automatically take screenshots at the end of a round


	m_iLogo = 0;
	m_iFOV = 0;
	CVAR_CREATE( "cl_msgs_captures", "1", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
	CVAR_CREATE( "cl_msgs_who_hit_me", "1", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
	CVAR_CREATE( "cl_msgs_who_i_hit", "1", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
	CVAR_CREATE( "cl_msgs_team_change", "1", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
	CVAR_CREATE( "cl_msgs_my_team_change", "1", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
	CVAR_CREATE( "cl_msgs_commands", "1", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
	CVAR_CREATE( "cl_msgs_round", "1", FCVAR_CLIENTDLL | FCVAR_ARCHIVE ) ;

	CVAR_CREATE( "cl_dynamicxhair", "1", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
	CVAR_CREATE( "cl_capturetextlevel", "4", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );

	CVAR_CREATE( "zoom_sensitivity_ratio", "1.2", 0 );
	m_pCvarStealMouse = CVAR_CREATE( "hud_capturemouse", "1", FCVAR_ARCHIVE );
	m_pCvarDraw = CVAR_CREATE( "hud_draw", "1", FCVAR_ARCHIVE );
	cl_lw = gEngfuncs.pfnGetCvarPointer( "cl_lw" );
	g_ShowFog = gEngfuncs.pfnRegisterVariable("cl_showfog", "1", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
	g_ParticleCount = gEngfuncs.pfnRegisterVariable("cl_particlecount", "100", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
	g_ParticleDebug = gEngfuncs.pfnRegisterVariable("cl_particledebug", "0", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
	g_ParticleSorts = gEngfuncs.pfnRegisterVariable("cl_particlesorts", "3", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
	CVAR_CREATE( "cl_grassamount", "100", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
	g_HintBoxLevel = gEngfuncs.pfnRegisterVariable("cl_hintboxlevel", "1", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
	g_ShowPoints = gEngfuncs.pfnRegisterVariable("cl_showpoints", "1", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
	g_Text_ypos = gEngfuncs.pfnRegisterVariable("cl_text_ypos", "112", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
	
	m_pSpriteList = NULL;

	// Clear any old HUD list
	if ( m_pHudList )
	{
		HUDLIST *pList;
		while ( m_pHudList )
		{
			pList = m_pHudList;
			m_pHudList = m_pHudList->pNext;
			free( pList );
		}
		m_pHudList = NULL;
	}

	// In case we get messages before the first update -- time will be valid
	m_flTime = 1.0;

	//Ben
	m_Health.Init();
	m_PredefinedMessage.Init();
	m_Points.Init();
	m_Clock.Init();//Baer
	m_ColourMessage.Init();
	m_Crosshair.Init();
//	m_CommanderMenu.Init();
//	m_ArmLegShot.Init();//Myristate
	m_Ammo.Init();
	m_SayText.Init();
	m_Spectator.Init();
	m_Geiger.Init();
	m_Train.Init();
	m_Message.Init();
	m_StatusBar.Init();
	m_DeathNotice.Init();
	m_AmmoSecondary.Init();
	m_TextMessage.Init();
	m_StatusIcons.Init();
	GetClientVoiceMgr()->Init(&g_VoiceStatusHelper, (vgui::Panel**)&gViewPort);

	m_Menu.Init();
	
	ServersInit();

	MsgFunc_ResetHUD(0, 0, NULL );

}

// CHud destructor
// cleans up memory allocated for m_rg* arrays
CHud :: ~CHud()
{
	delete pParticleManager;
	pParticleManager = NULL;
	delete [] m_rghSprites;
	delete [] m_rgrcRects;
	delete [] m_rgszSpriteNames;

	if ( m_pHudList )
	{
		HUDLIST *pList;
		while ( m_pHudList )
		{
			pList = m_pHudList;
			m_pHudList = m_pHudList->pNext;
			free( pList );
		}
		m_pHudList = NULL;
	}

	ServersShutdown();
}

// GetSpriteIndex()
// searches through the sprite list loaded from hud.txt for a name matching SpriteName
// returns an index into the gHUD.m_rghSprites[] array
// returns 0 if sprite not found
int CHud :: GetSpriteIndex( const char *SpriteName )
{
	// look through the loaded sprite name list for SpriteName
	for ( int i = 0; i < m_iSpriteCount; i++ )
	{
		if ( strncmp( SpriteName, m_rgszSpriteNames + (i * MAX_SPRITE_NAME_LENGTH), MAX_SPRITE_NAME_LENGTH ) == 0 )
			return i;
	}

	return -1; // invalid sprite
}

void CHud :: VidInit( void )
{
	m_scrinfo.iSize = sizeof(m_scrinfo);
	GetScreenInfo(&m_scrinfo);

	// ----------
	// Load Sprites
	// ---------
//	m_hsprFont = LoadSprite("sprites/%d_font.spr");
	
	m_hsprLogo = 0;	
	m_hsprCursor = 0;

	if (ScreenWidth < 640)
		m_iRes = 320;
	else
		m_iRes = 640;

	// Only load this once
	if ( !m_pSpriteList )
	{
		// we need to load the hud.txt, and all sprites within
		m_pSpriteList = SPR_GetList("sprites/hud.txt", &m_iSpriteCountAllRes);

		if (m_pSpriteList)
		{
			// count the number of sprites of the appropriate res
			m_iSpriteCount = 0;
			client_sprite_t *p = m_pSpriteList;
			for ( int j = 0; j < m_iSpriteCountAllRes; j++ )
			{
				if ( p->iRes == m_iRes )
					m_iSpriteCount++;
				p++;
			}

			delete [] m_rghSprites;
			delete [] m_rgrcRects;
			delete [] m_rgszSpriteNames;
			// allocated memory for sprite handle arrays
 			m_rghSprites = new HSPRITE[m_iSpriteCount];
			m_rgrcRects = new wrect_t[m_iSpriteCount];
			m_rgszSpriteNames = new char[m_iSpriteCount * MAX_SPRITE_NAME_LENGTH];

			p = m_pSpriteList;
			int index = 0;
			for ( j = 0; j < m_iSpriteCountAllRes; j++ )
			{
				if ( p->iRes == m_iRes )
				{
					char sz[256];
					sprintf(sz, "sprites/%s.spr", p->szSprite);
					m_rghSprites[index] = SPR_Load(sz);
					m_rgrcRects[index] = p->rc;
					strncpy( &m_rgszSpriteNames[index * MAX_SPRITE_NAME_LENGTH], p->szName, MAX_SPRITE_NAME_LENGTH );

					index++;
				}

				p++;
			}
		}
	}
	else
	{
		// we have already have loaded the sprite reference from hud.txt, but
		// we need to make sure all the sprites have been loaded (we've gone through a transition, or loaded a save game)
		client_sprite_t *p = m_pSpriteList;
		int index = 0;
		for ( int j = 0; j < m_iSpriteCountAllRes; j++ )
		{
			if ( p->iRes == m_iRes )
			{
				char sz[256];
				sprintf( sz, "sprites/%s.spr", p->szSprite );
				m_rghSprites[index] = SPR_Load(sz);
				index++;
			}

			p++;
		}
	}

	// assumption: number_1, number_2, etc, are all listed and loaded sequentially
	m_HUD_number_0 = GetSpriteIndex( "number_0" ); // ORiginal Halflife Number Index Keep For Compatibility

	m_iFontHeight = m_rgrcRects[m_HUD_number_0].bottom - m_rgrcRects[m_HUD_number_0].top;

	//Ben
	m_Health.VidInit();
	m_PredefinedMessage.VidInit();
	m_Points.VidInit();
	m_Clock.VidInit();//Baer
	m_ColourMessage.VidInit();
	m_Crosshair.VidInit();//BP
//	m_CommanderMenu.VidInit();//BP
	//	m_ArmLegShot.VidInit();//Myristate
	m_Ammo.VidInit();
	m_Spectator.VidInit();
	m_Geiger.VidInit();
	m_Train.VidInit();
	m_Message.VidInit();
	m_StatusBar.VidInit();
	m_DeathNotice.VidInit();
	m_SayText.VidInit();
	m_Menu.VidInit();
	m_AmmoSecondary.VidInit();
	m_TextMessage.VidInit();
	m_StatusIcons.VidInit();
	GetClientVoiceMgr()->VidInit();

	if(pParticleManager)
		delete pParticleManager;
	pParticleManager = new CParticleSystemManager;
	pParticleManager->PrecacheTextures();

	char szVersion[16];
	_snprintf(szVersion, sizeof(szVersion) - 1, "%s\0", const_cast<char*>(sBgVersion));

    gEngfuncs.Con_Printf("The Battle Grounds %s, Copyright (C) 2001 - 2006, The Battle Grounds Team and Contributors\n", szVersion);
    gEngfuncs.Con_Printf("The Battle Grounds %s comes with ABSOLUTELY NO WARRANTY\n", szVersion);
    gEngfuncs.Con_Printf("This is free software, and you are welcome to redistribute it\n");
    gEngfuncs.Con_Printf("This software is licensed under the GNU LGPL and comes with a copy of its source\n");
}

int CHud::MsgFunc_Logo(const char *pszName,  int iSize, void *pbuf)
{
	BEGIN_READ( pbuf, iSize );

	// update Train data
	m_iLogo = READ_BYTE();

	return 1;
}

float g_lastFOV = 0.0;

/*
============
COM_FileBase
============
*/
// Extracts the base name of a file (no path, no extension, assumes '/' as path separator)
void COM_FileBase ( const char *in, char *out)
{
	int len, start, end;

	len = strlen( in );
	
	// scan backward for '.'
	end = len - 1;
	while ( end && in[end] != '.' && in[end] != '/' && in[end] != '\\' )
		end--;
	
	if ( in[end] != '.' )		// no '.', copy to end
		end = len-1;
	else 
		end--;					// Found ',', copy to left of '.'


	// Scan backward for '/'
	start = len-1;
	while ( start >= 0 && in[start] != '/' && in[start] != '\\' )
		start--;

	if ( in[start] != '/' && in[start] != '\\' )
		start = 0;
	else 
		start++;

	// Length of new sting
	len = end - start + 1;

	// Copy partial string
	strncpy( out, &in[start], len );
	// Terminate it
	out[len] = 0;
}

/*
=================
HUD_IsGame

=================
*/
int HUD_IsGame( const char *game )
{
	const char *gamedir;
	char gd[ 1024 ];

	gamedir = gEngfuncs.pfnGetGameDirectory();
	if ( gamedir && gamedir[0] )
	{
		COM_FileBase( gamedir, gd );
		if ( !stricmp( gd, game ) )
			return 1;
	}
	return 0;
}

/*
=====================
HUD_GetFOV

Returns last FOV
=====================
*/
float HUD_GetFOV( void )
{
	if ( gEngfuncs.pDemoAPI->IsRecording() )
	{
		// Write it
		int i = 0;
		unsigned char buf[ 100 ];

		// Active
		*( float * )&buf[ i ] = g_lastFOV;
		i += sizeof( float );

		Demo_WriteBuffer( TYPE_ZOOM, i, buf );
	}

	if ( gEngfuncs.pDemoAPI->IsPlayingback() )
	{
		g_lastFOV = g_demozoom;
	}
	g_lastFOV = 90;
	return 90;
}

int CHud::MsgFunc_SetFOV(const char *pszName,  int iSize, void *pbuf)
{
	BEGIN_READ( pbuf, iSize );

	int newfov = 90;
	int def_fov = 90;

	//Weapon prediction already takes care of changing the fog. ( g_lastFOV ).
	if ( cl_lw && cl_lw->value )
		return 1;

	g_lastFOV = newfov;

	if ( newfov == 0 )
	{
		m_iFOV = def_fov;
	}
	else
	{
		m_iFOV = newfov;
	}

	// the clients fov is actually set in the client data update section of the hud

	// Set a new sensitivity
	if ( m_iFOV == def_fov )
	{  
		// reset to saved sensitivity
		m_flMouseSensitivity = 0;
	}
	else
	{  
		// set a new sensitivity that is proportional to the change from the FOV default
		m_flMouseSensitivity = sensitivity->value * ((float)newfov / (float)def_fov) * CVAR_GET_FLOAT("zoom_sensitivity_ratio");
	}

	return 1;
}


void CHud::AddHudElem(CHudBase *phudelem)
{
	HUDLIST *pdl, *ptemp;

//phudelem->Think();

	if (!phudelem)
		return;

	pdl = (HUDLIST *)malloc(sizeof(HUDLIST));
	if (!pdl)
		return;

	memset(pdl, 0, sizeof(HUDLIST));
	pdl->p = phudelem;

	if (!m_pHudList)
	{
		m_pHudList = pdl;
		return;
	}

	ptemp = m_pHudList;

	while (ptemp->pNext)
		ptemp = ptemp->pNext;

	ptemp->pNext = pdl;
}

float CHud::GetSensitivity( void )
{
	return m_flMouseSensitivity;
}

// fixes the buffer overrun in hl
char* CHud::HandleServerString(char *szServerMsg)
{
	char szString[2048];
	szString[sizeof(szString)-1] = '\0';

	if(szServerMsg == NULL)
	{
		gEngfuncs.pfnConsolePrint("Null string being sent\n");
		return NULL;
	}
	
	else
	{
		int iResult = sizeof(szString)-1;
		iResult = _snprintf(szString, iResult, "%s\0", szServerMsg);

		if(iResult < UNDEFINED)
		{
			gEngfuncs.pfnConsolePrint("Bigger than allowed string being sent\n");
			return NULL;
		}
		else
		{
			szString[sizeof(szString)-1] = '\0';
			_snprintf(m_szString, sizeof(m_szString)-1, "%s", szString);
			return (char*)m_szString;
		}
	}
}

