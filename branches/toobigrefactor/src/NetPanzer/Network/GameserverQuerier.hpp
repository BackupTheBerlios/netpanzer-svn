/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez

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
 *
 * Created on September 14, 2013, 12:15 PM
 */

#ifndef GAMESERVERQUERIER_HPP
#define	GAMESERVERQUERIER_HPP

#include "Network/UDPSocket.hpp"
#include "Network/ServerAddress.hpp"
#include "ArrayUtil/PtrArray.hpp"
#include "Classes/GameServerInfo.hpp"

class ServerListDataSource;

class GameserverQuerier : public network::UDPSocketObserver
{
public:
    GameserverQuerier(ServerListDataSource * ds);
    ~GameserverQuerier();
    
    void checkTimeouts();
    
    void beginQuery(const PtrArray<ServerAddress>& list);
    void endQuery();
    
    bool isQuerying() const { return udpsocket != 0; }
    
private:
    GameserverQuerier(const GameserverQuerier&);
    
    void sendQuery( GameServerInfo * info );
    void sendNextQuery();
    void parseServerData(GameServerInfo * info, NPString& data);
    
    void onDataReceived(network::UDPSocket *so, const network::Address &from, const char *data, const int len);
    void onSocketError(network::UDPSocket *so);
    
    PtrArray<const ServerAddress> not_queried;
    PtrArray<GameServerInfo> checking_servers;
    network::UDPSocket * udpsocket;
    ServerListDataSource * ds;
};

#endif	/* GAMESERVERQUERIER_HPP */

