#ifndef __REQUESTTHREAD_HPP__
#define __REQUESTTHREAD_HPP__

#include <iostream>
#include <time.h>
#include <netinet/in.h>

namespace masterserver
{

class MasterServer;

/** A thread that handles a single client request */
class RequestThread
{
public:
    RequestThread(MasterServer* master, std::iostream* stream,
            struct sockaddr_in addr);
    ~RequestThread();

    time_t getStartTime()
    { return starttime; }
    
private:
    static void* threadMain(void* data);
    void run();
    
    MasterServer* masterserver;
    std::iostream* stream;
    struct sockaddr_in addr;
    pthread_t thread;
    volatile bool running;
    time_t starttime;
};

}

#endif

