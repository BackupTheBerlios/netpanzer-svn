#ifndef __SERVERINFO_HPP__
#define __SERVERINFO_HPP__

#include <string>
#include <arpa/inet.h>
#include <time.h>

class ServerInfo
{
public:
    std::string gamename;
    struct sockaddr_in address;
    time_t lastheartbeat;
};

#endif
