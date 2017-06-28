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

//This file was creatd on the 22/9/2K+2 at 16:05 by Ben Banfield for the Battle Grounds modification of Half-Life
//Its purpose is to define the CBrownBess class and demonstrate the CBrownBess class in action

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"
#include "weapon_data.h"

void CBrownBess::SetAllVars( void )
{
	pData->p_model = "models/p_brownbess.mdl";
	pData->v_model = "models/v_brownbess.mdl";
	pData->w_model = "models/w_brownbess.mdl";
	pData->player_anim = "brownbess";

	pData->snd_prim_fire = "weapons/brownbess/fire1.wav";
	pData->snd_second_fire = "weapons/brownbess/fire2.wav";
	pData->snd_reload = "weapons/brownbess/reload.wav";
	pData->snd_draw = "weapons/brownbess/draw.wav";
	pData->snd_holster = "weapons/brownbess/holster.wav";
	pData->snd_idle = "weapons/brownbess/idle.wav";
	pData->snd_hit = "weapons/brownbess/hit.wav";
	pData->snd_hitwall = "weapons/brownbess/hitwall.wav";

	pData->event_second = "events/melee.sc";

	pData->flStandMoving = 12.0;
	pData->flStandStill = 4.0;
	pData->flCrouchMoving = 12.0;
	pData->flCrouchStill = 4.0;

	pData->iId = WEAPON_BESS;
	pData->flPrimaryDmgModifier = BESS_FIRE_DAMAGE;
	pData->flSecondaryDmgModifier = BESS_BAYONET_DAMAGE;

	pData->iWeight = BESS_WEIGHT;

	pData->classname = MAKE_STRING("weapon_BrownBess");

	pData->flBladeLength = 80.0;
	pData->bBayonet = true;
	pData->flSpeedModifier = BESS_SPEED;

	pData->flDeployDelay = 0.0;
}

LINK_ENTITY_TO_CLASS(weapon_BrownBess, CBrownBess);


