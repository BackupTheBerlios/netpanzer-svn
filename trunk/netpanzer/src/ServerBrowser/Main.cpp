#include <config.h>

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <time.h>

#include "optionmm/command_line.hpp"

#include "Util/FileSystem.hpp"
#include "IRC/Connection.hpp"
#include "ServerList.hpp"

int main(int argc, char** argv)
{
    srand(time(0));

    BrowserConfig* config = new BrowserConfig;
    config->ircserver = "irc.freenode.net";
    config->connect_timeout = 20;
    config->query_timeout = 20;

    using namespace optionmm;
    command_line commandline("netpanzerbrowser", "0.1",
            "Copyright(c) 2004 Matthias Braun", "", argc, argv);

    option<std::string, true, false> ircserver_option
        ('s', "server", "connect to the specified irc server", "");
    commandline.add(&ircserver_option);
    bool_option showplayers_option('p', "showplayers",
            "display player count", false);
    commandline.add(&showplayers_option);
    bool_option showmap_option('m', "showmap",
            "display currently played map", false);
    commandline.add(&showmap_option);
    option<int> timeout_option('t', "timeout",
            "timeout for connecting the server", 0);
    commandline.add(&timeout_option);
    option<int> query_timeout_option('q', "querytimeout",
            "timeout for querying the servers", 0);
    commandline.add(&query_timeout_option);

    if(!commandline.process() || commandline.help() || commandline.version())
        return 0;

    if(ircserver_option.value() != "")
        config->ircserver = ircserver_option.value();
    config->showplayers = showplayers_option.value();
    config->showmap = showmap_option.value();
    if(timeout_option.value())
        config->connect_timeout = timeout_option.value();
    if(query_timeout_option.value())
        config->query_timeout = query_timeout_option.value();

    if(SDL_Init(0)) {
        std::cerr << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }
  
    try {
        FileSystem::initialize(argv[0], "netpanzer", "netpanzer");
    } catch(std::exception& e) {
        std::cerr <<  e.what() << std::endl;
        SDL_Quit();
        return 1;    
    }                                                                               
    
    ServerList* serverlist = new ServerList(config);

    try {
        serverlist->connect();
        serverlist->queryServerList();

        // wait 20 seconds for answers
        sleep(config->query_timeout);
    } catch(std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        
        delete serverlist;
        SDL_Quit();
        FileSystem::shutdown();
        return 1;
    }

    SDL_Quit();
    FileSystem::shutdown();
    return 0;
}

