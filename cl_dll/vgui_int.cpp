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
#include"vgui_int.h"
#include<VGUI_Label.h>
#include<VGUI_BorderLayout.h>
#include<VGUI_LineBorder.h>
#include<VGUI_SurfaceBase.h>
#include<VGUI_TextEntry.h>
#include<VGUI_ActionSignal.h>
#include<string.h>
#include "hud.h"
#include "cl_util.h"
#include "camera.h"
#include "kbutton.h"
#include "cvardef.h"
#include "usercmd.h"
#include "const.h"
#include "camera.h"
#include "in_defs.h"
#include "vgui_TeamFortressViewport.h"
//#include "vgui_ControlConfigPanel.h"

namespace
{

class TexturePanel : public Panel , public ActionSignal
{
private:
	int _bindIndex;
	TextEntry* _textEntry;
public:
	TexturePanel() : Panel(0,0,256,276)
	{
		_bindIndex=2700;
		_textEntry=new TextEntry("2700",0,0,128,20);
		_textEntry->setParent(this);
		_textEntry->addActionSignal(this);
	}
public:
	virtual bool isWithin(int x,int y)
	{
		return _textEntry->isWithin(x,y);
	}
public:
	virtual void actionPerformed(Panel* panel)
	{
		char buf[256];
		_textEntry->getText(0,buf,256);
		sscanf(buf,"%d",&_bindIndex);
	}
protected:
	virtual void paintBackground()
	{
			Panel::paintBackground();
			
			int wide,tall;
			getPaintSize(wide,tall);
		
			drawSetColor(0,0,255,0);
			drawSetTexture(_bindIndex);
			drawTexturedRect(0,19,257,257);
	}

};

}

using namespace vgui;

void VGui_ViewportPaintBackground(int extents[4])
{
	gEngfuncs.VGui_ViewportPaintBackground(extents);
}

void* VGui_GetPanel()
{
	return (Panel*)gEngfuncs.VGui_GetPanel();
}

void VGui_Startup()
{
	Panel* root=(Panel*)VGui_GetPanel();
	root->setBgColor(128,128,0,0);
	//root->setNonPainted(false);
	//root->setBorder(new LineBorder());
	root->setLayout(new BorderLayout(0));

	
	//root->getSurfaceBase()->setEmulatedCursorVisible(true);

	if (gViewPort != NULL)
	{
//		root->removeChild(gViewPort);

		// free the memory
//		delete gViewPort;
//		gViewPort = NULL;

		gViewPort->Initialize();
	}
	else
	{
		gViewPort = new TeamFortressViewport(0,0,root->getWide(),root->getTall());
		gViewPort->setParent(root);
	}

	/*
	TexturePanel* texturePanel=new TexturePanel();
	texturePanel->setParent(gViewPort);
	*/

}

void VGui_Shutdown()
{
	delete gViewPort;
	gViewPort = NULL;
}





