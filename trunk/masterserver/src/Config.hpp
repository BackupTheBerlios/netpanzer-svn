#ifndef __CONFIGFILE_HPP__
#define __CONFIGFILE_HPP__

#include <vector>
#include <string>

/** A very simple configfile parser, which parses key/value pairs from a config
 * file and stores them
 */
class ConfigFile
{
public:
    ConfigFile(const std::string& filename);
    ~ConfigFile();

    int getPort()
    { return port; }
    int getGameServerTimeout()
    { return gameservertimeout; }
    int getRequestTimeout()
    { return requesttimeout; }
    int getConnectionLimit()
    { return connectionlimit; }
    std::vector<std::string>& getBindAddresses()
    { return bindaddresses; }

private:
    std::string token;
    void nextToken();
    
    int port;
    int gameservertimeout;
    int requesttimeout;
    int connectionlimit;
    std::vector<std::string> bindaddresses;
};

#endif

