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

//This file was created on 20/10/2K+2 at 13:37 by Ben Banfield for the Battle Grounds modification of Half-Life
//Its purpose is to provide an interface between the engines copy of the event funcs and our game's functions inside CEvent

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


//bg special includes
#define WEAPON_CLASS_NO
#include "weapon_data.h"
#include "ev_bg.h"

extern "C" char PM_FindTextureType( char *name );
extern int iLastEventIdx;
extern "C" int CL_IsThirdPerson( void );

float g_flTimeWeaponIdle = 0.0;

//create a event manager
CEvent *pEvent = new CEvent;
CBgEffects *pEffects = NULL;

typedef enum bullet_e
{
	BULLET_NONE = 0,
	BULLET_BLADE,
	BULLET_SHOT,
	BULLET_CANNON,
} ;

//set all event variables to clen and default values
void CEvent::ClearAllEventVars()
{
	delete pEffects;
	pEffects = new CBgEffects;

	//no index yet
	idx = 0;

	//give all angles a default value
	origin = Vector(0, 0, 0);
	angles = Vector(0, 0, 0);
	forward = Vector(0, 0, 0);
	up = Vector(0, 0, 0);
	right = Vector(0, 0, 0);
	vecVelocity = Vector(0, 0, 0);
	vecSrc = Vector(0, 0, 0);
	vecPos = Vector(0, 0, 0);
	vecAiming = Vector(0, 0, 0);

	// no weapon data atm
	pData = NULL;
	//no trace info either
	pTrace = NULL;
	//no entity info
	pEntity = NULL;

	//entity
	iEnt = 0;
	//texture name
	sTextureName = NULL;
	//texture type
	cTextureType = 0;
	
	iLastEventIdx = 0.0;
	//no saved args
	this->args = NULL;
}

//sets all the class varaibles to the ones passed in the event structure
void CEvent::FillEventVars(struct event_args_s *args)
{
	//reset the class vars first
	ClearAllEventVars();

	//get the client index
	idx = args->entindex;
	iLastEventIdx = idx;

	//copy the origin, velocity and the angles over
	VectorCopy( args->origin, origin );
	VectorCopy( args->velocity, vecVelocity );
	VectorCopy( args->angles, angles );
	//get the individual sections of the angles
	AngleVectors( angles, forward, right, up );

	//get the data of the current gun
	if(args->iparam2)
	{
		pData = GetGunsData(args);
	}

	//gets the guns position
	EV_GetGunPosition( args, vecPos, origin );
	VectorCopy( forward, vecAiming );

	if( EV_IsLocal( idx ) && !CL_IsThirdPerson())
	{
		//grab the attachment
		vecSrc = gEngfuncs.GetViewModel()->attachment[1];
	}
	else
	{
		vecSrc = vecPos + 40 * vecAiming;
	}

	//copy the event args
	this->args = args;
}

//returns the type of texture we have shot at
char CEvent::TextureTypeAtEnd()
{
	char chTextureType = CHAR_TEX_STONE;
	char *pTextureName;
	char texname[ 64 ], szbuffer[ 64 ];

	pTextureName = (char *)gEngfuncs.pEventAPI->EV_TraceTexture( pEvent->pTrace->ent, pEvent->vecSrc, pEvent->vecEnd );

	//copied from EV_HLDM_PlayTextureSound
	if ( pTextureName )
	{
		strcpy( texname, pTextureName );
		pTextureName = texname;

		// strip leading '-0' or '+0~' or '{' or '!'
		if (*pTextureName == '-' || *pTextureName == '+')
		{
			pTextureName += 2;
		}

		if (*pTextureName == '{' || *pTextureName == '!' || *pTextureName == '~' || *pTextureName == ' ')
		{
			pTextureName++;
		}

		strcpy( szbuffer, pTextureName );
		szbuffer[ CBTEXTURENAMEMAX - 1 ] = 0;

		// get texture type
		chTextureType = PM_FindTextureType( szbuffer );     
	}
	return chTextureType;
}

// handles the melee events
void CEvent::EV_Melee( event_args_s *args )
{
	FillEventVars(args);

	if( EV_IsLocal( idx ) )
	{
		//if we are dealing with a melee weapon
		if(pData->iWeaponType == MELEE)
		{
			//HACK!!
			int body = GetGunsData(args)->iWeight;
			g_iSwing++;
			gEngfuncs.pEventAPI->EV_WeaponAnimation( (MELEE_ATTACK1 + (g_iSwing % 4)), body );
		}
		else
		{
			if(args->bparam1 == 1) // empty
				gEngfuncs.pEventAPI->EV_WeaponAnimation( BASE_STAB_EMPTY, 1 );
			else
				gEngfuncs.pEventAPI->EV_WeaponAnimation( BASE_STAB, 1 );
		}
	}

	//if we are dealing with a melee weapon
	if(pData->iWeaponType == MELEE)
	{
		gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, pData->snd_prim_fire, 1, ATTN_NORM, 0, 94 + gEngfuncs.pfnRandomLong( 0, 0xf )); 
	}
	else
	{
		gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, pData->snd_second_fire, 1, ATTN_NORM, 0, 94 + gEngfuncs.pfnRandomLong( 0, 0xf )); 
	}
}

//Generic Gun Events
void CEvent::EV_FireGun( event_args_t *args)
{
	// Ben Addition to Raiko Contrib - Melee Only
	if(g_iMeleeOnly == 1)
		return;
	// Ben End

	FillEventVars(args);
	EV_FireBullets( this->idx, this->forward, this->right, this->up, 1, this->vecPos, this->vecAiming, 8192, BULLET_SHOT, 2, 0, this->args->fparam1, this->args->fparam2 );

	if ( EV_IsLocal( idx ) )
	{
		if(args->iparam1 == 1)
		{
			gEngfuncs.pEventAPI->EV_WeaponAnimation( BASE_SHOOT1, 1 );
		}
		else
		{
			gEngfuncs.pEventAPI->EV_WeaponAnimation( BASE_SHOOT2, 1 );
		}

		V_PunchAxis( 0, gEngfuncs.pfnRandomFloat( -2, 2 ) );
	}

	if(args->iparam1 == 1)
	{
		gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, GetGunsData(args)->snd_prim_fire, 1, ATTN_NORM, 0, 94 + gEngfuncs.pfnRandomLong( 0, 0xf )); 
	}
	else
	{
		gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, GetGunsData(args)->snd_second_fire, 1, ATTN_NORM, 0, 94 + gEngfuncs.pfnRandomLong( 0, 0xf )); 
	}

	//get the longest position on the line
	for(int i = 0; i < 3; i++)
	{
		vecEnd[i] = vecPos[ i ] + (8192 * vecAiming[ i ]);
	}
}

void CEvent::EV_Animation(event_args_s *args)
{
	FillEventVars(args);
	if ( EV_IsLocal( idx ) )
	{
		gEngfuncs.pEventAPI->EV_WeaponAnimation(args->iparam1, args->iparam2);
	}

}

void CEvent::EV_RoundEnd(event_args_s *arg)
{
	int pitch = 100;
	gEngfuncs.pEventAPI->EV_StopAllSounds(arg->entindex, CHAN_AUTO);

	gEngfuncs.pEventAPI->EV_PlaySound(arg->entindex, arg->origin, CHAN_AUTO, team_win_sounds[arg->iparam1], VOL_NORM, ATTN_NORM, 0, pitch);

}

//map the events which are called by the engine on an event, to the functions in our class
extern "C"
{
	//generic weapon usedge function
	void EV_FireGun( struct event_args_s *args ) { pEvent->EV_FireGun(args); }
	void EV_Melee( struct event_args_s *args ) { pEvent->EV_Melee(args); }

	//animation event
	void EV_Animation(struct event_args_s *args ) { pEvent->EV_Animation(args); }

	//round end 
	void EV_RoundEnd( struct event_args_s *args ) { pEvent->EV_RoundEnd(args); }
}
