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
*/

#include "extdll.h"
#include "eiface.h"
#include "util.h"
#include "game.h"

cvar_t	displaysoundlist = {"displaysoundlist","0"};

// multiplayer server rules
cvar_t	fragsleft	= {"mp_fragsleft","0", FCVAR_SERVER | FCVAR_UNLOGGED };	  // Don't spam console/log files/users with this changing
cvar_t	timeleft	= {"mp_timeleft","30" , FCVAR_SERVER | FCVAR_UNLOGGED };	  // "      "

//Ben - Bg CVAR's

// are speccies allowed?
cvar_t  allow_spectators = { "allow_spectators", "1.0", FCVAR_SERVER };		// 0 prevents players from being spectators

//length of a round
cvar_t	roundtime	= {"mp_roundtime", "15", FCVAR_SERVER };
//time between end of round and everything restarting
cvar_t	restarttime	= {"mp_restarttime", "1", FCVAR_SERVER };
//number of rounds before a new map
cvar_t	roundsleft	= {"mp_roundsleft", "0", FCVAR_SERVER };
//paused time at the start of a round
cvar_t	roundpause	= {"mp_pausetime", "2", FCVAR_SERVER };
//style of respawning used
cvar_t	respawnstyle = {"mp_respawnstyle", "1", FCVAR_SERVER };
// auto balance teams
cvar_t	balance_teams = {"mp_autoteambalance", "1", FCVAR_SERVER };
// number of players teams can be unbalanced by
cvar_t	team_difference = {"mp_limitteams", "2", FCVAR_SERVER };
// restart round var
cvar_t	restart_round = {"sv_restartround", "0.0", FCVAR_SERVER };
// bg version number cvar - so i don't have to guess :)
cvar_t	version = {"mp_version", const_cast<char*>(sBgVersion), FCVAR_UNLOGGED | FCVAR_SERVER | FCVAR_SPONLY };
// logging detail cvar
cvar_t	log_detail = {"mp_logdetail", "2", FCVAR_SERVER };
// win points cvar
cvar_t	win_points = {"mp_winbonus", "200", FCVAR_SERVER };
// controls spec modes available
cvar_t	chasecam = {"mp_forcechasecam", "0", FCVAR_SERVER };

// class limit cvars
cvar_t	classlimit_lighta = {"mp_limit_light_a", "-1", FCVAR_SERVER };
cvar_t	classlimit_lightb = {"mp_limit_light_b", "-1", FCVAR_SERVER };
cvar_t	classlimit_mediuma = {"mp_limit_medium_a", "-1", FCVAR_SERVER };
cvar_t	classlimit_mediumb = {"mp_limit_medium_b", "-1", FCVAR_SERVER };
cvar_t	classlimit_heavya = {"mp_limit_heavy_a", "-1", FCVAR_SERVER };
cvar_t	classlimit_heavyb = {"mp_limit_heavy_b", "-1", FCVAR_SERVER };

// Raiko Contrib - Melee Only
cvar_t meleeonly = {"mp_meleeonly", "0", FCVAR_SERVER};
// Raiko Contrib - Stationary Reload
cvar_t stationaryreload = {"mp_stationaryreload", "0", FCVAR_SERVER};
// Raiko Contrib - LB Score
cvar_t linebattle = {"mp_linebattle", "0", FCVAR_SERVER};
cvar_t britscore = {"mp_britishscore", "0", FCVAR_SERVER | FCVAR_UNLOGGED};
cvar_t amerscore = {"mp_americanscore", "0", FCVAR_SERVER | FCVAR_UNLOGGED};
// Raiko Contrib End

// BP LaserTag
cvar_t lasertag = {"mp_lasertag", "0", FCVAR_SERVER};

// Zer0 Contrib - No voice commands
cvar_t voice_commands = {"mp_voice_commands", "1", FCVAR_SERVER};
// Zer0 Contrib End

// Ben - Bg CVAR End

// multiplayer server rules
cvar_t	friendlyfire= {"mp_friendlyfire","0", FCVAR_SERVER };
cvar_t	fraglimit	= {"mp_fraglimit","0", FCVAR_SERVER };
cvar_t	timelimit	= {"mp_timelimit","0", FCVAR_SERVER };
cvar_t	falldamage	= {"mp_falldamage","1", FCVAR_SERVER };
cvar_t	aimcrosshair= {"mp_autocrosshair","0", FCVAR_SERVER };
cvar_t	decalfrequency	= {"decalfrequency","30", FCVAR_SERVER };

cvar_t  mp_chattime = {"mp_chattime","6", FCVAR_SERVER };

// Engine Cvars
cvar_t 	*g_psv_gravity = NULL;
cvar_t	*g_psv_aim = NULL;
cvar_t	*g_footsteps = NULL;


// END Cvars for Skill Level settings

// Register your console variables here
// This gets called one time when the game is initialied
void GameDLLInit( void )
{
	// Register cvars here:

	g_psv_gravity = CVAR_GET_POINTER( "sv_gravity" );
	g_psv_aim = CVAR_GET_POINTER( "sv_aim" );
	g_footsteps = CVAR_GET_POINTER( "mp_footsteps" );

	CVAR_REGISTER (&allow_spectators);
	CVAR_REGISTER (&displaysoundlist);

	CVAR_REGISTER (&roundtime);
	CVAR_REGISTER (&restarttime);
	CVAR_REGISTER (&roundsleft);
	CVAR_REGISTER (&roundpause);

	CVAR_REGISTER (&respawnstyle);

	CVAR_REGISTER (&balance_teams);
	CVAR_REGISTER (&team_difference);

	CVAR_REGISTER (&restart_round);

	CVAR_REGISTER (&version);

	CVAR_REGISTER (&log_detail);

	CVAR_REGISTER (&win_points);

	CVAR_REGISTER (&chasecam);

	CVAR_REGISTER (&classlimit_lighta);
	CVAR_REGISTER (&classlimit_lightb);
	CVAR_REGISTER (&classlimit_mediuma);
	CVAR_REGISTER (&classlimit_mediumb);
	CVAR_REGISTER (&classlimit_heavya);
	CVAR_REGISTER (&classlimit_heavyb);

	// Raiko Contrib - Melee Only
	CVAR_REGISTER(&meleeonly);
	//Raiko Contrib - Stationary Reload
	CVAR_REGISTER(&stationaryreload);
	// Raiko Contrib - LB Score
	CVAR_REGISTER(&linebattle);
	CVAR_REGISTER(&britscore);
	CVAR_REGISTER(&amerscore);
	// Raiko Contrib End

	// BP LaserTag
	CVAR_REGISTER(&lasertag);

	// Zer0 Contrib - No voice commands
	CVAR_REGISTER(&voice_commands);
	// Zer0 Contrib End

	CVAR_REGISTER (&fraglimit);
	CVAR_REGISTER (&timelimit);

	CVAR_REGISTER (&fragsleft);
	CVAR_REGISTER (&timeleft);

	CVAR_REGISTER (&friendlyfire);
	CVAR_REGISTER (&falldamage);
	CVAR_REGISTER (&aimcrosshair);
	CVAR_REGISTER (&decalfrequency);

	CVAR_REGISTER (&mp_chattime);

	SERVER_COMMAND( "exec skill.cfg\n" );
}

