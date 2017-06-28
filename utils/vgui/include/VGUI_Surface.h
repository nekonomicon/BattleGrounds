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


#ifndef VGUI_SURFACE_H
#define VGUI_SURFACE_H

#include<VGUI.h>
#include<VGUI_SurfaceBase.h>
#include<VGUI_Dar.h>

namespace vgui
{

class Panel;
class Cursor;

class VGUIAPI Surface : public SurfaceBase
{
public:
	Surface(Panel* embeddedPanel);
public:
	virtual void setTitle(const char* title);
	virtual bool setFullscreenMode(int wide,int tall,int bpp);
	virtual void setWindowedMode();
	virtual void setAsTopMost(bool state);
	virtual int  getModeInfoCount();
	virtual void createPopup(Panel* embeddedPanel);
	virtual bool hasFocus();
	virtual bool isWithin(int x,int y);
protected:
	virtual int  createNewTextureID(void);
	virtual void drawSetColor(int r,int g,int b,int a);
	virtual void drawFilledRect(int x0,int y0,int x1,int y1);
	virtual void drawOutlinedRect(int x0,int y0,int x1,int y1);
	virtual void drawSetTextFont(Font* font);
	virtual void drawSetTextColor(int r,int g,int b,int a);
	virtual void drawSetTextPos(int x,int y);
	virtual void drawPrintText(const char* text,int textLen);
	virtual void drawSetTextureRGBA(int id,const char* rgba,int wide,int tall);
	virtual void drawSetTexture(int id);
	virtual void drawTexturedRect(int x0,int y0,int x1,int y1);
	virtual void invalidate(Panel *panel);
	virtual bool createPlat();
	virtual bool recreateContext();
	virtual void enableMouseCapture(bool state);
	virtual void setCursor(Cursor* cursor);
	virtual void swapBuffers();
	virtual void pushMakeCurrent(Panel* panel,bool useInsets);
	virtual void popMakeCurrent(Panel* panel);
	virtual void applyChanges();
protected:
	class SurfacePlat* _plat;
	bool               _needsSwap;
	Panel*             _embeddedPanel;
	Dar<char*>         _modeInfoDar;
	friend class App;
	friend class Panel;
};

}

#endif

