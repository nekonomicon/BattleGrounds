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

/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

//=========================================================
// nodes.h
//=========================================================

#ifndef	NODES_H
#define	NODES_H

#define	bits_NODE_GROUP_REALM	1

class CLink
{
public:
	entvars_t	*m_pLinkEnt;// the entity that blocks this connection (doors, etc)
};


class CGraph
{
public:
	bool	m_fGraphPresent;// is the graph in memory?
	bool	m_fGraphPointersSet;// are the entity pointers for the graph all set?

	int		m_cLinks;// total number of links
	CLink	*m_pLinkPool;// big list of all node connections

	void	InitGraph( void );
	int		AllocNodes ( void );
	
	int		CheckNODFile(char *szMapName);
	int		FLoadGraph(char *szMapName);
	int		FSetGraphPointers(void);
	void	ShowNodeConnections ( int iNode );
	int		FindNearestNode ( const Vector &vecOrigin, CBaseEntity *pEntity );
	int		FindNearestNode ( const Vector &vecOrigin, int afNodeTypes );

};

extern CGraph WorldGraph;

#endif
