/*
Copyright (C) 1998 Pyrosoft Inc. (www.pyrosoftgames.com), Matthew Bogue

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#ifndef _NETWORK_SERVER_WINSOCK_HPP
#define _NETWORK_SERVER_WINSOCK_HPP

#include "NetworkServer.hpp"

class NetworkServerWinSock : public NetworkServer
 {
  protected:
   int winsockErrorToNetworkError( int winsock_error );


  public:

   NetworkServerWinSock();
   ~NetworkServerWinSock();
   
   virtual int openSession( int connection_type, int session_flags );
   virtual int hostSession( void );
   virtual int closeSession( void );

   virtual int sendMessage( NetMessage *message, unsigned long size, int flags );
   virtual int sendMessage( NetMessage *message, unsigned long size, PlayerID &player_id, int flags );

   virtual int getMessage( NetMessage *message );

   virtual void shutdownClientTransport( PlayerID &client_id );

 };
#endif // ** _NETWORK_SERVER_WINSOCK_HPP
