#include <config.h>

#include <iostream>

#ifdef USE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

#include "ServerConsole.hpp"
#include "Util/Exception.hpp"

ServerConsole::ServerConsole(DedicatedGameManager* newmanager)
    : running(false), thread(0), manager(newmanager)
{
}

ServerConsole::~ServerConsole()
{
#ifdef USE_READLINE
    // sometimes readline doesn't get a chance to clean this up...
    rl_deprep_terminal();
#endif
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

#ifdef USE_READLINE

static char* dupstr(char* s) {
    size_t len = strlen(s)+1;
    char* result = (char*) malloc(len);
    memcpy(result, s, len);
    return result;
}

// code from readline example
static char* command_generator(const char* text, int state)
{
    static int list_index, len;
    char* name;

    if(!state) {
        list_index = 0;
        len = strlen(text);
    }

    while( (name = commands[list_index].name) != 0) {
        list_index++;
        
        if(strncmp(name, text, len) == 0)
            return dupstr(name);
    }

    return 0; 
}

static char** command_completion(const char* text, int start, int)
{
    char** matches = 0;

    if(start == 0)
        matches = rl_completion_matches(text, command_generator);

    return matches;
}

void ServerConsole::run()
{
    rl_readline_name = PACKAGE_NAME;
    rl_attempted_completion_function = command_completion;
    rl_initialize();
    
    running = true;
    while(running) {
        char* line = readline("netpanzer-Server: ");
        if(!line || *line == 0)
            continue;

        add_history(line);
        
        executeCommand(line);
        free(line);
    }
}

#else

void ServerConsole::run()
{
    running = true;
    while(running) {
        char buf[256];
        fgets(buf, sizeof(buf), stdin);

        executeCommand(buf);
    }
}

#endif

