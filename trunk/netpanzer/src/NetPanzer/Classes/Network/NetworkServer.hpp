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
#include "NetworkReturnCodes.hpp"
#include "Network/TCPListenSocket.hpp"
#include "Network/ClientSocket.hpp"

#include "Util/Timer.hpp"

using namespace network;

class MessageClassHandler;

class ServerClientListData
{
public:
    bool          wannadie;
    ClientSocket *client_socket;

    ServerClientListData()
        : wannadie(false), client_socket(0)
    { }
};

class NetworkServer
{
protected:


    static void updateKeepAliveState();
    static void resetClientList();
    
public:
    static bool addClientToSendList( ClientSocket * client );
    static void cleanUpClientList();
    static void dropClient(Uint16 playerid);
    static void removePlayerSocket(const Uint16 player_id);

    static void openSession();
    static void hostSession();
    static void closeSession();

    static void broadcastMessage(NetMessage *message, size_t size);
    
    static void sendMessage(Uint16 player_index, NetMessage* message,
            size_t size);
        
    static void sendRemaining();
    
    static std::string getIP(Uint16 player_index);

protected:
    static void onClientDisconected(ClientSocket *s, const char * msg);
    static MessageClassHandler* getPacketHandler();

private:
    friend class MessageRouter;
    friend class ServerPacketHandler;
    NetworkServer();
    ~NetworkServer();

};

#endif // ** _NETWORKSERVER_HPP
