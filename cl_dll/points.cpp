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

//This file was created on the 4/8/2k+2 at 16:51 by Ben Banfield for the Battle Grounds modification of Half-Life
//Its purpose is to define the class which will display the status of all points on the hud

#include "STDIO.H"
#include "STDLIB.H"

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include <string.h>
#include "triangleapi.h"

#include "vgui_int.h"
#include "vgui_TeamFortressViewport.h"
#include "vgui_ServerBrowser.h"

cvar_t *g_ShowPoints;

// cl_capturetextlevel
const int iShortText = 1;
const int iShortTextWithTime = 2;
const int iLongText = 3;
const int iLongTextWithTime = 4;

DECLARE_MESSAGE(m_Points, Points)

int CHudPoints::Init()
{
	HOOK_MESSAGE( Points );

	gHUD.AddHudElem(this);

	for(int i = 0; i < MAX_POINTS; i++)
	{
		if(m_aPoints[i]) {
			delete m_aPoints[i];
		}
		m_aPoints[i] = NULL;
	}

	m_bInvolvedInCapture = false;

	return 1;
}

int CHudPoints::VidInit()
{
	for(int i = 0; i < MAX_POINTS; i++) {
		if(m_aPoints[i]) {
			delete m_aPoints[i];
		}
		m_aPoints[i] = NULL;
	}

	// icons
	m_HUD_IconWhite = gHUD.GetSpriteIndex( "point_white" );
	m_HUD_IconRed = gHUD.GetSpriteIndex( "point_red" );
	m_HUD_IconBlue = gHUD.GetSpriteIndex( "point_blue" );

	m_hIconWhite = gHUD.GetSprite( m_HUD_IconWhite );
	m_hIconRed = gHUD.GetSprite( m_HUD_IconRed );
	m_hIconBlue = gHUD.GetSprite( m_HUD_IconBlue );
	m_hClock = gHUD.GetSprite( gHUD.GetSpriteIndex( "capture_clock" ) );
	m_hFillUp = gHUD.GetSprite( gHUD.GetSpriteIndex( "capture_fillup" ) );

	m_pAreaIconWhite = &gHUD.GetSpriteRect( m_HUD_IconWhite );
	m_pAreaIconRed = &gHUD.GetSpriteRect( m_HUD_IconRed );
	m_pAreaIconBlue = &gHUD.GetSpriteRect( m_HUD_IconBlue );
	m_pAreaClock = &gHUD.GetSpriteRect( gHUD.GetSpriteIndex( "capture_clock" ) );
	m_pAreaFillUp = &gHUD.GetSpriteRect( gHUD.GetSpriteIndex( "capture_fillup" ) );

	return 1;
}

void CHudPoints::ResetAllPoints()
{
	int i = 0;
	for(; i < MAX_POINTS; i++)
	{
		if(m_aPoints[i]) {
			delete m_aPoints[i];
		}
		m_aPoints[i] = NULL;
	}
	m_bInvolvedInCapture = false;
}

int CHudPoints::MsgFunc_Points(const char *pszName, int iSize, void *pbuf)
{
	int iIndex = 0;

	// point variables which are specific to certain types of points
	int iTimeNeeded = 0;
	int iPlayersInPoint = 0;
	int iPlayersNeeded = 0;
	int iHealthStarted = 0;
	int iHealth = 0;

	BEGIN_READ(pbuf, iSize);

	int iID	= READ_SHORT();

	if(iID == -1) {
		ResetAllPoints();
		return 1;
	}

	int iPosition = READ_SHORT();

	if(iPosition != UNDEFINED)
		iIndex = iPosition;
	else
		iIndex = iID;

	int iStatus = READ_SHORT();
	char *name = READ_STRING(sizeof(m_aPoints[0]->name)-1);
	int iType = READ_SHORT();
	int iTeamAllowed = READ_SHORT();
	int iTeamInvolved = READ_SHORT();

	if(iType == CAPTURE_POINT) 
	{
		iTimeNeeded = READ_SHORT();
		iPlayersInPoint = READ_SHORT();
		iPlayersNeeded = READ_SHORT();
	}
	else if(iType == DESTRUCTION_POINT)
	{
		iHealthStarted = READ_SHORT();
		iHealth = READ_SHORT();
	}

	if(m_aPoints[iIndex])
	{
		_snprintf(m_aPoints[iIndex]->name, sizeof(m_aPoints[iID]->name)-1, "%s", name);
		m_aPoints[iIndex]->name[sizeof(m_aPoints[iID]->name)-1] = '\0';

		m_aPoints[iIndex]->iID = iID;
		m_aPoints[iIndex]->iType = iType;
		m_aPoints[iIndex]->iStatus = iStatus;
		m_aPoints[iIndex]->iTeamAllowed = iTeamAllowed;
		m_aPoints[iIndex]->iTeamInvolved = iTeamInvolved;

		m_aPoints[iIndex]->iTimeNeededForCapture = iTimeNeeded;
		m_aPoints[iIndex]->iPlayersInPoint = iPlayersInPoint;
		m_aPoints[iIndex]->iPlayersNeeded = iPlayersNeeded;

		m_aPoints[iIndex]->iHealthStarted = iHealthStarted;
		m_aPoints[iIndex]->iHealth = iHealth;

		if(iType == CAPTURE_POINT && iStatus == FREE) {
			m_aPoints[iIndex]->flTimeCaptureStarted = 0.0;
		}
	} else {
		// -create a new point if the point with this id doesn't exist
		m_aPoints[iIndex] = new point_s;

		strncpy(m_aPoints[iIndex]->name, name, (256-2));
		m_aPoints[iIndex]->iID = iID;
		m_aPoints[iIndex]->iType = iType;
		m_aPoints[iIndex]->iStatus = iStatus;
		m_aPoints[iIndex]->iTeamAllowed = iTeamAllowed;
		m_aPoints[iIndex]->iTeamInvolved = iTeamInvolved;

		m_aPoints[iIndex]->iTimeNeededForCapture = iTimeNeeded;
		m_aPoints[iIndex]->iTimeAdded = 0;
		m_aPoints[iIndex]->flTimeCaptureStarted = 0.0;
		m_aPoints[iIndex]->iPlayersInPoint = iPlayersInPoint;
		m_aPoints[iIndex]->iPlayersNeeded = iPlayersNeeded;

		m_aPoints[iIndex]->iHealthStarted = iHealthStarted;
		m_aPoints[iIndex]->iHealth = iHealth;
	}

	m_iFlags |= HUD_ACTIVE;

	return 1;
}

int CHudPoints::Draw( float flTime )
{
	if ( gHUD.m_iHideHUDDisplay & (HIDEHUD_ALL) )
		return 1;

	if( g_ShowPoints->value < 1)
		return 1;

	const int EITHER_TEAM = UNDEFINED;
	const int x_max = 512;

	int iBritishTargets = 0;
	int iAmericanTargets = 0;
	int iEitherTargets = 0;

	int i = 0;
	for(; i < MAX_POINTS; i++)
	{
		if(m_aPoints[i])
		{
			point_s *pPoint = m_aPoints[i];
			if(pPoint->iTeamAllowed == BRITS)
				iBritishTargets++;

			else if(pPoint->iTeamAllowed == AMERS)
				iAmericanTargets++;

			else if(pPoint->iTeamAllowed == EITHER_TEAM)
				iEitherTargets++;

			else {}

			// add in the time this point was received if none has been set
			if(pPoint->iTimeAdded == 0)
				pPoint->iTimeAdded = flTime;

			// clean up points from previous maps as we can't have points timestamped with times that haven't happened yet
			if(pPoint->iTimeAdded > flTime)
			{
				delete pPoint;
				pPoint = NULL;
				continue;
			}

			// add in the capture start time
			if(pPoint->iType == CAPTURE_POINT && pPoint->flTimeCaptureStarted == 0.0 && pPoint->iStatus == CAPTURING)
				pPoint->flTimeCaptureStarted = flTime;

			if(pPoint->iType == CAPTURE_POINT && pPoint->iStatus == FREE)
				pPoint->flTimeCaptureStarted = 0.0;
		}
	}

	int x = XRES2(5), y = YRES2(10), r = 0, g = 0, b = 0;

	if(g_iUser1)
		y += YRES(30);

	if(gEngfuncs.IsSpectateOnly())
		y += YRES(15);

	if(iEitherTargets > 0)
	{
		UnpackRGB(r, g, b, RGB_WHITEISH);
		gHUD.DrawHudString(x, y, x_max, "Either Team:", r, g, b);
				
		y += YRES2(20);

		for(i = 0; i < MAX_POINTS; i++)
		{
			if(m_aPoints[i])
			{
				point_s *pPoint = m_aPoints[i];
				if(pPoint->iTeamAllowed == EITHER_TEAM)
					DrawPoint(pPoint, flTime, x, y);
			}
		}
	}
	if(iAmericanTargets > 0)
	{
		UnpackRGB(r, g, b, RGB_BLUEISH);
		gHUD.DrawHudString(x, y, x_max, "American only Targets:", r, g, b);
		y += YRES2(20);

		for(i = 0; i < MAX_POINTS; i++)
		{
			if(m_aPoints[i])
			{
				point_s *pPoint = m_aPoints[i];
				if(pPoint->iTeamAllowed == AMERS)
					DrawPoint(pPoint, flTime, x, y);
			}
		}
	}
	if(iBritishTargets > 0)
	{
		UnpackRGB(r, g, b, RGB_REDISH);
		gHUD.DrawHudString(x, y, x_max, "British only Targets:", r, g, b);
		y += YRES2(20);

		for(i = 0; i < MAX_POINTS; i++)
		{
			if(m_aPoints[i])
			{
				point_s *pPoint = m_aPoints[i];
				if(pPoint->iTeamAllowed == BRITS)
					DrawPoint(pPoint, flTime, x, y);
			}
		}
	}

	// capture clock
	if(m_bInvolvedInCapture) {
		int r = 0; int g = 0; int b = 0;
		int y_length = 0; int x_length = 0;
		int x = 0; int y = 0;
		int iFrame = 0;
		float flCapturedRatio = 0.0f;

		// fill up sprite
		y_length = m_pAreaFillUp->bottom - m_pAreaFillUp->top;
		x_length = m_pAreaFillUp->right - m_pAreaFillUp->left;	
		
		x = ((ScreenWidth / 2) - (XRES2(x_length) / 2));
		y = (YRES2(m_pAreaClock->bottom - m_pAreaClock->top) - YRES2(y_length)) / 2;
		
		point_s *pPoint = NULL;
		for(int i = 0; i < MAX_POINTS; i++ ) {
			if(m_aPoints[i] && (m_aPoints[i]->iID == m_iInvolvedInCaptureId)) {
				pPoint = m_aPoints[i];
				break;
			}
		}

		// don't show the clock if we aren't capturing the point atm
		if(pPoint->iStatus != CAPTURING)
			return 1;

		// no clock on tiny captures
		if(pPoint->iTimeNeededForCapture <= 1.0)
			return 1;

		flCapturedRatio = ((flTime - pPoint->flTimeCaptureStarted) / pPoint->iTimeNeededForCapture);

		if (flCapturedRatio > 1)
			flCapturedRatio = 1;
		if (flCapturedRatio < 0)
			flCapturedRatio = 0;

		iFrame = (int)(flCapturedRatio * 24);

		UnpackRGB(r, g, b, RGB_WHITEISH);

		if(pPoint->iTeamInvolved == BRITS) {
			r = 201; g = 0; b = 0;
		} else if(pPoint->iTeamInvolved == AMERS) {
			r = 0; g = 19; b = 201;
		}

		SPR_Set(m_hFillUp, r, g, b);
		SPR_DrawHoles(iFrame, x, y, m_pAreaFillUp, true);

		// clock
		y_length = m_pAreaClock->bottom - m_pAreaClock->top;
		x_length = m_pAreaClock->right - m_pAreaClock->left;	
		
		x = ((ScreenWidth / 2) - (XRES2(x_length) / 2));
		y = 0;

		r = 255; g = 255; b = 255;

		SPR_Set(m_hClock, r, g, b);
		SPR_DrawHoles(0, x, y, m_pAreaClock, true);
	}

	return 1;
}

void CHudPoints::DrawPoint(point_s *pPoint, float flTime, int &x, int &y) 
{
	int r = 0; int g = 0; int b = 0; int x_length = 0; int y_length = 0;
	char name[512];

	const int EITHER_TEAM = UNDEFINED;
	const int x_max = 512;

	if(pPoint->iType == CAPTURE_POINT)
	{
		int iFrame = 0;
		int iTimeTillCapture = 0;
		float flCapturedRatio = 0.0f;
		char szTime[32];
		sprintf(szTime, "\0"); // keep clean
		

		if(pPoint->iStatus != CAPTURING) 
		{	
			pPoint->flTimeCaptureStarted = 0.0;
		} 
		else if(m_bInvolvedInCapture != true) 
		{
			flCapturedRatio = ((flTime - (float)pPoint->flTimeCaptureStarted) / (float)pPoint->iTimeNeededForCapture);
		} 
		else 
		{
			flCapturedRatio = ((flTime - (float)pPoint->flTimeCaptureStarted) / (float)pPoint->iTimeNeededForCapture);
		}
				
		if(pPoint->iStatus == CAPTURING) 
		{
			iTimeTillCapture = (int)((float)pPoint->iTimeNeededForCapture - (flTime - (float)pPoint->flTimeCaptureStarted));
			sprintf(szTime, " - %i", iTimeTillCapture + 1);	
		}
		
		// capture point display options
		switch((int)CVAR_GET_FLOAT("cl_capturetextlevel"))
		{
		case iShortText:
			sprintf(name, "%s (%i/%i)", 
				pPoint->name,
				pPoint->iPlayersInPoint,
				pPoint->iPlayersNeeded);
			break;
		case iShortTextWithTime:
			
			sprintf(name, "%s (%i/%i)%s", 
				pPoint->name,
				pPoint->iPlayersInPoint,
				pPoint->iPlayersNeeded,
				szTime);
			break;
		case iLongText:
			sprintf(name, "%s: %s - Players: (%i/%i)", 
				pPoint->name,
				szPointStatus[pPoint->iType][pPoint->iStatus], 
				pPoint->iPlayersInPoint,
				pPoint->iPlayersNeeded);
			break;
		case iLongTextWithTime:
			sprintf(name, "%s: %s - Players: (%i/%i)%s", 
				pPoint->name,
				szPointStatus[pPoint->iType][pPoint->iStatus], 
				pPoint->iPlayersInPoint,
				pPoint->iPlayersNeeded,
				szTime);
			break;
		default:
			sprintf(name, "%s%s", 
				pPoint->name,
				szTime);
			break;
		}

		sprintf(szTime, "\n\0");
		
		float flBlinkSpeed = 0.5f;
		int iExponential = 8;
		HSPRITE hIcon = 0;
		wrect_t *pIconArea = NULL;
		int iIconIndex = 0;

		// icon white
		UnpackRGB(r, g, b, RGB_WHITEISH);

		// make it blink if it is being captured
		if (flCapturedRatio > 0.01f) {
			flBlinkSpeed = flBlinkSpeed + (flCapturedRatio*10 * flCapturedRatio*10)/200;
			r = 64 * (cos(flTime * flBlinkSpeed)) + 128;
			g = 64 * (cos(flTime * flBlinkSpeed)) + 128;
			b = 64 * (cos(flTime * flBlinkSpeed)) + 128;
		}

		if(pPoint->iTeamInvolved == EITHER_TEAM) {
			hIcon = m_hIconWhite;
			pIconArea = m_pAreaIconWhite;
			iIconIndex = m_HUD_IconWhite;
		} else if(pPoint->iTeamInvolved == BRITS) {
			hIcon = m_hIconRed;
			pIconArea = m_pAreaIconRed;
			iIconIndex = m_HUD_IconRed;
		} else if(pPoint->iTeamInvolved == AMERS) {
			hIcon = m_hIconBlue;
			pIconArea = m_pAreaIconBlue;
			iIconIndex = m_HUD_IconBlue;
		} else{}

		iFrame = (iTimeTillCapture > 0 ? 1 : 0);

		SPR_Set(hIcon, r, g, b);
		y_length = YRES2(gHUD.GetSpriteRect(iIconIndex).bottom - gHUD.GetSpriteRect(iIconIndex).top);
		x_length = XRES2(gHUD.GetSpriteRect(iIconIndex).right - gHUD.GetSpriteRect(iIconIndex).left);
		SPR_DrawHoles(iFrame, x, y, pIconArea, true);
	}

	else if(pPoint->iType == DESTRUCTION_POINT) {
		if(pPoint->iStatus == DESTROYED) {
			sprintf(name, "%s: %s", 
				pPoint->name, 
				szPointStatus[pPoint->iType][(pPoint->iStatus - 10)]);
		} else {
			sprintf(name, "%s: %s (%i/%i)", 
				pPoint->name, 
				szPointStatus[pPoint->iType][(pPoint->iStatus - 10)],
				pPoint->iHealth,
				pPoint->iHealthStarted);
		}
	}
	
	if(pPoint->iTeamInvolved == EITHER_TEAM)
		UnpackRGB(r, g, b, RGB_WHITEISH);
	else if(pPoint->iTeamInvolved == BRITS)
			UnpackRGB(r, g, b, RGB_REDISH);
	else if(pPoint->iTeamInvolved == AMERS)
		UnpackRGB(r, g, b, RGB_BLUEISH);
	else{}

	// points being captured pulse slow
	if(pPoint->iStatus == CAPTURING)
	{
		if (pPoint->iTeamInvolved == AMERS)
		{
			r += 188 * (sin(flTime*4) + 1)/2;
			g += 139 * (sin(flTime*4) + 1)/2;
			b += 7 * (sin(flTime*4) + 1)/2;
		}
		if (pPoint->iTeamInvolved == BRITS)
		{
			g += 238 * (sin(flTime*4) + 1)/2;
			b += 238 * (sin(flTime*4) + 1)/2;
		}
	}

	// flags with not enough players in the capture area to start capture blink fast
	if ((pPoint->iPlayersInPoint < pPoint->iPlayersNeeded) && (pPoint->iPlayersInPoint > 0))
	{
		if((pPoint->iStatus != HELD) && (pPoint->iStatus != CAPTURING) && (pPoint->iTeamInvolved == BRITS))
		{
			g += 238 * (int)((sin(flTime*8) + 2.7f)/2);
			b += 238 * (int)((sin(flTime*8) + 2.7f)/2);
		}
		if((pPoint->iStatus != HELD) && (pPoint->iStatus != CAPTURING) && (pPoint->iTeamInvolved == AMERS))
		{
			r += 188 * (int)((sin(flTime*8) + 2.7f)/2);
			g += 139 * (int)((sin(flTime*8) + 2.7f)/2);
			b += 7 * (int)((sin(flTime*8) + 2.7f)/2);
		}
	}

	int iSpacing = XRES2(3);
	gHUD.DrawHudString((x + x_length + iSpacing), y + ((YRES2(y_length))/5), x_max, (char*)name, r, g, b);
	y += YRES2(y_length*2/3) + YRES2(3);
}

char *CHudPoints::ReturnNameById( int iId ) {
	for(int i = 0; i < MAX_POINTS; i++ ) {
		if(m_aPoints[i] && (m_aPoints[i]->iID == iId))
			return m_aPoints[i]->name;
	}
	return NULL;
}

void CHudPoints::StartClock(int iId) {
	m_bInvolvedInCapture = true;
	m_iInvolvedInCaptureId = iId;
}

void CHudPoints::ClearClock(int iId) {
	m_bInvolvedInCapture = false;
}

int CHudPoints::ReturnStatusById( int iId ) {
	for(int i = 0; i < MAX_POINTS; i++ ) {
		if(m_aPoints[i] && (m_aPoints[i]->iID == iId))
			return m_aPoints[i]->iStatus;
	}
	return NULL;
}
