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

//This file was created on the 20/10/2K+2 at 12:52 by Ben Banfield for the Battle Grounds modifcation of Hlaf-Life
//Its purpose is to declare the class which will handle all bg specific events

#ifndef EVENT_BG_H
#define EVENT_BG_H

class CBgEffects;
//a nice class to keep the engine away from the nity gritty of the events
class CEvent
{
public:
	//give bg effects access to our private details
	friend class CBgEffects;

	//constructor
	CEvent() { ClearAllEventVars(); g_iSwing = 0; }
	~CEvent() { ClearAllEventVars(); g_iSwing = 0; }

	//generic gun shot event handler
	void EV_FireGun( struct event_args_s *args);
	//generic melee event handler
	void EV_Melee( struct event_args_s *args);
	//generic animation event handler
	void EV_Animation( struct event_args_s *args);
	// round end event
	void EV_RoundEnd( struct event_args_s *args );

	//standard hl event functions
	char *EV_DamageDecal( physent_t *pe );
	int EV_CheckTracer( int idx, float *vecSrc, float *end, float *forward, float *right, int iBulletType, int iTracerFreq, int *tracerCount );
	void EV_FireBullets( int idx, float *forward, float *right, float *up, int cShots, float *vecSrc, float *vecDirShooting, float flDistance, int iBulletType, int iTracerFreq, int *tracerCount, float flSpreadX, float flSpreadY );
	
	char TextureTypeAtEnd( void );

private:
	//swing counter
	int	g_iSwing;
	//player index
	int idx;
	//vectors
	vec3_t origin, angles, up, right, forward, vecVelocity, vecSrc, vecAiming, vecEnd, vecPos;
	//weapon data
	CWeaponData *pData;
	//trace results
	pmtrace_t *pTrace;
	//entity at the end info
	physent_t *pEntity;
	//event args
	event_args_s *args;

	//entity number
	int iEnt;
	//texture name 
	char* sTextureName;
	//texture type
	char cTextureType;

	//clean all the variables in this class, so there is no chance we are using something from a previous event
	void ClearAllEventVars();
	//fill all the variables in this class, with the details from the current event
	void FillEventVars(struct event_args_s *args);
};

//deals with punch axis
extern void V_PunchAxis(int aixs, float punch);

//this is specially required for the tomahawk callback
extern void EV_TomahawkTouch1( struct tempent_s *ent, pmtrace_t *ptr );

//gets the gun data and somewhere to store it
extern CWeaponData* GetGunsData(struct event_args_s *args);
extern CEvent *pEvent;

//class for effects
class CBgEffects
{
public:
	CBgEffects();
	~CBgEffects() {}

	void DisplayEffects( void );
	void DecalAtPosition(pmtrace_t *pTrace);
private:
	void SetEffects( void );

	void SparkAtEnd( void );
	void Blood( void );

	void SoundAtEnd( void );
	void DecalAtEnd( void );

	void SmokeAtGun( void );
	void SmokeAtEnd( void );

	bool bSparkAtEnd;
	bool bSmokeAtEnd;
	bool bBlood;
};
#endif