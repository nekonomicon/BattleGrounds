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

//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#ifndef VGUI_HELPERS_H
#define VGUI_HELPERS_H
#ifdef _WIN32
#pragma once
#endif


#include "vgui_panel.h"
#include "vgui_label.h"


inline int PanelTop(vgui::Panel *pPanel)	{int x,y,w,h; pPanel->getBounds(x,y,w,h); return y;}
inline int PanelLeft(vgui::Panel *pPanel)	{int x,y,w,h; pPanel->getBounds(x,y,w,h); return x;}
inline int PanelRight(vgui::Panel *pPanel)	{int x,y,w,h; pPanel->getBounds(x,y,w,h); return x+w;}
inline int PanelBottom(vgui::Panel *pPanel)	{int x,y,w,h; pPanel->getBounds(x,y,w,h); return y+h;}
inline int PanelWidth(vgui::Panel *pPanel)	{int x,y,w,h; pPanel->getBounds(x,y,w,h); return w;}
inline int PanelHeight(vgui::Panel *pPanel)	{int x,y,w,h; pPanel->getBounds(x,y,w,h); return h;}

// Places child at the requested position inside pParent. iAlignment is from Label::Alignment.
void AlignPanel(vgui::Panel *pChild, vgui::Panel *pParent, int alignment);


#endif // VGUI_HELPERS_H

