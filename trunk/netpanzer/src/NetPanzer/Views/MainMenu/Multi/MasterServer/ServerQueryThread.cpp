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
#include <config.h>

#include "ServerQueryThread.hpp"

#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <ctype.h>
#include "Util/Exception.hpp"
#include "GameConfig.hpp"
#include "Core/NetworkGlobals.hpp"
#include "Util/StringTokenizer.hpp"
#include "Util/StreamTokenizer.hpp"
#include "Util/Log.hpp"
#include "Network/SocketStream.hpp"

static const size_t MAX_QUERIES = 3;
static const Uint32 QUERY_TIMEOUT = 5 * 1000;

namespace masterserver
{

ServerQueryThread::ServerQueryThread(ServerList* newserverlist)
    : running(false), stream(0), shutdown_mutex(0), thread(0),
      serverlist(newserverlist), state(STATE_QUERYMASTERSERVER), udpsocket(0),
      queries(0)
{
    // parse masterserverlist
    std::string server;
    StringTokenizer tokenizer(gameconfig->masterservers, ',');
    while( (server = tokenizer.getNextToken()) != "") {
        masterservers.push_back(server);
    }
    std::random_shuffle(masterservers.begin(), masterservers.end());

    if(masterservers.size() == 0) {
        state = STATE_NOSERVERS;
        return;
    }

    udpsocket = new network::UDPSocket(false);
   
    shutdown_mutex = SDL_CreateMutex();
    printf("startit.\n");
    thread = SDL_CreateThread(threadMain, this);
}

ServerQueryThread::~ServerQueryThread()
{
    printf("killit\n");
    SDL_mutexP(shutdown_mutex);
    running = false;
    if(stream) {
        printf("cancel.\n");
        stream->cancel();
    }
    SDL_mutexV(shutdown_mutex);
    printf("waitit.\n");
    SDL_WaitThread(thread, 0);
    printf("Joined.\n");

    delete udpsocket;
    SDL_DestroyMutex(shutdown_mutex);
}

int
ServerQueryThread::threadMain(void* data)
{
    ServerQueryThread* _this = reinterpret_cast<ServerQueryThread*> (data);

    _this->run();
    return 0;
}

void
ServerQueryThread::run()
{
    try {
        running = true;
        while(running) {
            switch(state) {
                case STATE_QUERYMASTERSERVER:
                    queryMasterServer();
                    break;
                case STATE_QUERYSERVERS:
                    queryServers();
                    break;
                default:
                    break;
            }
            // sleep a little bit
            SDL_Delay(10); // this destroys ping times :-(
        }
    } catch(std::exception& e) {
        LOGGER.warning("Unexpected exception in query thread: %s", e.what());
    } catch(...) {
        LOGGER.warning("Unexpected exception in query thread");
    }
}

void
ServerQueryThread::queryMasterServer()
{
    if(masterservers.empty()) {
        LOGGER.warning("No success querying masterserver.");
        state = STATE_ERROR;
        return;
    }

    network::TCPSocket* tcpsocket = 0;

    try {
        printf("Resolve.\n");
        network::Address ip
            = network::Address::resolve(masterservers.back(), 28900);

        printf("Connect.\n");
        tcpsocket = new network::TCPSocket(ip, false);
        printf("Connok.\n");
        stream = new network::SocketStream(*tcpsocket);
        StreamTokenizer tokenizer(*stream, '\\');

        if(!running)
            throw std::runtime_error("query aborted");

        // send query
        *stream << "\\list\\gamename\\master\\final"
                << "\\list\\gamename\\netpanzer\\protocol\\"
                << NETPANZER_PROTOCOL_VERSION << "\\final\\" << std::flush;
        
        ServerInfo* lastserver = 0;
        std::string newMasterServers;

        // parse master server list
        while(!stream->eof() && running) {
            std::string token = tokenizer.getNextToken();
            if(token == "ip") {
                if(newMasterServers != "")
                    newMasterServers += ",";
                newMasterServers += tokenizer.getNextToken();
            } else if(token == "port") {
                tokenizer.getNextToken();
                // ignored
            } else if(token == "final") {
                break;
            } else {
                std::stringstream msg;
                msg << "Unknown token '" 
                    << token << "' when querying masterserver (master list)";
                throw std::runtime_error(msg.str());
            }
        }

        // parse server list
        while(!stream->eof() && running) {
            std::string token = tokenizer.getNextToken();
            if(token == "ip") {
                ServerInfo info;
                info.querying = true;
                info.address = tokenizer.getNextToken();
                if(info.address == "")
                    break;
            
                // add server into list
                SDL_mutexP(serverlist->mutex);
                serverlist->push_back(info);
                lastserver = &(serverlist->back());
                not_queried.push_back(lastserver);
                SDL_mutexV(serverlist->mutex);
            } else if(token == "port") {
                std::stringstream portstr(tokenizer.getNextToken());
                portstr >> lastserver->port;
            } else if(token == "final") {
                break;
            } else {
                std::stringstream msg;
                msg << "Unknown token '" 
                    << token << "' when querying masterserver (game list)";
                throw std::runtime_error(msg.str());
            }
        }

        state = STATE_QUERYSERVERS;
        gameconfig->masterservers = newMasterServers;
    } catch(std::exception& e) {
        LOGGER.warning("Problem querying masterserver: %s.", e.what());
        masterservers.pop_back();
    }

    printf("trydel.\n");
    SDL_mutexP(shutdown_mutex);
    printf("delstream.\n");
    delete stream;
    stream = 0;
    delete tcpsocket;
    SDL_mutexV(shutdown_mutex);
}

void
ServerQueryThread::queryServers()
{    
    Uint32 now = SDL_GetTicks();
    
    // check for timed out servers
    for(std::vector<ServerInfo*>::iterator i = querying.begin();
            i != querying.end(); ) {
        ServerInfo& server = *(*i);
        if(now - server.querystartticks > QUERY_TIMEOUT) {
            server.name = "Timeout (" + server.address + ")";
            server.address = "";
            server.querying = false;
            i = querying.erase(i);
            continue;
        }

        ++i;
    }
       
    if(querying.size() < MAX_QUERIES && !not_queried.empty()) {
        // send a query to a server
        ServerInfo& server = *not_queried.back();
        not_queried.pop_back();

        // resolve address
        try {
            server.ipaddress = network::Address::resolve(server.address,
                    server.port);
        } catch(std::exception& e) {
            LOGGER.warning(e.what());
            return;
        }
         
        // send query
        std::string query = "\\status\\final\\";

        udpsocket->send(server.ipaddress, query.c_str(), query.size());

        server.querystartticks = now;
        querying.push_back(&server);
    }

    // part2 receive data

    char buffer[4096];
    network::Address addr;
    size_t size = udpsocket->recv(addr, buffer, sizeof(buffer));
    if(size == 0)
        return;

    // find server with this address
    ServerInfo* server = 0;
    SDL_mutexP(serverlist->mutex);
    for(ServerList::iterator i = serverlist->begin();
            i != serverlist->end(); ++i) {
        if(i->ipaddress == addr) {
            server = &(*i);
            break;
        }       
    }
    SDL_mutexV(serverlist->mutex);
    if(server == 0) { // random data from elsewhere
        return;
    }

    server->ping = now - server->querystartticks;
    
    std::string packetstr(buffer, size);
    StringTokenizer tokenizer(packetstr, '\\');

    std::string token;
    while( (token = tokenizer.getNextToken()) != "") {
        if(token == "hostname") {
            server->name = tokenizer.getNextToken();
        } else if(token == "mapname") {
            server->map = tokenizer.getNextToken();
        } else if(token == "numplayers") {
            std::stringstream str(tokenizer.getNextToken());
            str >> server->players;
        } else if(token == "maxplayers") {
            std::stringstream str(tokenizer.getNextToken());
            str >> server->maxplayers;
        } else {
            // handle more tokens...
        }
    }
    server->querying = false;
    for(std::vector<ServerInfo*>::iterator i = querying.begin();
            i != querying.end(); ) {
        if((*i) == server)                                          
            i = querying.erase(i);
        else
            ++i;
    }
}

const char*
ServerQueryThread::getStateMessage() const
{
    switch(state) {
        case STATE_NOSERVERS:
            return "No Masterservers configured";
        case STATE_QUERYMASTERSERVER:
            return "Querying Masterserver";
        case STATE_ERROR:
            return "No response from Masterserver";
        case STATE_QUERYSERVERS:
        case STATE_DONE:
            return "No servers found";
    }

    return "Querying Masterserver";
}

} // masterserver

