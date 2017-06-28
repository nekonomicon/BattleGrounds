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

#ifndef VGUI_SCROLLBAR2_H
#define VGUI_SCROLLBAR2_H
#ifdef _WIN32
#pragma once
#endif

#include<VGUI.h>
#include<VGUI_Panel.h>
#include<VGUI_Dar.h>

namespace vgui
{

class IntChangeSignal;
class Button;
class Slider2;

//-----------------------------------------------------------------------------
// Purpose: Hacked up version of the vgui scrollbar
//-----------------------------------------------------------------------------
class VGUIAPI ScrollBar2 : public Panel
{
public:
	ScrollBar2(int x,int y,int wide,int tall,bool vertical);
public:
	virtual void    setValue(int value);
	virtual int     getValue();
	virtual void    addIntChangeSignal(IntChangeSignal* s); 
	virtual void    setRange(int min,int max);
	virtual void    setRangeWindow(int rangeWindow);
	virtual void    setRangeWindowEnabled(bool state);
	virtual void    setSize(int wide,int tall);
	virtual bool    isVertical();
	virtual bool    hasFullRange();
	virtual void    setButton(Button *button,int index);
	virtual Button* getButton(int index);
	virtual void    setSlider(Slider2 *slider);
	virtual Slider2 *getSlider();
	virtual void 	doButtonPressed(int buttonIndex);
	virtual void    setButtonPressedScrollValue(int value);
	virtual void    validate();
public: //bullshit public 
	virtual void fireIntChangeSignal();
protected:
	virtual void performLayout();
protected:
	Button* _button[2];
	Slider2 *_slider;
	Dar<IntChangeSignal*> _intChangeSignalDar;
	int     _buttonPressedScrollValue;
};

}

#endif // VGUI_SCROLLBAR2_H
