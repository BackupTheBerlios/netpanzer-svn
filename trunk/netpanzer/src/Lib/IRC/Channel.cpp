#include <config.h>

#include <sstream>

#include "Connection.hpp"
#include "Channel.hpp"

namespace IRC
{

Channel::Channel(Connection* newconnection, const std::string& newname)
    : connection(newconnection), name(newname)
{
}

Channel::~Channel()
{
    std::stringstream buf;
    buf << "PART " << name << "\n";
    connection->send(buf.str());
}

void Channel::send(const std::string& text)
{
    std::stringstream buf;
    buf << "MSG " << name << " " << text << "\n";
    connection->send(buf.str());
}

} // end of namespace IRC

