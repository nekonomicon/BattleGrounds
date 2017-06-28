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


#ifndef VGUI_FRAME_H
#define VGUI_FRAME_H

#include<VGUI.h>
#include<VGUI_Panel.h>
#include<VGUI_Dar.h>

namespace vgui
{

class Button;
class FrameSignal;

class VGUIAPI Frame : public Panel
 {
 public:
  Frame(int x,int y,int wide,int tall);
 public:
  virtual void   setSize(int wide,int tall);
  virtual void   setInternal(bool state);
  virtual void   paintBackground();
  virtual bool   isInternal();
  virtual Panel* getClient();
  virtual void   setTitle(const char* title);
  virtual void   getTitle(char* buf,int bufLen);
  virtual void	 setMoveable(bool state);
  virtual void   setSizeable(bool state);
  virtual bool   isMoveable();
  virtual bool   isSizeable();
  virtual void   addFrameSignal(FrameSignal* s);
  virtual void   setVisible(bool state);
  virtual void   setMenuButtonVisible(bool state);
  virtual void   setTrayButtonVisible(bool state);
  virtual void   setMinimizeButtonVisible(bool state);
  virtual void   setMaximizeButtonVisible(bool state);
  virtual void   setCloseButtonVisible(bool state);
 public: //bullshit public
  virtual void fireClosingSignal();
  virtual void fireMinimizingSignal();
 protected:
  char*             _title;
  bool              _internal;
  bool              _sizeable;
  bool              _moveable;
  Panel*            _topGrip;
  Panel*            _bottomGrip;
  Panel*            _leftGrip;
  Panel*            _rightGrip;
  Panel*            _topLeftGrip;
  Panel*            _topRightGrip;
  Panel*            _bottomLeftGrip;
  Panel*            _bottomRightGrip;
  Panel*            _captionGrip;
  Panel*            _client;
  Button*           _trayButton;
  Button*           _minimizeButton;
  Button*           _maximizeButton;
  Button*           _closeButton;
  Button*           _menuButton;
  Dar<FrameSignal*> _frameSignalDar;
  Frame*            _resizeable;
 };

}

#endif
