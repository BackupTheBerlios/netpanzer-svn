#include <config.h>

#include <iostream>

#include "ServerConsole.hpp"
#include "Util/Exception.hpp"

ServerConsole::ServerConsole(DedicatedGameManager* newmanager)
    : running(false), thread(0), manager(newmanager)
{
}

ServerConsole::~ServerConsole()
{
    if(thread)
        SDL_KillThread(thread);
}

class CommandHelp
{
public:
    char* name;
    char* help;
};

static CommandHelp commands[] = {
    { "help", "Display this help message" },
    { "quit", "Shutdown the netPanzer server" },
    { "status", "Display server status" },
    { "say", "Prints a message on client displays" },
    { "map mapname", "Change map" },
    { "kick n",
        "Kick player with number n (you can lookup numbers with \"status\")" },
    { 0, 0 }
};

void ServerConsole::executeCommand(const std::string& command)
{
    if(command == "help") {
        std::cout << "Commands:\n";
        for(size_t i = 0; commands[i].name != 0; ++i) {
            std::cout << "   " << commands[i].name << "     "
                << commands[i].help << "\n";
        }
    } else if(command == "quit") {
        running = false;
        manager->pushCommand(ServerCommand(ServerCommand::QUIT));
    } else if(command == "status") {
        manager->pushCommand(ServerCommand(ServerCommand::STATUS));
    } else if(command == "") {
    } else {
        std::cout << "Unknown command.\n";
    }
}

void ServerConsole::commandQuit()
{
}

void ServerConsole::startThread()
{
    typedef int (*threadfunc) (void*);
    thread = SDL_CreateThread( (threadfunc) _run, this);

    if(!thread)
        throw Exception("Couldn't start console thread.");
}

int ServerConsole::_run(ServerConsole* _this)
{
    _this->run();
    return 0;
}

void ServerConsole::run()
{
    running = true;
    while(running) {
        char buf[256];
       
        std::cout << "netpanzer-server: ";
        fgets(buf, sizeof(buf), stdin);
        // eleminated \n at the end
        buf[strlen(buf)-1] = '\0';

        executeCommand(buf);
    }
}

