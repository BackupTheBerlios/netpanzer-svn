#include <config.h>

#include <iostream>
#include <unistd.h>

#include "Util/FileSystem.hpp"
#include "IRC/Connection.hpp"

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
        return 1;
    }
    
    IRC::Connection* connection 
        = new IRC::Connection("taylor.local:6667", "TestI");
    sleep(1);
    connection->joinChannel("#test");
    while(true)
        ;

    SDL_Quit();
    FileSystem::shutdown();
    return 0;
}

