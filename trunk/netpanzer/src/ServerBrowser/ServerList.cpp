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

static const char CHANNEL[] = "#netpanzerlob";
static const char SERVER_QUERY[] = "-Who's running a server?";
static const char RESPONSE_HEADER[] = "-I'm running ";
static const int _NETPANZER_DEFAULT_PORT_TCP = 3030;

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
    const char* mess = text.c_str();
    
    // old code for parsing server response
    if(strncmp(mess, RESPONSE_HEADER, sizeof(RESPONSE_HEADER)-1) != 0)
        return;
    
   const char *p=mess+strlen(RESPONSE_HEADER);
   const char *map;
   int players=atoi(p);
   if((p=strchr(p,'/'))==0) {
       LOG(("bad server description: %s\n",mess));
       return;
   }
   int max_players=atoi(++p);
   int port=_NETPANZER_DEFAULT_PORT_TCP;
   char *port_str;
   if((port_str=strstr(p,"port:"))!=0) {
       port=atoi(port_str+5);
   }
   if((map=strstr(p,"map:"))==0) {
       LOG(("no map name: %s\n",mess));
       return;
   }
   map+=4;
 
   std::cout << sender.getName() << ": " << sender.getHost();
   if(config->showplayers)
       std::cout << ", " << players << "/" << max_players;
   if(config->showmap)
       std::cout << ", " << map;
   std::cout << std::endl;
}

