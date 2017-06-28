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

//This file was created on the 6/6/03 at 15:50 by Ben Banfield for the Battle Grounds modification for Half-Life
//Its purpose is to define a hud class which will pass a message to the correct message type, helping internationalisation support

#include "STDIO.H"
#include "STDLIB.H"

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include <string.h>

#include "vgui_int.h"
#include "vgui_TeamFortressViewport.h"
#include "vgui_ServerBrowser.h"

#include "message_defs.h"
#include "event_api.h"

DECLARE_MESSAGE(m_PredefinedMessage, MakeMessage);

int CPredefinedMessage::Init( void )
{
	HOOK_MESSAGE( MakeMessage );

	gHUD.AddHudElem(this);

	m_PointEntered = false;
	m_PointIdEntered = -1;

	m_iFlags |= HUD_ACTIVE;
	return 1;
}

int CPredefinedMessage::VidInit( void )
{
	m_iFlags |= HUD_ACTIVE;
	return 1;
}

int CPredefinedMessage::MsgFunc_MakeMessage( const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);

	int iMessageType = READ_SHORT();

	if(iMessageType == iVoiceCommands)
	{
		int iCommand = READ_SHORT();
		int iPlayerId = READ_SHORT();

		if(GetClientVoiceMgr()->IsPlayerBlocked(iPlayerId))
		{
			m_iFlags |= HUD_ACTIVE;
			return 1;
		}

		// grab the players name
		GetPlayerInfo( iPlayerId, &g_PlayerInfoList[iPlayerId] );
		const char *pName = g_PlayerInfoList[iPlayerId].name;

		char szCommand[512];
		char szCommand2[128];

		_snprintf(szCommand2, sizeof(szCommand2) - 1,"%s\0", 
			gHUD.m_TextMessage.BufferedLocaliseTextString(szCommandTitles[iCommand]));
		
		_snprintf(szCommand, sizeof(szCommand) - 1,"%c(Command) %s: %s\n\0", 
			2,
			
			pName, 
			szCommand2);

		if(((int)CVAR_GET_FLOAT("cl_msgs_commands")) == 1)
			gHUD.m_SayText.SayTextPrint(szCommand, sizeof(szCommand), iPlayerId);
	}
	else if(iMessageType == iHitMsg_Attacker)
	{
		int iHitbox = READ_SHORT();
		int iVictimId = READ_SHORT();
		int iDamage = READ_SHORT();

		GetPlayerInfo( iVictimId, &g_PlayerInfoList[iVictimId] );
		const char *pName = g_PlayerInfoList[iVictimId].name;

		char szMessage[512];
		char szHit[64];
		char szWhere[128];
		char szHitBox[128];

		_snprintf(szHit, sizeof(szHit) - 1,"%s", 
			gHUD.m_TextMessage.BufferedLocaliseTextString(szHitgroupOther[iHitSomeone]));

		_snprintf(szWhere, sizeof(szWhere) - 1,"%s", 
			gHUD.m_TextMessage.BufferedLocaliseTextString(szHitgroupOther[iHitWhere]));

		_snprintf(szHitBox, sizeof(szHitBox) - 1,"%s", 
			gHUD.m_TextMessage.BufferedLocaliseTextString(szHitgroupTitles[iHitbox]));
		
		// message makeup = Hit, Name, which, hitbox, Damage
		_snprintf(szMessage, sizeof(szMessage) - 1, "%s %s %s %s\n\0", 
			szHit, 
			pName, 
			szWhere, 
			szHitBox);

		// add the message in
		if(((int)CVAR_GET_FLOAT("cl_msgs_who_i_hit")) == 1)
			gHUD.m_ColourMessage.AddMessage(255, 255, 255, -1, 200, szMessage, 2.0);
		
		//BP We draw the Damage Points in red
		char szDamagePoints[256];	
		_snprintf(szDamagePoints, sizeof(szDamagePoints) - 1, "%d Damage Points\n\0", iDamage); 
		if(((int)CVAR_GET_FLOAT("cl_msgs_who_i_hit")) == 1)
			gHUD.m_ColourMessage.AddMessage(255, 0, 0, -1, 210, szDamagePoints, 2.0);
		
	}
	else if(iMessageType == iHitMsg_Victim)
	{
		int iHitbox = READ_SHORT();
		int iAttackerId = READ_SHORT();
		int iDamage = READ_SHORT();

		GetPlayerInfo( iAttackerId, &g_PlayerInfoList[iAttackerId] );
		const char *pName = g_PlayerInfoList[iAttackerId].name;

		char szMessage[512];
		char szHit[64];
		char szWhere[128];
		char szHitBox[128];
		char szHitBy[64];

		_snprintf(szHit, sizeof(szHit) - 1,"%s", 
			gHUD.m_TextMessage.BufferedLocaliseTextString(szHitgroupOther[iHitMe]));

		_snprintf(szWhere, sizeof(szWhere) - 1,"%s", 
			gHUD.m_TextMessage.BufferedLocaliseTextString(szHitgroupOther[iHitWhereMe]));

		_snprintf(szHitBox, sizeof(szHitBox) - 1,"%s", 
			gHUD.m_TextMessage.BufferedLocaliseTextString(szHitgroupTitles[iHitbox]));

		_snprintf(szHitBy, sizeof(szHitBy) - 1,"%s", 
			gHUD.m_TextMessage.BufferedLocaliseTextString(szHitgroupOther[iHitBy]));


		// message makeup = Hit, which, hitbox, by, Name,
		_snprintf(szMessage, sizeof(szMessage) - 1, "%s %s %s %s %s\n\0", 
			szHit, 
			szWhere, 
			szHitBox, 
			szHitBy, 
			pName);

		// add the message in
		if(((int)CVAR_GET_FLOAT("cl_msgs_who_hit_me")) == 1)
			gHUD.m_ColourMessage.AddMessage(255, 255, 255, -1, 200, szMessage, 2.0);
		
		//BP We draw the Damage Points in red
		char szDamagePoints[256];	
		_snprintf(szDamagePoints, sizeof(szDamagePoints) - 1, "%d Damage Points\n\0", iDamage); 
		if(((int)CVAR_GET_FLOAT("cl_msgs_who_hit_me")) == 1)
			gHUD.m_ColourMessage.AddMessage(255, 0, 0, -1, 210, szDamagePoints, 2.0);
	}
	else if(iMessageType == iTeamClass)
	{
		int iPlayerId = READ_SHORT();
		int iTeam = READ_SHORT();
		int iClass = READ_SHORT();

		// grab the players name
		GetPlayerInfo( iPlayerId, &g_PlayerInfoList[iPlayerId] );
		const char *pName = g_PlayerInfoList[iPlayerId].name;

		char szMessage[512];
		if(iTeam != SPECS)
		{
			char szJoinedTeam[256];
			char szClassPlayAs[256];
			char szTeamInvolved[256];

			_snprintf(szJoinedTeam, sizeof(szJoinedTeam) - 1,"%s", 
				gHUD.m_TextMessage.BufferedLocaliseTextString(szTeamClassTitles[iTeamFightFor]));

			_snprintf(szClassPlayAs, sizeof(szClassPlayAs) - 1,"%s", 
				gHUD.m_TextMessage.BufferedLocaliseTextString(szTeamClassTitles[iClassFightAs]));

			if(iTeam == BRITS)
			{
				_snprintf(szTeamInvolved, sizeof(szTeamInvolved) - 1, "%s", 
					gHUD.m_TextMessage.BufferedLocaliseTextString(szPointTitles[iPointBritish]));
			}
			else if(iTeam == AMERS)
			{
				_snprintf(szTeamInvolved, sizeof(szTeamInvolved) - 1, "%s", 
					gHUD.m_TextMessage.BufferedLocaliseTextString(szPointTitles[iPointAmerican]));
			}

			_snprintf(szMessage, sizeof(szMessage) - 1,"%s %s %s %s %s\n\0", 
				pName, 
				szClassPlayAs,
				szClassNames[iTeam][iClass],
				szJoinedTeam,
				szTeamInvolved);
		}
		else
		{
			_snprintf(szMessage, sizeof(szMessage) - 1,"%s %s\n\0", 
				pName, 
				gHUD.m_TextMessage.BufferedLocaliseTextString(szTeamClassTitles[iBecomeSpec]));
		}
		if(((int)CVAR_GET_FLOAT("cl_msgs_team_change")) == 1)
			gHUD.m_SayText.SayTextPrint(szMessage, sizeof(szMessage), 0);
	}
	else if(iMessageType == iTeamClassMe)
	{
		int iTeam = READ_SHORT();
		int iClass = READ_SHORT();

		char szMessage[512];
		char szClassPlayAs[256];
		_snprintf(szClassPlayAs, sizeof(szClassPlayAs) - 1,"%s", 
			gHUD.m_TextMessage.BufferedLocaliseTextString(szTeamClassTitles[iClassRespawnAs]));

		if(iTeam != SPECS)
		{
			char szJoinedTeam[256];
			_snprintf(szJoinedTeam, sizeof(szJoinedTeam) - 1,"%s", 
				gHUD.m_TextMessage.BufferedLocaliseTextString(szTeamClassTitles[iTeamRespawnOn]));
			char szJoinedTeamEnd[128];
			_snprintf(szJoinedTeamEnd, sizeof(szJoinedTeamEnd) - 1,"%s", 
				gHUD.m_TextMessage.BufferedLocaliseTextString(szTeamClassTitles[iTeamRespawnOnEnd]));
		
			char szTeamInvolved[256];
			if(iTeam == BRITS)
			{
				_snprintf(szTeamInvolved, sizeof(szTeamInvolved) - 1, "%s", 
					gHUD.m_TextMessage.BufferedLocaliseTextString(szPointTitles[iPointBritish]));
			}
			else if(iTeam == AMERS)
			{
				_snprintf(szTeamInvolved, sizeof(szTeamInvolved) - 1, "%s", 
					gHUD.m_TextMessage.BufferedLocaliseTextString(szPointTitles[iPointAmerican]));
			}
			
			_snprintf(szMessage, sizeof(szMessage) - 1,"%s %s %s %s\n\0",
				szClassPlayAs,
				szClassNames[iTeam][iClass],
				szJoinedTeam,
				szTeamInvolved);
		}
		else
		{
			_snprintf(szMessage, sizeof(szMessage) - 1,"%s %s\0", 
				szClassPlayAs,
				Spectators);
		}
		if(((int)CVAR_GET_FLOAT("cl_msgs_my_team_change")) == 1)
			gHUD.m_SayText.SayTextPrint(szMessage, sizeof(szMessage), 0);

	}
	else if(iMessageType == iCapturePointMessages)
	{
		int iPointId = READ_SHORT();
		int iPlayerId = READ_SHORT();
		int iTeamId = READ_SHORT();
		int iPointEvent = READ_SHORT();
		int iPlayersInvolved = READ_SHORT();

		char szCapturePointName[256];
		_snprintf(szCapturePointName, sizeof(szCapturePointName) - 1, "%s\0", 
			gHUD.m_Points.ReturnNameById(iPointId));

		// grab the players name
		GetPlayerInfo( iPlayerId, &g_PlayerInfoList[iPlayerId] );
		const char *pName = g_PlayerInfoList[iPlayerId].name;

		// grab the team's name
		char szTeamInvolved[256];
		if(iTeamId == BRITS)
		{
			_snprintf(szTeamInvolved, sizeof(szTeamInvolved) - 1, "%s", 
				gHUD.m_TextMessage.BufferedLocaliseTextString(szPointTitles[iPointBritish]));
		}
		else if(iTeamId == AMERS)
		{
			_snprintf(szTeamInvolved, sizeof(szTeamInvolved) - 1, "%s", 
				gHUD.m_TextMessage.BufferedLocaliseTextString(szPointTitles[iPointAmerican]));
		}
		else{}

		char szMessage[512];

		char szFor[64];
		_snprintf(szFor, sizeof(szFor) -1, "%s", 
			gHUD.m_TextMessage.BufferedLocaliseTextString(szPointTitles[iPointFor]));

		if(iPointEvent == iPointEvent_Capturing) 
		{
			_snprintf(szMessage, sizeof(szMessage) - 1, "%i %s %s %s %s\n\0", 
				iPlayersInvolved, 
				gHUD.m_TextMessage.BufferedLocaliseTextString(szPointTitles[iPointCapturing]),
				szCapturePointName,
				szFor,
				szTeamInvolved);

			if ((m_PointEntered) && (m_PointIdEntered == iPointId))
			{
				gHUD.m_Points.StartClock(iPointId);
					
gEngfuncs.Con_Printf("iPointEvent_Capturing - you are in cap point - someone else started the capture - Capture Clock started for Point %i (player id: %i)\n\0", iPointId, iPlayerId);

			}
			
			if (gEngfuncs.pEventAPI->EV_IsLocal(iPlayerId-1)) 
			{
				gHUD.m_Points.StartClock(iPointId);
				m_PointEntered = true;
				m_PointIdEntered = iPointId;
					
gEngfuncs.Con_Printf("iPointEvent_Capturing you started the capture of a multiple people capture - Capture Clock started for Point %i (player id: %i)\n\0", iPointId, iPlayerId);

			}

		}
		else if(iPointEvent == iPointEvent_Captured) 
		{
			_snprintf(szMessage, sizeof(szMessage) - 1, "%i %s %s %s %s\n\0", 
				iPlayersInvolved, 
				gHUD.m_TextMessage.BufferedLocaliseTextString(szPointTitles[iPointCaptured]),
				szCapturePointName,
				szFor,
				szTeamInvolved);
			
			if (( iPointId == m_PointIdEntered) && (m_PointEntered))
			{
				for (int points = 0; points < 20; points++)
					gHUD.m_Points.ClearClock(points);
				m_PointEntered = false;
				m_PointIdEntered = -1;

gEngfuncs.Con_Printf("iPointEvent_Captured - Point capture complete - Capture Clock stopped for Point %i (player id: %i)\n\0", iPointId, iPlayerId);


			}
		}
		else if(iPointEvent == iPointEvent_Abandoned) 
		{
			char szAbandonedEnd[256];
			_snprintf(szAbandonedEnd, sizeof(szAbandonedEnd) - 1, "%s", 
				gHUD.m_TextMessage.BufferedLocaliseTextString(szPointTitles[iPointAbandonedEnd]));

			_snprintf(szMessage, sizeof(szMessage) - 1, "%s %s %s %s\n\0",
				gHUD.m_TextMessage.BufferedLocaliseTextString(szPointTitles[iPointAbandonedStart]),
				szCapturePointName,
				szAbandonedEnd,
				szTeamInvolved);
						
			if (( iPointId == m_PointIdEntered) && (m_PointEntered))
			{
				for (int points = 0; points < 20; points++)
					gHUD.m_Points.ClearClock(points);
				m_PointEntered = false;
				m_PointIdEntered = -1;

gEngfuncs.Con_Printf("iPointEvent_Abandoned - Area abandonned - Capture Clock stopped for Point %i (player id: %i)\n\0", iPointId, iPlayerId);

			}
		}
		else if(iPointEvent == iPointEvent_Lost)
		{
			_snprintf(szMessage, sizeof(szMessage) - 1, "%s %s %s\n\0",
				szCapturePointName,
				gHUD.m_TextMessage.BufferedLocaliseTextString(szPointTitles[iPointLost]),
				szTeamInvolved);
		}
		else if(iPointEvent == iPointEvent_Player_Entered) 
		{
			_snprintf(szMessage, sizeof(szMessage) - 1, "%s %s %s\n\0",
				pName,
				gHUD.m_TextMessage.BufferedLocaliseTextString(szPointTitles[iPlayerEntered]),
				szCapturePointName);

			if ((gHUD.m_Points.ReturnStatusById(iPointId) != iPointCaptured) && (gEngfuncs.pEventAPI->EV_IsLocal(iPlayerId-1)))
			{
				m_PointEntered = true;
				m_PointIdEntered = iPointId;

gEngfuncs.Con_Printf("iPointEvent_Player_Entered - Player enter logged for Point %i (player id: %i)\n\0", iPointId, iPlayerId);

			} 
			
		}
		else if(iPointEvent == iPointEvent_Player_Exited) 
		{
			_snprintf(szMessage, sizeof(szMessage) - 1, "%s %s %s\0",
				pName,
				gHUD.m_TextMessage.BufferedLocaliseTextString(szPointTitles[iPlayerExited]),
				szCapturePointName);
 
			if ((m_PointEntered && m_PointIdEntered == iPointId) && (gEngfuncs.pEventAPI->EV_IsLocal(iPlayerId-1))) 
			{
				for (int points = 0; points < 20; points++)
					gHUD.m_Points.ClearClock(points);
				m_PointEntered = false;
				m_PointIdEntered = -1;

gEngfuncs.Con_Printf("iPointEvent_Player_Exited - Capture Clock stopped for Point %i (player id: %i)\n\0", iPointId, iPlayerId);

			}
		}
		else if(iPointEvent == iPointEvent_Player_Capturing) 
		{
			_snprintf(szMessage, sizeof(szMessage) - 1, "%s %s %s %s %s\n\0", pName, 
				gHUD.m_TextMessage.BufferedLocaliseTextString(szPointTitles[iPlayerCapturing]),
				szCapturePointName,
				szFor,
				szTeamInvolved);
			
			if (gEngfuncs.pEventAPI->EV_IsLocal(iPlayerId-1)) 
			{
				gHUD.m_Points.StartClock(iPointId);

gEngfuncs.Con_Printf("iPointEvent_Player_Capturing - Capture Clock started for Point %i (player id: %i)\n\0", iPointId, iPlayerId);


				m_PointEntered = true;
				m_PointIdEntered = iPointId;
			}
		}

		else if(iPointEvent == iPointEvent_Player_Captured) 
		{
			_snprintf(szMessage, sizeof(szMessage) - 1, "%s %s %s %s %s\n\0", 
				pName, 
				gHUD.m_TextMessage.BufferedLocaliseTextString(szPointTitles[iPlayerCaptured]), 
				szCapturePointName,
				szFor,
				szTeamInvolved);

			if ((m_PointIdEntered == iPointId) && (gEngfuncs.pEventAPI->EV_IsLocal(iPlayerId-1)))
			{
				for (int points = 0; points < 20; points++)
					gHUD.m_Points.ClearClock(points);
				m_PointEntered = false;
				m_PointIdEntered = -1;

gEngfuncs.Con_Printf("iPointEvent_Player_Captured - Capture Clock stopped for Point %i (player id: %i)\n\0", iPointId, iPlayerId);

			}
		}

		// add the message in
		if(((int)CVAR_GET_FLOAT("cl_msgs_captures")) == 1)
			gHUD.m_ColourMessage.AddMessage(255, 255, 255, -1, 280, szMessage, 2.0);
	}
	// alert the user of who won a round
	else if(iMessageType = iRoundWon) {
		int iWinningTeam = READ_SHORT();

		char szMessage[512];

		char szRoundPost[512];
		_snprintf(szRoundPost, sizeof(szRoundPost) - 1, "%s\0", 
			gHUD.m_TextMessage.BufferedLocaliseTextString(szRoundOverTitles[iRoundPost]));

		_snprintf(szMessage, sizeof(szMessage) - 1, "%s %s %s\n\0",
			gHUD.m_TextMessage.BufferedLocaliseTextString(szRoundOverTitles[iRoundPre]),
			szTeamNames[iWinningTeam],
			szRoundPost);

		int r = 0; int g = 0; int b = 0;
		if(iWinningTeam == BRITS) {
			UnpackRGB(r, g, b, RGB_REDISH);
		} else {
			UnpackRGB(r, g, b, RGB_BLUEISH);
		}

		if(((int)CVAR_GET_FLOAT("cl_msgs_round")) == 1)
			gHUD.m_ColourMessage.AddMessage(r, g, b, -1, 280, szMessage, 4.0);

		// round is over -> clear all capture clocks assuming there are never more than 20 cap points
		for (int points = 0; points < 20; points++)
			gHUD.m_Points.ClearClock(points);
		m_PointEntered = false;
		m_PointIdEntered = -1;
	}

	m_iFlags |= HUD_ACTIVE;
	return 1;
}