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

//This file was created on the 6/6/03 at 15:53 by Ben Banfield for the Battle Grounds modification for Half-Life
//Its purpose is to define messages sent to the client.

#ifndef MESSAGES_H
#define MESSAGES_H

const int iVoiceCommands = 1; // chat
const int iHitMsg_Attacker = 2; // colour message
const int iHitMsg_Victim = 3; // colour message
const int iTeamClass = 4; // chat
const int iTeamClassMe = 5; // chat 
const int iCapturePointMessages = 6; // colour message
const int iRoundWon = 7;

const int TITLE_LENGTH = 30;
const int NUM_VOICE_COMMANDS = 17;

const int iCmd_Yes = 0;
const int iCmd_No = 1;
const int iCmd_Follow = 2;
const int iCmd_Leave_To_Me = 3;
const int iCmd_Need_Help = 4;
const int iCmd_Spread_Out = 5;
const int iCmd_Brit_Battlecry = 6;
const int iCmd_Amer_Battlecry = 7;
const int iCmd_Medical_Aid = 8;
const int iCmd_Move_On = 9;
const int iCmd_Retreat = 10;
const int iCmd_Rally = 11;
const int iCmd_Halt = 12;
const int iCmd_Line_Up = 13;
const int iCmd_Make_Ready = 14;
const int iCmd_Present_Arms = 15;
const int iCmd_Fire = 16;
const int iCmd_Cease_Fire = 17;

const char szCommandTitles[NUM_VOICE_COMMANDS+1][TITLE_LENGTH] =
{
	"#CMD_YES",
	"#CMD_NO",
	"#CMD_FOLLOW",
	"#CMD_LEAVE_TO_ME",
	"#CMD_NEED_HELP",
	"#CMD_SPREAD_OUT",
	"#CMD_BRIT_BATTLECRY",
	"#CMD_AMER_BATTLECRY",
	"#CMD_MEDICAL_AID",
	"#CMD_MOVE_ON",
	"#CMD_RETREAT",
	"#CMD_RALLY",
	"#CMD_HALT",
	"#CMD_LINE_UP",
	"#CMD_MAKE_READY",
	"#CMD_PRESENT_ARMS",
	"#CMD_FIRE",
	"#CMD_CEASE_FIRE"
};

const int NUM_HITGROUPS = 7;
const char szHitgroupTitles[NUM_HITGROUPS+1][TITLE_LENGTH] =
{
	"#HIT_GENERIC",
	"#HIT_HEAD",
	"#HIT_CHEST",
	"#HIT_STOMACH",
	"#HIT_LEFT_ARM",
	"#HIT_RIGHT_ARM",
	"#HIT_LEFT_LEG",
	"#HIT_RIGHT_LEG"
};

const int NUM_HIT_OTHER = 4;

const int iHitMe = 0;
const int iHitSomeone = 1;
const int iHitWhere = 2;
const int iHitWhereMe = 3;
const int iHitBy = 4;

const char szHitgroupOther[NUM_HIT_OTHER+1][TITLE_LENGTH] =
{
	"#HIT_HITME",
	"#HIT_HITYOU",
	"#HIT_WHERE",
	"#HIT_WHEREME",
	"#HIT_BY"
};

const int NUM_TEAM_CLASS_MESSAGES = 5;

const int iBecomeSpec = 0;
const int iClassFightAs = 1;
const int iTeamFightFor = 2;
const int iClassRespawnAs = 3;
const int iTeamRespawnOn = 4;
const int iTeamRespawnOnEnd = 5;

const char szTeamClassTitles[NUM_TEAM_CLASS_MESSAGES+1][TITLE_LENGTH] =
{
	"#SPEC_BECOME",
	"#CLASS_FIGHT_AS",
	"#TEAM_FIGHT_FOR",
	"#CLASS_RESPAWN_AS",
	"#TEAM_RESPAWN_ON",
	"#TEAM_RESPAWN_ON_END"
};

const int NUM_POINT_MESSAGES = 11;

const int iPointCapturing = 0;
const int iPointCaptured = 1;
const int iPointAbandonedStart = 2;
const int iPointAbandonedEnd = 3;
const int iPointLost = 4;
const int iPlayerEntered = 5;
const int iPlayerExited = 6;
const int iPlayerCapturing = 7;
const int iPlayerCaptured = 8;
const int iPointBritish = 9;
const int iPointAmerican = 10;
const int iPointFor = 11;

const char szPointTitles[NUM_POINT_MESSAGES+1][TITLE_LENGTH] =
{
	"#POINT_CAPTURING",
	"#POINT_CAPTURED",
	"#POINT_ABANDONED_START",
	"#POINT_ABANDONED_END",
	"#POINT_LOST",
	"#POINT_PLAYER_ENTERED",
	"#POINT_PLAYER_EXITED",
	"#POINT_PLAYER_CAPTURING",
	"#POINT_PLAYER_CAPTURED",
	"#POINT_BRITISH_TEAM",
	"#POINT_AMERICAN_TEAM",
	"#POINT_FOR"
};

const int iPointEvent_Capturing = 1;
const int iPointEvent_Captured = 2;
const int iPointEvent_Abandoned = 3;
const int iPointEvent_Lost = 4;
const int iPointEvent_Player_Entered = 5;
const int iPointEvent_Player_Exited = 6;
const int iPointEvent_Player_Capturing = 7;
const int iPointEvent_Player_Captured = 8;

const int iRoundPre = 0;
const int iRoundPost = 1;

const int NUM_ROUND_OVER_MESSAGES = 2;
const char szRoundOverTitles[NUM_ROUND_OVER_MESSAGES+1][TITLE_LENGTH] = 
{
	"#ROUND_PRE",
	"#ROUND_POST"
};

#endif

