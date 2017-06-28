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
//
//  parsemsg.cpp

#include "hud.h"

typedef unsigned char byte;
#define true 1

static byte *gpBuf;
static int giSize;
static int giRead;
static int giBadRead;

void BEGIN_READ( void *buf, int size )
{
	giRead = 0;
	giBadRead = 0;
	giSize = size;
	gpBuf = (byte*)buf;
}


int READ_CHAR( void )
{
	int     c;
	
	if (giRead + 1 > giSize)
	{
		giBadRead = true;
		return -1;
	}
		
	c = (signed char)gpBuf[giRead];
	giRead++;
	
	return c;
}

int READ_BYTE( void )
{
	int     c;
	
	if (giRead+1 > giSize)
	{
		giBadRead = true;
		return -1;
	}
		
	c = (unsigned char)gpBuf[giRead];
	giRead++;
	
	return c;
}

int READ_SHORT( void )
{
	int     c;
	
	if (giRead+2 > giSize)
	{
		giBadRead = true;
		return -1;
	}
		
	c = (short)( gpBuf[giRead] + ( gpBuf[giRead+1] << 8 ) );
	
	giRead += 2;
	
	return c;
}

int READ_WORD( void )
{
	return READ_SHORT();
}


int READ_LONG( void )
{
	int     c;
	
	if (giRead+4 > giSize)
	{
		giBadRead = true;
		return -1;
	}
		
 	c = gpBuf[giRead] + (gpBuf[giRead + 1] << 8) + (gpBuf[giRead + 2] << 16) + (gpBuf[giRead + 3] << 24);
	
	giRead += 4;
	
	return c;
}

float READ_FLOAT( void )
{
	union
	{
		byte    b[4];
		float   f;
		int     l;
	} dat;
	
	dat.b[0] = gpBuf[giRead];
	dat.b[1] = gpBuf[giRead+1];
	dat.b[2] = gpBuf[giRead+2];
	dat.b[3] = gpBuf[giRead+3];
	giRead += 4;
	
//	dat.l = LittleLong (dat.l);

	return dat.f;   
}

char* READ_STRING( int i = 2047 )
{
	static char     string[2048];
	int             l,c,iSize;


	(i > (2048-1)) ? iSize = 2047 : iSize = i-1;

	string[0] = 0;

	l = 0;
	do
	{
		if ( giRead+1 > giSize )
			break; // no more characters

		// check against the sent arguament
		if( l + 1 > iSize)
			break;

		c = READ_CHAR();
		if (c == -1 || c == 0)
			break;
		string[l] = (char)c;
		l++;
	} while (l < sizeof(string)-1);
	
	string[l] = '\0';
	
	if(&string == NULL)
	{
	#ifdef _DEBUG
		_asm{ int 3};
#endif
		gEngfuncs.pfnConsolePrint("Null String in read_string\n");
	}

	return string;
}

float READ_COORD( void )
{
	return (float)(READ_SHORT() * (1.0/8));
}

float READ_ANGLE( void )
{
	return (float)(READ_CHAR() * (360.0/256));
}

float READ_HIRESANGLE( void )
{
	return (float)(READ_SHORT() * (360.0/65536));
}

