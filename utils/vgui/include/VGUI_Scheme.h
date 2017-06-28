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


#ifndef VGUI_SCHEME_H
#define VGUI_SCHEME_H

#include<VGUI.h>


namespace vgui
{

class Font;
class Cursor;

class VGUIAPI Scheme
{
public:
	enum SchemeColor
	{
		sc_user=0,
		sc_black,
		sc_white,
		sc_primary1,
		sc_primary2,
		sc_primary3,
		sc_secondary1,
		sc_secondary2,
		sc_secondary3,
		sc_last,
	};
	enum SchemeFont
	{
		sf_user=0,
		sf_primary1,
		sf_primary2,
		sf_primary3,
		sf_secondary1,
		sf_last,
	};
	enum SchemeCursor
	{
		scu_user=0,
		scu_none,
		scu_arrow,
		scu_ibeam,
		scu_hourglass,
		scu_crosshair,
		scu_up,
		scu_sizenwse,
		scu_sizenesw,
		scu_sizewe,
		scu_sizens,
		scu_sizeall,
		scu_no,
		scu_hand,
		scu_last,
	};
public:
	Scheme();
public:
	virtual void    setColor(SchemeColor sc,int r,int g,int b,int a);
	virtual void    getColor(SchemeColor sc,int& r,int& g,int& b,int& a);
	virtual void    setFont(SchemeFont sf,Font* font);
	virtual Font*   getFont(SchemeFont sf);
	virtual void    setCursor(SchemeCursor sc,Cursor* cursor);
	virtual Cursor* getCursor(SchemeCursor sc);
protected:
	int     _color[sc_last][4];
    Font*   _font[sf_last];
	Cursor* _cursor[scu_last];
    friend class Panel;
    friend class Canvas;
};

}

#endif