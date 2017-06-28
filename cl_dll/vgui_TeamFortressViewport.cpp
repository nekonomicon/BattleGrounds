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

//=========== (C) Copyright 1996-2001 Valve, L.L.C. All rights reserved. ===========
//
// The copyright to the contents herein is the property of Valve, L.L.C.
// The contents may be used and/or copied only with the written permission of
// Valve, L.L.C., or in accordance with the terms and conditions stipulated in
// the agreement/contract under which the contents have been supplied.
//
// Purpose: Client DLL VGUI Viewport
//
// $Workfile:     $
// $Date: 2003/11/30 12:06:08 $
//
//-----------------------------------------------------------------------------
// $Log: vgui_TeamFortressViewport.cpp,v $
// Revision 1.3  2003/11/30 12:06:08  ben
// Updated for RC 3.6
//
// Revision 1.2  2003/11/15 22:35:17  ben
// Added Stationary Reload Contribution from Raiko
//
// Revision 1.1.1.1  2003/11/15 13:17:11  bgmod
// Inital Commit
//
// $NoKeywords: $
//=============================================================================
#include<VGUI_Cursor.h>
#include<VGUI_Frame.h>
#include<VGUI_Label.h>
#include<VGUI_Surface.h>
#include<VGUI_BorderLayout.h>
#include<VGUI_Panel.h>
#include<VGUI_ImagePanel.h>
#include<VGUI_Button.h>
#include<VGUI_ActionSignal.h>
#include<VGUI_InputSignal.h>
#include<VGUI_MenuSeparator.h>
#include<VGUI_TextPanel.h>
#include<VGUI_LoweredBorder.h>
#include<VGUI_LineBorder.h>
#include<VGUI_Scheme.h>
#include<VGUI_Font.h>
#include<VGUI_App.h>
#include<VGUI_BuildGroup.h>

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
#include "pm_shared.h"
#include "../engine/keydefs.h"
#include "demo.h"
#include "demo_api.h"

#include "vgui_int.h"
#include "vgui_TeamFortressViewport.h"
#include "vgui_ServerBrowser.h"
#include "vgui_ScorePanel.h"
#include "vgui_SpectatorPanel.h"

#include "bg_defs.h"

#include "particle_header.h"

extern int g_iVisibleMouse;
extern CHudHintbox		gHintBox;
class CCommandMenu;
int g_iPlayerClass;
int g_iTeamNumber;
int g_iUser1;
int g_iUser2;
int g_iUser3;
int g_iMeleeOnly = 0;
int g_iLaserTag = 0;
int g_iStationaryReload = 0;
bool g_bCrouched;
float g_flStamina = 0.0;
float g_flHealth = 0.0;

// Scoreboard positions
#define SBOARD_INDENT_X			XRES(55)
#define SBOARD_INDENT_Y			YRES(40)

// low-res scoreboard indents
#define SBOARD_INDENT_X_512		30
#define SBOARD_INDENT_Y_512		30

#define SBOARD_INDENT_X_400		0
#define SBOARD_INDENT_Y_400		20

void IN_ResetMouse( void );
extern CMenuPanel *CMessageWindowPanel_Create( const char *szMOTD, const char *szTitle, int iShadeFullscreen, int iRemoveMe, int x, int y, int wide, int tall );
extern float * GetClientColor( int clientIndex );

using namespace vgui;

// Team Colors
int iNumberOfTeamColors = 5;
int iTeamColors[5][3] =
{
	{ 255, 255, 255 },	// white
	{ 255, 16, 16 },	// british red
	{ 66, 115, 247 },	// american blue
	{ 20, 100, 20 },	// spectator green
	{ 220, 175, 10 },	// connecting orange
};

// Get the name of TGA file, based on GameDir
char* GetVGUITGAName(const char *pszName)
{
	int i;
	char sz[256]; 
	static char gd[256]; 
	const char *gamedir;

	if (ScreenWidth < 640)
		i = 320;
	else
		i = 640;
	sprintf(sz, pszName, i);

	gamedir = gEngfuncs.pfnGetGameDirectory();
	sprintf(gd, "%s/gfx/vgui/%s.tga",gamedir,sz);

	return gd;
}

//================================================================
// COMMAND MENU
//================================================================
void CCommandMenu::AddButton( CommandButton *pButton )
{
	if (m_iButtons >= MAX_BUTTONS)
		return;

	m_aButtons[m_iButtons] = pButton;
	m_iButtons++;
	pButton->setParent( this );
	pButton->setFont( Scheme::sf_primary3 );

	// give the button a default key binding
	if ( m_iButtons < 10 )
	{
		pButton->setBoundKey( m_iButtons + '0' );
	}
	else if ( m_iButtons == 10 )
	{
		pButton->setBoundKey( '0' );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Tries to find a button that has a key bound to the input, and
//			presses the button if found
// Input  : keyNum - the character number of the input key
// Output : Returns true if the command menu should close, false otherwise
//-----------------------------------------------------------------------------
bool CCommandMenu::KeyInput( int keyNum )
{
	// loop through all our buttons looking for one bound to keyNum
	for ( int i = 0; i < m_iButtons; i++ )
	{
		if ( !m_aButtons[i]->IsNotValid() )
		{
			if ( m_aButtons[i]->getBoundKey() == keyNum )
			{
				// hit the button
				if ( m_aButtons[i]->GetSubMenu() )
				{
					// open the sub menu
					gViewPort->SetCurrentCommandMenu( m_aButtons[i]->GetSubMenu() );
					return false;
				}
				else
				{
					// run the bound command
					m_aButtons[i]->fireActionSignal();
					return true;
				}
			}
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: clears the current menus buttons of any armed (highlighted) 
//			state, and all their sub buttons
//-----------------------------------------------------------------------------
void CCommandMenu::ClearButtonsOfArmedState( void )
{
	for ( int i = 0; i < GetNumButtons(); i++ )
	{
		m_aButtons[i]->setArmed( false );

		if ( m_aButtons[i]->GetSubMenu() )
		{
			m_aButtons[i]->GetSubMenu()->ClearButtonsOfArmedState();
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pSubMenu - 
// Output : CommandButton
//-----------------------------------------------------------------------------
CommandButton *CCommandMenu::FindButtonWithSubmenu( CCommandMenu *pSubMenu )
{
	for ( int i = 0; i < GetNumButtons(); i++ )
	{
		if ( m_aButtons[i]->GetSubMenu() == pSubMenu )
			return m_aButtons[i];
	}

	return NULL;
}

// Recalculate the visible buttons
bool CCommandMenu::RecalculateVisibles( int iYOffset, bool bHideAll )
{
	int		i, iCurrentY = 0;
	int		iVisibleButtons = 0;

	// Cycle through all the buttons in this menu, and see which will be visible
	for (i = 0; i < m_iButtons; i++)
	{
		int iClass = m_aButtons[i]->GetPlayerClass();

		if ( (iClass && iClass != g_iPlayerClass ) || ( m_aButtons[i]->IsNotValid() ) || bHideAll )
		{
			m_aButtons[i]->setVisible( false );
			if ( m_aButtons[i]->GetSubMenu() != NULL )
			{
				(m_aButtons[i]->GetSubMenu())->RecalculateVisibles( 0, true );
			}
		}
		else
		{
 			// If it's got a submenu, force it to check visibilities
			if ( m_aButtons[i]->GetSubMenu() != NULL )
			{
				if ( !(m_aButtons[i]->GetSubMenu())->RecalculateVisibles( 0 , false ) )
				{
					// The submenu had no visible buttons, so don't display this button
					m_aButtons[i]->setVisible( false );
					continue;
				}
			}

			m_aButtons[i]->setVisible( true );
			iVisibleButtons++;
		}
	}

	// Set Size
	setSize( _size[0], (iVisibleButtons * (m_flButtonSizeY-1)) + 1 );

	if ( iYOffset )
	{
		m_iYOffset = iYOffset;
	}

	for (i = 0; i < m_iButtons; i++)
	{
		if ( m_aButtons[i]->isVisible() )
		{
			if ( m_aButtons[i]->GetSubMenu() != NULL )
				(m_aButtons[i]->GetSubMenu())->RecalculateVisibles( iCurrentY + m_iYOffset, false );
			

			// Make sure it's at the right Y position
			// m_aButtons[i]->getPos( iXPos, iYPos );

			if ( m_iDirection )
			{
				m_aButtons[i]->setPos( 0, (iVisibleButtons-1) * (m_flButtonSizeY-1) - iCurrentY );
			}
			else
			{
				m_aButtons[i]->setPos( 0, iCurrentY );
			}

			iCurrentY += (m_flButtonSizeY-1);
		}
	}

	return iVisibleButtons?true:false;
}

// Make sure all submenus can fit on the screen
void CCommandMenu::RecalculatePositions( int iYOffset )
{
	int iTop;
	int iAdjust = 0;

	m_iYOffset+= iYOffset;

	if ( m_iDirection )
		iTop = ScreenHeight - (m_iYOffset + _size[1] );
	else
		iTop = m_iYOffset;

	if ( iTop < 0 )
		iTop = 0;

	// Calculate if this is going to fit onscreen, and shuffle it up if it won't
	int iBottom = iTop + _size[1];

	if ( iBottom > ScreenHeight )
	{
		// Move in increments of button sizes
		while (iAdjust < (iBottom - ScreenHeight))
		{
			iAdjust += m_flButtonSizeY - 1;
		}

		iTop -= iAdjust;

		// Make sure it doesn't move off the top of the screen (the menu's too big to fit it all)
		if ( iTop < 0 )
		{
			iAdjust -= (0 - iTop);
			iTop = 0;
		}
	}

	setPos( _pos[0], iTop );

	// We need to force all menus below this one to update their positions now, because they
	// might have submenus riding off buttons in this menu that have just shifted.
	for (int i = 0; i < m_iButtons; i++)
		m_aButtons[i]->UpdateSubMenus( iAdjust );
}


// Make this menu and all menus above it in the chain visible
void CCommandMenu::MakeVisible( CCommandMenu *pChildMenu )
{
/*
	// Push down the button leading to the child menu
	for (int i = 0; i < m_iButtons; i++)
	{
		if ( (pChildMenu != NULL) && (m_aButtons[i]->GetSubMenu() == pChildMenu) )
		{
			m_aButtons[i]->setArmed( true );
		}
		else
		{
			m_aButtons[i]->setArmed( false );
		}
	}
*/

	setVisible(true);
	if (m_pParentMenu)
		m_pParentMenu->MakeVisible( this );
}

//================================================================
// CreateSubMenu
CCommandMenu *TeamFortressViewport::CreateSubMenu( CommandButton *pButton, CCommandMenu *pParentMenu, int iYOffset, int iXOffset )
{
	int iXPos = 0;
	int iYPos = 0;
	int iWide = CMENU_SIZE_X;
	int iTall = 0;
	int iDirection = 0;

	if (pParentMenu)
	{
		iXPos = m_pCurrentCommandMenu->GetXOffset() + (CMENU_SIZE_X - 1) + iXOffset;
		iYPos = m_pCurrentCommandMenu->GetYOffset() + iYOffset;
		iDirection = pParentMenu->GetDirection();
	}

	CCommandMenu *pMenu = new CCommandMenu(pParentMenu, iDirection, iXPos, iYPos, iWide, iTall );
	pMenu->setParent(this);
	pButton->AddSubMenu( pMenu );
	pButton->setFont( Scheme::sf_primary3 );
	pMenu->m_flButtonSizeY = m_pCurrentCommandMenu->m_flButtonSizeY;

	// Create the Submenu-open signal
	InputSignal *pISignal = new CMenuHandler_PopupSubMenuInput(pButton, pMenu);
	pButton->addInputSignal(pISignal);

	// Put a > to show it's a submenu
	CImageLabel *pLabel = new CImageLabel( "arrow", CMENU_SIZE_X - SUBMENU_SIZE_X, SUBMENU_SIZE_Y );
	pLabel->setParent(pButton);
	pLabel->addInputSignal(pISignal);

	// Reposition
	pLabel->getPos( iXPos, iYPos );
	pLabel->setPos( CMENU_SIZE_X - pLabel->getImageWide(), (BUTTON_SIZE_Y - pLabel->getImageTall()) / 2 );

	// Create the mouse off signal for the Label too
	if (!pButton->m_bNoHighlight)
		pLabel->addInputSignal( new CHandler_CommandButtonHighlight(pButton) );

	return pMenu;
}

//-----------------------------------------------------------------------------
// Purpose: Makes sure the memory allocated for TeamFortressViewport is nulled out
// Input  : stAllocateBlock - 
// Output : void *
//-----------------------------------------------------------------------------
void *TeamFortressViewport::operator new( size_t stAllocateBlock )
{
//	void *mem = Panel::operator new( stAllocateBlock );
	void *mem = ::operator new( stAllocateBlock );
	memset( mem, 0, stAllocateBlock );
	return mem;
}

//-----------------------------------------------------------------------------
// Purpose: InputSignal handler for the main viewport
//-----------------------------------------------------------------------------
class CViewPortInputHandler : public InputSignal
{
public:
	bool bPressed;

	CViewPortInputHandler()
	{
	}

	virtual void cursorMoved(int x,int y,Panel* panel) {}
	virtual void cursorEntered(Panel* panel) {}
	virtual void cursorExited(Panel* panel) {}
	virtual void mousePressed(MouseCode code,Panel* panel) 
	{
		if ( code != MOUSE_LEFT )
		{
			// send a message to close the command menu
			// this needs to be a message, since a direct call screws the timing
			gEngfuncs.pfnClientCmd( "ForceCloseCommandMenu\n" );
		}
	}
	virtual void mouseReleased(MouseCode code,Panel* panel)
	{
	}

	virtual void mouseDoublePressed(MouseCode code,Panel* panel) {}
	virtual void mouseWheeled(int delta,Panel* panel) {}
	virtual void keyPressed(KeyCode code,Panel* panel) {}
	virtual void keyTyped(KeyCode code,Panel* panel) {}
	virtual void keyReleased(KeyCode code,Panel* panel) {}
	virtual void keyFocusTicked(Panel* panel) {}
};


//================================================================
TeamFortressViewport::TeamFortressViewport(int x,int y,int wide,int tall) : Panel(x,y,wide,tall), m_SchemeManager(wide,tall)
{

	gViewPort = this;
	m_iInitialized = false;
	m_pTeamMenu = NULL;
	m_pBritClassMenu = NULL;
	//Ben - Initilizes our american class menu pointer
	m_pAmerClassMenu = NULL;
	//Ben
	m_pScoreBoard = NULL;
	m_pSpectatorPanel = NULL;
	m_pCurrentMenu = NULL;
	m_pCurrentCommandMenu = NULL;

	Initialize();
	addInputSignal( new CViewPortInputHandler );

	int r, g, b, a;
	
	Scheme* pScheme = App::getInstance()->getScheme();

	// primary text color
	// Get the colors
	//!! two different types of scheme here, need to integrate
	SchemeHandle_t hPrimaryScheme = m_SchemeManager.getSchemeHandle( "Primary Button Text" );
	{
		// font
		pScheme->setFont( Scheme::sf_primary1, m_SchemeManager.getFont(hPrimaryScheme) );

		// text color
		m_SchemeManager.getFgColor( hPrimaryScheme, r, g, b, a );
		pScheme->setColor(Scheme::sc_primary1, 255, 255, 255, 0 );		// sc_primary1 is non-transparent orange

		// background color (transparent black)
		m_SchemeManager.getBgColor( hPrimaryScheme, r, g, b, a );
		pScheme->setColor(Scheme::sc_primary3, r, g, b, a );

		// armed foreground color
		m_SchemeManager.getFgArmedColor( hPrimaryScheme, r, g, b, a );
		pScheme->setColor(Scheme::sc_secondary2, r, g, b, a );

		// armed background color
		m_SchemeManager.getBgArmedColor( hPrimaryScheme, r, g, b, a );
		pScheme->setColor(Scheme::sc_primary2, r, g, b, a );

		//!! need to get this color from scheme file
		// used for orange borders around buttons
		m_SchemeManager.getBorderColor( hPrimaryScheme, r, g, b, a );
		// pScheme->setColor(Scheme::sc_secondary1, r, g, b, a );
		pScheme->setColor(Scheme::sc_secondary1, 255, 255, 255, 0);
	}

	// Change the second primary font (used in the scoreboard)
	SchemeHandle_t hScoreboardScheme = m_SchemeManager.getSchemeHandle( "Scoreboard Text" );
	{
		pScheme->setFont(Scheme::sf_primary2, m_SchemeManager.getFont(hScoreboardScheme) );
	}
	
	// Change the third primary font (used in command menu)
	SchemeHandle_t hCommandMenuScheme = m_SchemeManager.getSchemeHandle( "CommandMenu Text" );
	{
		pScheme->setFont(Scheme::sf_primary3, m_SchemeManager.getFont(hCommandMenuScheme) );
	}

	App::getInstance()->setScheme(pScheme);

	// VGUI MENUS
	CreateTeamMenu();
	CreateBritClassMenu();
	//Ben - Calls he function which creates our new class menu
	CreateAmerClassMenu();
	//Ben
	CreateSpectatorMenu();
	CreateScoreBoard();
	// Init command menus
	m_iNumMenus = 0;
	m_iCurrentTeamNumber = m_iUser1 = m_iUser2 = m_iUser3 = 0;

	m_StandardMenu = CreateCommandMenu("commandmenu.txt", 0, CMENU_TOP, false, CMENU_SIZE_X, BUTTON_SIZE_Y, 0 );
	m_SpectatorOptionsMenu = CreateCommandMenu("spectatormenu.txt", 1, YRES(32), true, CMENU_SIZE_X, BUTTON_SIZE_Y / 2, 0 );	// above bottom bar, flat design
	m_SpectatorCameraMenu = CreateCommandMenu("spectcammenu.txt", 1, YRES(32), true, XRES( 200 ), BUTTON_SIZE_Y / 2, ScreenWidth - ( XRES ( 200 ) + 15 ) );	// above bottom bar, flat design

	CreateServerBrowser();
}

//-----------------------------------------------------------------------------
// Purpose: Called everytime a new level is started. Viewport clears out it's data.
//-----------------------------------------------------------------------------
void TeamFortressViewport::Initialize( void )
{
	// Force each menu to Initialize
	
	// BP - Clear Particle Systems
	pParticleManager->RemoveSystems();	
	
	if (m_pBritClassMenu)
	{
		m_pBritClassMenu->Initialize();
	}
	//Ben Initializes our 2nd class menu
	if (m_pAmerClassMenu)
	{
		m_pAmerClassMenu->Initialize();
	}
	//Ben
	if (m_pScoreBoard)
	{
		m_pScoreBoard->Initialize();
		HideScoreBoard();
	}
	if (m_pSpectatorPanel)
	{
		// Spectator menu doesn't need initializing
		m_pSpectatorPanel->setVisible( true );
	}

	// Make sure all menus are hidden
	HideVGUIMenu();
	HideCommandMenu();

	// Clear out some data
	m_iGotAllMOTD = true;
	m_flScoreBoardLastUpdated = 0;
	m_flSpectatorPanelLastUpdated = 0;

	// reset player info
	g_iPlayerClass = 0;
	g_iTeamNumber = 0;

	strcpy(m_sMapName, "");
	strcpy(m_szServerName, "");
	for (int i = UNDEFINED; i < NUM_TEAMS; i++)
	{
		m_iValidClasses[i] = 0;
		strcpy(m_sTeamNames[i], "");
	}

	App::getInstance()->setCursorOveride( App::getInstance()->getScheme()->getCursor(Scheme::scu_none) );
}

TeamFortressViewport::~TeamFortressViewport()
{
	delete m_pAmerClassMenu;
	delete m_pBritClassMenu;
	delete [] *m_pCommandMenus;
	delete m_pScoreBoard;
	delete m_pServerBrowser;
	delete m_pSpectatorPanel;
	delete [] *m_pTeamButtons;
	delete m_pTeamMenu;
}

class CException;
//-----------------------------------------------------------------------------
// Purpose: Read the Command Menu structure from the txt file and create the menu.
//			Returns Index of menu in m_pCommandMenus
//-----------------------------------------------------------------------------
int TeamFortressViewport::CreateCommandMenu( char * menuFile, int direction, int yOffset, bool flatDesign, float flButtonSizeX, float flButtonSizeY, int xOffset )
{
	// COMMAND MENU
	// Create the root of this new Command Menu

	int newIndex = m_iNumMenus;
	
	m_pCommandMenus[newIndex] = new CCommandMenu(NULL, direction, xOffset, yOffset, flButtonSizeX, 300);	// This will be resized once we know how many items are in it
	m_pCommandMenus[newIndex]->setParent(this);
	m_pCommandMenus[newIndex]->setVisible(false);
	m_pCommandMenus[newIndex]->m_flButtonSizeY = flButtonSizeY;
	m_pCommandMenus[newIndex]->m_iSpectCmdMenu = direction;

	m_iNumMenus++;

	// Read Command Menu from the txt file
	char token[1024];
	char *pfile = (char*)gEngfuncs.COM_LoadFile( menuFile, 5, NULL);
	if (!pfile)
	{
		gEngfuncs.Con_DPrintf( "Unable to open %s\n", menuFile);
		SetCurrentCommandMenu( NULL );
		return newIndex;
	}

try
{
	// Now start parsing the menu structure
	m_pCurrentCommandMenu = m_pCommandMenus[newIndex];
	char szLastButtonText[32] = "file start";
	pfile = gEngfuncs.COM_ParseFile(pfile, token);
	while ( ( strlen ( token ) > 0 ) && ( m_iNumMenus < MAX_MENUS ) )
	{
		// Keep looping until we hit the end of this menu
		while ( token[0] != '}' && ( strlen( token ) > 0 ) )
		{
			char cText[32] = "";
			char cBoundKey[32] = "";
			char cCustom[32] = "";
			static const int cCommandLength = 128;
			char cCommand[cCommandLength] = "";
			char szMap[MAX_MAPNAME] = "";
			int	 iPlayerClass = 0;
			int  iCustom = false;
			int  iTeamOnly = -1;
			int  iToggle = 0;
			int  iButtonY;
			bool bGetExtraToken = true;
			CommandButton *pButton = NULL;
			
			// We should never be here without a Command Menu
			if (!m_pCurrentCommandMenu)
			{
				gEngfuncs.Con_Printf("Error in %s file after '%s'.\n",menuFile, szLastButtonText );
				m_iInitialized = false;
				return newIndex;
			}

			// token should already be the bound key, or the custom name
			strncpy( cCustom, token, 32 );
			cCustom[31] = '\0';

			// See if it's a custom button
			if (!strcmp(cCustom, "CUSTOM") )
			{
				iCustom = true;

				// Get the next token
				pfile = gEngfuncs.COM_ParseFile(pfile, token);
			}
			// See if it's a map
			else if (!strcmp(cCustom, "MAP") )
			{
				// Get the mapname
				pfile = gEngfuncs.COM_ParseFile(pfile, token);
				strncpy( szMap, token, MAX_MAPNAME );
				szMap[MAX_MAPNAME-1] = '\0';

				// Get the next token
				pfile = gEngfuncs.COM_ParseFile(pfile, token);
			}
			else if ( !strncmp(cCustom, "TEAM", 4) ) // TEAM1, TEAM2, TEAM3, TEAM4
			{
				// make it a team only button
				iTeamOnly = atoi( cCustom + 4 );
				
				// Get the next token
				pfile = gEngfuncs.COM_ParseFile(pfile, token);
			}
			else if ( !strncmp(cCustom, "TOGGLE", 6) ) 
			{
				iToggle = true;
				// Get the next token
				pfile = gEngfuncs.COM_ParseFile(pfile, token);
			}
			else
			{
				// See if it's a Class
				for (int i = 1; i <= NUM_CLASSES; i++)
				{
					for(int j = 1; j <= NUM_TEAMS; j++)
					{
						if ( !strcmp(token, (const char*)szClassCodes[j][i]) )
						{
							// Save it off
							iPlayerClass = i;

							// Get the button text
							pfile = gEngfuncs.COM_ParseFile(pfile, token);
							break;
						}
					}
				}
			}

			// Get the button bound key
			strncpy( cBoundKey, token, 32 );
			cText[31] = '\0';

			// Get the button text
			pfile = gEngfuncs.COM_ParseFile(pfile, token);
			strncpy( cText, token, 32 );
			cText[31] = '\0';

			// save off the last button text we've come across (for error reporting)
			strcpy( szLastButtonText, cText );

			// Get the button command
			pfile = gEngfuncs.COM_ParseFile(pfile, token);
			strncpy( cCommand, token, cCommandLength );
			cCommand[cCommandLength - 1] = '\0';

			iButtonY = (BUTTON_SIZE_Y-1) * m_pCurrentCommandMenu->GetNumButtons();
			
			// Custom button handling
			if ( iCustom )
			{
				pButton = CreateCustomButton( cText, cCommand, iButtonY );

				// Get the next token to see if we're a menu
				pfile = gEngfuncs.COM_ParseFile(pfile, token);

				if ( token[0] == '{' )
				{
					strcpy( cCommand, token );
				}
				else
				{
					bGetExtraToken = false;
				}
			}
			else if ( szMap[0] != '\0' )
			{
				// create a map button
				pButton = new MapButton(szMap, cText, xOffset, iButtonY, flButtonSizeX, flButtonSizeY );
			}
			else if ( iTeamOnly != -1)
			{
				// button that only shows up if the player is on team iTeamOnly
				pButton = new TeamOnlyCommandButton( iTeamOnly, cText, xOffset, iButtonY, flButtonSizeX, flButtonSizeY, flatDesign );
			}
			else if ( iToggle && direction == 0 )
			{
				pButton = new ToggleCommandButton( cCommand, cText, xOffset, iButtonY, flButtonSizeX, flButtonSizeY, flatDesign );
			}
			else if ( direction == 1 )
			{
				if ( iToggle )
					pButton = new SpectToggleButton( cCommand, cText, xOffset, iButtonY, flButtonSizeX, flButtonSizeY, flatDesign );
				else
					pButton = new SpectButton( cText, xOffset, iButtonY, flButtonSizeX, flButtonSizeY );
			}
			else
			{
				// normal button
				pButton = new CommandButton( iPlayerClass, cText, xOffset, iButtonY, flButtonSizeX, flButtonSizeY, flatDesign );
			}

			// add the button into the command menu
			if ( pButton )
			{
				m_pCurrentCommandMenu->AddButton( pButton );
				pButton->setBoundKey( cBoundKey[0] );
				pButton->setParentMenu( m_pCurrentCommandMenu );

				// Override font in CommandMenu
				pButton->setFont( Scheme::sf_primary3 );
			}

			// Find out if it's a submenu or a button we're dealing with
			if ( cCommand[0] == '{' )
			{
				if ( m_iNumMenus >= MAX_MENUS )
				{
					gEngfuncs.Con_Printf( "Too many menus in %s past '%s'\n",menuFile, szLastButtonText );
				}
				else
				{
					// Create the menu
						m_pCommandMenus[m_iNumMenus] = CreateSubMenu(pButton, m_pCurrentCommandMenu, iButtonY );
					m_pCurrentCommandMenu = m_pCommandMenus[m_iNumMenus];
					m_iNumMenus++;
				}
			}
			else if ( !iCustom )
			{
				// Create the button and attach it to the current menu
				if ( iToggle )
					pButton->addActionSignal(new CMenuHandler_ToggleCvar(cCommand));
				else
				pButton->addActionSignal(new CMenuHandler_StringCommand(cCommand));
				// Create an input signal that'll popup the current menu
				pButton->addInputSignal( new CMenuHandler_PopupSubMenuInput(pButton, m_pCurrentCommandMenu) );
			}

			// Get the next token
			if ( bGetExtraToken )
			{
				pfile = gEngfuncs.COM_ParseFile(pfile, token);
			}
		}

		// Move back up a menu
		m_pCurrentCommandMenu = m_pCurrentCommandMenu->GetParentMenu();

		pfile = gEngfuncs.COM_ParseFile(pfile, token);
	}
}
catch( CException *e )
{
	e;
	//e->Delete();
	e = NULL;
	m_iInitialized = false;
	return newIndex;
}

	SetCurrentMenu( NULL );
	SetCurrentCommandMenu( NULL );
	gEngfuncs.COM_FreeFile( pfile );

	m_iInitialized = true;
	return newIndex;
}

//-----------------------------------------------------------------------------
// Purpose: Creates all the class choices under a spy's disguise menus, and
//			maps a command to them
// Output : CCommandMenu
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pButtonText - 
//			*pButtonName - 
// Output : CommandButton
//-----------------------------------------------------------------------------
CommandButton *TeamFortressViewport::CreateCustomButton( char *pButtonText, char *pButtonName, int iYOffset )
{
	CommandButton *pButton = NULL;
	CCommandMenu  *pMenu = NULL;

	// ChangeTeam
	if ( !strcmp( pButtonName, "!CHANGETEAM" ) )
	{
		// ChangeTeam Submenu
		pButton = new CommandButton(pButtonText, 0, BUTTON_SIZE_Y * 2, CMENU_SIZE_X, BUTTON_SIZE_Y);

		// Create the submenu
		pMenu = CreateSubMenu(pButton, m_pCurrentCommandMenu, iYOffset );
		m_pCommandMenus[m_iNumMenus] = pMenu;
		m_iNumMenus++;

		// ChangeTeam buttons
		for (int i = 0; i < NUM_TEAMS; i++)
		{
			char sz[256]; 
			sprintf(sz, "jointeam %d", i+1);
			m_pTeamButtons[i] = new CommandButton(szTeamNames[i], 0, BUTTON_SIZE_Y, CMENU_SIZE_X, BUTTON_SIZE_Y);
			m_pTeamButtons[i]->addActionSignal(new CMenuHandler_StringCommandWatch( sz ));
			pMenu->AddButton( m_pTeamButtons[i] ); 
		}

		// Auto Assign button
		m_pTeamButtons[4] = new TeamButton(5, gHUD.m_TextMessage.BufferedLocaliseTextString( "#Team_AutoAssign" ), 0, BUTTON_SIZE_Y, CMENU_SIZE_X, BUTTON_SIZE_Y);
		m_pTeamButtons[4]->addActionSignal(new CMenuHandler_StringCommand( "jointeam 5" ));
		pMenu->AddButton( m_pTeamButtons[4] ); 
	}
	// ChangeClass
	else if ( !strcmp( pButtonName, "!CHANGECLASS" ) )
	{
		// Create the Change class menu
		pButton = new ClassButton(-1, pButtonText, 0, BUTTON_SIZE_Y, CMENU_SIZE_X, BUTTON_SIZE_Y, false);

		// ChangeClass Submenu
		pMenu = CreateSubMenu(pButton, m_pCurrentCommandMenu, iYOffset );
		m_pCommandMenus[m_iNumMenus] = pMenu;
		m_iNumMenus++;

		for (int i = 1; i <= NUM_CLASSES; i++ )
		{
			char sz[256]; 

			// ChangeClass buttons
			CHudTextMessage::LocaliseTextString( (const char*)szClassNames[g_iTeamNumber][i], sz, 256 );
			CommandButton *pClassButton = new CommandButton(  sz, 0, BUTTON_SIZE_Y, CMENU_SIZE_X, BUTTON_SIZE_Y, false);

			sprintf(sz, "joinclass %i", i);
			pClassButton->addActionSignal(new CMenuHandler_StringCommandWatch(sz));
			pMenu->AddButton( pClassButton );
		}
	}
	// Map Briefing
	else if ( !strcmp( pButtonName, "!MAPBRIEFING" ) )
	{
		pButton = new CommandButton(pButtonText, 0, BUTTON_SIZE_Y * m_pCurrentCommandMenu->GetNumButtons(), CMENU_SIZE_X, BUTTON_SIZE_Y);
		pButton->addActionSignal(new CMenuHandler_TextWindow(MENU_MAPBRIEFING));
		// Create an input signal that'll popup the current menu
		pButton->addInputSignal( new CMenuHandler_PopupSubMenuInput(pButton, m_pCurrentCommandMenu) );
	}
	// Class Descriptions
	else if ( !strcmp( pButtonName, "!CLASSDESC" ) )
	{
		pButton = new CommandButton(pButtonText, 0, BUTTON_SIZE_Y * m_pCurrentCommandMenu->GetNumButtons(), CMENU_SIZE_X, BUTTON_SIZE_Y, false);
		pButton->addActionSignal(new CMenuHandler_TextWindow(MENU_CLASSHELP));
		// Create an input signal that'll popup the current menu
		pButton->addInputSignal( new CMenuHandler_PopupSubMenuInput(pButton, m_pCurrentCommandMenu) );
	}
	else if ( !strcmp( pButtonName, "!SERVERINFO" ) )
	{
		pButton = new CommandButton(pButtonText, 0, BUTTON_SIZE_Y * m_pCurrentCommandMenu->GetNumButtons(), CMENU_SIZE_X, BUTTON_SIZE_Y, false);
		pButton->addActionSignal(new CMenuHandler_TextWindow(MENU_INTRO));
		// Create an input signal that'll popup the current menu
		pButton->addInputSignal( new CMenuHandler_PopupSubMenuInput(pButton, m_pCurrentCommandMenu) );
	}

	return pButton;
}

void TeamFortressViewport::ToggleServerBrowser()
{
	if (!m_iInitialized)
		return;

	if ( !m_pServerBrowser )
		return;

	if ( m_pServerBrowser->isVisible() )
	{
		m_pServerBrowser->setVisible( false );
	}
	else
	{
		m_pServerBrowser->setVisible( true );
	}

	UpdateCursorState();
}

//=======================================================================
void TeamFortressViewport::ShowCommandMenu(int menuIndex)
{
	if (!m_iInitialized)
		return;

	//Already have a menu open.
	if ( m_pCurrentMenu )  
		return;

	// is the command menu open?
	if ( m_pCurrentCommandMenu == m_pCommandMenus[menuIndex] )
	{
		HideCommandMenu();
		return;
	}

	// Not visible while in intermission
	if ( gHUD.m_iIntermission )
		return;

	// Recalculate visible menus
	UpdateCommandMenu( menuIndex );
	HideVGUIMenu();

	SetCurrentCommandMenu( m_pCommandMenus[menuIndex] );
	m_flMenuOpenTime = gHUD.m_flTime;
	UpdateCursorState();

	// get command menu parameters
	for ( int i = 2; i < gEngfuncs.Cmd_Argc(); i++ )
	{
		const char *param = gEngfuncs.Cmd_Argv( i - 1 );
		if ( param )
		{
			if ( m_pCurrentCommandMenu->KeyInput(param[0]) )
			{
				// kill the menu open time, since the key input is final
				HideCommandMenu();
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Handles the key input of "-commandmenu"
// Input  : 
//-----------------------------------------------------------------------------
void TeamFortressViewport::InputSignalHideCommandMenu()
{
	if (!m_iInitialized)
		return;

	// if they've just tapped the command menu key, leave it open
	if ( (m_flMenuOpenTime + 0.3) > gHUD.m_flTime )
		return;

	HideCommandMenu();
}

//-----------------------------------------------------------------------------
// Purpose: Hides the command menu
//-----------------------------------------------------------------------------
void TeamFortressViewport::HideCommandMenu()
{
	if (!m_iInitialized)
		return;

	if ( m_pCommandMenus[m_StandardMenu] )
	{
		m_pCommandMenus[m_StandardMenu]->ClearButtonsOfArmedState();
	}

	if ( m_pCommandMenus[m_SpectatorOptionsMenu] )
	{
		m_pCommandMenus[m_SpectatorOptionsMenu]->ClearButtonsOfArmedState();
	}

	if ( m_pCommandMenus[m_SpectatorCameraMenu] )
	{
		m_pCommandMenus[m_SpectatorCameraMenu]->ClearButtonsOfArmedState();
	}

	m_flMenuOpenTime = 0.0f;
	SetCurrentCommandMenu( NULL );
	UpdateCursorState();
}

//-----------------------------------------------------------------------------
// Purpose: Bring up the scoreboard
//-----------------------------------------------------------------------------
void TeamFortressViewport::ShowScoreBoard( void )
{
	if (m_pScoreBoard)
	{
		// No Scoreboard in single-player
		if ( gEngfuncs.GetMaxClients() > 1 )
		{
			m_pScoreBoard->Open();
			UpdateCursorState();
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Returns true if the scoreboard is up
//-----------------------------------------------------------------------------
bool TeamFortressViewport::IsScoreBoardVisible( void )
{
	if (m_pScoreBoard)
		return m_pScoreBoard->isVisible();

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: Hide the scoreboard
//-----------------------------------------------------------------------------
void TeamFortressViewport::HideScoreBoard( void )
{
	// Prevent removal of scoreboard during intermission
	if ( gHUD.m_iIntermission )
		return;

	if (m_pScoreBoard)
	{
		m_pScoreBoard->setVisible(false);

		GetClientVoiceMgr()->StopSquelchMode();

		UpdateCursorState();
	}
}

//-----------------------------------------------------------------------------
// Purpose: Activate's the player special ability
//			called when the player hits their "special" key
//-----------------------------------------------------------------------------
void TeamFortressViewport::InputPlayerSpecial( void )
{
	if (!m_iInitialized)
		return;

	// if it's any other class, just send the command down to the server
//	ClientCmd( "_special" );
}

// Set the submenu of the Command Menu
void TeamFortressViewport::SetCurrentCommandMenu( CCommandMenu *pNewMenu )
{
	for (int i = 0; i < m_iNumMenus; i++)
		m_pCommandMenus[i]->setVisible(false);

	m_pCurrentCommandMenu = pNewMenu;

	if (m_pCurrentCommandMenu)
		m_pCurrentCommandMenu->MakeVisible( NULL );
}

void TeamFortressViewport::UpdateCommandMenu(int menuIndex)
{
	m_pCommandMenus[menuIndex]->RecalculateVisibles( 0, false );
	m_pCommandMenus[menuIndex]->RecalculatePositions( 0 );
}

void COM_FileBase ( const char *in, char *out);

void TeamFortressViewport::UpdateSpectatorPanel()
{
	m_iUser1 = g_iUser1;
	m_iUser2 = g_iUser2;
	m_iUser3 = g_iUser3;
		
	if (!m_pSpectatorPanel)
		return;

	if ( g_iUser1 && gHUD.m_pCvarDraw->value && !gHUD.m_iIntermission)	// don't draw in dev_overview mode
	{
		char bottomText[128];
		char helpString2[128];
		char tempString[128];
		char * name;
		int player = 0;

		// check if spectator combinations are still valid
		gHUD.m_Spectator.CheckSettings();

		if ( !m_pSpectatorPanel->isVisible() )
		{
			m_pSpectatorPanel->setVisible( true );	// show spectator panel, but
			m_pSpectatorPanel->ShowMenu( false );	// dsiable all menus/buttons
			
			_snprintf( tempString, sizeof( tempString ) - 1, "%c%s", HUD_PRINTCENTER, CHudTextMessage::BufferedLocaliseTextString( "#Spec_Duck" ) );
			tempString[ sizeof( tempString ) - 1 ] = '\0';

			gHUD.m_TextMessage.MsgFunc_TextMsg( NULL, strlen( tempString ) + 1, tempString );
		}
		
		sprintf(bottomText,"#Spec_Mode%d", g_iUser1 );
		sprintf(helpString2,"#Spec_Mode%d", g_iUser1 );

		if ( gEngfuncs.IsSpectateOnly() )
			strcat(helpString2, " - HLTV");

		// check if we're locked onto a target, show the player's name
		if ( (g_iUser2 > 0) && (g_iUser2 <= gEngfuncs.GetMaxClients()) && (g_iUser1 != OBS_ROAMING) )
		{
			player = g_iUser2;
		}

		// special case in free map and inset off, don't show names
		if ( (g_iUser1 == OBS_MAP_FREE) && !gHUD.m_Spectator.m_pip->value )
			name = NULL;
		else
			name = g_PlayerInfoList[player].name;

		// create player & health string
		if ( player && name )
		{
			strcpy( bottomText, name );

			sprintf( bottomText, "%s (Health: %1.f)", bottomText, ((g_flHealth / MAX_HEALTH) * 100));
		}

		// in first person mode colorize player names
		if ( (g_iUser1 == OBS_IN_EYE) && player )
		{
			float * color = GetClientColor( player );
			int r = color[0]*255;
			int g = color[1]*255;
			int b = color[2]*255;
			
			// set team color, a bit transparent
			m_pSpectatorPanel->m_BottomMainLabel->setFgColor(r,g,b,0);
		}
		else
		{	// restore GUI color
			m_pSpectatorPanel->m_BottomMainLabel->setFgColor( 143, 143, 54, 0 );
		}

		// add sting auto if we are in auto directed mode
		if ( gHUD.m_Spectator.m_autoDirector->value )
		{
			char tempString[128];
			sprintf(tempString, "#Spec_Auto %s", helpString2);
			strcpy( helpString2, tempString );
		}

		m_pSpectatorPanel->m_BottomMainLabel->setText( CHudTextMessage::BufferedLocaliseTextString( bottomText ) );

		
		// update extra info field
		char szText[64];

		if ( gEngfuncs.IsSpectateOnly() )
		{
			// in HLTV mode show number of spectators
			_snprintf( szText, 63, "%s: %d", CHudTextMessage::BufferedLocaliseTextString( "#Spectators" ), gHUD.m_Spectator.m_iSpectatorNumber );
		}
		else
		{
			// otherwise show map name
			char szMapName[64];
			COM_FileBase( gEngfuncs.pfnGetLevelName(), szMapName );

			_snprintf ( szText, 63, "%s: %s",CHudTextMessage::BufferedLocaliseTextString( "#Spec_Map" ), szMapName );
		}

		szText[63] = 0;

		m_pSpectatorPanel->m_ExtraInfo->setText ( szText );

		int timer = (int)(gHUD.m_Clock.m_iClockTime - ( gHUD.m_flTime - gHUD.m_Clock.m_iLastClockUpdate));

		if ( timer < 0 )
			 timer	= 0;

		_snprintf ( szText, 63, "%d:%02d\n", (timer / 60), (timer % 60) );
		
		szText[63] = 0;
				
		m_pSpectatorPanel->m_CurrentTime->setText( szText ); 

		// update spectator panel
		gViewPort->m_pSpectatorPanel->Update();
	}
	else
	{
		if ( m_pSpectatorPanel->isVisible() )
		{
			m_pSpectatorPanel->setVisible( false );
			m_pSpectatorPanel->ShowMenu( false );	// dsiable all menus/buttons
		}
	}

	m_flSpectatorPanelLastUpdated = gHUD.m_flTime + 1.0; // update every seconds
}

//======================================================================
void TeamFortressViewport::CreateScoreBoard( void )
{
	int xdent = SBOARD_INDENT_X, ydent = SBOARD_INDENT_Y;
	if (ScreenWidth == 512)
	{
		xdent = SBOARD_INDENT_X_512; 
		ydent = SBOARD_INDENT_Y_512;
	}
	else if (ScreenWidth == 400)
	{
		xdent = SBOARD_INDENT_X_400; 
		ydent = SBOARD_INDENT_Y_400;
	}

	m_pScoreBoard = new ScorePanel(xdent, ydent, ScreenWidth - (xdent * 2), ScreenHeight - (ydent * 2));
	m_pScoreBoard->setParent(this);
	m_pScoreBoard->setVisible(false);
}

void TeamFortressViewport::CreateServerBrowser( void )
{
	m_pServerBrowser = new ServerBrowser( 0, 0, ScreenWidth, ScreenHeight );
	m_pServerBrowser->setParent(this);
	m_pServerBrowser->setVisible(false);
}


//======================================================================
// Set the VGUI Menu
void TeamFortressViewport::SetCurrentMenu( CMenuPanel *pMenu )
{
	m_pCurrentMenu = pMenu;
	if ( m_pCurrentMenu )
	{
		// Don't open menus in demo playback
		if ( gEngfuncs.pDemoAPI->IsPlayingback() )
			return;

		m_pCurrentMenu->Open();
	}
}

//================================================================
// Text Window
CMenuPanel* TeamFortressViewport::CreateTextWindow( int iTextToShow )
{
	char sz[256];
	char *cText = NULL;
	char *pfile = NULL;
	static const int MAX_TITLE_LENGTH = 32;
	char cTitle[MAX_TITLE_LENGTH];

	if ( iTextToShow == SHOW_MOTD )
	{
		if (!m_szServerName || !m_szServerName[0])
		{
			strcpy( cTitle, "Battle Grounds" );
		}
		//however if there is a name of sorts
		else
		{
			//Thanks go to Kukyona for this line of code
			//copie the server name & a small bit of text in together
			_snprintf( cTitle, MAX_TITLE_LENGTH, "BG: %s\0", m_szServerName );
		}
		cTitle[MAX_TITLE_LENGTH-1] = 0;
		cText = m_szMOTD;
	}
	else if ( iTextToShow == SHOW_MAPBRIEFING )
	{
		const char *level = gEngfuncs.pfnGetLevelName();
		if (!level)
			return NULL;

		strcpy( sz, level );
		char *ch = strchr( sz, '.' );
		*ch = '\0';
		strcat( sz, ".txt" );

		// pull out the map name
		strcpy( m_sMapName, level );
		ch = strchr( m_sMapName, '.' );
		if ( ch )
		{
			*ch = 0;
		}

		ch = strchr( m_sMapName, '/' );
		if ( ch )
		{
			// move the string back over the '/'
			memmove( m_sMapName, ch+1, strlen(ch)+1 );
		}

		pfile = (char*)gEngfuncs.COM_LoadFile( sz, 5, NULL );

		if (!pfile)
			return NULL;

		cText = pfile;

		strncpy( cTitle, m_sMapName, MAX_TITLE_LENGTH );
		cTitle[MAX_TITLE_LENGTH-1] = 0;
	}
	else if ( iTextToShow == SHOW_CLASSDESC )
	{
		char temp_text[64];
		sprintf(temp_text, "#Help");

		char temp_title[32];
		sprintf(temp_title, "#Title_%s", szClassCodes[g_iTeamNumber][g_iPlayerClass]);

		CHudTextMessage::LocaliseTextString(temp_text, cTitle, MAX_TITLE_LENGTH);
		CHudTextMessage::LocaliseTextString(temp_title, temp_text, MAX_TITLE_LENGTH);

		sprintf(sz, "classes/long_%s.txt", szClassCodes[g_iTeamNumber][g_iPlayerClass]);

		char *pfile = (char*)gEngfuncs.COM_LoadFile( sz, 5, NULL );
		if (pfile)
		{
			cText = pfile;
		}
		else
		{
			sprintf(sz, "classes/%s.txt", szClassCodes[g_iTeamNumber][g_iPlayerClass]);
			char *pfile = (char*)gEngfuncs.COM_LoadFile( sz, 5, NULL );
			cText = pfile;
		}
	}
	else if ( iTextToShow == SHOW_SPECHELP )
	{
		CHudTextMessage::LocaliseTextString( "#Spec_Help_Title", cTitle, MAX_TITLE_LENGTH );
		cTitle[MAX_TITLE_LENGTH-1] = 0;
		
		char *pfile = CHudTextMessage::BufferedLocaliseTextString( "#Spec_Help_Text" );
		if ( pfile )
		{
			cText = pfile;
		}
	}

	// if we're in the game (ie. have selected a class), flag the menu to be only grayed in the dialog box, instead of full screen
	CMenuPanel *pMOTDPanel = CMessageWindowPanel_Create( cText, cTitle, g_iPlayerClass == UNDEFINED, false, 0, 0, ScreenWidth, ScreenHeight );
	pMOTDPanel->setParent( this );

	if ( pfile )
		gEngfuncs.COM_FreeFile( pfile );

	return pMOTDPanel;
}

//================================================================
// VGUI Menus
void TeamFortressViewport::ShowVGUIMenu( int iMenu )
{
	CMenuPanel *pNewMenu = NULL;

	// Don't open menus in demo playback
	if ( gEngfuncs.pDemoAPI->IsPlayingback() )
		return;

	// Don't open any menus except the MOTD during intermission
	// MOTD needs to be accepted because it's sent down to the client 
	// after map change, before intermission's turned off
	if ( gHUD.m_iIntermission && iMenu != MENU_INTRO )
		return;

	// Don't create one if it's already in the list
	if (m_pCurrentMenu)
	{
		CMenuPanel *pMenu = m_pCurrentMenu;
		while (pMenu != NULL)
		{
			if (pMenu->GetMenuID() == iMenu)
				return;
			pMenu = pMenu->GetNextMenu();
		}
	}

	switch ( iMenu )
	{
	case MENU_TEAM:		
		pNewMenu = ShowTeamMenu(); 
		break;

	// Map Briefing removed now that it appears in the team menu
	case MENU_MAPBRIEFING:
		pNewMenu = CreateTextWindow( SHOW_MAPBRIEFING );
		break;

	case MENU_INTRO:
		pNewMenu = CreateTextWindow( SHOW_MOTD );
		break;

	case MENU_CLASSHELP:
		pNewMenu = CreateTextWindow( SHOW_CLASSDESC );
		break;

	case MENU_SPECHELP:
		pNewMenu = CreateTextWindow( SHOW_SPECHELP );
		break;

	case MENU_BRITISH_CLASS:
		pNewMenu = ShowBritClassMenu();
		break;

	//Ben - Shows our second class menu
	case MENU_AMERICAN_CLASS:
		pNewMenu = ShowAmerClassMenu();
		break;
	//Ben

	default:
		break;
	}

	if (!pNewMenu)
		return;

	// Close the Command Menu if it's open
	HideCommandMenu();

	pNewMenu->SetMenuID( iMenu );
	pNewMenu->SetActive( true );
	pNewMenu->setParent(this);

	// See if another menu is visible, and if so, cache this one for display once the other one's finished
	if (m_pCurrentMenu)
	{
		if ((m_pCurrentMenu->GetMenuID() == MENU_BRITISH_CLASS && iMenu == MENU_TEAM) || (m_pCurrentMenu->GetMenuID() == MENU_AMERICAN_CLASS && iMenu == MENU_TEAM) || m_pCurrentMenu == pNewMenu)
		{
			CMenuPanel *temp = m_pCurrentMenu;
			m_pCurrentMenu->Close();
			m_pCurrentMenu = pNewMenu;
			m_pCurrentMenu->SetNextMenu( temp );
			m_pCurrentMenu->Open();
			UpdateCursorState();
		}
		else
		{
			m_pCurrentMenu->SetNextMenu( pNewMenu );
		}
	}
	else
	{
		m_pCurrentMenu = pNewMenu;
		m_pCurrentMenu->Open();
		UpdateCursorState();
	}
}

// Removes all VGUI Menu's onscreen
void TeamFortressViewport::HideVGUIMenu()
{
	while (m_pCurrentMenu)
	{
		HideTopMenu();
	}
}

// Remove the top VGUI menu, and bring up the next one
void TeamFortressViewport::HideTopMenu()
{
	if (m_pCurrentMenu)
	{
		// Close the top one
		m_pCurrentMenu->Close();

		// Bring up the next one
		gViewPort->SetCurrentMenu( m_pCurrentMenu->GetNextMenu() );
	}

	UpdateCursorState();
}

// Return TRUE if the HUD's allowed to print text messages
bool TeamFortressViewport::AllowedToPrintText( void )
{
	// Prevent text messages when fullscreen menus are up
	if ( m_pCurrentMenu && g_iPlayerClass == 0 )
	{
		int iId = m_pCurrentMenu->GetMenuID();
		if ( iId == MENU_TEAM || iId == MENU_BRITISH_CLASS || iId == MENU_AMERICAN_CLASS || iId == MENU_INTRO || iId == MENU_CLASSHELP )
			return true;
	}

	return true;
}

//======================================================================================
// TEAM MENU
//======================================================================================
// Bring up the Team selection Menu
CMenuPanel* TeamFortressViewport::ShowTeamMenu()
{
	// Don't open menus in demo playback
	if ( gEngfuncs.pDemoAPI->IsPlayingback() )
		return NULL;

	m_pTeamMenu->Reset();
	return m_pTeamMenu;
}

void TeamFortressViewport::CreateTeamMenu()
{
	// Create the panel
	m_pTeamMenu = new CTeamMenuPanel(100, false, 0, 0, ScreenWidth, ScreenHeight);
	m_pTeamMenu->setParent( this );
	m_pTeamMenu->setVisible( false );
}

//======================================================================================
// CLASS MENU
//======================================================================================
// Bring up the Class selection Menu
CMenuPanel* TeamFortressViewport::ShowBritClassMenu()
{
	// Don't open menus in demo playback
	if ( gEngfuncs.pDemoAPI->IsPlayingback() )
		return NULL;

	m_pBritClassMenu->Reset();
	return m_pBritClassMenu;
}

void TeamFortressViewport::CreateBritClassMenu()
{
	// Create the panel
	m_pBritClassMenu = new CBritClassMenu(100, false, 0, 0, ScreenWidth, ScreenHeight);
	m_pBritClassMenu->setParent(this);
	m_pBritClassMenu->setVisible( false );
}

//Ben -Show & creates our american class menu
//======================================================================================
// CLASS MENU
//======================================================================================
// Bring up the Class selection Menu
CMenuPanel* TeamFortressViewport::ShowAmerClassMenu()
{
	// Don't open menus in demo playback
	if ( gEngfuncs.pDemoAPI->IsPlayingback() )
		return NULL;

//	m_pSpectatorMenu->setVisible(false);
	m_pAmerClassMenu->Reset();
	return m_pAmerClassMenu;
}

void TeamFortressViewport::CreateAmerClassMenu()
{
	// Create the panel
	m_pAmerClassMenu = new CAmerClassMenu(100, false, 0, 0, ScreenWidth, ScreenHeight);
	m_pAmerClassMenu->setParent(this);
	m_pAmerClassMenu->setVisible( false );
}
//======================================================================================
// SPECTATOR MENU
//======================================================================================
// Spectator "Menu" explaining the Spectator buttons
void TeamFortressViewport::CreateSpectatorMenu()
{
	// Create the Panel
	m_pSpectatorPanel = new SpectatorPanel(0, 0, ScreenWidth, ScreenHeight);
	m_pSpectatorPanel->setParent(this);
	m_pSpectatorPanel->setVisible(false);
	m_pSpectatorPanel->Initialize();
}

//======================================================================================
// UPDATE HUD SECTIONS
//======================================================================================
// We've got an update on player info
// Recalculate any menus that use it.
void TeamFortressViewport::UpdateOnPlayerInfo()
{
	if (m_pTeamMenu)
		m_pTeamMenu->Update();//Myristate
	if (m_pBritClassMenu)
		m_pBritClassMenu->Update();
	if (m_pAmerClassMenu)
		m_pAmerClassMenu->Update();
	if (m_pScoreBoard)
		m_pScoreBoard->Update();
}

void TeamFortressViewport::UpdateCursorState()
{
	// Need cursor if any VGUI window is up
	if ( m_pSpectatorPanel->m_menuVisible || m_pCurrentMenu || m_pTeamMenu->isVisible() || m_pServerBrowser->isVisible() || GetClientVoiceMgr()->IsInSquelchMode() )
	{
		g_iVisibleMouse = true;
		App::getInstance()->setCursorOveride( App::getInstance()->getScheme()->getCursor(Scheme::scu_arrow) );
		return;
	}
	else if ( m_pCurrentCommandMenu )
	{
		// commandmenu doesn't have cursor if hud_capturemouse is turned off
		if ( gHUD.m_pCvarStealMouse->value != 0.0f )
		{
			g_iVisibleMouse = true;
			App::getInstance()->setCursorOveride( App::getInstance()->getScheme()->getCursor(Scheme::scu_arrow) );
			return;
		}
	}

	// Don't reset mouse in demo playback
	if ( !gEngfuncs.pDemoAPI->IsPlayingback() )
	{
		IN_ResetMouse();
	}

	g_iVisibleMouse = false;
	App::getInstance()->setCursorOveride( App::getInstance()->getScheme()->getCursor(Scheme::scu_none) );
}

void TeamFortressViewport::UpdateHighlights()
{
	if (m_pCurrentCommandMenu)
		m_pCurrentCommandMenu->MakeVisible( NULL );
}

void TeamFortressViewport::GetAllPlayersInfo( void )
{
	for ( int i = 1; i < MAX_PLAYERS; i++ )
	{
		GetPlayerInfo( i, &g_PlayerInfoList[i] );

		if ( g_PlayerInfoList[i].thisplayer )
			m_pScoreBoard->m_iPlayerNum = i;  // !!!HACK: this should be initialized elsewhere... maybe gotten from the engine
	}
}

void TeamFortressViewport::paintBackground()
{
	if (m_pScoreBoard)
	{
		int x, y;
		getApp()->getCursorPos(x, y);
		m_pScoreBoard->cursorMoved(x, y, m_pScoreBoard);
	}

	// See if the command menu is visible and needs recalculating due to some external change
	if ( g_iTeamNumber != m_iCurrentTeamNumber )
	{
		UpdateCommandMenu( m_StandardMenu );

		if(m_pBritClassMenu )
		{
//			m_pSpectatorMenu->setVisible(false);
			m_pBritClassMenu->Update();
		}
		if(m_pAmerClassMenu)
		{
//			m_pSpectatorMenu->setVisible(false);
			m_pAmerClassMenu->Update();
		}

		m_iCurrentTeamNumber = g_iTeamNumber;
	}

	if ( g_iPlayerClass != m_iCurrentPlayerClass )
	{
		UpdateCommandMenu( m_StandardMenu );

		m_iCurrentPlayerClass = g_iPlayerClass;
	}

	// See if the Spectator Menu needs to be update
	if (	( g_iUser1 != m_iUser1 || g_iUser2 != m_iUser2 ) ||
			( m_flSpectatorPanelLastUpdated < gHUD.m_flTime ) )
	{
		UpdateSpectatorPanel();
	}

	// Update the Scoreboard, if it's visible
	if ( m_pScoreBoard->isVisible() && (m_flScoreBoardLastUpdated < gHUD.m_flTime) )
	{
		m_pScoreBoard->Update();
		m_flScoreBoardLastUpdated = gHUD.m_flTime + 0.5;
	}

	int extents[4];
	getAbsExtents(extents[0],extents[1],extents[2],extents[3]);
	VGui_ViewportPaintBackground(extents);
}

//================================================================
// Input Handler for Drag N Drop panels
void CDragNDropHandler::cursorMoved(int x,int y,Panel* panel)
{
	if(m_bDragging)
	{
		App::getInstance()->getCursorPos(x,y);			
		m_pPanel->setPos(m_iaDragOrgPos[0]+(x-m_iaDragStart[0]),m_iaDragOrgPos[1]+(y-m_iaDragStart[1]));
		
		if(m_pPanel->getParent()!=null)
		{			
			m_pPanel->getParent()->repaint();
		}
	}
}

void CDragNDropHandler::mousePressed(MouseCode code,Panel* panel)
{
	int x,y;
	App::getInstance()->getCursorPos(x,y);
	m_bDragging=true;
	m_iaDragStart[0]=x;
	m_iaDragStart[1]=y;
	m_pPanel->getPos(m_iaDragOrgPos[0],m_iaDragOrgPos[1]);
	App::getInstance()->setMouseCapture(panel);

	m_pPanel->setDragged(m_bDragging);
	m_pPanel->requestFocus();
} 

void CDragNDropHandler::mouseReleased(MouseCode code,Panel* panel)
{
	m_bDragging=false;
	m_pPanel->setDragged(m_bDragging);
	App::getInstance()->setMouseCapture(null);
}

//BP ParticleEmitter START
// reads a msg to control a particle emitter
int TeamFortressViewport::MsgFunc_Particles( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );

	particle_system_management pSystem;
	pSystem.iID = READ_SHORT();
	bool bTurnOff = (!!(READ_BYTE()));

	// turn off means deleting the system client side
	if(bTurnOff == true) {
		pParticleManager->RemoveSystem(pSystem.iID);
	} else {
		// read the rest of the msg in
		pSystem.vPosition.x = READ_COORD();
		pSystem.vPosition.y = READ_COORD();
		pSystem.vPosition.z = READ_COORD();
		pSystem.vDirection.x = READ_COORD();
		pSystem.vDirection.y = READ_COORD();
		pSystem.vDirection.z = READ_COORD();
		unsigned int iPresetSystem = READ_SHORT();

		// to reinit particle on runtime we hijack preset 9999
		if (iPresetSystem == 9999)
		{
			// BP - Clear Particle Systems	
			pParticleManager->RemoveParticles();
			pParticleManager->RemoveTextures();
			pParticleManager->RemoveSystems();
			return 1;
		}

		// no present, we just create according to the file
		if (iPresetSystem == 0) {
			char sDefinitionFile[256];
			_snprintf(sDefinitionFile, sizeof(sDefinitionFile) - 1, "%s\0", READ_STRING());

			// create system
			pParticleManager->CreateMappedPS(sDefinitionFile, &pSystem);
		} else {
			// create according to the presets
			pParticleManager->CreatePresetPS(iPresetSystem, &pSystem);
		}
	}

	return 1;
}
//BP ParticleEmitter END

//BP Grass Entity 
int TeamFortressViewport::MsgFunc_Grass( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );

	particle_system_management pSystem;
	pSystem.iID = READ_SHORT();
	bool bTurnOff = (!!(READ_BYTE()));

	// turn off means deleting the system client side
	if(bTurnOff == true) {
		pParticleManager->RemoveSystem(pSystem.iID);
	} else {
		// read the rest of the msg in
		pSystem.vAbsMax.x = READ_COORD();
		pSystem.vAbsMax.y = READ_COORD();
		pSystem.vAbsMax.z = READ_COORD();
		pSystem.vAbsMin.x = READ_COORD();
		pSystem.vAbsMin.y = READ_COORD();
		pSystem.vAbsMin.z = READ_COORD();

		char sDefinitionFile[256];
		_snprintf(sDefinitionFile, sizeof(sDefinitionFile) - 1, "%s\0", READ_STRING());

		pParticleManager->CreateGrassPS(sDefinitionFile, &pSystem);
	}
	return 1;
}


//================================================================
// Number Key Input
bool TeamFortressViewport::SlotInput( int iSlot )
{
	// If there's a menu up, give it the input
	if ( m_pCurrentMenu )
		return m_pCurrentMenu->SlotInput( iSlot );

	return FALSE;
}

// Direct Key Input
int	TeamFortressViewport::KeyInput( int down, int keynum, const char *pszCurrentBinding )
{
	// Enter gets out of Spectator Mode by bringing up the Team Menu
	if (m_iUser1 && gEngfuncs.Con_IsVisible() == false )
	{
//		if ( down && (keynum == K_ENTER || keynum == K_KP_ENTER) )
//			ShowVGUIMenu( MENU_TEAM );
	}

	// Open Text Window?
	if (m_pCurrentMenu && gEngfuncs.Con_IsVisible() == false)
	{
		int iMenuID = m_pCurrentMenu->GetMenuID();

		// Get number keys as Input for Team/Class menus
		if (iMenuID == MENU_TEAM || iMenuID == MENU_BRITISH_CLASS || iMenuID == MENU_AMERICAN_CLASS)
		{
			// Escape gets you out of Team/Class menus if the Cancel button is visible
			if ( keynum == K_ESCAPE )
			{
				if ( (iMenuID == MENU_TEAM && g_iTeamNumber) || (iMenuID == MENU_BRITISH_CLASS && g_iPlayerClass) || (iMenuID == MENU_AMERICAN_CLASS && g_iPlayerClass))
				{
					HideTopMenu();
					return 0;
				}
			}

			for (int i = '0'; i <= '9'; i++)
			{
				if ( down && (keynum == i) )
				{
					SlotInput( i - '0' );
					return 0;
				}
			}
		}

		// Grab enter keys to close TextWindows
		if ( down && (keynum == K_ENTER || keynum == K_KP_ENTER || keynum == K_SPACE || keynum == K_ESCAPE) )
		{
			if ( iMenuID == MENU_MAPBRIEFING || iMenuID == MENU_INTRO || iMenuID == MENU_CLASSHELP )
			{
				HideTopMenu();
				return 0;
			}
		}

		// Grab jump key on Team Menu as autoassign
		if ( pszCurrentBinding && down && !strcmp(pszCurrentBinding, "+jump") )
		{
			if (iMenuID == MENU_TEAM)
			{
				m_pTeamMenu->SlotInput(5);
				return 0;
			}
		}

	}

	// if we're in a command menu, try hit one of it's buttons
	if ( down && m_pCurrentCommandMenu )
	{
		// Escape hides the command menu
		if ( keynum == K_ESCAPE )
		{
			HideCommandMenu();
			return 0;
		}

		// only trap the number keys
		if ( keynum >= '0' && keynum <= '9' )
		{
			if ( m_pCurrentCommandMenu->KeyInput(keynum) )
			{
				// a final command has been issued, so close the command menu
				HideCommandMenu();
			}

			return 0;
		}
	}

	return 1;
}

//================================================================
// Message Handlers
int TeamFortressViewport::MsgFunc_ValClass(const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );
	
	for (int i = 0; i < 5; i++)
		m_iValidClasses[i] = READ_SHORT();

	// Force the menu to update
	UpdateCommandMenu( m_StandardMenu );

	return 1;
}

int TeamFortressViewport::MsgFunc_TeamNames(const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );
	
	m_iNumberOfTeams = READ_BYTE();

	for (int i = 0; i < NUM_TEAMS; i++)
	{
		int teamNum = i + 1;

		gHUD.m_TextMessage.LocaliseTextString( READ_STRING(), m_sTeamNames[teamNum], MAX_TEAMNAME_SIZE );

		// Set the team name buttons
		if (m_pTeamButtons[i])
			m_pTeamButtons[i]->setText( m_sTeamNames[teamNum] );

	}

	// Update the Team Menu
	//if (m_pTeamMenu)//Myristate
	//	m_pTeamMenu->Update();

	return 1;
}

int TeamFortressViewport::MsgFunc_VGUIMenu(const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );

	int iMenu = READ_BYTE();

	// Map briefing includes the name of the map (because it's sent down before the client knows what map it is)
	if (iMenu == MENU_MAPBRIEFING)
	{
		_snprintf( m_sMapName, sizeof(m_sMapName)-1, "%s\0", READ_STRING());
	}

	// Bring up the menu6
	ShowVGUIMenu( iMenu );

	return 1;
}

int TeamFortressViewport::MsgFunc_MOTD( const char *pszName, int iSize, void *pbuf )
{
	if (m_iGotAllMOTD)
		m_szMOTD[0] = 0;

	BEGIN_READ( pbuf, iSize );

	m_iGotAllMOTD = READ_BYTE();

	int roomInArray = sizeof(m_szMOTD) - strlen(m_szMOTD) - 1;

	strncat( m_szMOTD, gHUD.HandleServerString(READ_STRING()), roomInArray >= 0 ? roomInArray : 0 );
	m_szMOTD[ sizeof(m_szMOTD)-1 ] = '\0';

	// don't show MOTD for HLTV spectators
	if ( m_iGotAllMOTD && !gEngfuncs.IsSpectateOnly() )
	{
		ShowVGUIMenu( MENU_INTRO );
	}

	return 1;
}


int TeamFortressViewport::MsgFunc_ServerName( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );

	_snprintf( m_szServerName, MAX_SERVERNAME_LENGTH - 1, "%s\0", READ_STRING());

	return 1;
}

int TeamFortressViewport::MsgFunc_ScoreInfo( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );
	short cl = READ_BYTE();
	signed long int damage_done = READ_LONG();
	signed long int capture_points = READ_LONG();
	short deaths = READ_SHORT();
	short playerclass = READ_SHORT();
	short teamnumber = READ_SHORT();

	if ( cl > 0 && cl <= MAX_PLAYERS )
	{
		g_PlayerExtraInfo[cl].damage_done = damage_done;
		g_PlayerExtraInfo[cl].capture_points = capture_points;
		g_PlayerExtraInfo[cl].deaths = deaths;

		if(playerclass > UNDEFINED)
			g_PlayerExtraInfo[cl].playerclass = playerclass;
		if(teamnumber > UNDEFINED)
			g_PlayerExtraInfo[cl].teamnumber = teamnumber;

		UpdateOnPlayerInfo();
	}
	// rebuild the list of teams
	m_pScoreBoard->RebuildTeams();

	return 1;
}

// Message handler for TeamScore message
// accepts three values:
//		string: team name
//		short: teams kills
//		short: teams deaths 
// if this message is never received, then scores will simply be the combined totals of the players.
int TeamFortressViewport::MsgFunc_TeamScore( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );

	int iTeamNum = READ_SHORT();
	int i = iTeamNum;

	if ( i > NUM_TEAMS )
		return 1;

	// use this new score data instead of combined player scoresw
	g_TeamInfo[i].scores_overridden = true;
	g_TeamInfo[i].extra_points += READ_LONG();

	// rebuild the list of teams
	m_pScoreBoard->RebuildTeams();
	return 1;
}

// Message handler for TeamInfo message
// accepts two values:
//		byte: client number
//		string: client team name
int TeamFortressViewport::MsgFunc_TeamInfo( const char *pszName, int iSize, void *pbuf )
{
	if (!m_pScoreBoard)
		return 1;

	BEGIN_READ( pbuf, iSize );
	short cl = READ_BYTE();
	
	if ( cl > 0 && cl <= MAX_PLAYERS )
	{  
		// set the players team
		_snprintf( g_PlayerExtraInfo[cl].teamname, NAME_LENGTH - 1, "%s\0", READ_STRING());
	}

	// rebuild the list of teams
	m_pScoreBoard->RebuildTeams();

	return 1;
}

void TeamFortressViewport::DeathMsg( int killer, int victim )
{
	m_pScoreBoard->DeathMsg(killer,victim);
}

int TeamFortressViewport::MsgFunc_Spectator( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );

	short cl = READ_BYTE();
	if ( cl > 0 && cl <= MAX_PLAYERS )
	{
		g_IsSpectator[cl] = READ_BYTE();
	}

	return 1;
}

int TeamFortressViewport::MsgFunc_AllowSpec( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );

	m_iAllowSpectators = READ_BYTE();

	// Force the menu to update
	UpdateCommandMenu( m_StandardMenu );

	return 1;
}

//resets the team score
int TeamFortressViewport::MsgFunc_ResetScores( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );

	//this doesn't particaully matter tbh
	READ_BYTE();

	GetAllPlayersInfo();

	int i = 0;

	for(; i < MAX_PLAYERS; i++ )
	{
		if ( g_PlayerInfoList[i].name == NULL )
			continue; 

		extra_player_info_t *pl_info = &g_PlayerExtraInfo[i];
		pl_info->damage_done = 0.0;
		pl_info->capture_points = 0.0;
		pl_info->deaths = 0.0;
	}

	for(i = 0; i <= NUM_TEAMS; i++)
	{
		if(&g_TeamInfo[i] != NULL)
		{
			g_TeamInfo[i].deaths = 0.0;
			g_TeamInfo[i].damage_done = 0.0;
			g_TeamInfo[i].capture_points = 0.0;
			g_TeamInfo[i].extra_damage = 0.0;
			g_TeamInfo[i].extra_deaths = 0.0;
			g_TeamInfo[i].extra_points = 0.0;
		}
	}

	m_pScoreBoard->RebuildTeams();

	return 1;
}

int TeamFortressViewport::MsgFunc_HintBox( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );
	int iId = READ_SHORT(); 

	if ((iId == HINTBOX_CLASS_LIGHTA))
	{
		int iTeam = READ_SHORT(); 
		int iClass = READ_SHORT();
		if(iTeam == AMERS)
		{
			switch (iClass)
			{
			case 1:
				gHintBox.Set(HINTBOX_CLASS_LIGHTA);
				break;
			case 2:
				gHintBox.Set(HINTBOX_CLASS_MEDA);
				break;
			case 3:
				gHintBox.Set(HINTBOX_CLASS_HEAVYA);
				break;
			default:
				break;
			}
		}
		else if(iTeam == BRITS)
		{
			switch (iClass)
			{
			case LIGHT:
				gHintBox.Set(HINTBOX_CLASS_LIGHTB);
				break;
			case 2:
				gHintBox.Set(HINTBOX_CLASS_MEDB);
				break;
			case 3:
				gHintBox.Set(HINTBOX_CLASS_HEAVYB);
				break;
			default:
				break;
			}
		}
	}
	else
		gHintBox.Set(iId);

    return 1;
}

int TeamFortressViewport::MsgFunc_FullScore( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );

	long iBritishScore = READ_LONG();
	long iAmericanScore = READ_LONG();

	g_TeamInfo[BRITS].capture_points = iBritishScore;
	g_TeamInfo[AMERS].capture_points = iAmericanScore;

	// rebuild the list of teams
	m_pScoreBoard->RebuildTeams();
	return 1;
}

//BP Commandermenu
/*
int TeamFortressViewport::MsgFunc_ResComPoint( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );
	int iSpecialId = READ_SHORT();
	
	if ((iSpecialId > 0) && ( iSpecialId < 100 ))
		gHUD.m_CommanderMenu.PostExecute(iSpecialId);

	if ((iSpecialId > 100) && ( iSpecialId < 200 ))
		gHUD.m_CommanderMenu.Fill(iSpecialId);

	return 1;
}
*/
