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

//=========== (C) Copyright 1996-2002 Valve, L.L.C. All rights reserved. ===========
//
// The copyright to the contents herein is the property of Valve, L.L.C.
// The contents may be used and/or copied only with the written permission of
// Valve, L.L.C., or in accordance with the terms and conditions stipulated in
// the agreement/contract under which the contents have been supplied.
//

#include "VGUI_Font.h"
#include <VGUI_TextImage.h>

#include "hud.h"
#include "cl_util.h"
#include "camera.h"
#include "kbutton.h"
#include "cvardef.h"
#include "usercmd.h"
#include "const.h"
#include "camera.h"
#include "in_defs.h"
#include "parsemsg.h"

#include "vgui_int.h"
#include "vgui_TeamFortressViewport.h"
#include "vgui_ServerBrowser.h"

// Class Menu Dimensions
#define CLASSMENU_TITLE_X				XRES(40)
#define CLASSMENU_TITLE_Y				YRES(32)
#define CLASSMENU_TOPLEFT_BUTTON_X		XRES(40)
#define CLASSMENU_TOPLEFT_BUTTON_Y		YRES(80)
#define CLASSMENU_BUTTON_SIZE_X			XRES(124)
#define CLASSMENU_BUTTON_SIZE_Y			YRES(24)
#define CLASSMENU_BUTTON_SPACER_Y		YRES(8)
#define CLASSMENU_WINDOW_X				XRES(176)
#define CLASSMENU_WINDOW_Y				YRES(80)
#define CLASSMENU_WINDOW_SIZE_X			XRES(424)
#define CLASSMENU_WINDOW_SIZE_Y			YRES(312)
#define CLASSMENU_WINDOW_TEXT_X			XRES(150)
#define CLASSMENU_WINDOW_TEXT_Y			YRES(80)
#define CLASSMENU_WINDOW_NAME_X			XRES(155)
#define CLASSMENU_WINDOW_NAME_Y			YRES(8)
#define CLASSMENU_WINDOW_PLAYERS_Y		YRES(42)
#define CLASSMENU_WINDOW_NAME_WIDE		XRES(250)
#define CLASSMENU_WINDOW_NAME_TALL		YRES(20)

CAmerClassMenu::CAmerClassMenu(int iTrans, int iRemoveMe, int x,int y,int wide,int tall) : CMenuPanel(iTrans, iRemoveMe, x,y,wide,tall)
{
	cl_entity_t *player;

	player = gEngfuncs.GetLocalPlayer();

	// don't show class graphics at below 640x480 resolution
	bool bShowClassGraphic = true;
	if ( ScreenWidth < 640 )
	{
		bShowClassGraphic = false;
	}

	memset( m_pClassImages, 0, sizeof(m_pClassImages) );

	// Get the scheme used for the Titles
	CSchemeManager *pSchemes = gViewPort->GetSchemeManager();

	// schemes
	SchemeHandle_t hTitleScheme = pSchemes->getSchemeHandle( "Title Font" );
	SchemeHandle_t hClassWindowText = pSchemes->getSchemeHandle( "Briefing Text" );

	// color schemes
	int r, g, b, a;

	// Create the title
	Label *pLabel = new Label( "", CLASSMENU_TITLE_X, CLASSMENU_TITLE_Y );
	pLabel->setParent( this );
	pLabel->setFont( pSchemes->getFont(hTitleScheme) );
	pSchemes->getFgColor( hTitleScheme, r, g, b, a );
	pLabel->setFgColor( r, g, b, a);
	pSchemes->getBgColor( hTitleScheme, r, g, b, a );
	pLabel->setBgColor( r, g, b, a );
	pLabel->setContentAlignment( vgui::Label::a_west );
	pLabel->setText(gHUD.m_TextMessage.BufferedLocaliseTextString("Select Your Class"));

	// Create the Scroll panel
	m_pScrollPanel = new CTFScrollPanel( CLASSMENU_WINDOW_X, CLASSMENU_WINDOW_Y, CLASSMENU_WINDOW_SIZE_X, CLASSMENU_WINDOW_SIZE_Y );
	m_pScrollPanel->setParent(this);
	//force the scrollbars on, so after the validate clientClip will be smaller
	m_pScrollPanel->setScrollBarAutoVisible(false, false);
	m_pScrollPanel->setScrollBarVisible(true, true);
	m_pScrollPanel->setBorder( new LineBorder( Color(255, 255, 255, 0) ) );
	m_pScrollPanel->validate();

	int clientWide=m_pScrollPanel->getClient()->getWide();

	//turn scrollpanel back into auto show scrollbar mode and validate
	m_pScrollPanel->setScrollBarAutoVisible(false,true);
	m_pScrollPanel->setScrollBarVisible(false,false);
	m_pScrollPanel->validate();

	// Create the Class buttons
	for (int i = 1; i <= NUM_CLASSES; i++)
	{
		char sz[256]; 
		int iYPos = CLASSMENU_TOPLEFT_BUTTON_Y + ( (CLASSMENU_BUTTON_SIZE_Y + CLASSMENU_BUTTON_SPACER_Y) * i );

		sprintf(sz, "joinclass %d", i);
		ActionSignal *pASignal = new CMenuHandler_StringCommandWatch( sz, true );

		sprintf(sz, "%s", szClassNames[AMERS][i]);
	
		m_pButtons[i] = new ClassButton( i, sz, CLASSMENU_TOPLEFT_BUTTON_X, iYPos, CLASSMENU_BUTTON_SIZE_X, CLASSMENU_BUTTON_SIZE_Y, true);
		// RandomPC uses '0'
		if ( i >= i && i <= NUM_CLASSES )
		{
			sprintf(sz,"%d",i);
		}
		else
		{
			sprintf(sz,"0");
		}
		m_pButtons[i]->setBoundKey( sz[0] );
		m_pButtons[i]->setContentAlignment( vgui::Label::a_west );
		m_pButtons[i]->addActionSignal( pASignal );
		m_pButtons[i]->addInputSignal( new CHandler_MenuButtonOver(this, i) );
		m_pButtons[i]->setParent( this );

		// Create the Class Info Window
		//m_pClassInfoPanel[i] = new CTransparentPanel( 255, CLASSMENU_WINDOW_X, CLASSMENU_WINDOW_Y, CLASSMENU_WINDOW_SIZE_X, CLASSMENU_WINDOW_SIZE_Y );
		m_pClassInfoPanel[i] = new CTransparentPanel( 255, 0, 0, clientWide, CLASSMENU_WINDOW_SIZE_Y );
		m_pClassInfoPanel[i]->setParent( m_pScrollPanel->getClient() );
		//m_pClassInfoPanel[i]->setVisible( false );

		// don't show class pic in lower resolutions
		//int textOffs = XRES(8);

		//if ( bShowClassGraphic )
		//{
		int textOffs = CLASSMENU_WINDOW_NAME_X;
	//	}

		// Create the Class Name Label
		sprintf(sz, "Class: %s", szClassNames[AMERS][i]);
		char* localName=CHudTextMessage::BufferedLocaliseTextString( sz );
		Label *pNameLabel = new Label( "", textOffs, CLASSMENU_WINDOW_NAME_Y, CLASSMENU_WINDOW_NAME_WIDE, CLASSMENU_WINDOW_NAME_TALL);
		pNameLabel->setFont( pSchemes->getFont(hTitleScheme) ); 
		pNameLabel->setParent( m_pClassInfoPanel[i] );
		pSchemes->getFgColor( hTitleScheme, r, g, b, a );
		pNameLabel->setFgColor( r, g, b, a );
		pSchemes->getBgColor( hTitleScheme, r, g, b, a );
		pNameLabel->setBgColor( r, g, b, a );
		pNameLabel->setContentAlignment( vgui::Label::a_west );
		//pNameLabel->setBorder(new LineBorder());
		pNameLabel->setText(localName);

		// Create the Class Image
		if ( bShowClassGraphic )
		{
			sprintf( sz, "%s", szClassCodes[AMERS][i] );


			m_pClassImages[i] = new CImageLabel( sz, 0, 0, CLASSMENU_WINDOW_TEXT_X, CLASSMENU_WINDOW_TEXT_Y );

			CImageLabel *pLabel = m_pClassImages[i];
			pLabel->setParent( m_pClassInfoPanel[i] );
			//pLabel->setBorder(new LineBorder());

			pLabel->setVisible( true );
			
				
			// Reposition it based upon it's size
			int xOut, yOut;
			pNameLabel->getTextSize( xOut, yOut );
			pLabel->setPos( (CLASSMENU_WINDOW_TEXT_X - pLabel->getWide()) / 2, yOut /2 );
			
		}

		// Create the Player count string
		gHUD.m_TextMessage.LocaliseTextString( "Currently On Your Team", m_sPlayersOnTeamString, STRLENMAX_PLAYERSONTEAM );
		m_pPlayers[i] = new Label( "", textOffs, CLASSMENU_WINDOW_PLAYERS_Y);
		m_pPlayers[i]->setParent( m_pClassInfoPanel[i] );
		m_pPlayers[i]->setBgColor( 0, 0, 0, 255 );
		m_pPlayers[i]->setContentAlignment( vgui::Label::a_west );
		m_pPlayers[i]->setFont( pSchemes->getFont(hClassWindowText) );

		// Open up the Class Briefing File
		sprintf(sz, "classes/%s.txt", szClassCodes[AMERS][i]);
		char *cText = "Class Description Not Available.";
		char *pfile = (char*)gEngfuncs.COM_LoadFile( sz, 5, NULL );
		if (pfile)
			cText = pfile;
		
		// Create the Text info window
//		TextPanel *pTextWindow = new TextPanel(cText, textOffs, CLASSMENU_WINDOW_TEXT_Y, (CLASSMENU_WINDOW_SIZE_X - textOffs)-5, CLASSMENU_WINDOW_SIZE_Y - CLASSMENU_WINDOW_TEXT_Y);  //(CLASSMENU_WINDOW_SIZE_X - textOffs)-5, CLASSMENU_WINDOW_SIZE_Y - CLASSMENU_WINDOW_TEXT_Y);
		TextPanel *pTextWindow = new TextPanel(cText, textOffs, CLASSMENU_WINDOW_TEXT_Y, CLASSMENU_WINDOW_NAME_WIDE, CLASSMENU_WINDOW_NAME_TALL);
		pTextWindow->setParent( m_pClassInfoPanel[i] );
		pTextWindow->setFont( pSchemes->getFont(hClassWindowText) );
		pSchemes->getFgColor( hClassWindowText, r, g, b, a );
		pTextWindow->setFgColor( r, g, b, a);
		pSchemes->getBgColor( hClassWindowText, r, g, b, a );
		pTextWindow->setBgColor( r, g, b, a );

		// Resize the Info panel to fit it all
		int wide_label, tall_label;
		int wide_text,tall_text;

		pTextWindow->getTextImage()->getTextSizeWrapped(wide_text,tall_text);
		pNameLabel->getTextSize(wide_label, tall_label);

		int wide, tall;
		if(wide_label > wide_text)
		{
			wide = wide_label;
			tall = tall_label;
		}
		else
		{
			wide = wide_text;
			tall = tall_text;
		}

		pTextWindow->setSize(wide,tall);
//		pTextWindow->setBorder(new LineBorder());
		int xx,yy;
		pTextWindow->getPos(xx,yy);
		int maxX=xx+wide;
		int maxY=yy+tall;

		//check to see if the image goes lower than the text
		//just use the red teams [0] images
		if(m_pClassImages[i]!=null)
		{
			m_pClassImages[i]->getPos(xx,yy);
			if((yy+m_pClassImages[i]->getTall())>maxY)
			{
				maxY=yy+m_pClassImages[i]->getTall();
			}
		}

		m_pClassInfoPanel[i]->setSize( maxX , maxY );
		if (pfile) gEngfuncs.COM_FreeFile( pfile );
		//m_pClassInfoPanel[i]->setBorder(new LineBorder());

	}

	// Create the Cancel button
//	/*
	int j = NUM_CLASSES+1;
	int CanPos = CLASSMENU_TOPLEFT_BUTTON_Y + ( (CLASSMENU_BUTTON_SIZE_Y + CLASSMENU_BUTTON_SPACER_Y) * j );
	m_pCancelButton = new CommandButton("Cancel", CLASSMENU_TOPLEFT_BUTTON_X, CanPos, CLASSMENU_BUTTON_SIZE_X, CLASSMENU_BUTTON_SIZE_Y);
	m_pCancelButton->setParent( this );
	m_pCancelButton->setBoundKey( '0' );
	m_pCancelButton->addInputSignal( new CHandler_MenuButtonOver(this, 0) );
	m_pCancelButton->addActionSignal( new CMenuHandler_TextWindow(HIDE_TEXTWINDOW) );

	/*
	int j = NUM_CLASSES+1;
	// Create the Cancel button

	m_pButtons[j] = new ClassButton( j, gHUD.m_TextMessage.BufferedLocaliseTextString( "Cancel" ), CLASSMENU_TOPLEFT_BUTTON_X,CanPos, CLASSMENU_BUTTON_SIZE_X, CLASSMENU_BUTTON_SIZE_Y, true);
	m_pButtons[j]->setBoundKey( j );
	m_pButtons[j]->setContentAlignment( vgui::Label::a_west );
	m_pButtons[j]->addActionSignal( new CMenuHandler_TextWindow(HIDE_TEXTWINDOW) );
	m_pButtons[j]->addInputSignal( new CHandler_MenuButtonOver(this, j) );
	m_pButtons[j]->setParent( this );
	m_pButtons[j]->setVisible( true );
	*/
}
/*
void CAmerClassMenu::Reset(void)
{
    m_pCancelButton->setArmed( false );
    CMenuPanel::Reset();
}
*/
void CAmerClassMenu::Initialize(void)
{
    setVisible(false);
}

// Update
void CAmerClassMenu::Update()
{
	// Don't allow the player to join a team if they're not in a team
	if (!g_iTeamNumber)
		return;

	int	 iYPos = CLASSMENU_TOPLEFT_BUTTON_Y;

	// Cycle through the rest of the buttons
	for (int i = 1; i <= NUM_CLASSES; i++)
	{
		bool bCivilian = (gViewPort->GetValidClasses() == -1);

		if ( bCivilian )
		{
			// If this team can only be civilians, only the civilian button's visible
			if (i == 0)
			{
				m_pButtons[0]->setVisible( true );
				SetActiveInfo( 0 );
				iYPos += CLASSMENU_BUTTON_SIZE_Y + CLASSMENU_BUTTON_SPACER_Y;
			}
			else
			{
				m_pButtons[i]->setVisible( false );
			}
		}
		else 
		{
			if ( m_pButtons[i]->IsNotValid() || i == 0 )
			{
				m_pButtons[i]->setVisible( false );
			}
			else
			{
				m_pButtons[i]->setVisible( true );
				m_pButtons[i]->setPos( CLASSMENU_TOPLEFT_BUTTON_X, iYPos );
				iYPos += CLASSMENU_BUTTON_SIZE_Y + CLASSMENU_BUTTON_SPACER_Y;

				// Start with the first option up
				if (!m_iCurrentInfo)
					SetActiveInfo( i );
			}
		}

		// Now count the number of teammembers of this class
		int iTotal = 0;
		for ( int j = 1; j < MAX_PLAYERS; j++ )
		{
			if ( g_PlayerInfoList[j].name == NULL )
				continue; // empty player slot, skip
			if ( g_PlayerExtraInfo[j].teamname[0] == 0 )
				continue; // skip over players who are not in a team
			if ( g_PlayerInfoList[j].thisplayer )
				continue; // skip this player
			if ( g_PlayerExtraInfo[j].teamnumber != g_iTeamNumber )
				continue; // skip over players in other teams

			// If this team is forced to be civilians, just count the number of teammates
			if ( g_PlayerExtraInfo[j].playerclass != i && !bCivilian )
				continue;

			iTotal++;
		}

		char sz[256]; 
		sprintf(sz, m_sPlayersOnTeamString, iTotal);
		m_pPlayers[i]->setText( sz );

		// Set the text color to the teamcolor
		m_pPlayers[i]->setFgColor( iTeamColors[g_iTeamNumber][0], iTeamColors[g_iTeamNumber][1], iTeamColors[g_iTeamNumber][2], 0 );

		// set the graphic to be the team pick
		
			// unset all the other images
			if ( m_pClassImages[i] )
			{
				m_pClassImages[i]->setVisible( false );
			}

			// set the current team image
			if ( m_pClassImages[i] != NULL )
			{
				m_pClassImages[i]->setVisible( true );
			}
			else if ( m_pClassImages[i] )
			{
				m_pClassImages[i]->setVisible( true );
			}

		
	}

	// If the player already has a class, make the cancel button visible
	if ( g_iPlayerClass )
	{
		m_pButtons[NUM_CLASSES+1]->setVisible( true );
	}
	else
	{
		m_pButtons[NUM_CLASSES+1]->setVisible( false );
	}
}

//======================================
// Key inputs for the Class Menu
bool CAmerClassMenu::SlotInput( int iSlot )
{
	if ( (iSlot < UNDEFINED) || (iSlot > NUM_CLASSES) )
		return false;
	if ( !m_pButtons[ iSlot ] )
		return false;

	// Is the button pushable? (0 is special case)
	if (iSlot == 0)
	{
		m_pCancelButton->fireActionSignal();
		return true;
	}

	if ( !(m_pButtons[ iSlot ]->IsNotValid()) )
	{
		m_pButtons[ iSlot ]->fireActionSignal();
		return true;
	}

	return false;
}

//======================================
// Update the Class menu before opening it
void CAmerClassMenu::Open( void )
{
	Update();
	CMenuPanel::Open();
}

//======================================
// Mouse is over a class button, bring up the class info
void CAmerClassMenu::SetActiveInfo( int iInput )
{
	// Remove all the Info panels and bring up the specified one
	for (int i = 1; i <= NUM_CLASSES; i++)
	{
		m_pButtons[i]->setArmed( false );
		m_pClassInfoPanel[i]->setVisible( false );
	}
	m_pCancelButton->setArmed(false);

	if ( iInput > NUM_CLASSES || iInput <= 0 )
	{
		iInput = 0;
		m_pCancelButton->setArmed( true );
		return;
	}

	m_pButtons[iInput]->setArmed( true );
	m_pClassInfoPanel[iInput]->setVisible( true );
	m_iCurrentInfo = iInput;

	m_pScrollPanel->setScrollValue(0,0);
	m_pScrollPanel->validate();
}

