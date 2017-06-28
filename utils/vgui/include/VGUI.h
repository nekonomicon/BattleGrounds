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


#ifndef VGUI_H
#define VGUI_H

//If you are going to add stuff to the vgui core...
//
//Keep it simple.
//
//Never put code in a header.
//
//The name of the class is the name of the the file
//
//Each class gets its own .cpp file for its definition and a .h for its header. Helper
//classes can be used but only within the .cpp and not referenceable from anywhere else.
//
//Don't add unneeded files. Keep the API clean.
//
//No platform specific code in vgui\lib-src\vgui dir. Code in vgui\lib-src\vgui should 
//only include from vgui\include or standard C includes. ie, if I see windows.h included
//anywhere but vgui\lib-src\win32 I will hunt you down and kill you. Don't give me any crap
//that mfc is platform inspecific.
//
//Always use <> and not "" for includes
//
//Use minimum dependencies in headers. Don't include another header if you can get away
//with forward declaring (which is usually the case)
//
//No macros in headers. They are tools of satan. This also means no use of DEFINEs, use enum
//
//Minimize global functions
//
//No global variables.
//
//Panel is getting pretty plump, try and avoid adding junk to it if you can

//TODO: Look and Feel support
//		add Panel::setPaintProxy, if _paintProxy exists, it calls _paintProxy->paint 
//		instead of Panel::paint. Components should implement their painting in a seperate
//      plugin class. Perhaps to encourage this, Panel::paint should just go away completely
//      The other option is to have Panel have the interface Paintable
//      class Paintable
//      {
//      public:
//			virtual void paint()=0;
//      };
//      Then a component can implement its paint in the class itself and then call 
//		setPaintProxy(this). If this is the case _paintProxy->paint should always be called
//      and never Panel::paint from within paintTraverse
//TODO: Figure out the 'Valve' Look and Feel and implement that instead of a the Java one
//TODO: Determine ownership policy for Borders, Layouts, etc..
//TODO: tooltips support
//TODO: ComboKey (hot key support)
//TODO: add Background.cpp, remove paintBackground from all components
//		Panel implements setBackground, Panel::paintBackground calls _background->paintBackground
//		similiar to the way Border works. 
//TODO: Builtin components should never overide paintBackground, only paint
//TODO: All protected members should be converted to private
//TODO: All member variables should be moved to the top of the class prototype
//TODO: All private methods should be prepended with private
//TODO: Use of word internal in method names is not consistent and confusing
//TODO: Cleanup so bullshit publics are properly named, maybe even figure out
//      a naming convention for them
//TODO: Breakup InputSignal into logical pieces
//TODO: Button is in a state of disarray, it should have ButtonModel support
//TODO: get rid of all the stupid strdup laziness, convert to vgui_strdup
//TODO: actually figure out policy on String and implement it consistently
//TODO: implement createLayoutInfo for other Layouts than need it
//TODO: BorderLayout should have option for a null LayoutInfo defaulting to center
//TODO: SurfaceBase should go away, put it in Surface
//TODO: ActionSignals and other Signals should just set a flag when they fire.
//		then App can come along later and fire all the signals
//TODO: Change all method naming to starting with a capital letter.

#ifdef _WIN32
# define VGUIAPI __declspec( dllexport )
#else
# define VGUIAPI
#endif

#define null 0L

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;
typedef unsigned long  ulong;

namespace vgui
{

VGUIAPI void  vgui_setMalloc(void* (*malloc)(size_t size));
VGUIAPI void  vgui_setFree(void (*free)(void* memblock));
VGUIAPI void  vgui_strcpy(char* dst,int dstLen,const char* src);
VGUIAPI char* vgui_strdup(const char* src);
VGUIAPI int   vgui_printf(const char* format,...);
VGUIAPI int   vgui_dprintf(const char* format,...);
VGUIAPI int   vgui_dprintf2(const char* format,...);

}

#endif

