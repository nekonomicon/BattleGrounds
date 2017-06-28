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
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
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
****/

#include "hud.h"
#include "cl_util.h"
#include "demo.h"
#include "demo_api.h"
#include <memory.h>

#define DLLEXPORT __declspec( dllexport )

int g_demosniper = 0;
int g_demosniperdamage = 0;
float g_demosniperorg[3];
float g_demosniperangles[3];
float g_demozoom;

// FIXME:  There should be buffer helper functions to avoid all of the *(int *)& crap.

extern "C" 
{
	void DLLEXPORT Demo_ReadBuffer( int size, unsigned char *buffer );
}

/*
=====================
Demo_WriteBuffer

Write some data to the demo stream
=====================
*/
void Demo_WriteBuffer( int type, int size, unsigned char *buffer )
{
	int pos = 0;
	unsigned char buf[ 32 * 1024 ];
	*( int * )&buf[pos] = type;
	pos+=sizeof( int );

	memcpy( &buf[pos], buffer, size );

	// Write full buffer out
	gEngfuncs.pDemoAPI->WriteBuffer( size + sizeof( int ), buf );
}

/*
=====================
Demo_ReadBuffer

Engine wants us to parse some data from the demo stream
=====================
*/
void DLLEXPORT Demo_ReadBuffer( int size, unsigned char *buffer )
{
	int type;
	int i = 0;

	type = *( int * )buffer;
	i += sizeof( int );
	switch ( type )
	{
	case TYPE_SNIPERDOT:
		g_demosniper = *(int * )&buffer[ i ];
		i += sizeof( int );
		
		if ( g_demosniper )
		{
			g_demosniperdamage = *( int * )&buffer[ i ];
			i += sizeof( int );

			g_demosniperangles[ 0 ] = *(float *)&buffer[i];
			i += sizeof( float );
			g_demosniperangles[ 1 ] = *(float *)&buffer[i];
			i += sizeof( float );
			g_demosniperangles[ 2 ] = *(float *)&buffer[i];
			i += sizeof( float );
			g_demosniperorg[ 0 ] = *(float *)&buffer[i];
			i += sizeof( float );
			g_demosniperorg[ 1 ] = *(float *)&buffer[i];
			i += sizeof( float );
			g_demosniperorg[ 2 ] = *(float *)&buffer[i];
			i += sizeof( float );
		}
		break;
	case TYPE_ZOOM:
		g_demozoom = *(float * )&buffer[ i ];
		i += sizeof( float );
		break;
	default:
		gEngfuncs.Con_DPrintf( "Unknown demo buffer type, skipping.\n" );
		break;
	}
}