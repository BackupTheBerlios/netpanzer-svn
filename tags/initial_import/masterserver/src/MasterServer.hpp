#ifndef __MASTERSERVER_HPP__
#define __MASTERSERVER_HPP__

#include <vector>
#include <list>
#include <iostream>
#include <arpa/inet.h>
#include "ServerInfo.hpp"
#include "Tokenizer.hpp"
#include "RequestThread.hpp"

namespace masterserver
{

class MasterServer
{
public:
    MasterServer();
    ~MasterServer();

    void run();
    
private:
    friend class RequestThread;
    
    /** this function is threadsafe */
    void parseHeartbeat(std::iostream& stream,
            struct sockaddr_in* addr, Tokenizer& tokenizer);
    /** this function is threadsafe */
    void parseList(std::iostream& stream,
            struct sockaddr_in* addr, Tokenizer& tokenizer);
    void updateServerInfo(ServerInfo& info, Tokenizer& tokenizer);
    
    int sock;
    pthread_mutex_t serverlist_mutex;
    std::vector<ServerInfo> serverlist;
    std::list<RequestThread*> threads;
};

}

#endif

