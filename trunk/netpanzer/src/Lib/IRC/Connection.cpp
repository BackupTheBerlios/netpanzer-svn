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
#include "Client.hpp"

// #define IRC_DEBUG

namespace IRC
{

Connection::Connection(const std::string& server, const std::string& newnick)
    : irc_server_socket(0), nickname(newnick), running_thread(0), callback(0)
{
    connection_ready = false;
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
    std::vector<Channel*>::iterator i;
    for(i = channels.begin(); i != channels.end(); ++i)
        delete *i;

    std::vector<Client*>::iterator c;
    for(c = clients.begin(); c != clients.end(); ++c)
        delete *c;

    stopThread();
    disconnect();
}

void Connection::setCallback(Callback* newcallback)
{
    callback = newcallback;
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

            parseResponse(buffer);
        }
    } catch(std::exception& e) {
        LOG(("IRC exception: %s", e.what()));
    }

    return 0;
}

void Connection::parseResponse(const std::string& buffer)
{
#ifdef IRC_DEBUG
    LOG(("IRC read: %s", buffer.c_str()));
#endif
                                                         
    size_t pos = 0;
    // parse prefix (at least the name)
    std::string prefix;
    std::string user;
    std::string host;
    if(buffer[0] == ':' && 1 < buffer.size()) {
        size_t prefixstart = 1;
        while(pos < buffer.size() && !isspace(buffer[pos]) 
                                  && buffer[pos] != '!')
            ++pos;
        prefix = buffer.substr(prefixstart, pos-prefixstart);

        // parse user
        if(buffer[pos] == '!' && pos+1 < buffer.size()) {
            ++pos;
            size_t userstart = pos;
            while(pos < buffer.size() && !isspace(buffer[pos])
                                      && buffer[pos] != '@')
                    ++pos;
            user = buffer.substr(userstart, pos-userstart);
        }
        // parse host
        if(buffer[pos] == '@' && pos+1 < buffer.size()) {
            ++pos;
            size_t hoststart = pos;
            while(pos < buffer.size() && !isspace(buffer[pos]))
                ++pos;
            host = buffer.substr(hoststart, pos-hoststart);        
        }
    }
    // skip spaces before command
    while(pos < buffer.size() && isspace(buffer[pos]))
        ++pos;
    
    // parse command
    size_t commandstart = pos;
    while(pos < buffer.size() && !isspace(buffer[pos]))
        ++pos;
    std::string command = buffer.substr(commandstart, pos - commandstart);

    // parse params
    std::vector<std::string> params;
    while(pos+1 < buffer.size() 
            && (isspace(buffer[pos]) || buffer[pos] == ':')) {
        bool trailing = false;
        // skip heading spaces
        while(pos < buffer.size() && isspace(buffer[pos]))
            ++pos;

        // eventually skip heading :
        if(buffer[pos] == ':') {
            trailing = true;
            ++pos;
        }
        
        size_t paramstart = pos;
        while(pos < buffer.size() && (trailing || !isspace(buffer[pos])))
            ++pos;
        params.push_back(buffer.substr(paramstart, pos-paramstart));
    }

    // DEBUG...
#ifdef IRC_DEBUG
    printf("(%s!%s@%s) '%s' - ", prefix.c_str(), user.c_str(), host.c_str(),
                                 command.c_str());
    for(size_t i=0; i<params.size(); ++i) {
        if(i>0)
            printf(", ");
        printf("'%s'", params[i].c_str());
    }
    printf("\n");
#endif
   
    // react to the command
    if(command == "PING" && params.size() > 0) {
        std::stringstream pongmsg;
        pongmsg << "PONG :" << params[0] << "\n";
        send(pongmsg.str());
        return;
    }
    
    if(command == "JOIN" && params.size() >= 1) {
        // channel already in the list?
        Channel* channel = findChannel(params[0]);
        if(findChannel(params[0]) != 0) {
            // someone else joined the channel
            if(prefix != nickname) {
                Client* client = findClient(prefix, user, host);
                if(!client)
                    client = createClient(prefix, user, host);
                channel->clients.push_back(client);
                if(channel->callback)
                    channel->callback->channelJoin(channel, *client);
            }
            return;
        }

        channel = new Channel(this, params[0]);
        channels.push_back(channel);
        
        if(callback) {
            callback->ircJoin(channel);
        }

        return;
    }
    
    if(command == "PRIVMSG" && params.size() >= 2) {
        Client* client = findClient(prefix, user, host);
        if(!client)
            client = createClient(prefix, user, host);
        
        // is it a message to a channel?
        if(params[0][0] == '#') {
            Channel* channel = findChannel(params[0]);
            if(!channel)
                return;
            
            if(channel->callback) {
                channel->callback->channelMessage(channel, *client, params[1]);
            }
            return;
        }
        // a private message to us
        if(params[0] == nickname) {
            if(callback)
                callback->ircMessage(*client, params[1]);
            return;
        }
        LOG(("Unknown PRIVMSG received"));
        return;
    }
    
    if(command == "PART" && params.size() >= 2) {
        Channel* channel = findChannel(params[0]);
        if(!channel)
            return;

        Client* client = findClient(prefix, user, host);
        if(!client)
            return;

        if(channel->callback)
            channel->callback->channelPart(channel, *client);
        channel->removeClient(client);

        return;
    }
    
    if(command == "QUIT") {
        Client* client = findClient(prefix, user, host);
        if(!client)
            return;

        // remove client from all channels
        std::vector<Channel*>::iterator i;
        for(i = channels.begin(); i != channels.end(); ++i) {
            Channel* channel = *i;

            if(!channel->containsClient(client))
                continue;

            if(channel->callback)
                channel->callback->channelPart(channel, *client);
            channel->removeClient(client);
        }
        
        // remove Client
        std::vector<Client*>::iterator c;
        for(c = clients.begin(); c != clients.end(); ++c) {
            Client* client = *c;
            
            if(client->name == prefix) {
                clients.erase(c);
                break;
            }
        }
        
        return;
    }
    
    if(command == "NICK") {
        Client* client = findClient(prefix, user, host);
        if(!client)
            client = createClient(prefix, user, host);

        // look for all channels this client participates in
        std::vector<Channel*>::iterator i;
        for(i = channels.begin(); i != channels.end(); ++i) {
            Channel* channel = *i;

            if(channel->containsClient(client) && channel->callback)
                channel->callback->channelRename(channel, *client, params[0]);
        }

        client->name = params[0];
        return;
    }

    // a more or less hacky way to determine the point where we have a fine
    // connection...
    if(command == "242" || command == "243" || command == "251" 
        || command == "252" || command == "253" || command == "254"
        || command == "255") {
        connection_ready = true;
        return;
    }
    
    // error message?
    if(command[0] == '4') {
        if(callback)
            callback->ircError(command, params.size() >= 1 ? params[0] : "",
                    params.size() >= 0 ? params[params.size()-1] : "");
        return;
    }
}

bool Connection::isReady() const
{
    return connection_ready;
}
    
void Connection::setNickName(const std::string& nick)
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

Channel* Connection::findChannel(const std::string& name)
{
    std::vector<Channel*>::iterator i;
    for(i = channels.begin(); i != channels.end(); ++i) {
        Channel* channel = *i;

        if(channel->getName() == name)
            return channel;
    }

    return 0;
}

Client* Connection::findClient(const std::string& name,
        const std::string& user, const std::string& host)
{
    std::vector<Client*>::iterator i;
    for(i = clients.begin(); i != clients.end(); ++i) {
        Client* client = *i;

        if(client->getName() == name) {
            if(user != "")
                client->user = user;
            if(host != "")
                client->host = host;
            
            return client;                                 
        }
    }
    
    return 0;
}

Client* Connection::createClient(const std::string& name,
    const std::string& user, const std::string& host)
{
    Client* client = new Client(name);
    clients.push_back(client);

    if(user != "")
        client->user = user;
    if(host != "")
        client->host = host;
    
    return client;
}

} // end of namespace IRC
