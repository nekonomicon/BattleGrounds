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


#ifndef VGUI_IMAGE_H
#define VGUI_IMAGE_H

#include<VGUI.h>
#include<VGUI_Scheme.h>
#include<VGUI_Color.h>

//TODO:: needs concept of insets

namespace vgui
{

class Panel;

class VGUIAPI Image
{
friend class Panel;
private:
	int                 _pos[2];
	int                 _size[2];
	Panel*              _panel;
	Color               _color;
public:
	Image();
public:
	virtual void setPos(int x,int y);
	virtual void getPos(int& x,int& y);
	virtual void getSize(int& wide,int& tall);
	virtual void setColor(Color color);
	virtual void getColor(Color& color);
protected:
	virtual void setSize(int wide,int tall);
	virtual void drawSetColor(Scheme::SchemeColor sc);
	virtual void drawSetColor(int r,int g,int b,int a);
	virtual void drawFilledRect(int x0,int y0,int x1,int y1);
	virtual void drawOutlinedRect(int x0,int y0,int x1,int y1);
	virtual void drawSetTextFont(Scheme::SchemeFont sf);
	virtual void drawSetTextFont(Font* font);
	virtual void drawSetTextColor(Scheme::SchemeColor sc);
	virtual void drawSetTextColor(int r,int g,int b,int a);
	virtual void drawSetTextPos(int x,int y);
	virtual void drawPrintText(const char* str,int strlen);
	virtual void drawPrintText(int x,int y,const char* str,int strlen);
	virtual void drawPrintChar(char ch);
	virtual void drawPrintChar(int x,int y,char ch);
	virtual void drawSetTextureRGBA(int id,const char* rgba,int wide,int tall);
	virtual void drawSetTexture(int id);
	virtual void drawTexturedRect(int x0,int y0,int x1,int y1);
	virtual void paint(Panel* panel);
public:
	virtual void doPaint(Panel* panel);
};
}

#endif