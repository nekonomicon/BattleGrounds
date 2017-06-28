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

//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============

#if !defined ( EV_HLDMH )
#define EV_HLDMH
// bullet types
typedef enum bullet_e
{
	BULLET_NONE = 0,
	BULLET_BLADE,
	BULLET_SHOT,
	BULLET_CANNON,
};

enum knife_e {
	KNIFE_IDLE = 0,
	KNIFE_DRAW,
	KNIFE_HOLSTER,
	KNIFE_ATTACK1HIT,
	KNIFE_ATTACK1MISS,
	KNIFE_ATTACK2MISS,
	KNIFE_ATTACK2HIT,
	KNIFE_ATTACK3MISS,
	KNIFE_ATTACK3HIT,
	KNIFE_IDLE2,
	KNIFE_IDLE3
};

enum sabre_e {
	SABRE_IDLE = 0,
	SABRE_DRAW,
	SABRE_HOLSTER,
	SABRE_ATTACK1HIT,
	SABRE_ATTACK1MISS,
	SABRE_ATTACK2MISS,
	SABRE_ATTACK2HIT,
	SABRE_ATTACK3MISS,
	SABRE_ATTACK3HIT,
	SABRE_IDLE2,
	SABRE_IDLE3
};

enum tomahawk_e {
	TOMAHAWK_IDLE = 0,	
	TOMAHAWK_DRAW,
	TOMAHAWK_HOLSTER,
	TOMAHAWK_THROW,
	TOMAHAWK_ATTACK,
	TOMAHAWK_ATTACK2,
	TOMAHAWK_ATTACK3
};

#endif // EV_HLDMH
