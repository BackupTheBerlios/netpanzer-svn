/*
Copyright (C) 2003 Matthias Braun <matze@braunis.de>

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
#ifndef __IRC_CHANNEL_HPP__
#define __IRC_CHANNEL_HPP__

#include <vector>
#include <string>

namespace IRC
{

class Client;
class ChannelCallback;

class Channel
{
public:
    ~Channel();
    
    const std::string& getName()
    { return name; }

    void sendMessage(const std::string& message);

    void setCallback(ChannelCallback* newcallback)
    { callback = newcallback; }

private:
    friend class Connection;
    Channel(Connection* newconnection, const std::string& newname);

    bool containsClient(Client* client);
    void removeClient(Client* client);

    Connection* connection;
    std::string name;
    std::vector<Client*> clients;

    ChannelCallback* callback;
};

} // end of namespace IRC

#endif

