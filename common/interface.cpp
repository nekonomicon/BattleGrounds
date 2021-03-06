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

//========= Copyright � 1996-2001, Valve LLC, All rights reserved. ============

#include <string.h>
#include <stdlib.h>
#include "interface.h"

#ifndef _WIN32  // LINUX
#include <dlfcn.h>
#include <unistd.h> // getcwd
#include <stdio.h> // sprintf
#endif


// ------------------------------------------------------------------------------------ //
// InterfaceReg.
// ------------------------------------------------------------------------------------ //
InterfaceReg *InterfaceReg::s_pInterfaceRegs = NULL;


InterfaceReg::InterfaceReg( InstantiateInterfaceFn fn, const char *pName ) :
	m_pName(pName)
{
	m_CreateFn = fn;
	m_pNext = s_pInterfaceRegs;
	s_pInterfaceRegs = this;
}



// ------------------------------------------------------------------------------------ //
// CreateInterface.
// ------------------------------------------------------------------------------------ //
EXPORT_FUNCTION IBaseInterface *CreateInterface( const char *pName, int *pReturnCode )
{
	InterfaceReg *pCur;
	
	for(pCur=InterfaceReg::s_pInterfaceRegs; pCur; pCur=pCur->m_pNext)
	{
		if(strcmp(pCur->m_pName, pName) == 0)
		{
			if ( pReturnCode )
			{
				*pReturnCode = IFACE_OK;
			}
			return pCur->m_CreateFn();
		}
	}
	
	if ( pReturnCode )
	{
		*pReturnCode = IFACE_FAILED;
	}
	return NULL;	
}


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif


#ifdef _WIN32
HINTERFACEMODULE Sys_LoadModule(const char *pModuleName)
{
	return (HINTERFACEMODULE)LoadLibrary(pModuleName);
}

#else  // LINUX
HINTERFACEMODULE Sys_LoadModule(const char *pModuleName)
{
	// Linux dlopen() doesn't look in the current directory for libraries.
	// We tell it to, so people don't have to 'install' libraries as root.

	char szCwd[1024];
	char szAbsoluteLibFilename[1024];

	getcwd( szCwd, sizeof( szCwd ) );
	if ( szCwd[ strlen( szCwd ) - 1 ] == '/' )
		szCwd[ strlen( szCwd ) - 1 ] = 0;

	sprintf( szAbsoluteLibFilename, "%s/%s", szCwd, pModuleName );

	return (HINTERFACEMODULE)dlopen( szAbsoluteLibFilename, RTLD_NOW );
}

#endif


#ifdef _WIN32
void Sys_FreeModule(HINTERFACEMODULE hModule)
{
	if(!hModule)
		return;

	FreeLibrary((HMODULE)hModule);
}

#else  // LINUX
void Sys_FreeModule(HINTERFACEMODULE hModule)
{
	if(!hModule)
		return;

	dlclose( (void *)hModule );
}

#endif


//-----------------------------------------------------------------------------
// Purpose: returns the instance of this module
// Output : interface_instance_t
//-----------------------------------------------------------------------------
CreateInterfaceFn Sys_GetFactoryThis( void )
{
	return CreateInterface;
}


//-----------------------------------------------------------------------------
// Purpose: returns the instance of the named module
// Input  : *pModuleName - name of the module
// Output : interface_instance_t - instance of that module
//-----------------------------------------------------------------------------

#ifdef _WIN32
CreateInterfaceFn Sys_GetFactory( HINTERFACEMODULE hModule )
{
	if(!hModule)
		return NULL;

	return (CreateInterfaceFn)GetProcAddress((HMODULE)hModule, CREATEINTERFACE_PROCNAME);
}

#else  // LINUX
CreateInterfaceFn Sys_GetFactory( HINTERFACEMODULE hModule )
{
	if(!hModule)
		return NULL;

	return (CreateInterfaceFn)dlsym( (void *)hModule, CREATEINTERFACE_PROCNAME );
}

#endif
