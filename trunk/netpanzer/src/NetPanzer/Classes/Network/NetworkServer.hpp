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
#ifndef _NETWORKSERVER_HPP
#define _NETWORKSERVER_HPP

#include <list>
#include "NetworkInterface.hpp"
#include "NetworkReturnCodes.hpp"
#include "Network/TCPListenSocket.hpp"
#include "Network/ClientSocket.hpp"

#include "Util/Timer.hpp"
#include "Classes/PlayerID.hpp"

using namespace network;

class ServerClientListData
{
public:
    bool          wannadie;
    ClientSocket *client_socket;

    ServerClientListData()
        : wannadie(false), client_socket(0)
    { }
};

class NetworkServer : public NetworkInterface,
                      public TCPListenSocketObserver,
                      public ClientSocketObserver
{
protected:
    typedef std::list<ServerClientListData*> ClientList;
    ClientList client_list;

    NetPacket net_packet;

    Timer keep_alive_emit_timer;

    void updateKeepAliveState();
    void resetClientList();

    void netPacketClientKeepAlive(const NetPacket* packet);
    void netPacketServerPingRequest(const NetPacket* packet);

    void processNetPacket(const NetPacket* packet);

    /// removes client that have the wannadie flag set to true
    void cleanupClients();
    
public:
    NetworkServer();
    virtual ~NetworkServer();

    bool addClientToSendList( ClientSocket * client );

    void openSession();
    
    void hostSession();
    
    void closeSession();

    void sendMessage(NetMessage *message, size_t size);
    
    void sendMessage(NetClientID network_id, NetMessage* message,
            size_t size);
    
    void sendMessage(const PlayerID& player_id, NetMessage *message,
            size_t size)
    {
        sendMessage(player_id.getNetworkID(), message, size);
    }
    
    void sendRemaining();

    bool getPacket(NetPacket* packet);

    void dropClient(ClientSocket * client);
    void shutdownClientTransport( ClientSocket * client );
    
    ClientSocket * getClientSocketByPlayerIndex ( Uint16 index );

    void checkIncoming();
    std::string getIP(NetClientID network_id);

protected:
    TCPSocketObserver * onNewConnection(TCPListenSocket *so,const Address &fromaddr);
    void onSocketError(TCPListenSocket *so);
    void onClientConnected(ClientSocket *s);
    void onClientDisconected(ClientSocket *s, const char * msg);

private:
    TCPListenSocket * socket;
};

extern NetworkServer *SERVER;

#endif // ** _NETWORKSERVER_HPP
