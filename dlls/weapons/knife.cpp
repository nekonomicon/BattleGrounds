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

//Thsi file was created on 14/9/2k+1 at 23:53 by Ben bafield for Battleground modifiaction of Half-Life
//Its purpose is to implement the knife class weapon

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"
#include "weapon_data.h"

void CKnife::SetAllVars()
{
	pData->p_model = "models/p_knife.mdl";
	pData->v_model = "models/v_knife.mdl";
	pData->w_model = "models/w_knife.mdl";
	pData->player_anim = "knife";
	pData->bSwitchBodies = true;

	pData->snd_prim_fire = "weapons/knife/swing.wav";
	pData->snd_draw = "weapons/knife/draw.wav";
	pData->snd_holster = "weapons/knife/holster.wav";
	pData->snd_hit = "weapons/knife/hit.wav";
	pData->snd_hitwall = "weapons/knife/hitwall.wav";

	pData->event_second = "events/melee.sc";

	pData->flDeployDelay = 0.4;
	pData->flAttackDelay = 1.1;

	pData->flBladeLength = 46;
	pData->flSpeedModifier = KNIFE_SPEED;
	pData->flSecondaryDmgModifier = KNIFE_DAMAGE;
	pData->iWeight = KNIFE_WEIGHT;

	pData->iDefaultGive = 0;
	pData->iMaxAmmo = WEAPON_NOCLIP;
	pData->ammo_name = NULL;

	pData->iId = WEAPON_KNIFE;
	pData->iWeaponType = MELEE;

	pData->classname = MAKE_STRING("weapon_Knife");
}

LINK_ENTITY_TO_CLASS( weapon_Knife, CKnife );
