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

// This file was created on the 23/9/2K+2 at 19:22 by Ben Banfield for the Battle Grounds modification of Half-Life
// Its purpose is to define the CPistolB classn and gun based onthe CBaseWeapons class

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"
#include "weapon_data.h"

void CPistolB::SetAllVars()
{
	pData->p_model = "models/p_pistol_british.mdl";
	pData->v_model = "models/v_pistol_british.mdl";
	pData->w_model = "models/w_pistol_british.mdl";
	pData->player_anim = "pistol_b";

	pData->snd_prim_fire = "weapons/pistol_b/fire1.wav";
	pData->snd_second_fire = "weapons/pistol_b/fire2.wav";
	pData->snd_reload = "weapons/pistol_b/reload.wav";
	pData->snd_draw = "weapons/pistol_b/draw.wav";
	pData->snd_holster = "weapons/pistol_b/holster.wav";

	pData->iId = WEAPON_PISTOLB;
	pData->flPrimaryDmgModifier = PISTOLB_FIRE_DAMAGE;
	pData->iWeight = PISTOLB_WEIGHT;
	pData->flSpeedModifier = PISTOLB_SPEED;

	pData->flStandMoving = 9.0;
	pData->flStandStill = 7.0;
	pData->flCrouchMoving = 7.5;
	pData->flCrouchStill = 5.0;

	pData->iDefaultGive = PISTOLB_DEFAULT_GIVE;
	pData->iMaxAmmo = PISTOLB_MAX_CARRY;

	pData->flReloadTime = 5.0;
	pData->flTimeToIdle = 1.5;

	pData->classname = MAKE_STRING("weapon_PistolB");

	pData->iWeaponType = PISTOL; // pistol

	pData->flDeployDelay = 0.8;
}

LINK_ENTITY_TO_CLASS(weapon_PistolB, CPistolB);
