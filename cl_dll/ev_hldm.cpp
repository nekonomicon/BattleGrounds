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

#include "hud.h"
#include "cl_util.h"
#include "const.h"
#include "entity_state.h"
#include "cl_entity.h"
#include "entity_types.h"
#include "usercmd.h"
#include "pm_defs.h"
#include "pm_materials.h"

#include "eventscripts.h"

#include "r_efx.h"
#include "event_api.h"
#include "event_args.h"
#include "in_defs.h"

//bg special includes
#define WEAPON_CLASS_NO
#include "weapon_data.h"
#include "ev_bg.h"

extern CBgEffects *pEffects;

//returns the correct decal skin depending on which texture is hit
char *CEvent::EV_DamageDecal( physent_t *pe )
{

	static char decalname[ 32 ];
	int idx;

	//added this check, to stop crashes when we hit nothing
	if(pe == NULL)
	{
		idx = gEngfuncs.pfnRandomLong( 0, 4 );
		sprintf( decalname, "{shot%i", idx + 1 );
		return decalname;
	}

	if ( pe->classnumber == 1 )
	{
		idx = gEngfuncs.pfnRandomLong( 0, 2 );
		sprintf( decalname, "{break%i", idx + 1 );
	}
	else if ( pe->rendermode != kRenderNormal )
	{
		sprintf( decalname, "{bproof1" );
	}
	else
	{
		idx = gEngfuncs.pfnRandomLong( 0, 4 );
		sprintf( decalname, "{shot%i", idx + 1 );
	}
	return decalname;
}

//helps predict the position of tracers
int CEvent::EV_CheckTracer( int idx, float *vecSrc, float *end, float *forward, float *right, int iBulletType, int iTracerFreq, int *tracerCount )
{
	int tracer = 0;
	int i;
	qboolean player = idx >= 1 && idx <= gEngfuncs.GetMaxClients() ? true : false;

	if ( iTracerFreq != 0 && ( (*tracerCount)++ % iTracerFreq) == 0 )
	{
		vec3_t vecTracerSrc;

		if ( player )
		{
			vec3_t offset( 0, 0, -4 );

			// adjust tracer position for player
			for ( i = 0; i < 3; i++ )
			{
				vecTracerSrc[ i ] = vecSrc[ i ] + offset[ i ] + right[ i ] * 2 + forward[ i ] * 16;
			}
		}
		else
		{
			VectorCopy( vecSrc, vecTracerSrc );
		}
		
		if ( iTracerFreq != 1 )		// guns that always trace also always decal
			tracer = 1;

	}

	return tracer;
}
/*
================
FireBullets

Go to the trouble of combining multiple pellets into a single damage call.
================
*/
void CEvent::EV_FireBullets( int idx, float *forward, float *right, float *up, int cShots, float *vecSrc, float *vecDirShooting, float flDistance, int iBulletType, int iTracerFreq, int *tracerCount, float flSpreadX, float flSpreadY )
{
	int i = 0;
	pmtrace_t tr;
	int iShot = 0;
	
	for ( iShot = 1; iShot <= cShots; iShot++ )	
	{
		vec3_t vecDirL, vecEndL;

		for( i = 0 ; i < 3; i++ )
		{
			vecDirL[i] = vecDirShooting[i] +  flSpreadX * right[ i ] + flSpreadY * up [ i ];
			vecEndL[i] = this->vecPos[ i ] + flDistance * vecDirL[ i ];
		}

		this->vecEnd = vecEndL;

		gEngfuncs.pEventAPI->EV_SetUpPlayerPrediction( false, true );
	
		// Store off the old count
		gEngfuncs.pEventAPI->EV_PushPMStates();
	
		// Now add in all of the players.
		gEngfuncs.pEventAPI->EV_SetSolidPlayers ( pEvent->idx - 1 );	

		gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );
		gEngfuncs.pEventAPI->EV_PlayerTrace( this->vecPos, vecEndL, PM_STUDIO_BOX, -1, &tr );

		pTrace = &tr;

		pEntity = gEngfuncs.pEventAPI->EV_GetPhysent(pTrace->ent);

		iEnt = gEngfuncs.pEventAPI->EV_IndexFromTrace( &tr );
		sTextureName = (char*)gEngfuncs.pEventAPI->EV_TraceTexture( iEnt, vecPos, vecEnd );

		cTextureType = TextureTypeAtEnd();

		//do all the effects
		pEffects->DisplayEffects();

		gEngfuncs.pEventAPI->EV_PopPMStates();

	}
}

// checks whether 2 players are on the same team
// or whether we will be respawning on that team
int EV_TFC_IsAllyTeam( int iTeam2 )
{
	cl_entity_t *pPlayer = gEngfuncs.GetLocalPlayer();
	extern extra_player_info_t  g_PlayerExtraInfo[MAX_PLAYERS+1];
	int iTeam = g_PlayerExtraInfo[pPlayer->index].teamnumber;

	if(iTeam == iTeam2)
		return 1;
	else
		return 0;
}
