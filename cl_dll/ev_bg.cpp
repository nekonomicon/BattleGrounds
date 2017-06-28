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

//This file was created on the 21st of October 2001 at 16:20 by Ben Banfield for the Battle Gronuds Modification of Half-Life
//Its purpose is to provide client side/event based actions for Battle Grounds

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
#include "triangleapi.h"
#include "event_api.h"
#include "event_args.h"
#include "in_defs.h"

#include <string.h>
#define WEAPON_CLASS_NO
#include "weapon_data.h"
#include "ev_bg.h"

#include "particle_header.h"

extern "C" char PM_FindTextureType( char *name );
extern CEvent *pEvent;
extern CParticleSystemManager *pParticleManager;

//set all the display capabilites to off by default
CBgEffects::CBgEffects()
{
	bBlood = false;
	bSparkAtEnd = false;
	bSmokeAtEnd = false;
}

//display all effects that we want that are allowed
void CBgEffects::DisplayEffects()
{
	//sets which effects will or won't be displayed
	SetEffects();

	//if we have hit something that bleeds, we can surely say which hitgroup we are making it bleed in
	if(bBlood)
	{
		Blood();
	}

	//do the spark and the decal
	else if(bSparkAtEnd)
	{
		SparkAtEnd();
		DecalAtEnd();
		SmokeAtEnd();
		SoundAtEnd();
	}
	//no matter what we hit, we will be firing a gun
	SmokeAtGun();
}

//sets which effects can be displayed
void CBgEffects::SetEffects()
{
	// Reset Effects display
	bBlood = false;
	bSparkAtEnd = false;
	bSmokeAtEnd = false;

	//if we have hit a entity with a moveable bounding box or the texture at the end is a flesh texture
	if((pEvent->pEntity && pEvent->pEntity->solid == SOLID_SLIDEBOX) || (pEvent->cTextureType == CHAR_TEX_FLESH))
	{
		bBlood = true;
	}
	//if the texture at the end is named sky
	else if(pEvent->sTextureName && !(stricmp((const char *)pEvent->sTextureName, "sky")))
	{
		bSparkAtEnd = false;
		bSmokeAtEnd = false;
	}
	else
	{
		bBlood = false;
		bSparkAtEnd = true;
		bSmokeAtEnd = true;
	}

	//if we have hit a swamp change the spark and smoke dispaly options off
	if((pEvent->cTextureType == CHAR_TEX_SLOSH) || (pEvent->cTextureType == CHAR_TEX_SWAMP))
	{
		bBlood = false;
		bSparkAtEnd = false;
		bSmokeAtEnd = false;
	}

}


//blood related functions
void CBgEffects::Blood()
{
}

//end of trace related functions
void CBgEffects::DecalAtPosition(pmtrace_t *pTrace)
{
	int iRand;
	char *decalName;
	
	decalName = pEvent->EV_DamageDecal( pEvent->pEntity );

	gEngfuncs.pEfxAPI->R_BulletImpactParticles( pTrace->endpos );

	iRand = gEngfuncs.pfnRandomLong(0,0x7FFF);

	switch( iRand % 5)
	{
		case 0:	gEngfuncs.pEventAPI->EV_PlaySound( 0, pTrace->endpos, 0, "weapons/ric1.wav", 1.0, ATTN_NORM, 0, PITCH_NORM ); break;
		case 1:	gEngfuncs.pEventAPI->EV_PlaySound( 0, pTrace->endpos, 0, "weapons/ric2.wav", 1.0, ATTN_NORM, 0, PITCH_NORM ); break;
		case 2:	gEngfuncs.pEventAPI->EV_PlaySound( 0, pTrace->endpos, 0, "weapons/ric3.wav", 1.0, ATTN_NORM, 0, PITCH_NORM ); break;
		case 3:	gEngfuncs.pEventAPI->EV_PlaySound( 0, pTrace->endpos, 0, "weapons/ric4.wav", 1.0, ATTN_NORM, 0, PITCH_NORM ); break;
		case 4:	gEngfuncs.pEventAPI->EV_PlaySound( 0, pTrace->endpos, 0, "weapons/ric5.wav", 1.0, ATTN_NORM, 0, PITCH_NORM ); break;
	}

	// Only decal brush models such as the world etc.
	if (  decalName && decalName[0] && pEvent->pEntity && ( pEvent->pEntity->solid == SOLID_BSP || pEvent->pEntity->movetype == MOVETYPE_PUSHSTEP ) )
	{
		if ( CVAR_GET_FLOAT( "r_decals" ) )
		{
			gEngfuncs.pEfxAPI->R_DecalShoot( 
				gEngfuncs.pEfxAPI->Draw_DecalIndex( gEngfuncs.pEfxAPI->Draw_DecalIndexFromName( decalName ) ), 
				gEngfuncs.pEventAPI->EV_IndexFromTrace( pTrace ), 0, pTrace->endpos, 0 );
		}
	}
}

void CBgEffects::DecalAtEnd()
{
	DecalAtPosition(pEvent->pTrace);
}

void CBgEffects::SparkAtEnd()
{
	gEngfuncs.pEfxAPI->R_SparkEffect(pEvent->pTrace->endpos, 20, -100, 100 );
	gEngfuncs.pEfxAPI->R_SparkStreaks(pEvent->pTrace->endpos, 20, -100, 100 );

	dlight_t *dl = gEngfuncs.pEfxAPI->CL_AllocDlight (0);

	VectorCopy (pEvent->pTrace->endpos, dl->origin);

	dl->radius = 30;
	dl->color.r = 254;
	dl->color.g = 160;
	dl->color.b = 24;
	dl->die = gEngfuncs.GetClientTime(); + 0.3;
}

void CBgEffects::SoundAtEnd()
{
}

void CBgEffects::SmokeAtEnd()
{
	if ((pEvent->cTextureType == CHAR_TEX_SAND) || (pEvent->cTextureType == CHAR_TEX_DIRT) 
		|| (pEvent->cTextureType == CHAR_TEX_MUD) || (pEvent->cTextureType == CHAR_TEX_SWAMP) 
		|| (pEvent->cTextureType == CHAR_TEX_WOOD) || (pEvent->cTextureType == CHAR_TEX_GRASS) 
		|| (pEvent->cTextureType == CHAR_TEX_GRASS2) || (pEvent->cTextureType == CHAR_TEX_LEAVES))
	{
		pParticleManager->CreateBrownPS(pEvent->pTrace->endpos, -pEvent->vecAiming);
	} else  {
		pParticleManager->CreateWhitePS(pEvent->pTrace->endpos, -pEvent->vecAiming);	
	}
	particle_system_management pSystem;
}

//at gun effects new particle system
void CBgEffects::SmokeAtGun()
{
	pParticleManager->CreateBarrelPS(pEvent->vecSrc, pEvent->vecAiming);
//	new CFlintlockSmokeParticleSystem(pEvent->vecSrc);

	/*int m_iSmoke = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/gfx_musketsmoke.spr" );
 
	float* fSrc = NULL;
 
	TEMPENTITY *pSmoke = gEngfuncs.pEfxAPI->R_TempSprite(
						pEvent->vecSrc,
						pEvent->vecAiming,
						1,
						m_iSmoke,
						kRenderTransAdd,
						kRenderFxNone,
						100,
						60,
						FTENT_SPRANIMATE);
 
	if(pSmoke)
	{
		pSmoke->fadeSpeed = 10;
		pSmoke->entity.curstate.rendercolor.r = 150;
		pSmoke->entity.curstate.rendercolor.g = 150;
		pSmoke->entity.curstate.rendercolor.b = 150;
		pSmoke->entity.curstate.renderamt = 100;
	}*/
}
