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

//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============

#if !defined( HUD_SERVERSH )
#define HUD_SERVERSH
#pragma once

#define NET_CALLBACK /* */

// Dispatchers
void NET_CALLBACK ListResponse( struct net_response_s *response );
void NET_CALLBACK ServerResponse( struct net_response_s *response );
void NET_CALLBACK PingResponse( struct net_response_s *response );
void NET_CALLBACK RulesResponse( struct net_response_s *response );
void NET_CALLBACK PlayersResponse( struct net_response_s *response );

void ServersInit( void );
void ServersShutdown( void );
void ServersThink( double time );
void ServersCancel( void );

// Get list and get server info from each
void ServersList( void );

// Query for IP / IPX LAN servers
void BroadcastServersList( int clearpending );

void ServerPing( int server );
void ServerRules( int server );
void ServerPlayers( int server );

int	ServersGetCount( void );
const char *ServersGetInfo( int server );
int	ServersIsQuerying( void );
void SortServers( const char *fieldname );

#endif // HUD_SERVERSH