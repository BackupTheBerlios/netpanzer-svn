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

#include "NetworkInterface.hpp"
#include "NetworkReturnCodes.hpp"

#include "ArrayUtil/Timer.hpp"
#include "PlayerID.hpp"
#include "ArrayUtil/LinkListSingleTemplate.hpp"

class ServerClientListData
{
public:
    PlayerID       client_id;
    bool        keep_alive_state;
    Timer          keep_alive_timer;
    unsigned char  no_guarantee_sequence_counter;

    ServerClientListData *next;

    ServerClientListData()
    {
        no_guarantee_sequence_counter = 0;
        keep_alive_state = false;
        next = 0;
    }
};

class ServerClientList : public LinkListSingleTemplate< ServerClientListData >
{
public:
    ServerClientListData * getClientData( PlayerID client_id );

    void addClient( ServerClientListData *client_data );

    bool removeClient( PlayerID client_id );

    bool getFullClientID( PlayerID *client_id );

    inline void resetIterator( ServerClientListData **iterator )
    {
        (*iterator) = front;
    }

    inline ServerClientListData * incIteratorPtr( ServerClientListData **iterator )
    {
        ServerClientListData *client_data;

        if( (*iterator) != 0 ) {
            client_data = (*iterator);
            (*iterator) = (*iterator)->next;
            return( client_data );
        } else {
            return( 0 );
        }
    }
};

#define _CLIENT_KEEP_ALIVE_THRESHOLD     (60)  // in seconds
#define _SERVER_KEEP_ALIVE_SEND_INTERVAL  (2)  // in seconds

class NetworkServer : public NetworkInterface
{
protected:
    ServerClientList client_list;

    NetPacket net_packet;

    Timer keep_alive_emit_timer;

    void updateKeepAliveState();
    void resetClientList();

    void netMessageClientKeepAlive( NetMessage *message );
    void netMessageServerPingRequest( NetMessage *message );
    void netMessageTransportClientAccept( NetMessage *message );

    void processNetMessage( NetMessage *message );

    bool dontSendUDPHackFlag;
public:
    NetworkServer();
    virtual ~NetworkServer();

    bool addClientToSendList(PlayerID &client_player_id );
    bool removeClientFromSendList(PlayerID &client_player_id );

    bool activateKeepAlive(PlayerID &client_player_id);
    bool deactivateKeepAlive(PlayerID &client_player_id);

    void lostPacketHack()
    {
        dontSendUDPHackFlag = true;
    }

    virtual void openSession() = 0;
    virtual void hostSession() = 0;
    virtual void closeSession() = 0;

    virtual int sendMessage(NetMessage *message, size_t size, int flags) = 0;
    virtual int sendMessage(const PlayerID& player_id,
                            NetMessage *message, size_t size, int flags) = 0;

    virtual int getMessage(NetMessage *message) = 0;

    virtual void dropClient(PlayerID client_id);
    virtual void shutdownClientTransport(const PlayerID &client_id) = 0;

    virtual void checkIncoming() = 0;
};

#endif // ** _NETWORKSERVER_HPP
