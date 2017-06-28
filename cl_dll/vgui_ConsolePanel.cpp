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

#include"vgui_ConsolePanel.h"
#include"hud.h"
#include<VGUI_ActionSignal.h>
#include<VGUI_TextGrid.h>
#include<VGUI_TextEntry.h>
#include<VGUI_EtchedBorder.h>
#include<VGUI_LoweredBorder.h>

using namespace vgui;


namespace
{

class Handler : public ActionSignal
{
private:
	
	ConsolePanel* _consolePanel;

public:

	Handler(ConsolePanel* consolePanel)
	{
		_consolePanel=consolePanel;
	}

public:

	virtual void actionPerformed(Panel* panel)
	{
		_consolePanel->doExecCommand();
	}

};

}



ConsolePanel::ConsolePanel(int x,int y,int wide,int tall) : Panel(x,y,wide,tall)
{
	setBorder(new EtchedBorder());
	
	_textGrid=new TextGrid(80,21,5,5,200,100);
	_textGrid->setBorder(new LoweredBorder());
	_textGrid->setParent(this);
	
	_textEntry=new TextEntry("",5,5,200,20);
	_textEntry->setParent(this);
	_textEntry->addActionSignal(new Handler(this));
}

int ConsolePanel::print(const char* text)
{
	return _textGrid->printf("%s",text);
}

int ConsolePanel::vprintf(const char* format,va_list argList)
{
	return _textGrid->vprintf(format,argList);
}

int ConsolePanel::printf(const char* format,...)
{
	va_list argList;
	va_start(argList,format);
	int ret=vprintf(format,argList);
	va_end(argList);
	return ret;
}

void ConsolePanel::doExecCommand()
{
	char buf[2048];
	_textEntry->getText(0,buf,2048);
	_textEntry->setText(null,0);
	gEngfuncs.pfnClientCmd(buf);
}

void ConsolePanel::setSize(int wide,int tall)
{
	Panel::setSize(wide,tall);
		
	getPaintSize(wide,tall);

	_textGrid->setBounds(5,5,wide-10,tall-35);
	_textEntry->setBounds(5,tall-25,wide-10,20);
}





