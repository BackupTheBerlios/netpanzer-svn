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

void Channel::sendMessage(const std::string& text)
{
    std::stringstream buf;
    buf << "PRIVMSG " << name << " :" << text << "\n";
    connection->send(buf.str());
}

bool Channel::containsClient(Client* client)
{
    std::vector<Client*>::iterator i;
    for(i = clients.begin(); i != clients.end(); ++i) {
        if(*i == client)
            return true;
    }

    return false;
}

void Channel::removeClient(Client* client)
{
    std::vector<Client*>::iterator i;
    for(i = clients.begin(); i != clients.end(); ++i) {
        if(*i == client) {
            clients.erase(i);
            break;
        }
    }
}

} // end of namespace IRC

