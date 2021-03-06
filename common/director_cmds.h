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

// director_cmds.h
// sub commands for svc_director

#define DRC_ACTIVE				0	// tells client that he's an spectator and will get director command
#define DRC_STATUS				1	// send status infos about proxy 
#define DRC_CAMERA				2	// set the actual director camera position
#define DRC_EVENT				3	// informs the dircetor about ann important game event


#define DRC_FLAG_PRIO_MASK		0x0F	//	priorities between 0 and 15 (15 most important)
#define DRC_FLAG_SIDE			(1<<4)	
#define DRC_FLAG_DRAMATIC		(1<<5)



// commands of the director API function CallDirectorProc(...)

#define DRCAPI_NOP					0	// no operation
#define DRCAPI_ACTIVE				1	// de/acivates director mode in engine
#define DRCAPI_STATUS				2   // request proxy information
#define DRCAPI_SETCAM				3	// set camera n to given position and angle
#define DRCAPI_GETCAM				4	// request camera n position and angle
#define DRCAPI_DIRPLAY				5	// set director time and play with normal speed
#define DRCAPI_DIRFREEZE			6	// freeze directo at this time
#define DRCAPI_SETVIEWMODE			7	// overview or 4 cameras 
#define DRCAPI_SETOVERVIEWPARAMS	8	// sets parameter for overview mode
#define DRCAPI_SETFOCUS				9	// set the camera which has the input focus
#define DRCAPI_GETTARGETS			10	// queries engine for player list
#define DRCAPI_SETVIEWPOINTS		11	// gives engine all waypoints


