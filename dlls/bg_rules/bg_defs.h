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

//This file was created on the 16/3/2K+2 at 13:31 by Ben Banfield for the Battle Grounds modfiication of Half-Life
//Its purpose is to allow common defines to be shared between the server & the client dll

#ifndef BG_DEFS
#define BG_DEFS

//general defines
const char* const sBgVersion = "1.0F";

const int NUM_TEAMS = 3;
const int NUM_CLASSES = 3;
const int NAME_LENGTH = 20;

//dispaly names for the teams
const char szTeamNames[NUM_TEAMS+1][NAME_LENGTH] =
{
	"",
	"British",
	"Americans",
	"Spectators"
};

const char szClassNames[NUM_TEAMS+1][NUM_CLASSES+1][NAME_LENGTH] =
{
	{
		""
	},
	//British Classes
	{
		"",
		"Royal Commander",
		"Royal Infantry",
		"Loyalist"

	},
	//American Classes
	{
		"",
		"Continental Officer",
		"Minuteman",
		"Continental Soldier"
	},
	//Spectator Classe
	{
		"",
		"",
		"",
		""
	}
};

//short names for the classes, which will be used for models, text file, etc
const char szClassCodes[NUM_TEAMS+1][NUM_CLASSES+1][NAME_LENGTH] =
{
	{
		""
	},
	//British Classes
	{
		"",
		"light_b",
		"medium_b",
		"heavy_b"
	},
	//American Classes
	{
		"",
		"light_a",
		"medium_a",
		"heavy_a"
	},
	//Spectator Classe
	{
		""
	}
};


// class numbers
const int LIGHT = 1;
const int MEDIUM = 2;
const int HEAVY = 3;

//class speeds
const int iClassSpeeds[NUM_TEAMS+1][NUM_CLASSES+1] =
{
	{
		0
	},
	{
		0,
		235,
		235,
		235
	},
	{
		0,
		235,
		235,
		235
	},
	{
		250,
		250,
		250,
		250
	}
};

//numbers of the teams to access the arrays & more
const int UNDEFINED = 0;
const int BRITS = 1;
const int AMERS = 2;
const int SPECS = 3;

//quick access to the team names
const char* const British = szTeamNames[BRITS];
const char* const Americans = szTeamNames[AMERS];
const char* const Spectators = szTeamNames[SPECS];

const int MENU_DEFAULT = 1;
const int MENU_TEAM = 2;
const int MENU_BRITISH_CLASS = 3;
const int MENU_AMERICAN_CLASS = 4;
const int MENU_MAPBRIEFING = 5;
const int MENU_INTRO = 6;
const int MENU_CLASSHELP = 7;
const int MENU_CLASSHELP2 = 8;
const int MENU_REPEATHELP = 9;
const int MENU_SPECHELP = 10;

const float MAX_STAMINA = 100;
//amount of stamina regained a second - in this case it would take 20seconds to fill up on stamina again
const float STAMINA_REGAIN_MOVING = (-10);
const float STAMINA_REGAIN_STILL = (-20);
const int MAX_HEALTH = 100;
const float STAMINA_LOSS_JUMP = 51;

typedef enum menus_e
{
	NO_MENU,
	//included 3 voice menus to save time later on if 3 menus are needed
	VOICE_MENU1,
	VOICE_MENU2,
	VOICE_MENU3,
	TEAM_MENU,
	BCLASS_MENU,
	ACLASS_MENU
} TEXT_MENU;

const int MAX_MESSAGES = 16;

const char szPointStatus[3+1][3][NAME_LENGTH] =
{
	{
		"",
		"",
		""
	},
	{
		"Free",
		"Capturing",
		"Captured"
	},
	{
		"Full Health",
		"Damaged",
		"Destroyed"
	},
	{
		"No-one Escaped",
		"Some Escaped",
		"Enough Escaped"
	}
};

// status of a point
typedef enum point_state_e
{
	NO_STATE = -2,
	HUD_RESET = -1,

	// capture point states
	FREE = 0,
	CAPTURING = 1,
	HELD = 2,
	
	// destruction point states
	FULL_HEALTH = 10,
	DAMAGED = 11,
	DESTROYED = 12,

	// escpae point states
	NO_ESCAPED = 20,
	SOME_ESCAPED = 21,
	ALL_REQUIRED_ESCAPED = 22,
} POINT_STATE;

// shows which teams can use this point
typedef enum target_team_e
{
	EITHER = 0,
	BRITISH = 1,
	AMERICANS = 2,
} TARGET_TEAM;

// shows whether a player can hold or not
typedef enum should_hold_e
{
	NOT_SET = 0,
	NO = 1,
	YES = 2,
} SHOULD_HOLD;

// shows which type of point this point is
typedef enum point_type_e
{
	NO_TYPE = 0,
	CAPTURE_POINT = 1,
	DESTRUCTION_POINT = 2,
	ESCAPE_POINT = 3,
} POINT_TYPE;

const int CLOCK_ROUND = 0;
const int CLOCK_PAUSE = 1;

// logging levels
typedef enum LOG_LEVEL
{
	BASIC_LOGGING = 0,
	DAMAGE_LOGGING = 1,
	POINT_LOGGING = 2,
	ROUND_LOGGING = 3,
	ALL_LOGGING = 4,
};

// team winning sounds
const char team_win_sounds[NUM_TEAMS+1][25] = 
{
	"",
	"misc/win_british.wav",
	"misc/win_american.wav",
	""
};

const int SHOT_CLEAR = -1;
const int SHOT_NONE = 0;
const int SHOT_LEFT_ARM = 1;
const int SHOT_RIGHT_ARM = 2;
const int SHOT_LEFT_LEG = 3;
const int SHOT_RIGHT_LEG = 4;

const int HITGROUP_GENERIC = 0;
const int HITGROUP_HEAD = 1;
const int HITGROUP_CHEST = 2;
const int HITGROUP_STOMACH = 3;
const int HITGROUP_LEFTARM = 4;	
const int HITGROUP_RIGHTARM = 5;
const int HITGROUP_LEFTLEG = 6;
const int HITGROUP_RIGHTLEG = 7;

const char szHitBox[HITGROUP_RIGHTLEG+1][NAME_LENGTH] =
{
	"Generic",
	"Head",
	"Chest",
	"Stomach",
	"Left Arm",
	"Right Arm",
	"Left Leg",
	"Right Leg"
};

const int MAX_POINTS = 10;

// Raiko Contrib - LB Score
#ifndef CLIENT_DLL
#define LINEBATTLE CVAR_GET_FLOAT( "mp_linebattle")
#define BRITISHSCORE CVAR_GET_FLOAT("mp_britishscore")
#define AMERICANSCORE CVAR_GET_FLOAT("mp_americanscore")
#endif
// Raiko Contrib End

// Hintbox help messages
const int HINTBOX_JUMP = 1;
const int HINTBOX_CROUCH = 2;
const int HINTBOX_RELOAD = 3;
const int HINTBOX_FLAGCAP = 4;
const int HINTBOX_FLAGOVERLOAD = 5;
const int HINTBOX_HINTBOX = 6;
const int HINTBOX_LOWSTAMINA = 7;
const int HINTBOX_TEAMSCORE = 8;
const int HINTBOX_PERSONALSCORE = 9;
const int HINTBOX_MUTE = 10;
const int HINTBOX_LOWAMMO = 11;	
const int HINTBOX_MELEE = 12;
const int HINTBOX_MELEE2 = 13;
const int HINTBOX_CAMPING = 14;
const int HINTBOX_CROUCH2 = 15;
const int HINTBOX_INACCURATE = 16;
const int HINTBOX_DISPLAYMODE = 17;
const int HINTBOX_PROGSPAWNING = 18;

// Hintbox class stats
const int HINTBOX_CLASS_LIGHTA = 21;
const int HINTBOX_CLASS_MEDA = 22;
const int HINTBOX_CLASS_HEAVYA = 23;
const int HINTBOX_CLASS_LIGHTB = 24;
const int HINTBOX_CLASS_MEDB = 25;
const int HINTBOX_CLASS_HEAVYB = 26;

// Hintbox advanced help messages


//BP commanders special ability/aura values
const int RUSH_DURATION = 20; //seconds			
const int RUSH_COST = 100; //required points	
const int RUSH_AURA_RANGE = 20; //HL-units
const float RUSH_SPEEDMODIFIER = 10.0f; //rushing speed factor

// commanders menu special abilities
const int iRush = 1;
const int iAccuracy = 2;
const int iDamage = 3;
const int iArtillery = 4;
const int iMarkEnemy = 5;
const int iReinforcements = 6;

// and for messages to synchronize if an ability is full or not
const int iRushFull = 101;
const int iArtilleryFull = 104;

#endif
