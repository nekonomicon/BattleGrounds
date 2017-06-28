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

//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

// Triangle rendering, if any

#include "hud.h"
#include "cl_util.h"

// Triangle rendering apis are in gEngfuncs.pTriAPI

#include "const.h"
#include "entity_state.h"
#include "cl_entity.h"
#include "triangleapi.h"
#include "pm_shared.h"
#include "particle_header.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glaux.h>


#define DLLEXPORT __declspec( dllexport )

extern "C"
{
	void DLLEXPORT HUD_DrawNormalTriangles( void );
	void DLLEXPORT HUD_DrawTransparentTriangles( void );
};

extern float g_fFogColor[4];
extern float g_fStartDist;
extern float g_fEndDist;
extern int g_iWaterLevel;
cvar_t *g_ShowFog;

void RenderFog ( void )
{
	//Not in water and we want fog.
	if(g_iWaterLevel < 3 && g_fStartDist > 0 && g_fEndDist > 0)
	{
		gEngfuncs.pTriAPI->Fog ( g_fFogColor, g_fStartDist, g_fEndDist, true );
		glClearColor(g_fFogColor[0], g_fFogColor[1], g_fFogColor[2], g_fFogColor[3]);
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glFogfv(GL_FOG_COLOR, g_fFogColor);
		glFogf(GL_FOG_DENSITY, 2.0f);
		glHint(GL_FOG_HINT, GL_DONT_CARE);	
		glFogf(GL_FOG_START, g_fStartDist);
		glFogf(GL_FOG_END, g_fEndDist);
		glEnable(GL_FOG);
	}
}

void StopFog( void )
{
	gEngfuncs.pTriAPI->Fog( g_fFogColor, g_fStartDist, g_fEndDist, false);
	glDisable(GL_FOG);
}

/*
=================
HUD_DrawNormalTriangles

Non-transparent triangles-- add them here
=================
*/
void DLLEXPORT HUD_DrawNormalTriangles( void )
{
	gHUD.m_Spectator.DrawOverview();
}

/*
=================
HUD_DrawTransparentTriangles

Render any triangles with transparent rendermode needs here
=================
*/
class CException;
void DLLEXPORT HUD_DrawTransparentTriangles( void )
{
	try {
		pParticleManager->UpdateSystems();
	} catch( CException *e ) {
		e;
		e = NULL;
		gEngfuncs.Con_Printf("There was a serious error within the particle engine. Particles will return on map change\n");
		delete pParticleManager;
		pParticleManager = NULL;
	}
	
	if((g_ShowFog->value == 1) && (g_iUser1 < OBS_MAP_FREE)  && (g_fStartDist != 0 && g_fEndDist != 0))
		RenderFog();
	else
		StopFog();
}