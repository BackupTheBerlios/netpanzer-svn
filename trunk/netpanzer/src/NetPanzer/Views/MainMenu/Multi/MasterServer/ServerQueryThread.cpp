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
#include <ctype.h>
#include <SDL_net.h>
#include "Util/Exception.hpp"
#include "GameConfig.hpp"
#include "Core/NetworkGlobals.hpp"
#include "SDLNetSocketStream.hpp"
#include "Tokenizer.hpp"

namespace masterserver
{

ServerQueryThread::ServerQueryThread(ServerList* newserverlist)
    : running(false), thread(0), serverlist(newserverlist),
      state(STATE_QUERYMASTERSERVER)
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
    
    thread = SDL_CreateThread(threadMain, this);
}

ServerQueryThread::~ServerQueryThread()
{
    running = false;
    SDL_KillThread(thread);
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
                SDL_mutexV(serverlist->mutex);
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
}

} // masterserver

