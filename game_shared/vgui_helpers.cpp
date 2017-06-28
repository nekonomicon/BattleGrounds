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

#include "vgui_helpers.h"


using namespace vgui;


void AlignPanel(Panel *pChild, Panel *pParent, int alignment)
{
	int w, h, cw, ch;
	pParent->getSize(w, h);
	pChild->getSize(cw, ch);

	int xCenter = (w - cw) / 2;
	int yCenter = (h - ch) / 2;

	if(alignment == Label::a_west)
		pChild->setPos(0, yCenter);
	else if(alignment == Label::a_northwest)
		pChild->setPos(0,0);
	else if(alignment == Label::a_north)
		pChild->setPos(xCenter, 0);
	else if(alignment == Label::a_northeast)
		pChild->setPos(w - cw, 0);
	else if(alignment == Label::a_east)
		pChild->setPos(w - cw, yCenter);
	else if(alignment == Label::a_southeast)
		pChild->setPos(w - cw, h - ch);
	else if(alignment == Label::a_south)
		pChild->setPos(xCenter, h - ch);
	else if(alignment == Label::a_southwest)
		pChild->setPos(0, h - ch);
	else if(alignment == Label::a_center)
		pChild->setPos(xCenter, yCenter);
}




