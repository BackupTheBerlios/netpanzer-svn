/*
Copyright (C) 2003 Hankin Chick

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
#ifndef __IRCLobby_h__
#define __IRCLobby_h__

#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_thread.h>
#include <SDL_net.h>

#include "Channel.hpp"
#include "Callback.hpp"

namespace IRC
{

class Connection
{
public:
    Connection(const std::string& servername, const std::string& nickname);
    virtual ~Connection();

    void setCallback(Callback* callback);

    void setNickName(const std::string& newnick);
    const std::string& getNickName()
    { return nickname; }

    Channel* joinChannel(const std::string& channelname);
    Channel* findChannel(const std::string& channelname);
    
private:
    friend class Channel;
    
    void startThread();
    void stopThread();

    void connect();
    void disconnect();
    
    static int threadEntry(void* t);
    int threadMain();
    
    void send(const std::string& text);
    void read(std::string& buffer);

    void parseResponse(const std::string& buffer);

    Client* getCreateClient(const std::string& name);

    TCPsocket irc_server_socket;

    std::string nickname;
    std::string server_host;
    int server_port;
    
    SDL_Thread *running_thread;

    std::vector<Channel*> channels;
    std::vector<Client*> clients;

    Callback* callback;
};

} // end of namespace IRC

#endif
