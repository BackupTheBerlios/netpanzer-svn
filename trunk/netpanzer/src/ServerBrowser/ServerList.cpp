#include <config.h>

#include <iostream>
#include <sstream>
#include <string>

#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "ServerList.hpp"
#include "Util/Log.hpp"
#include "Util/Exception.hpp"
#include "IRC/Channel.hpp"
#include "IRC/Client.hpp"

static const char* CHANNEL = "#netp_servers";
static const char* SERVER_QUERY = "-Who's running a server?";

ServerList::ServerList(BrowserConfig* newconfig)
    : config(newconfig), connection(0), channeljoined(false)
{
}

ServerList::~ServerList()
{
    delete connection;
}

void ServerList::connect()
{
    std::stringstream nickname;
    nickname << "netp_sb" << (rand() % 1000);

    connection = new IRC::Connection(config->ircserver, nickname.str());
    connection->setCallback(this);

    // now wait some time for connection
    time_t timeout = 2 * 60; // set timeout to 2 minutes
    time_t starttime = time(0);
    while(starttime - time(0) <= timeout && ! connection->isReady()) {
        sleep(1);
    }

    if(!connection->isReady()) {
        throw Exception("Timeout during connect");
    }

    connection->joinChannel(CHANNEL);
    timeout = 10; // 10 seconds timeout for joining channel
    starttime = time(0);
    while(starttime - time(0) <= timeout && !channeljoined) {
        sleep(1);
    }

    if(!channeljoined)
        throw Exception("Couldn't join channel %s", CHANNEL);
}

void ServerList::queryServerList()
{
    IRC::Channel* channel = connection->findChannel(CHANNEL);
    if(!channel)
        return;

    channel->sendMessage(SERVER_QUERY);
}

void ServerList::ircError(std::string& number, const std::string& arg,
        const std::string& text)
{
    // do we need a new nickname?
    if(number == "433" || number == "436") {
        std::stringstream nickname;
        nickname << "netp_sb" << (rand() % 1000);
        connection->setNickName(nickname.str());
        
        return;
    }

    // otherwise display the error
    LOG(("IRC Error: %s: %s (%s)", number.c_str(), text.c_str(),
                arg.c_str()));
}

void ServerList::ircJoin(IRC::Channel* channel)
{
    if(channel->getName() == CHANNEL) {
        channel->setCallback(this);
        channeljoined = true;
    }
}

void ServerList::ircMessage(const IRC::Client& sender, const std::string& text)
{
    // TODO parse server response
    std::cout << "Private response: (" << sender.getName()
              << ") - " << text << std::endl;
}
