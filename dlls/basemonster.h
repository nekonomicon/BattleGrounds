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
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
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
*/

#ifndef BASEMONSTER_H
#define BASEMONSTER_H

class CBaseMonster : public CBaseToggle
{
public:
	Activity			m_Activity;// what the monster is doing (animation)
	Activity			m_IdealActivity;// monster should switch to this activity
	int					m_LastHitGroup; // the last body region that took damage
	int					m_bitsDamageType;	// what types of damage has monster (player) taken
	BYTE				m_rgbTimeBasedDamage[CDMG_TIMEBASED];
	MONSTERSTATE		m_MonsterState;// monster's current state
	MONSTERSTATE		m_IdealMonsterState;// monster should change to this state
	int					m_afConditions;
	int					m_afMemory;
	float				m_flNextAttack;		// cannot attack again until this time
	EHANDLE				m_hEnemy;		 // the entity that the monster is fighting.
	EHANDLE				m_hTargetEnt;	 // the entity that the monster is trying to reach
	float				m_flFieldOfView;// width of monster's field of view ( dot product )
	int					m_bloodColor;		// color of blood particless
	Vector				m_HackedGunPos;	// HACK until we can query end of gun
	Vector				m_vecEnemyLKP;// last known position of enemy. (enemy's origin)


	void KeyValue( KeyValueData *pkvd );

	void MakeIdealYaw( Vector vecTarget );
	virtual float ChangeYaw ( int speed );
	virtual bool HasHumanGibs( void );
	virtual bool HasAlienGibs( void );
	virtual void FadeMonster( void );	// Called instead of GibMonster() when gibs are disabled
	virtual void GibMonster( void );
	virtual Activity GetDeathActivity ( void );
	Activity GetSmallFlinchActivity( void );
	virtual void BecomeDead( void );
	bool		 ShouldGibMonster( int iGib );
	void		 CallGibMonster( void );
	virtual bool	ShouldFadeOnDeath( void );
	bool FCheckAITrigger( void );// checks and, if necessary, fires the monster's trigger target. 
	virtual int IRelationship ( CBaseEntity *pTarget );
	virtual int TakeHealth( float flHealth, int bitsDamageType );
	virtual int TakeDamage( entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType);
	int			DeadTakeDamage( entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType );
	float DamageForce( float damage );
	virtual void Killed( entvars_t *pevAttacker, int iGib );
	virtual void PainSound ( void ) { return; };

	void RadiusDamage(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int iClassIgnore, int bitsDamageType );
	void RadiusDamage(Vector vecSrc, entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int iClassIgnore, int bitsDamageType );

	inline void	SetConditions( int iConditions ) { m_afConditions |= iConditions; }
	inline void	ClearConditions( int iConditions ) { m_afConditions &= ~iConditions; }
	inline bool HasConditions( int iConditions ) { if ( m_afConditions & iConditions ) return true; return false; }
	inline bool HasAllConditions( int iConditions ) { if ( (m_afConditions & iConditions) == iConditions ) return true; return false; }

	inline void	Remember( int iMemory ) { m_afMemory |= iMemory; }
	inline void	Forget( int iMemory ) { m_afMemory &= ~iMemory; }
	inline bool HasMemory( int iMemory ) { if ( m_afMemory & iMemory ) return true; return false; }
	inline bool HasAllMemories( int iMemory ) { if ( (m_afMemory & iMemory) == iMemory ) return true; return false; }

	// This will stop animation until you call ResetSequenceInfo() at some point in the future
	inline void StopAnimation( void ) { pev->framerate = 0; }

	virtual void ReportAIState( void );
	virtual void MonsterInitDead( void );	// Call after animation/pose is set up
	void EXPORT CorpseFallThink( void );

	virtual void Look ( int iDistance );// basic sight function for monsters
	virtual CBaseEntity* BestVisibleEnemy ( void );// finds best visible enemy for attack
	CBaseEntity *CheckTraceHullAttack( float flDist, int iDamage, int iDmgType );
	virtual bool FInViewCone ( CBaseEntity *pEntity );// see if pEntity is in monster's view cone
	virtual bool FInViewCone ( Vector *pOrigin );// see if given location is in monster's view cone
	void TraceAttack( entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType);
	void MakeDamageBloodDecal ( int cCount, float flNoise, TraceResult *ptr, const Vector &vecDir );
	virtual bool	IsAlive( void ) { return (pev->deadflag != DEAD_DEAD); }

};


#endif
