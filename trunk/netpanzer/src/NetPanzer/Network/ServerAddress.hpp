/* 
 * File:   ServerAddress.hpp
 * Author: krom
 *
 * Created on September 14, 2013, 12:24 PM
 */

#ifndef SERVERADDRESS_HPP
#define	SERVERADDRESS_HPP

#include "Core/CoreTypes.hpp"

struct ServerAddress
{
    ServerAddress(const NPString& ip, const uint16_t port) : ip(ip), port(port) {}
    
    NPString ip;
    uint16_t port;
};

#endif	/* SERVERADDRESS_HPP */

