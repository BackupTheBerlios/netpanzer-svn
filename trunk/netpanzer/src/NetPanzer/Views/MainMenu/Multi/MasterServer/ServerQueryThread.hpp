/*
Copyright (C) 2004 by Matthias Braun <matze@braunis.de>

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
#ifndef __SERVERQUERYTHREAD_HPP__
#define __SERVERQUERYTHREAD_HPP__

#include <vector>
#include <string>
#include <SDL_thread.h>
#include "Network/TCPSocket.hpp"
#include "Network/UDPSocket.hpp"

#include "ServerList.hpp"

namespace masterserver
{

class ServerQueryThread
{
public:
    ServerQueryThread(ServerList* serverlist);
    ~ServerQueryThread();

    const char* getStateMessage() const;

private:
    static int threadMain(void* data);
    void run();

    void queryMasterServer();
    void queryServers();

    bool running;
    SDL_Thread* thread;
    
    ServerList* serverlist;

    enum State {
        STATE_QUERYMASTERSERVER,
        STATE_QUERYSERVERS,
        STATE_ERROR,
        STATE_DONE
    };
    State state;

    std::vector<std::string> masterservers;

    network::UDPSocket* udpsocket;

    std::vector<ServerInfo*> not_queried;
    int queries; // number of currently running queries
};

} // masterserver

#endif

