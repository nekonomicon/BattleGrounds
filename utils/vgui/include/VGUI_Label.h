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


#ifndef VGUI_LABEL_H
#define VGUI_LABEL_H

#include<VGUI.h>
#include<VGUI_Panel.h>
#include<VGUI_Scheme.h>

//TODO: this should use a TextImage for the text

namespace vgui
{

class Panel;
class TextImage;

class VGUIAPI Label : public Panel
{
public:
	enum Alignment
	{
		a_northwest=0,
		a_north,
		a_northeast,
		a_west,
		a_center,
		a_east,
		a_southwest,
		a_south,
		a_southeast,
	};
public:	
	Label(int textBufferLen,const char* text,int x,int y,int wide,int tall);
	Label(const char* text,int x,int y,int wide,int tall);
	Label(const char* text,int x,int y);
	Label(const char* text);
	
	inline Label() : Panel(0,0,10,10)
	{
		init(1,"",true);
	}
private:
	void init(int textBufferLen,const char* text,bool textFitted);
public:
	virtual void setImage(Image* image);
	virtual void setText(int textBufferLen,const char* text);
	virtual void setText(const char* format,...);
	virtual void setFont(Scheme::SchemeFont schemeFont);
	virtual void setFont(Font* font);
	virtual void getTextSize(int& wide,int& tall);
	virtual void getContentSize(int& wide,int& tall);
	virtual void setTextAlignment(Alignment alignment);
	virtual void setContentAlignment(Alignment alignment);
	virtual Panel* createPropertyPanel();
	virtual void setFgColor(int r,int g,int b,int a);
	virtual void setFgColor(vgui::Scheme::SchemeColor sc);
	virtual void setContentFitted(bool state);
protected:
	virtual void computeAlignment(int& tx0,int& ty0,int& tx1,int& ty1,int& ix0,int& iy0,int& ix1,int& iy1,int& minX,int& minY,int& maxX,int& maxY);
	virtual void paint();
	virtual void recomputeMinimumSize();
protected:
	bool       _textEnabled;
	bool       _imageEnabled;
	bool       _contentFitted;
	Alignment  _textAlignment;
	Alignment  _contentAlignment;
	TextImage* _textImage;
	Image*     _image;
};

}

#endif