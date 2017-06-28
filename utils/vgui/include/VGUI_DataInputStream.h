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


#ifndef VGUI_DATAINPUTSTREAM_H
#define VGUI_DATAINPUTSTREAM_H

#include<VGUI.h>
#include<VGUI_InputStream.h>

namespace vgui
{

class VGUIAPI DataInputStream : virtual public InputStream
{
private:
	InputStream* _is;
public:
	DataInputStream(InputStream* is);
public:
	virtual void  seekStart(bool& success);
	virtual void  seekRelative(int count,bool& success);
	virtual void  seekEnd(bool& success);
	virtual int   getAvailable(bool& success);
	//virtual uchar readUChar(bool& success);
	virtual void  readUChar(uchar* buf,int count,bool& success);
	virtual void  close(bool& success);
	virtual void  close();
public:
	virtual bool   readBool(bool& success);
	virtual char   readChar(bool& success);
	virtual uchar  readUChar(bool& success);
	virtual short  readShort(bool& success);
	virtual ushort readUShort(bool& success);
	virtual int    readInt(bool& success);
	virtual uint   readUInt(bool& success);
	virtual long   readLong(bool& success);
	virtual ulong  readULong(bool& success);
	virtual float  readFloat(bool& success);
	virtual double readDouble(bool& success);
	virtual void   readLine(char* buf,int bufLen,bool& success);
};

}

#endif