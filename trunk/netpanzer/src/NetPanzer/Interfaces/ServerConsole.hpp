#ifndef __SERVERCONSOLE_HPP__
#define __SERVERCONSOLE_HPP__

#include <string>
#include <SDL_thread.h>
#include "DedicatedGameManager.hpp"

class DedicatedGameManager;

class ServerConsole
{
public:
    ServerConsole(DedicatedGameManager* manager);
    ~ServerConsole();

    void startThread();
    void executeCommand(const std::string& command);
    
private:
    static int _run(ServerConsole* _this);
    void run();
    
    void commandQuit();

    volatile bool running;
    SDL_Thread* thread;
    DedicatedGameManager* manager;
};

#endif

