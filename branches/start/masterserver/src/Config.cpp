#include <config.h>

#include "Config.hpp"

#include <stdexcept>
#include <sstream>
#include <fstream>

ConfigFile::ConfigFile(const std::string& filename)
    : // default values
        port(28900),
        gameservertimeout(12*60),
        requesttimeout(20),
        connectionlimit(15)
{
    std::ifstream in(filename.c_str());
    if(!in.good()) {
        std::stringstream msg;
        msg << "Couldn't open configfile '" << filename << "'";    
        throw std::runtime_error(msg.str());
    }

    
}

void ConfigFile::nextToken()
{
    // skip leading spaces/newlines 
}
