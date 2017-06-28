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

//This file was created on the 27/4/03 at 17:32 by Ben Banfield for the Battle Grounds modification for Half-Life.
//Its purpose is to define the class which handles text messages to the client

#include "STDIO.H"
#include "STDLIB.H"

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include <string.h>

#include "vgui_int.h"
#include "vgui_TeamFortressViewport.h"
#include "vgui_ServerBrowser.h"

DECLARE_MESSAGE(m_ColourMessage, Colour)

int CHudColourMessage::Init(void)
{
	HOOK_MESSAGE(Colour);

	gHUD.AddHudElem(this);

	m_iFlags |= HUD_ACTIVE;

	for(int i = 0; i < MAX_MESSAGES; i++)
	{
		if(m_pMessages[i]) {
			delete m_pMessages[i];
		}
		m_pMessages[i] = NULL;
	}

	return 1;
}

int CHudColourMessage::VidInit()
{
	for(int i = 0; i < MAX_MESSAGES; i++)
	{
		if(m_pMessages[i]) {
			delete m_pMessages[i];
		}
		m_pMessages[i] = NULL;
	}
	return 1;
}

int CHudColourMessage::MsgFunc_Colour(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);

	int x = READ_SHORT();
	int y = READ_SHORT();

	int r = READ_SHORT();
	int g = READ_SHORT();
	int b = READ_SHORT();

	char *text = READ_STRING(255);

	float flLife = (float)READ_SHORT();

	AddMessage(r, g ,b ,x, y, text, flLife);
	m_iFlags |= HUD_ACTIVE;

	return 1;
}

void CHudColourMessage::AddMessage(int r, int g, int b, int x, int y, char *text, float life)
{
	char szString[255];
	memset(szString, 0, sizeof(szString));
	_snprintf(szString, 254, "%s\0", text);
	
	char c = szString[0];

	char szFinalMessage[255];
	memset(szFinalMessage, 0, sizeof(szFinalMessage));

	int iWidth = 0;
	int n = 1;
	int i = 0;
	while(n < 255)
	{
		if(c == '%' || c == '\n' || c == 0)
		{
			c = szString[n++];
			continue;
		}
		strncpy(&szFinalMessage[i], &c, sizeof(c));
		i++;
		iWidth += gHUD.m_scrinfo.charWidths[c];
		c = szString[n++];
	}

	int xPos = 0;

	if ( x == -1 )
	{
		xPos = (ScreenWidth - iWidth) / 2;
	}
	else
	{
		if ( x < 0 )
			xPos = (1.0 + XRES(x)) * ScreenWidth - iWidth;	// Alight right
		else
			xPos = XRES(x) * ScreenWidth;
	}

	if ( xPos < 0 )
		xPos = 0;
	if ( xPos + iWidth > ScreenWidth )
		xPos = ScreenWidth - iWidth;

	if(y == -1)
		y = YRES(240);
	else
		y = YRES(y);

	for(i = 0; i < MAX_MESSAGES; i++)
	{
		if(!m_pMessages[i])
		{
			m_pMessages[i] = new message_s;
			m_pMessages[i]->x = xPos;
			m_pMessages[i]->y = y;

			m_pMessages[i]->r = r;
			m_pMessages[i]->g = g;
			m_pMessages[i]->b = b;

			_snprintf(m_pMessages[i]->szMessage, 254, "%s\0", szFinalMessage);

			m_pMessages[i]->flLifeSpan = life;
			m_pMessages[i]->flTimeAdded = 0.0;
			m_iFlags |= HUD_ACTIVE;
			break;
		}
	}
	m_iFlags |= HUD_ACTIVE;
}

int CHudColourMessage::Draw( float fltime )
{
	if ( gHUD.m_iHideHUDDisplay & (HIDEHUD_ALL) )
		return 1;

	// loop through all our messages drawing them
	for(int i = 0; i < MAX_MESSAGES; i++)
	{
		if(m_pMessages[i])
		{
			// add in the time we started drawing this message
			if(m_pMessages[i]->flTimeAdded == 0.0)
				m_pMessages[i]->flTimeAdded = fltime;

			// remove dead messages
			if(((m_pMessages[i]->flTimeAdded + m_pMessages[i]->flLifeSpan) <= fltime) || (m_pMessages[i]->flTimeAdded > fltime))
			{
				delete m_pMessages[i];
				m_pMessages[i] = NULL;
				continue;
			}

			// check no other messages are on the same line
			for(int j = 0; j < MAX_MESSAGES; j++)
			{
				if(m_pMessages[j] && (m_pMessages[j] != m_pMessages[i]))
				{
					if(fabs(m_pMessages[i]->y - m_pMessages[j]->y) < 5)
					{
						if(m_pMessages[i]->flTimeAdded > m_pMessages[j]->flTimeAdded && m_pMessages[j]->flTimeAdded != 0.0)
						{
							delete m_pMessages[j];
							m_pMessages[j] = NULL;
						}
						else
						{
							delete m_pMessages[i];
							m_pMessages[i] = NULL;
							break;
						}
					}
				}
			}

			if(m_pMessages[i])
			{
				gHUD.DrawHudString(m_pMessages[i]->x, m_pMessages[i]->y, XRES(640),
					m_pMessages[i]->szMessage, m_pMessages[i]->r, m_pMessages[i]->g, m_pMessages[i]->b);
			}
		}
	}
	return 1;
}