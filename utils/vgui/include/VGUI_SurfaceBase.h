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


#ifndef VGUI_SURFACEBASE_H
#define VGUI_SURFACEBASE_H

#include<VGUI.h>
#include<VGUI_Dar.h>

namespace vgui
{

class Panel;
class Cursor;
class Font;
class App;
class ImagePanel;

class VGUIAPI SurfaceBase
{
public:
	SurfaceBase(Panel* embeddedPanel);
protected:
	~SurfaceBase();
public:
	virtual Panel* getPanel();
	virtual void   requestSwap();
	virtual void   resetModeInfo();
	virtual int    getModeInfoCount();
	virtual bool   getModeInfo(int mode,int& wide,int& tall,int& bpp);
	virtual App*   getApp();
	virtual void   setEmulatedCursorVisible(bool state);
	virtual void   setEmulatedCursorPos(int x,int y);
public:
	virtual void setTitle(const char* title)=0;
	virtual bool setFullscreenMode(int wide,int tall,int bpp)=0;
	virtual void setWindowedMode()=0;
	virtual void setAsTopMost(bool state)=0;
	virtual void createPopup(Panel* embeddedPanel)=0;
	virtual bool hasFocus()=0;
	virtual bool isWithin(int x,int y)=0;
	virtual int  createNewTextureID(void)=0;
protected:
	virtual void addModeInfo(int wide,int tall,int bpp);
protected:
	virtual void drawSetColor(int r,int g,int b,int a)=0;
	virtual void drawFilledRect(int x0,int y0,int x1,int y1)=0;
	virtual void drawOutlinedRect(int x0,int y0,int x1,int y1)=0;
	virtual void drawSetTextFont(Font* font)=0;
	virtual void drawSetTextColor(int r,int g,int b,int a)=0;
	virtual void drawSetTextPos(int x,int y)=0;
	virtual void drawPrintText(const char* text,int textLen)=0;
	virtual void drawSetTextureRGBA(int id,const char* rgba,int wide,int tall)=0;
	virtual void drawSetTexture(int id)=0;
	virtual void drawTexturedRect(int x0,int y0,int x1,int y1)=0;
	virtual void invalidate(Panel *panel)=0;
	virtual void enableMouseCapture(bool state)=0;
	virtual void setCursor(Cursor* cursor)=0;
	virtual void swapBuffers()=0;
	virtual void pushMakeCurrent(Panel* panel,bool useInsets)=0;
	virtual void popMakeCurrent(Panel* panel)=0;
	virtual void applyChanges()=0;
protected:
	bool       _needsSwap;
	App*       _app;
	Panel*     _embeddedPanel;
	Dar<char*> _modeInfoDar;
	ImagePanel* _emulatedCursor;
	Cursor*     _currentCursor;
friend class App;
friend class Panel;
};

}

#endif

