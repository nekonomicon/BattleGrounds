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


#ifndef VGUI_TEXTGRID_H
#define VGUI_TEXTGRID_H

#include<stdarg.h>
#include<VGUI.h>
#include<VGUI_Panel.h>

namespace vgui
{

class VGUIAPI TextGrid : public Panel
{
public:	
	TextGrid(int gridWide,int gridTall,int x,int y,int wide,int tall);
public:
	virtual void setGridSize(int wide,int tall);
	virtual void newLine();
	virtual void setXY(int x,int y);
	//virtual void setBgColor(int r,int g,int b);
	//virtual void setFgColor(int r,int g,int b);
	virtual int  vprintf(const char* format,va_list argList);
	virtual int  printf(const char* format,...);
protected:
	virtual void paintBackground();
protected:
	int   _xy[2];
	int   _bgColor[3];
	int   _fgColor[3];
	char* _grid; //[_gridSize[0]*_gridSize[1]*7] ch,br,bg,bb,fr,fg,fb
	int	  _gridSize[2];
};

}

#endif