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

// observer.cpp
//
// based on Robin's spectator tutorial

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"player.h"
#include	"weapons.h"
#include	"gamerules.h"
#include	"bg_rules.h"
#include	"pm_shared.h"

class CBaseBgRules;

extern int gmsgCurWeapon;
extern int gmsgSetFOV;
extern int gmsgTeamInfo;
extern int gmsgSpectator;
extern int gmsgDead;


// Player has become a spectator. Set it up.
// This was moved from player.cpp.
void CBasePlayer::StartObserver( Vector vecPosition, Vector vecViewAngle )
{
	// clear any clientside entities attached to this player
	MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, pev->origin );
	WRITE_BYTE( TE_KILLPLAYERATTACHMENTS );
	WRITE_BYTE( (BYTE)entindex() );
	MESSAGE_END();

	
	// Holster weapon immediately, to allow it to cleanup
	if (m_pActiveItem)
		m_pActiveItem->Holster( );
	

	if ( m_pTank )
	{
		m_pTank->Use( this, this, USE_OFF, 0 );
		m_pTank = NULL;
	}

	// clear out the suit message cache so we don't keep chattering
	SetSuitUpdate(NULL, false, 0);

	// Tell Ammo Hud that the player is dead
	MESSAGE_BEGIN( MSG_ONE, gmsgCurWeapon, NULL, pev );
		WRITE_BYTE(0);
		WRITE_BYTE(0XFF);
		WRITE_BYTE(0xFF);
	MESSAGE_END();

	// reset FOV
	m_iFOV = m_iClientFOV = 0;
	pev->fov = m_iFOV;
	MESSAGE_BEGIN( MSG_ONE, gmsgSetFOV, NULL, pev );
		WRITE_BYTE(0);
	MESSAGE_END();

	// Move them to the new position
	UTIL_SetOrigin(pev, vecPosition );

	// Setup flags
	m_iHideHUD |= HIDEHUD_WEAPONS | HIDEHUD_FLASHLIGHT | HIDEHUD_HEALTH;
	pev->effects |= EF_NODRAW;

	pev->origin = vecPosition;
	pev->velocity = g_vecZero;
	pev->view_ofs = g_vecZero;
	pev->v_angle  = vecViewAngle;	
	pev->angles = vecViewAngle;	
	pev->punchangle = g_vecZero;
	pev->fixangle = true;

	pev->solid = SOLID_NOT;
	pev->takedamage = DAMAGE_NO;
	pev->movetype = MOVETYPE_NOCLIP;
	pev->deadflag = DEAD_RESPAWNABLE;
	ClearBits( m_afPhysicsFlags, PFLAG_DUCKING );
	ClearBits( pev->flags, FL_DUCKING );
	pev->fuser3 = pev->health = 1.0;
	m_afPhysicsFlags |= PFLAG_OBSERVER;
	
	//Ben - Make them invisible to the world
	pev->renderamt = 0; 
	pev->rendermode = kRenderTransAlpha; 

	// Clear out the status bar
	m_fInitHUD = true;

	// Remove all the player's stuff
	RemoveAllItems( false );

	// Find a player to watch
	Observer_SetMode(OBS_ROAMING);

	// Tell all clients this player is now a spectator
	MESSAGE_BEGIN( MSG_ALL, gmsgSpectator );  
		WRITE_BYTE( ENTINDEX( edict() ) );
		WRITE_BYTE( 1 );
	MESSAGE_END();

	m_iDead = DEAD;
	MESSAGE_BEGIN(MSG_ALL, gmsgDead);
	WRITE_BYTE(entindex());
	WRITE_BYTE(m_iDead);
	MESSAGE_END();

	g_engfuncs.pfnSetClientMaxspeed(edict(), 250);

	m_bHasDied = true;
}

// Leave observer mode
void CBasePlayer::StopObserver( void )
{
	// Tell all clients this player is not a spectator anymore
	MESSAGE_BEGIN( MSG_ALL, gmsgSpectator );  
		WRITE_BYTE( ENTINDEX( edict() ) );
		WRITE_BYTE( 0 );
	MESSAGE_END();

	pev->iuser1 = 0;
	m_iSpectatorMode = 0;
	pev->iuser2 = 0; 
	pev->iuser3 = 0;
	m_hObserverTarget = NULL;

	pev->movetype = MOVETYPE_WALK;
	ClearBits( m_afPhysicsFlags, PFLAG_DUCKING );
	ClearBits( pev->flags, FL_DUCKING );
	pev->fuser3 = pev->health = MAX_HEALTH;
	m_afPhysicsFlags |= PFLAG_OBSERVER;

	//Ben - Extras to make the player & your hud normal again
	
    m_iHideHUD &= ~HIDEHUD_WEAPONS;
    m_iHideHUD &= ~HIDEHUD_FLASHLIGHT;
    m_iHideHUD &= ~HIDEHUD_HEALTH;
	m_iHideHUD &= ~HIDEHUD_ALL;

	m_iHideHUD = 0;

	pev->solid = SOLID_SLIDEBOX;
	m_afPhysicsFlags &= ~PFLAG_OBSERVER;
	pev->effects = 0;
	pev->takedamage = DAMAGE_YES;
	pev->deadflag = DEAD_RESPAWNABLE;

	//Ben - Reset the reneder modes
	pev->renderamt = 255; 
	pev->rendermode = kRenderNormal;

	UpdateClientData();
}

// Find the next client in the game for this player to spectate
void CBasePlayer::Observer_FindNextPlayer( bool bReverse )
{
	// MOD AUTHORS: Modify the logic of this function if you want to restrict the observer to watching
	//				only a subset of the players. e.g. Make it check the target's team.

	if(m_iSpectatorMode == 0 || pev->iuser1 == 0)
		return;

	unsigned int iForceChaseCam = (int)CVAR_GET_FLOAT("mp_forcechasecam");

	int		iStart;
	if ( m_hObserverTarget )
		iStart = ENTINDEX( m_hObserverTarget->edict() );
	else
		iStart = ENTINDEX( edict() );

	int iCurrent = iStart;
	m_hObserverTarget = NULL;
	int iDir = bReverse ? -1 : 1; 

	do
	{
		iCurrent += iDir;

		// Loop through the clients
		if (iCurrent > gpGlobals->maxClients)
			iCurrent = 1;
		if (iCurrent < 1)
			iCurrent = gpGlobals->maxClients;

		CBaseEntity *pEnt = UTIL_PlayerByIndex( iCurrent );
		if ( !pEnt )
			continue;

		if ( pEnt == this )
			continue;

		// Don't spec observers or invisible players
		if ( ((CBasePlayer*)pEnt)->IsObserver() || (pEnt->pev->effects & EF_NODRAW) )
			continue;

		//Add check to see if on same team. If player is continue, if on different team don't continue
		if( (iForceChaseCam == 1) && (pEnt->pev->team == pev->team) )
			continue;

		// MOD AUTHORS: Add checks on target here.

		m_hObserverTarget = pEnt;
		break;

	} while ( iCurrent != iStart );

	// Did we find a target?
	if ( m_hObserverTarget )
	{
		// Store the target in pev so the physics DLL can get to it
		pev->iuser2 = ENTINDEX( m_hObserverTarget->edict() );
		// Move to the target
		UTIL_SetOrigin(pev, (m_hObserverTarget->pev->origin + m_hObserverTarget->pev->view_ofs) );
	}
	else{}
}

// Handle buttons in observer mode
void CBasePlayer::Observer_HandleButtons()
{
	// Slow down mouse clicks
	if ( m_flNextObserverInput > gpGlobals->time )
		return;

	// Jump changes from modes: Chase to Roaming
	if ( m_afButtonPressed & IN_JUMP )
	{
		if ( m_iSpectatorMode == OBS_CHASE_LOCKED )
			Observer_SetMode( OBS_CHASE_FREE );

		else if ( m_iSpectatorMode == OBS_CHASE_FREE )
			Observer_SetMode( OBS_IN_EYE );

		else if ( m_iSpectatorMode == OBS_IN_EYE )
			Observer_SetMode( OBS_ROAMING );

		else if ( m_iSpectatorMode == OBS_ROAMING )
			Observer_SetMode( OBS_MAP_FREE );

		else if ( m_iSpectatorMode == OBS_MAP_FREE )
			Observer_SetMode( OBS_MAP_CHASE );

		else
			Observer_SetMode( OBS_CHASE_LOCKED );

		m_flNextObserverInput = gpGlobals->time + 0.2;
	}

	// Attack moves to the next player
	if ( m_afButtonPressed & IN_ATTACK && m_iSpectatorMode != OBS_ROAMING )
	{
		Observer_FindNextPlayer( false );

		m_flNextObserverInput = gpGlobals->time + 0.2;
	}

	// Attack2 moves to the prev player
	if ( m_afButtonPressed & IN_ATTACK2 && m_iSpectatorMode != OBS_ROAMING )
	{
		Observer_FindNextPlayer( true );

		m_flNextObserverInput = gpGlobals->time + 0.2;
	}

	//ben - this shouldn't be used here, but i think it will do
	if(m_hObserverTarget)
		pev->health = m_hObserverTarget->pev->health;
}

// Attempt to change the observer mode
void CBasePlayer::Observer_SetMode( int iMode )
{
	// Just abort if we're changing to the mode we're already in
	if ( iMode == pev->iuser1 )
		return;

	unsigned int iForceChaseCam = (int)CVAR_GET_FLOAT("mp_forcechasecam");
	if(iForceChaseCam == 2) {
		UTIL_SetOrigin(pev, (pev->origin + pev->view_ofs));
		return;
	}

	pev->iuser1 = iMode;
	m_iSpectatorMode = iMode;

	// Changing to Roaming?
	if ( iMode == OBS_ROAMING )
	{
		// MOD AUTHORS: If you don't want to allow roaming observers at all in your mod, just abort here.
		pev->iuser2 = 0;
		ClientPrint( pev, HUD_PRINTCENTER, "#Spec_Mode3" );
		g_engfuncs.pfnSetClientMaxspeed(ENT(pev), 250);
		return;
	}

	if(m_hObserverTarget)
	{
		pev->iuser2 = ENTINDEX(m_hObserverTarget->edict());
		pev->health = m_hObserverTarget->pev->health;
		UTIL_SetOrigin(pev, (m_hObserverTarget->pev->origin + m_hObserverTarget->pev->view_ofs));
	}

	// Changing to Chase Lock?
	if ( iMode == OBS_CHASE_LOCKED )
	{
		if (m_hObserverTarget)
		{
			ClientPrint( pev, HUD_PRINTCENTER, "#Spec_Mode1" );
		}
		else
		{
			ClientPrint( pev, HUD_PRINTCENTER, "#Spec_NoTarget"  );
			Observer_SetMode(OBS_ROAMING);
		}
	}

	// Changing to Chase Freelook?
	else if ( iMode == OBS_CHASE_FREE )
	{
		if (m_hObserverTarget)
		{
			ClientPrint( pev, HUD_PRINTCENTER, "#Spec_Mode2" );
		}
		else
		{
			ClientPrint( pev, HUD_PRINTCENTER, "#Spec_NoTarget"  );
			Observer_SetMode(OBS_ROAMING);
		}
	}

	else if( iMode == OBS_IN_EYE )
	{
		if (m_hObserverTarget)
		{
			ClientPrint( pev, HUD_PRINTCENTER, "#Spec_Mode4" );
		}
		else
		{
			ClientPrint( pev, HUD_PRINTCENTER, "#Spec_NoTarget"  );
			Observer_SetMode(OBS_ROAMING);
		}
	}

	else if( iMode == OBS_MAP_FREE )
	{
		pev->iuser2 = 0;
		ClientPrint( pev, HUD_PRINTCENTER, "#Spec_Mode5" );
		g_engfuncs.pfnSetClientMaxspeed(ENT(pev), 250);
	}

	else if( iMode == OBS_MAP_CHASE )
	{
		if (m_hObserverTarget)
		{
			ClientPrint( pev, HUD_PRINTCENTER, "#Spec_Mode6" );
		}
		else
		{
			ClientPrint( pev, HUD_PRINTCENTER, "#Spec_NoTarget"  );
			Observer_SetMode(OBS_ROAMING);
		}
	}
	else
	{
		Observer_SetMode(OBS_CHASE_LOCKED);
	}
//Ben
}
