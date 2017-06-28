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


#ifndef VGUI_BORDER_H
#define VGUI_BORDER_H

#include<VGUI.h>
#include<VGUI_Image.h>

//TODO: all borders should be titled

namespace vgui
{

class Panel;

class VGUIAPI Border : public Image
{
public:
	Border();
	Border(int left,int top,int right,int bottom);
public:
	virtual void setInset(int left,int top,int right,int bottom);
	virtual void getInset(int& left,int& top,int& right,int& bottom);
protected:
	virtual void drawFilledRect(int x0,int y0,int x1,int y1);
	virtual void drawOutlinedRect(int x0,int y0,int x1,int y1);
	virtual void drawSetTextPos(int x,int y);
	virtual void drawPrintText(int x,int y,const char* str,int strlen);
	virtual void drawPrintChar(int x,int y,char ch);
protected:
	int _inset[4];
private:
	Panel* _panel;
friend class Panel;
friend class BorderPair;
};

}

#endif