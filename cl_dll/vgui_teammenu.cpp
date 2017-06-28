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

#include "vgui_int.h"
#include "VGUI_Font.h"
#include "VGUI_ScrollPanel.h"
#include "VGUI_TextImage.h"

#include "hud.h"
#include "cl_util.h"
#include "vgui_TeamFortressViewport.h"

// Team Menu Dimensions
#define TEAMMENU_TITLE_X				XRES(40)
#define TEAMMENU_TITLE_Y				YRES(32)
#define TEAMMENU_TOPLEFT_BUTTON_X		XRES(40)
#define TEAMMENU_TOPLEFT_BUTTON_Y		YRES(80)
#define TEAMMENU_BUTTON_SIZE_X			XRES(124)
#define TEAMMENU_BUTTON_SIZE_Y			YRES(24)
#define TEAMMENU_BUTTON_SPACER_Y		YRES(8)
#define TEAMMENU_WINDOW_X				XRES(176)
#define TEAMMENU_WINDOW_Y				YRES(80)
#define TEAMMENU_WINDOW_SIZE_X			XRES(424)
#define TEAMMENU_WINDOW_SIZE_Y			YRES(312)
#define TEAMMENU_WINDOW_TITLE_X			XRES(16)
#define TEAMMENU_WINDOW_TITLE_Y			YRES(16)
#define TEAMMENU_WINDOW_TEXT_X			XRES(16)
#define TEAMMENU_WINDOW_TEXT_Y			YRES(48)
#define TEAMMENU_WINDOW_TEXT_SIZE_Y		YRES(178)
#define TEAMMENU_WINDOW_INFO_X			XRES(16)
#define TEAMMENU_WINDOW_INFO_Y			YRES(234)

CTeamMenuPanel::CTeamMenuPanel(int iTrans, int iRemoveMe, int x, int y, int wide, int tall) : CMenuPanel(iTrans, iRemoveMe, x,y,wide,tall)
{
	m_pPanel = new CTransparentPanel( 200, XRES(0), YRES(0), XRES(640), YRES(480));
	m_pPanel->setParent( this );
			
	m_pBackground = new CImageLabel( "background", XRES(0), YRES(0));

	int iHeight = m_pBackground->getImageTall();
	int iWidth = m_pBackground->getImageWide();
	int x_pos_background = (XRES(640) - iWidth) / 2;
	int y_pos_background = (YRES(480) - iHeight) / 2;

	m_pBackground->setPos(x_pos_background, y_pos_background);
	m_pBackground->setVisible( true );
	m_pBackground->setParent( m_pPanel);

	int x_pos = 0;
	int y_pos = 0;

	if(ScreenWidth >= 640)
	{
		x_pos = x_pos_background + 10;
		y_pos = y_pos_background + 95;
		iWidth = 190;
		iHeight = 150;
	}
	else
	{
		x_pos = x_pos_background + 5;
		y_pos = y_pos_background + 40;
		iWidth = 80;
		iHeight = 70;
	}
	
	m_pBritish = new CommandButton( "",x_pos, y_pos, iWidth, iHeight, true, true);
	m_pBritish->setVisible( true );
	m_pBritish->setParent( this );
	m_pBritish->addActionSignal( new CMenuHandler_StringCommandWatch( "jointeam 1", true ) );

	if(ScreenWidth >= 640)
	{
		x_pos = x_pos_background + 210;
		y_pos = y_pos_background + 95;
		iWidth = 190;
		iHeight = 150;
	}
	else
	{
		x_pos = x_pos_background + 90;
		y_pos = y_pos_background + 40;
		iWidth = 80;
		iHeight = 70;
	}
	
	m_pAmerican = new CommandButton( "", x_pos, y_pos, iWidth, iHeight, true, true);
	m_pAmerican->setVisible( true );
	m_pAmerican->setParent( this );
	m_pAmerican->addActionSignal( new CMenuHandler_StringCommandWatch( "jointeam 2", true ) );

	if(ScreenWidth >= 640)
	{
		x_pos = x_pos_background + 125;
		y_pos = y_pos_background + 280;
		iWidth = 165;
		iHeight = 80;
	}
	else
	{
		x_pos = x_pos_background + 54;
		y_pos = y_pos_background + 115;
		iWidth = 66;
		iHeight = 35;
	}

	m_pSpectatorEye = new CommandButton( "",x_pos, y_pos, iWidth, iHeight, true, true);
	m_pSpectatorEye->setVisible( true );
	m_pSpectatorEye->setParent( this );
	m_pSpectatorEye->addActionSignal( new CMenuHandler_StringCommandWatch( "jointeam 3", true ) );

	if(ScreenWidth >= 640)
	{
		x_pos = x_pos_background + 150;
		y_pos = y_pos_background + 375;
		iWidth = 125;
		iHeight = 20;
	}
	else
	{
		x_pos = x_pos_background + 65;
		y_pos = y_pos_background + 157;
		iWidth = 50;
		iHeight = 8;
	}

	m_pBritish2 = new CommandButton( "",x_pos, y_pos, iWidth, iHeight, true, true);
	m_pBritish2->setVisible( true );
	m_pBritish2->setParent( this );
	m_pBritish2->addActionSignal( new CMenuHandler_StringCommandWatch( "jointeam 1", true ) );

	if(ScreenWidth >= 640)
	{
		x_pos = x_pos_background + 150;
		y_pos = y_pos_background + 395;
		iWidth = 125;
		iHeight = 15;
	}
	else
	{
		x_pos = x_pos_background + 65;
		y_pos = y_pos_background + 167;
		iWidth = 50;
		iHeight = 7;
	}

	m_pAmerican2 = new CommandButton( "",x_pos, y_pos, iWidth, iHeight, true, true);
	m_pAmerican2->setVisible( true );
	m_pAmerican2->setParent( this );
	m_pAmerican2->addActionSignal( new CMenuHandler_StringCommandWatch( "jointeam 2", true ) );

	if(ScreenWidth >= 640)
	{
		x_pos = x_pos_background + 150;
		y_pos = y_pos_background + 410;
		iWidth = 125;
		iHeight = 18;
	}
	else
	{
		x_pos = x_pos_background + 65;
		y_pos = y_pos_background + 174;
		iWidth = 50;
		iHeight = 7;
	}

	m_pSpectator2 = new CommandButton( "",x_pos, y_pos, iWidth, iHeight, true, true);
	m_pSpectator2->setVisible( true );
	m_pSpectator2->setParent( this );
	m_pSpectator2->addActionSignal( new CMenuHandler_StringCommandWatch( "jointeam 3", true ) );

	if(ScreenWidth >= 640)
	{
		x_pos = x_pos_background + 150;
		y_pos = y_pos_background + 428;
		iWidth = 125;
		iHeight = 25;
	}
	else
	{
		x_pos = x_pos_background + 65;
		y_pos = y_pos_background + 181;
		iWidth = 50;
		iHeight = 7;
	}

	m_pAutoAssign = new CommandButton( "",x_pos, y_pos, iWidth, iHeight, true, true);
	m_pAutoAssign->setVisible( true );
	m_pAutoAssign->setParent( this );
	m_pAutoAssign->addActionSignal( new CMenuHandler_StringCommandWatch( "jointeam 5", true ) );

	if(ScreenWidth >= 640)
	{
		x_pos = x_pos_background + 205;
		y_pos = y_pos_background + 450;
		iWidth = 70;
		iHeight = 20;
	}
	else
	{
		x_pos = x_pos_background + 85;
		y_pos = y_pos_background + 190;
		iWidth = 30;
		iHeight = 8;
	}

	m_pCancel = new CommandButton( "",x_pos, y_pos, iWidth, iHeight, true, true);
	m_pCancel->setVisible( true );
	m_pCancel->setParent( this );
	m_pCancel->addActionSignal( new CMenuHandler_TextWindow(HIDE_TEXTWINDOW) );

}


void CTeamMenuPanel::SetActiveInfo( int iShowText )
{
}

bool CTeamMenuPanel::SlotInput(int iSlot)
{
	if(iSlot == BRITS)
	{
		m_pBritish->fireActionSignal();
		return true;
	}
	if(iSlot == AMERS)
	{
		m_pAmerican->fireActionSignal();
		return true;
	}
	if(iSlot == SPECS)
	{
		m_pSpectatorEye->fireActionSignal();
		return true;
	}
	if(iSlot == 5)
	{
		m_pAutoAssign->fireActionSignal();
		return true;
	}
	if(iSlot == UNDEFINED && g_iTeamNumber != UNDEFINED)
	{
		m_pCancel->fireActionSignal();
		return true;
	}
	return false;
}

void CTeamMenuPanel::Update( void )
{
	if(m_pCancel && g_iTeamNumber == UNDEFINED)
	{
		m_pCancel->setVisible( false );
	}
	else if(m_pCancel)
	{
		m_pCancel->setVisible( true );
	}
	else {}
}

void CTeamMenuPanel::Open( void )
{
	Update();
	CMenuPanel::Open();
}