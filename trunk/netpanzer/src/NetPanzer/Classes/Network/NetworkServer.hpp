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

#include "Util/Timer.hpp"
#include "PlayerID.hpp"

class ServerClientListData
{
public:
    PlayerID       client_id;
};

class NetworkServer : public NetworkInterface
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
    void netPacketTransportClientAccept(const NetPacket* packet);

    void processNetPacket(const NetPacket* packet);
public:
    NetworkServer();
    virtual ~NetworkServer();

    bool addClientToSendList(const PlayerID& client_player_id);
    void removeClientFromSendList(const PlayerID& client_player_id);

    virtual void openSession() = 0;
    virtual void hostSession() = 0;
    virtual void closeSession() = 0;

    virtual void sendMessage(NetMessage *message, size_t size) = 0;
    virtual void sendMessage(SocketClient::ID network_id, NetMessage* message,
            size_t size) = 0;
    void sendMessage(const PlayerID& player_id, NetMessage *message,
            size_t size) {
        sendMessage(player_id.getNetworkID(), message, size);
    }

    virtual bool getPacket(NetPacket* packet) = 0;

    virtual void dropClient(SocketClient::ID client_id);
    virtual void shutdownClientTransport(SocketClient::ID network_id) = 0;

    virtual void checkIncoming() = 0;
};

#endif // ** _NETWORKSERVER_HPP
