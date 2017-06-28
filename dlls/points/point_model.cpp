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

//This file was created on the 3/5/03 at 11:55 by Ben Banfield for the Battle Grounds modification for Half-Life
//Its purpose is to define class which will handle the ent which displays the models for a func_point

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "soundent.h"
#include "gamerules.h"
#include "animation.h"
#include "bg_rules.h"
#include "point.h"

// -sets the entity up
void CPointModel::Spawn( void )
{
	Precache( );

	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NONE;

	pev->frame			= 0;
	pev->nextthink		= gpGlobals->time + 0.1;
	m_iAnimate			= 1;
	m_flLastTime		= gpGlobals->time;

	SET_MODEL (ENT(pev), szDefaultFlag);

	m_flMaxFrame = (float) MODEL_FRAMES( pev->modelindex ) - 1;

//	SetThink( &CPointModel::FlagThink );
}

// -precaches the flags
void CPointModel::Precache( void )
{
	PRECACHE_MODEL(szDefaultFlag);
	PRECACHE_MODEL(szBritishFlag);
	PRECACHE_MODEL(szAmericanFlag);
}

// -stores the models the mapper wants to use
void CPointModel::KeyValue( KeyValueData *pKeyValueData )
{
	if(FStrEq(pKeyValueData->szKeyName, "default_flag"))
	{
		sprintf(szDefaultFlag, "%s", pKeyValueData->szValue);
	}
	else if(FStrEq(pKeyValueData->szKeyName, "british_flag"))
	{
		sprintf(szBritishFlag, "%s", pKeyValueData->szValue);
	}
	else if(FStrEq(pKeyValueData->szKeyName, "american_flag"))
	{
		sprintf(szAmericanFlag, "%s", pKeyValueData->szValue);
	}
	else
	{
		CBaseEntity::KeyValue( pKeyValueData );
	}
}

// -sets the current model depending on the team holding it
void CPointModel::SetModel( int iTeam )
{
	if(iTeam == BRITS)
		pev->model = MAKE_STRING(szBritishFlag);

	else if(iTeam == AMERS)
		pev->model = MAKE_STRING(szAmericanFlag);

	else
		pev->model = MAKE_STRING(szDefaultFlag);

	SET_MODEL (ENT(pev), STRING(pev->model));
}

// -returns the point this model gets its state from
char *CPointModel::ReturnTargetPoint( void )
{
	return (char *)STRING(pev->target);
}

void CPointModel::FlagThink( void )
{
	if ( ShouldAnimate() )
		Animate( pev->framerate * (gpGlobals->time - m_flLastTime) );

	pev->nextthink = gpGlobals->time + 0.1;
	m_flLastTime = gpGlobals->time;
}

// -animate the model
void CPointModel::Animate( float flFrames )
{ 
	pev->frame += flFrames;
	if ( m_flMaxFrame > 0 )
		pev->frame = fmod( pev->frame, m_flMaxFrame );
}

LINK_ENTITY_TO_CLASS( env_flag, CPointModel );

