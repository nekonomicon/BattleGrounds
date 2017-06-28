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

#ifndef VGUI_DEFAULTINPUTSIGNAL_H
#define VGUI_DEFAULTINPUTSIGNAL_H
#ifdef _WIN32
#pragma once
#pragma warning (disable : 4100)
#endif


#include "vgui_inputsignal.h"


namespace vgui
{
	// This class derives from vgui::InputSignal and implements empty defaults for all of its functions.
	class CDefaultInputSignal : public vgui::InputSignal
	{
	public:
		virtual void cursorMoved(int x,int y,Panel* panel) {};
		virtual void cursorEntered(Panel* panel) {};
		virtual void cursorExited(Panel* panel) {};
		virtual void mousePressed(MouseCode code,Panel* panel) {};
		virtual void mouseDoublePressed(MouseCode code,Panel* panel) {};
		virtual void mouseReleased(MouseCode code,Panel* panel) {};
		virtual void mouseWheeled(int delta,Panel* panel) {};
		virtual void keyPressed(KeyCode code,Panel* panel) {};
		virtual void keyTyped(KeyCode code,Panel* panel) {};
		virtual void keyReleased(KeyCode code,Panel* panel) {};
		virtual void keyFocusTicked(Panel* panel)  {};
	};
}


#endif // VGUI_DEFAULTINPUTSIGNAL_H
