#include <config.h>

#include <iostream>
#include <exception>

#include "Tokenizer.hpp"
#include "MasterServer.hpp"
#include "SocketStream.hpp"

int main(int argc, char** argv)
{   
    using namespace masterserver;
    
    MasterServer* masterserver = 0;
    try {
        masterserver = new MasterServer;
        masterserver->run();
    } catch(std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        delete masterserver;
        return 1;
    }
    
    delete masterserver;
    return 0;
}

