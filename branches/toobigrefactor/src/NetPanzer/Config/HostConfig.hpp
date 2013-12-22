/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Created on September 15, 2013, 11:59 AM
 */

#ifndef HOSTCONFIG_HPP
#define	HOSTCONFIG_HPP

#include "Core/CoreTypes.hpp"

class HostConfig
{
public:
    HostConfig();
    ~HostConfig();
    
    inline uint16_t getPort() const { return port; }
    inline const NPString& getBindAddress() const { return bindAddress; }
    inline const NPString& getMOTD() const { return motd; }
    inline const NPString& getName() const { return name; }
    inline bool isLogging() const { return logging; }
    inline bool isPublic() const { return _public; }
    
    inline void setPort(const uint16_t p) { port = p; }
    inline void setBindAddress(const NPString& ba) { bindAddress = ba; }
    inline void setMOTD(const NPString& m) { motd = m; }
    inline void setName(const NPString& n) { name = n; }
    inline void setLogging(const bool b) { logging = b; }
    inline void setPublic(const bool b) { _public = b; }
    
private:
    HostConfig(const HostConfig& );
    
    uint16_t port;
    NPString bindAddress;
    NPString motd;
    NPString name;
    bool     logging;
    bool     _public;
};

#endif	/* HOSTCONFIG_HPP */

