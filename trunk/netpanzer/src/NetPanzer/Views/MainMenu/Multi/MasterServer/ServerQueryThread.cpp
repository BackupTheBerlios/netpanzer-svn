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
#include <SDL_net.h>
#include "Util/Exception.hpp"
#include "GameConfig.hpp"
#include "Core/NetworkGlobals.hpp"
#include "SDLNetSocketStream.hpp"
#include "Util/StringTokenizer.hpp"
#include "Tokenizer.hpp"

static const int MAX_QUERIES = 3;

namespace masterserver
{

ServerQueryThread::ServerQueryThread(ServerList* newserverlist)
    : running(false), thread(0), serverlist(newserverlist),
      state(STATE_QUERYMASTERSERVER), udpsocket(0), queries(0)
{
    // parse masterserverlist
    const char* str = gameconfig->masterservers.c_str();
    char c;
    size_t i = 0;
    std::string currentstr;
    while( (c = str[i]) != 0) {
        if(c == ',') {
            masterservers.push_back(currentstr);
            currentstr = "";
        } else if(isspace(c)) {
            // do nothing
        } else {
            currentstr += c;
        }
        ++i;
    }
    if(currentstr != "")
        masterservers.push_back(currentstr);
    std::random_shuffle(masterservers.begin(), masterservers.end());

    udpsocket = SDLNet_UDP_Open(0);
    if(udpsocket == 0) {
        throw Exception("Couldn't create socket: %s",
                SDLNet_GetError());
    }
   
    thread = SDL_CreateThread(threadMain, this);
}

ServerQueryThread::~ServerQueryThread()
{
    running = false;
    SDL_KillThread(thread);
    SDLNet_UDP_Close(udpsocket);
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
}

void
ServerQueryThread::queryMasterServer()
{
    if(masterservers.empty()) {
        std::cerr << "No success querying masterserver.\n";
        state = STATE_ERROR;
        return;
    }

    try {
        IPaddress ip;
        if(SDLNet_ResolveHost(&ip,
                const_cast<char*>(masterservers.back().c_str()), 28900) < 0) {
            throw Exception("Couldn't resolve masterserver address '%s': %s",
                    masterservers.back().c_str(), SDLNet_GetError());
        }

        TCPsocket socket = SDLNet_TCP_Open(&ip);
        if(!socket) {
            throw Exception("Couldn't connect to '%s': %s",
                    masterservers.back().c_str(), SDLNet_GetError());
        }

        SDLNetSocketStream* stream = new SDLNetSocketStream(socket);
        Tokenizer* tokenizer = new Tokenizer(*stream);

        // send query
        *stream << "\\list\\gamename\\netpanzer\\protocol\\" <<
            _NETPANZER_PROTOCOL_VERSION << "\\final\\" << std::flush;
        
        ServerInfo* lastserver = 0;
        while(!stream->eof() && running) {
            std::string token = tokenizer->getNextToken();
            if(token == "ip") {
                ServerInfo info;
                info.querying = true;
                info.address = tokenizer->getNextToken();
                if(info.address == "")
                    break;
                
                // add server into list
                SDL_mutexP(serverlist->mutex);
                serverlist->push_back(info);
                lastserver = &(serverlist->back());
                not_queried.push_back(lastserver);
                SDL_mutexV(serverlist->mutex);
            } else if(token == "port") {
                std::stringstream portstr(tokenizer->getNextToken());
                portstr >> lastserver->port;
            } else if(token == "final") {
                break;
            } else {
                std::cerr << "Unknown token '" << token
                    << "' from masterserver.\n";
            }
        }

        state = STATE_QUERYSERVERS;
        std::cout << "Done querying servers.\n";

        delete tokenizer;
        delete stream;       
    } catch(std::exception& e) {
        std::cerr << "Problem querying masterserver: " << e.what() << "\n";
        masterservers.pop_back();
    }
}

void
ServerQueryThread::queryServers()
{    
    if(queries < MAX_QUERIES && !not_queried.empty()) {
        // send a query to a server
        ServerInfo& server = *not_queried.back();
        not_queried.pop_back();

        // resolve address
        if(SDLNet_ResolveHost(&server.ipaddress,
               const_cast<char*> (server.address.c_str()), server.port) < 0) {
            std::cerr << "Couldn't resolve address of server '"
                << server.address << "'.\n";
            return;
        }
         
        // send query
        std::string query = "\\status\\final\\";

        const void* data = query.c_str();
        size_t datasize = query.size();

        UDPpacket* sendpacket = SDLNet_AllocPacket(datasize);
        if(!sendpacket) {
            throw std::runtime_error("out of memory");
        }
        sendpacket->address = server.ipaddress;
        sendpacket->len = datasize;
        memcpy(sendpacket->data, data, datasize);
        int res = SDLNet_UDP_Send(udpsocket, -1, sendpacket);
        SDLNet_FreePacket(sendpacket);
        if(res != 1) {
            std::cerr << "Errro when sending info query: " << SDLNet_GetError();
            return;            
        }
        
        server.querystartticks = SDL_GetTicks();
        queries++;
    }

    // part2 receive data

    UDPpacket* packet = SDLNet_AllocPacket(4096);
    int res = SDLNet_UDP_Recv(udpsocket, packet);
    if(res < 0) {
        SDLNet_FreePacket(packet);
        std::cerr << "Network error when receiving from udpsocket.\n";
        state = STATE_ERROR;
        return;
    }
    if(res == 0)
        return;

    // find server with this address
    ServerInfo* server = 0;
    SDL_mutexP(serverlist->mutex);
    for(ServerList::iterator i = serverlist->begin();
            i != serverlist->end(); ++i) {
        if(i->ipaddress.host == packet->address.host &&
                i->ipaddress.port == packet->address.port) {
            server = &(*i);
            break;
        }       
    }
    SDL_mutexV(serverlist->mutex);
    if(server == 0) { // random data from elsewhere
        SDLNet_FreePacket(packet);
        return;
    }

    queries--;
    server->ping = SDL_GetTicks() - server->querystartticks;
    
    std::string packetstr((const char*) packet->data, packet->len);
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

    SDLNet_FreePacket(packet);
}

} // masterserver

