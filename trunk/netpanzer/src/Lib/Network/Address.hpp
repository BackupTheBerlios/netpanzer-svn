#ifndef __ADDRESS_HPP__
#define __ADDRESS_HPP__

#include <stdint.h>
#include <string>
#include "SocketHeaders.hpp"

namespace network
{

class Address
{
public:
    Address();
    Address(const Address& other);
    
    /** resolves a hostname or IP-Number together with a port and returns a
     * new Address object.
     */
    static Address Address::resolve(const std::string& name, uint16_t port);

    /** returns the ip address of this Address as string */
    std::string getIP() const;
    /** returns the port of this address */
    uint16_t getPort() const;

    void operator=(const Address& other);
    bool operator==(const Address& other) const;

private:
    friend class TCPSocket;
    friend class TCPListenSocket;
    friend class UDPSocket;
    struct sockaddr_in addr;
};

}

#endif

