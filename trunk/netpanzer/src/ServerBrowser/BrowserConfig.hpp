#ifndef __BROWSERCONFIG_HPP__
#define __BROWSERCONFIG_HPP__

#include <string>

class BrowserConfig
{
public:
    std::string ircserver;
    int connect_timeout;
    int query_timeout;
    bool showplayers;
    bool showmap;
};

#endif

