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

#if !defined( HUD_SERVERS_PRIVH )
#define HUD_SERVERS_PRIVH
#pragma once

#include "netadr.h"

class CHudServers
{
public:
	typedef struct request_s
	{
		struct request_s	*next;
		netadr_t					remote_address;
		int							context;
	} request_t;

	typedef struct server_s
	{
		struct server_s	*next;
		netadr_t				remote_address;
		char					*info;
		int						ping;
	} server_t;

	CHudServers();
	~CHudServers();

	void	Think( double time );
	void	QueryThink( void );
	int		isQuerying( void );

	int		LoadMasterAddresses( int maxservers, int *count, netadr_t *padr );

	void	RequestList( void );
	void	RequestBroadcastList( int clearpending );

	void	ServerPing( int server );
	void	ServerRules( int server );
	void	ServerPlayers( int server );

	void	CancelRequest( void );

	int		CompareServers( server_t *p1, server_t *p2 );

	void	ClearServerList( server_t **ppList );
	void	ClearRequestList( request_t **ppList );

	void	AddServer( server_t **ppList, server_t *p );

	void	RemoveServerFromList( request_t **ppList, request_t *item );

	request_t *FindRequest( int context, request_t *pList );

	int		ServerListSize( void );
	char	*GetServerInfo( int server );
	int		GetServerCount( void );
	void	SortServers( const char *fieldname );

	void	ListResponse( struct net_response_s *response );
	void	ServerResponse( struct net_response_s *response );
	void	PingResponse( struct net_response_s *response );
	void	RulesResponse( struct net_response_s *response );
	void	PlayersResponse( struct net_response_s *response );
private:
	
	server_t *GetServer( int server );

	//
	char				m_szToken[ 1024 ];
	int					m_nRequesting;
	int					m_nDone;

	double				m_dStarted;

	request_t	*m_pServerList;
	request_t	*m_pActiveList;
	
	server_t		*m_pServers;

	int					m_nServerCount;

	int					m_nActiveQueries;
	int					m_nQuerying;
	double				m_fElapsed;

	request_t			*m_pPingRequest;
	request_t			*m_pRulesRequest;
	request_t			*m_pPlayersRequest;
};

#endif // HUD_SERVERS_PRIVH