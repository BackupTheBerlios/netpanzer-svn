#include <config.h>

#include <stdexcept>
#include <sstream>

#include "SocketHeaders.hpp"
#include "Address.hpp"

namespace network
{

Address::Address()
{
    memset(&addr, 0, sizeof(addr));
}

Address::Address(const Address& other)
{
    memcpy(&addr, &other.addr, sizeof(addr));
}

void
Address::operator=(const Address& other)
{
    memcpy(&addr, &other.addr, sizeof(addr));
}

bool
Address::operator==(const Address& other) const
{
    if(addr.sin_addr.s_addr == other.addr.sin_addr.s_addr
            && addr.sin_port == other.addr.sin_port)
        return true;

    return false;
}

std::string
Address::getIP() const
{
    return std::string(inet_ntoa(addr.sin_addr));
}

uint16_t
Address::getPort() const
{
    return ntohs(addr.sin_port);
}

Address
Address::resolve(const std::string& name, uint16_t port)
{
    struct hostent* hentry = gethostbyname(name.c_str());
    if(!hentry) {
#ifdef WINSOCK
        std::stringstream msg;
        msg << "Couldn't resolve address '" << name << "' (code "
            << WSAGetLastError() << ")";
        throw std::runtime_error(msg.str());
#else
        std::stringstream msg;
        msg << "Couldn't resolve address '" << name << "': "
            << hstrerror(h_errno);
        throw std::runtime_error(msg.str());
#endif
    }

    Address result;
    result.addr.sin_family = AF_INET;
    result.addr.sin_addr.s_addr = ((struct in_addr*) hentry->h_addr)->s_addr;
    result.addr.sin_port = htons(port);

    return result;
}

}
