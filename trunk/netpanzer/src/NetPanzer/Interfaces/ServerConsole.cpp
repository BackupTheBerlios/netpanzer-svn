#include <config.h>

#include <iostream>

#include "ServerConsole.hpp"
#include "Util/Exception.hpp"
#include "Util/StringTokenizer.hpp"

ServerConsole::ServerConsole(DedicatedGameManager* newmanager)
    : running(false), thread(0), manager(newmanager)
{
}

ServerConsole::~ServerConsole()
{
    if(thread && running)
        SDL_KillThread(thread);
    else
        SDL_WaitThread(thread, 0);
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

void ServerConsole::executeCommand(const std::string& commandline)
{
    std::string command, argument;
    size_t i;
    for(i = 0; i < commandline.size() && !isspace(commandline[i]); ++i) {
        command += commandline[i];
    }
    ++i;
    for( ; i < commandline.size(); ++i)
        argument += commandline[i];
            
    if(command == "help") {
        std::cout << "Commands:\n";
        for(size_t i = 0; commands[i].name != 0; ++i) {
            std::cout << "   " << commands[i].name;
            for(size_t a = 0; a < 12-strlen(commands[i].name); ++a)
                std::cout << ' ';
            std::cout << commands[i].help << "\n";
        }
    } else if(command == "quit") {
        manager->pushCommand(ServerCommand(ServerCommand::QUIT));
        running = false;
    } else if(command == "status") {
        manager->pushCommand(ServerCommand(ServerCommand::STATUS));
    } else if(command == "kick") {
        if(argument != "")
            manager->pushCommand(ServerCommand(ServerCommand::KICK, argument));
        else
            std::cout << "Please specify the number of a player.\n";
    } else if(command == "map") {
        if(argument != "")
            manager->pushCommand(
                    ServerCommand(ServerCommand::MAPCHANGE, argument));
        else
            std::cout << "Please specify the name of a map.\n";
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

