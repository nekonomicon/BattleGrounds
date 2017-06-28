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


#ifndef VGUI_SCROLLPANEL_H
#define VGUI_SCROLLPANEL_H

#include<VGUI.h>
#include<VGUI_Panel.h>

//NOTE: You need to call validate anytime you change a scrollbar

namespace vgui
{

class ScrollBar;

class VGUIAPI ScrollPanel : public Panel
{
private:
	Panel*     _clientClip;
	Panel*     _client;
	ScrollBar* _horizontalScrollBar;
	ScrollBar* _verticalScrollBar;
	bool       _autoVisible[2];
public:
	ScrollPanel(int x,int y,int wide,int tall);
protected:
	virtual void setSize(int wide,int tall);
public:
	virtual void   setScrollBarVisible(bool horizontal,bool vertical);
	virtual void   setScrollBarAutoVisible(bool horizontal,bool vertical);
	virtual Panel* getClient();
	virtual Panel* getClientClip();
	virtual void   setScrollValue(int horizontal,int vertical);
	virtual void   getScrollValue(int& horizontal,int& vertical);
	virtual void   recomputeClientSize();
	virtual ScrollBar* getHorizontalScrollBar();
	virtual ScrollBar* getVerticalScrollBar();
	virtual void       validate();
public: //bullshit public
	virtual void recomputeScroll();
};

}






#endif
