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

// This file ws created on the 23/9/2K+2 at 19:05 by Ben Banfield for the Battle Grounds modification of Half-Life
// Its purpose is to define the CRevolutionnaire class and gun based on CBaseWeapon

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"
#include "weapon_data.h"

void CRevolutionnaire::SetAllVars()
{
	pData->p_model = "models/p_revolutionnaire.mdl";
	pData->v_model = "models/v_revolutionnaire.mdl";
	pData->w_model = "models/w_revolutionnaire.mdl";
	pData->player_anim = "revolutionnaire";

	pData->snd_prim_fire = "weapons/revolutionnaire/fire1.wav";
	pData->snd_second_fire = "weapons/revolutionnaire/fire2.wav";
	pData->snd_reload = "weapons/revolutionnaire/reload.wav";
	pData->snd_draw = "weapons/revolutionnaire/draw.wav";
	pData->snd_holster = "weapons/revolutionnaire/holster.wav";
	pData->snd_hit = "weapons/revolutionnaire/hit.wav";
	pData->snd_hitwall = "weapons/revolutionnaire/hitwall.wav";

	pData->event_second = "events/melee.sc";

	pData->iId = WEAPON_REVOL;
	pData->flPrimaryDmgModifier = REVOL_FIRE_DAMAGE;
	pData->flSecondaryDmgModifier = REVOL_BAYONET_DAMAGE;

	pData->iWeight = REVOL_WEIGHT;
	pData->flSpeedModifier = REVOL_SPEED;

	pData->flCrouchMoving = 11.0;
	pData->flStandStill = 6;
	pData->flStandMoving =  9.0;
	pData->flCrouchStill = 1.75;

	pData->iDefaultGive = REVOL_DEFAULT_GIVE;
	pData->iMaxAmmo = REVOL_MAX_CARRY;

	pData->classname = MAKE_STRING("weapon_Revolutionnaire");

	pData->flBladeLength = 75.0;
	pData->bBayonet = true;

	pData->flDeployDelay = 0.0;
}

LINK_ENTITY_TO_CLASS(weapon_Revolutionnaire, CRevolutionnaire);
