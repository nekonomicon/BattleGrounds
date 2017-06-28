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


#ifndef VGUI_EDITPANEL_H
#define VGUI_EDITPANEL_H

#include<VGUI.h>
#include<VGUI_Panel.h>
#include<VGUI_Dar.h>

namespace vgui
{

class Font;

class VGUIAPI EditPanel : public Panel
{
public:
	EditPanel(int x,int y,int wide,int tall);
public:
	virtual void doCursorUp();
	virtual void doCursorDown();
	virtual void doCursorLeft();
	virtual void doCursorRight();
	virtual void doCursorToStartOfLine();
	virtual void doCursorToEndOfLine();
	virtual void doCursorInsertChar(char ch);
	virtual void doCursorBackspace();
	virtual void doCursorNewLine();
	virtual void doCursorDelete();
	virtual void doCursorPrintf(char* format,...);
	virtual int  getLineCount();
	virtual int  getVisibleLineCount();
	virtual void setCursorBlink(bool state);
	virtual void setFont(Font* font);
	virtual void getText(int lineIndex, int offset,char* buf,int bufLen);

public: //bullshit public
	void getCursorBlink(bool& blink,int& nextBlinkTime);
protected:
	virtual void       paintBackground();
	virtual void       paint();
	virtual void       addLine();
	virtual Dar<char>* getLine(int lineIndex);
	virtual void       setChar(Dar<char>* lineDar,int x,char ch,char fill);
	virtual void       setChar(Dar<char>* lineDar,int x,char ch);
	virtual void       shiftLineLeft(Dar<char>* lineDar,int x,int count);
	virtual void       shiftLineRight(Dar<char>* lineDar,int x,int count);
private:
	virtual int        spatialCharOffsetBetweenTwoLines(Dar<char>* srcDar,Dar<char>* dstDar,int x);
protected:
	Dar<Dar<char>*> _lineDarDar;
	int             _cursor[2];
	bool            _cursorBlink;
	int             _cursorNextBlinkTime;
	Font*           _font;
};

}

#endif