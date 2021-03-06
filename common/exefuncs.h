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

//========= Copyright � 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

// exefuncs.h
#ifndef EXEFUNCS_H
#define EXEFUNCS_H

// Engine hands this to DLLs for functionality callbacks
typedef struct exefuncs_s
{
	int			fMMX;
	int			iCPUMhz;
	void		(*unused1)(void);
	void		(*unused2)(void);
	void		(*unused3)(void);
	void		(*unused4)(void);
	void		(*VID_ForceLockState)(int lk);
	int			(*VID_ForceUnlockedAndReturnState)(void);
	void		(*unused5)(void);
	void		(*unused6)(void);
	void		(*unused7)(void);
	void		(*unused8)(void);
	void		(*unused9)(void);
	void		(*unused10)(void);
	void		(*unused11)(void);
	void		(*unused12)(void);
	void		(*unused13)(void);
	void		(*unused14)(void);
	void		(*unused15)(void);
	void        (*ErrorMessage)(int nLevel, const char *pszErrorMessage);
	void		(*unused16)(void);
	void        (*Sys_Printf)(char *fmt, ...);
	void		(*unused17)(void);
	void		(*unused18)(void);
	void		(*unused19)(void);
	void		(*unused20)(void);
	void		(*unused21)(void);
	void		(*unused22)(void);
	void		(*unused23)(void);
	void		(*unused24)(void);
	void		(*unused25)(void);
	void		(*unused26)(void);
	void		(*unused27)(void);
} exefuncs_t;

#endif
