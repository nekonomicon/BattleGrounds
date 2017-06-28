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

#ifndef DECALS_H
#define DECALS_H

//
// Dynamic Decals
//
enum decal_e 
{	
	DECAL_GUNSHOT1 = 0, 
	DECAL_GUNSHOT2,
	DECAL_GUNSHOT3,
	DECAL_GUNSHOT4,
	DECAL_GUNSHOT5,
	DECAL_LAMBDA1,
	DECAL_LAMBDA2,
	DECAL_LAMBDA3,
	DECAL_LAMBDA4,
	DECAL_LAMBDA5,
	DECAL_LAMBDA6,
	DECAL_SCORCH1,
	DECAL_SCORCH2,
	DECAL_BLOOD1, 
	DECAL_BLOOD2, 
	DECAL_BLOOD3, 
	DECAL_BLOOD4, 
	DECAL_BLOOD5, 
	DECAL_BLOOD6, 
	DECAL_YBLOOD1, 
	DECAL_YBLOOD2, 
	DECAL_YBLOOD3, 
	DECAL_YBLOOD4, 
	DECAL_YBLOOD5, 
	DECAL_YBLOOD6, 
	DECAL_GLASSBREAK1,
	DECAL_GLASSBREAK2,
	DECAL_GLASSBREAK3,
	DECAL_BIGSHOT1,
	DECAL_BIGSHOT2,
	DECAL_BIGSHOT3,
	DECAL_BIGSHOT4,
	DECAL_BIGSHOT5,
	DECAL_SPIT1,
	DECAL_SPIT2,
	DECAL_BPROOF1,		// Bulletproof glass decal
	DECAL_GARGSTOMP1,	// Gargantua stomp crack
	DECAL_SMALLSCORCH1,	// Small scorch mark
	DECAL_SMALLSCORCH2,	// Small scorch mark
	DECAL_SMALLSCORCH3,	// Small scorch mark
	DECAL_MOMMABIRTH,	// Big momma birth splatter
	DECAL_MOMMASPLAT,
};

typedef struct 
{
	char	*name;
	int		index;
} DLL_DECALLIST;

extern DLL_DECALLIST gDecals[];

#endif	// DECALS_H
