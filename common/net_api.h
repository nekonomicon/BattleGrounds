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
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#if !defined( NET_APIH )
#define NET_APIH
#ifdef _WIN32
#pragma once
#endif

#if !defined ( NETADRH )
#include "netadr.h"
#endif

#define NETAPI_REQUEST_SERVERLIST	( 0 )  // Doesn't need a remote address
#define NETAPI_REQUEST_PING			( 1 )
#define NETAPI_REQUEST_RULES		( 2 )
#define NETAPI_REQUEST_PLAYERS		( 3 )
#define NETAPI_REQUEST_DETAILS		( 4 )

// Set this flag for things like broadcast requests, etc. where the engine should not
//  kill the request hook after receiving the first response
#define FNETAPI_MULTIPLE_RESPONSE ( 1<<0 )

typedef void ( *net_api_response_func_t ) ( struct net_response_s *response );

#define NET_SUCCESS						( 0 )
#define NET_ERROR_TIMEOUT				( 1<<0 )
#define NET_ERROR_PROTO_UNSUPPORTED		( 1<<1 )
#define NET_ERROR_UNDEFINED				( 1<<2 )

typedef struct net_adrlist_s
{
	struct net_adrlist_s	*next;
	netadr_t				remote_address;
} net_adrlist_t;

typedef struct net_response_s
{
	// NET_SUCCESS or an error code
	int			error;

	// Context ID
	int			context;
	// Type
	int			type;

	// Server that is responding to the request
	netadr_t	remote_address;

	// Response RTT ping time
	double		ping;
	// Key/Value pair string ( separated by backlash \ characters )
	// WARNING:  You must copy this buffer in the callback function, because it is freed
	//  by the engine right after the call!!!!
	// ALSO:  For NETAPI_REQUEST_SERVERLIST requests, this will be a pointer to a linked list of net_adrlist_t's
	void		*response;
} net_response_t;

typedef struct net_status_s
{
		// Connected to remote server?  1 == yes, 0 otherwise
	int			connected; 
	// Client's IP address
	netadr_t	local_address;
	// Address of remote server
	netadr_t	remote_address;
	// Packet Loss ( as a percentage )
	int			packet_loss;
	// Latency, in seconds ( multiply by 1000.0 to get milliseconds )
	double		latency;
	// Connection time, in seconds
	double		connection_time;
	// Rate setting ( for incoming data )
	double		rate;
} net_status_t;

typedef struct net_api_s
{
	// APIs
	void		( *InitNetworking )( void );
	void		( *Status ) ( struct net_status_s *status );
	void		( *SendRequest) ( int context, int request, int flags, double timeout, struct netadr_s *remote_address, net_api_response_func_t response );
	void		( *CancelRequest ) ( int context );
	void		( *CancelAllRequests ) ( void );
	char		*( *AdrToString ) ( struct netadr_s *a );
	int			( *CompareAdr ) ( struct netadr_s *a, struct netadr_s *b );
	int			( *StringToAdr ) ( char *s, struct netadr_s *a );
	const char *( *ValueForKey ) ( const char *s, const char *key );
	void		( *RemoveKey ) ( char *s, const char *key );
	void		( *SetValueForKey ) (char *s, const char *key, const char *value, int maxsize );
} net_api_t;

extern net_api_t netapi;

#endif // NET_APIH