#ifndef __SERVERCOMMAND_HPP__
#define __SERVERCOMMAND_HPP__

#include <string>

class ServerCommand
{
public:
    enum CommandType {
        QUIT, CHAT, MAPCHANGE, KICK
    };
    
    CommandType type;
    std::string argument;
};

#endif

