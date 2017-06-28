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


#ifndef VGUI_COLOR_H
#define VGUI_COLOR_H

#include<VGUI.h>
#include<VGUI_Scheme.h>

//TODO: rename getColor(r,g,b,a) to getRGBA(r,g,b,a)
//TODO: rename setColor(r,g,b,a) to setRGBA(r,g,b,a)
//TODO: rename getColor(sc) to getSchemeColor(sc)
//TODO: rename setColor(sc) to setSchemeColor(sc)

namespace vgui
{

class VGUIAPI Color
{
private:
	uchar               _color[4];
	Scheme::SchemeColor _schemeColor;
public:
	Color();
	Color(int r,int g,int b,int a);
	Color(Scheme::SchemeColor sc);
private:
	virtual void init();
public:
	virtual void setColor(int r,int g,int b,int a);
	virtual void setColor(Scheme::SchemeColor sc);
	virtual void getColor(int& r,int& g,int& b,int& a);
	virtual void getColor(Scheme::SchemeColor& sc);
	virtual int  operator[](int index);
};

}


#endif
