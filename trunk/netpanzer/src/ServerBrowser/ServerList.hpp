#ifndef __SERVERLIST_HPP__
#define __SERVERLIST_HPP__

#include "IRC/IRCCallback.hpp"

class ServerList : public IRC::Callback, public IRC::ChannelCallback
{
public:
    ServerList();
    ~ServerList();

    virtual void ircJoin(Channel* channel);

    virtual void channelMessage(Channel* channel, const std::string& sender,
                                const std::string& text);
};

#endif

