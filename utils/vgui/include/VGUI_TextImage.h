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


#ifndef VGUI_TEXTIMAGE_H
#define VGUI_TEXTIMAGE_H

#include<VGUI.h>
#include<VGUI_Image.h>
#include<VGUI_Scheme.h>


//TODO: need to add wrapping flag instead of being arbitrary about when wrapping and auto-resizing actually happens
//		This is probably why you are having problems if you had text in a constructor and then changed the font

namespace vgui
{

class Panel;
class Font;
class App;

class VGUIAPI TextImage : public Image
{
private:
	char*                     _text;
	int                       _textBufferLen;
	vgui::Scheme::SchemeFont  _schemeFont;
	vgui::Font*               _font;
	int                       _textColor[4];
	vgui::Scheme::SchemeColor _textSchemeColor;
public:	
	TextImage(int textBufferLen,const char* text);
	TextImage(const char* text);
private:
	virtual void  init(int textBufferLen,const char* text);
public:
	virtual void  getTextSize(int& wide,int& tall);
	virtual void  getTextSizeWrapped(int& wide,int& tall);
	virtual Font* getFont();
	virtual void  setText(int textBufferLen,const char* text);
	virtual void  setText(const char* text);
	virtual void  setFont(vgui::Scheme::SchemeFont schemeFont);
	virtual void  setFont(vgui::Font* font);
	virtual void  setSize(int wide,int tall);
protected:
	virtual void paint(Panel* panel);
};

}

#endif



