#ifndef __INFOTHREAD_HPP__
#define __INFOTHREAD_HPP__

#include <sstream>
#include <SDL_thread.h>
#include <SDL_net.h>

/** This class will create an additonal info thread and is responsible for
 * answering queries about the server status.
 */
class InfoThread
{
public:
    InfoThread(int port);
    ~InfoThread(); 

private:
    static int threadMain(void* data);
    void handleStatusRequests();

    void sendInfo(std::stringstream& out);
    void sendRules(std::stringstream& out);
    void sendPlayers(std::stringstream& out);
    
    UDPsocket socket;
    bool running;
    SDL_Thread* thread;
};

#endif
