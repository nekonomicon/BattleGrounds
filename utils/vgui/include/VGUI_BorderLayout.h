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


#ifndef VGUI_BORDERLAYOUT_H
#define VGUI_BORDERLAYOUT_H

#include<VGUI.h>
#include<VGUI_Layout.h>

namespace vgui
{

class LayoutInfo;

class VGUIAPI BorderLayout : public Layout
{
public:
	enum Alignment
	{
		a_center=0,
		a_north,
		a_south,
		a_east,
		a_west,
	};
private:
	int _inset;
public:
	BorderLayout(int inset);
public:
	virtual void        performLayout(Panel* panel);
	virtual LayoutInfo* createLayoutInfo(Alignment alignment);
};

}

#endif





