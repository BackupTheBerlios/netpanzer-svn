#include <config.h>

#include "ServerList.hpp"

ServerList::ServerList()
{
}

ServerList::~ServerList()
{
}

void ServerList::ircJoin(Channel* channel)
{
    if(channel->getName() == "#netpanzer_servers")
        channel->setCallback(this);
}

void ServerList::channelMessage(Channel* channel, const std::string& sender,
                                const std::string& text)
{
    // TODO parse server response
}

