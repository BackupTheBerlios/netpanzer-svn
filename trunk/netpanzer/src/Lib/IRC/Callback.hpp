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
#ifndef __IRC_CALLBACK_HPP__
#define __IRC_CALLBACK_HPP__

#include <string>

namespace IRC
{

class Channel;
class Client;

class Callback
{
public:
    Callback()
    { }
    virtual ~Callback()
    { }
    
    virtual void ircError(const std::string& /*number*/,
            const std::string& /*arg0*/, const std::string& /*text*/)
    { }
    
    virtual void ircMessage(const Client& /*from*/,
            const std::string& /*text*/)
    { }
    
    virtual void ircJoin(Channel* /*channel*/)
    { }
};

class ChannelCallback
{
public:
    virtual ~ChannelCallback()
    { }
    
    virtual void channelJoin(Channel* /*channel*/, const Client& /*client*/)
    { }

    virtual void channelPart(Channel* /*channel*/, const Client& /*client*/)
    { }
    
    virtual void channelRename(Channel* /*channel*/, const Client& /*client*/,
            const std::string& /*newnickname*/)
    { }

    virtual void channelMessage(Channel* /*channel*/, const Client& /*client*/,
            const std::string& /*text*/)
    { }
};

}

#endif

