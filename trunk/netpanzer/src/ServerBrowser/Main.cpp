#include <config.h>

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <time.h>

#include "Util/FileSystem.hpp"
#include "IRC/Connection.hpp"
#include "ServerList.hpp"

int main(int argc, char** argv)
{
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

    srand(time(0));

    BrowserConfig* config = new BrowserConfig;
    config->ircserver = "taylor.local:6667";
    ServerList* serverlist = new ServerList(config);

    try {
        serverlist->connect();
        serverlist->queryServerList();

        // wait 20 seconds for answers
        sleep(20);
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

