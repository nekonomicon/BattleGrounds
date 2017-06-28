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


#ifndef VGUI_FONT_H
#define VGUI_FONT_H

#include<VGUI.h>

namespace vgui
{

class BaseFontPlat;

//TODO: cursors and fonts should work like gl binds
class VGUIAPI Font
 {
 public:
  Font(const char* name,int tall,int wide,float rotation,int weight,bool italic,bool underline,bool strikeout,bool symbol);
  // If pFileData is non-NULL, then it will try to load the 32-bit (RLE) TGA file. If that fails,
  // it will create the font using the specified parameters.
  // pUniqueName should be set if pFileData and fileDataLen are set so it can determine if a font is already loaded.
  Font(const char* name,void *pFileData,int fileDataLen, int tall,int wide,float rotation,int weight,bool italic,bool underline,bool strikeout,bool symbol);
 private:
  virtual void init(const char* name,void *pFileData,int fileDataLen, int tall,int wide,float rotation,int weight,bool italic,bool underline,bool strikeout,bool symbol);
 public:
  BaseFontPlat* getPlat();
  virtual void getCharRGBA(int ch,int rgbaX,int rgbaY,int rgbaWide,int rgbaTall,uchar* rgba);
  virtual void getCharABCwide(int ch,int& a,int& b,int& c);
  virtual void getTextSize(const char* text,int& wide,int& tall);
  virtual int  getTall();
  virtual int  getId();
 protected:
  char*			_name;
  BaseFontPlat*	_plat;
  int			_id;
 friend class Surface;
 };


void Font_Reset();

}

#endif