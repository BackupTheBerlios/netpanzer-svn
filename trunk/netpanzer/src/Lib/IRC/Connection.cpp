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
#include <config.h>

#include <ctype.h>

#include <SDL_net.h>
#include <sstream>

#include "Util/Log.hpp"
#include "Util/Exception.hpp"
#include "Util/UtilInterface.hpp"

#include "Connection.hpp"

namespace IRC
{

Connection::Connection(const std::string& server, const std::string& newnick)
    : irc_server_socket(0), nickname(newnick), running_thread(0)
{
    server_port = 6667;

    size_t colon = server.find(':', 0);
    if(colon == std::string::npos) {
        server_host = server;
    } else {
        server_host = server.substr(0, colon);
        colon++;
        std::string port_str = server.substr(colon, server.length() - colon);
        server_port = atoi(port_str.c_str());
    }

    startThread();
}

Connection::~Connection()
{
    stopThread();
    disconnect();
}

void Connection::startThread()
{
    running_thread 
        = SDL_CreateThread( (int (*)(void*)) threadEntry, this);
    if(!running_thread)
        throw Exception("Couldn't start IRC thread.");
}

void Connection::stopThread()
{
    if(!running_thread)
        return;

    SDL_KillThread(running_thread);
    running_thread = 0;
}

void Connection::connect()
{
    IPaddress addr;
    
    // some old versions of SDL_net take a char* instead of const char*
    if(SDLNet_ResolveHost(&addr, const_cast<char*>(server_host.c_str()),
                server_port) < 0)
        throw Exception("Couldn't resolve server address '%s:%d'",
                server_host.c_str(), server_port);

    irc_server_socket = SDLNet_TCP_Open(&addr);
    if(!irc_server_socket)
        throw Exception("Couldn't connect to irc server '%s': %s",
                server_host.c_str(), SDLNet_GetError());

    // TODO proxy server

    // send our username
    std::stringstream buffer;
    buffer << "USER " << nickname << " 0 * :" << nickname << "\n";
    send(buffer.str());

    setNickName(nickname);
}

void Connection::disconnect()
{
    if(irc_server_socket) {
        send("QUIT\n");
        SDLNet_TCP_Close(irc_server_socket);
        irc_server_socket = 0;
    }
}

int Connection::threadEntry(void* data)
{
    Connection* connection = (Connection*) data;

    // don't throw any exceptions through thread boundaries
    try {
        return connection->threadMain();
    } catch(...) {
        return 1;
    }
}

int Connection::threadMain()
{
    try {
        connect();
        while(1) {
            std::string buffer;
            read(buffer);

            LOG(("IRC read: %s", buffer.c_str()));

            size_t pos = 0;
            // eventually skip prefix
            if(buffer[0] == ':') {
                while(pos < buffer.size() && !isspace(buffer[pos]))
                    ++pos;
            }
            
            while(pos < buffer.size() && isspace(buffer[pos]))
                ++pos;

            size_t commandstart = pos;
            while(pos < buffer.size() && !isspace(buffer[pos]))
                ++pos;
            size_t commandend = pos;
            std::string command = buffer.substr(commandstart, commandend);
            std::string rest = buffer.substr(commandend+1,
                    buffer.size()-commandend);

            LOG(("CM: %s - %s", command.c_str(), rest.c_str()));
            if(command == "PING") {
                std::stringstream pongmsg;
                pongmsg << "PONG " << rest << "\n";
                send(pongmsg.str());
            }
        }
    } catch(std::exception& e) {
        LOG(("IRC exception: %s", e.what()));
    }

    return 0;
}

void Connection::setNickName(const std::string &nick)
{
    std::stringstream buffer;
    buffer << "NICK " << nick << "\n";
    send(buffer.str());
}

Channel* Connection::joinChannel(const std::string& channel_name)
{
    std::stringstream buffer;
    buffer << "JOIN " << channel_name << "\n";
    send(buffer.str());

    return new Channel(this, channel_name);
}

void Connection::send(const std::string& line)
{
    if(SDLNet_TCP_Send(irc_server_socket,
                const_cast<char*> (line.c_str()),
                line.size()) != (int) line.size())
        throw Exception("Error when sending irc message '%s': %s",
                line.c_str(), SDLNet_GetError());
    
    LOGGER.debug("sending irc:%s",line.c_str());
}

void Connection::read(std::string& buffer)
{
    SDLNet_SocketSet socket_set = SDLNet_AllocSocketSet(1);
    SDLNet_TCP_AddSocket(socket_set, irc_server_socket);

    try {
        char c = 0;
        while(c != '\n') {
            SDLNet_CheckSockets(socket_set, 1000);
            if(!SDLNet_SocketReady(irc_server_socket))
                continue;

            if(SDLNet_TCP_Recv(irc_server_socket, &c, 1) <= 0)
                throw Exception("Couldn't read TCP: %s", SDLNet_GetError());
        
            if(c == '\r' || c == '\n')
                continue;

            buffer += c;
        }
    } catch(...) {
        SDLNet_FreeSocketSet(socket_set);
        throw;
    }

    SDLNet_FreeSocketSet(socket_set);
}

} // end of namespace IRC
