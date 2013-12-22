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
#include "Network/ClientSocket.hpp"

#include "Util/Timer.hpp"

using namespace network;

class NetworkServer : public NetworkInterface,
                      public ClientSocketObserver
{
protected:
    typedef std::list<ClientSocket*> ClientList;
    
    ClientList client_list;

    void updateKeepAliveState();
    void resetClientList();

public:
    NetworkServer();
    virtual ~NetworkServer();
    
    bool addClientToSendList( ClientSocket * client );
    bool isAlreadyConnected( ClientSocket * client );
    void cleanUpClientList();

    void openSession();
    
    void hostSession();
    
    void closeSession();

    void broadcastMessage(const NetMessage *message, size_t size);
    
    void sendMessage(const PlayerID player_index, NetMessage* message,
            size_t size);
        
    void sendRemaining();

    bool getPacket(NetPacket* packet);

    void niceDisconnect(ClientSocket * client);
    void kickClient(const PlayerID index);

    std::string getIP(const PlayerID player_index);
    
protected:
    void onClientConnected(ClientSocket *s);
    void onClientDisconected(ClientSocket *s, const char * msg);

};

extern NetworkServer *SERVER;

#endif // ** _NETWORKSERVER_HPP
