#ifndef __SERVERCONSOLE_HPP__
#define __SERVERCONSOLE_HPP__

#include <string>

class ServerConsole
{
public:
    ServerConsole();
    ~ServerConsole();

    void executeCommand(const std::string& command);
    
private:
    void commandQuit();
};

#endif

