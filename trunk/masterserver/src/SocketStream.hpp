/*
Copyright (C) 2004 Matthias Braun <matze@braunis.de>
 
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
*/
#ifndef __SOCKETSTREAM_HPP__
#define __SOCKETSTREAM_HPP__

#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <streambuf>
#include <iostream>

/** streambuf implementation for tcp network sockets. This makes them usable as
 * C++ istreams
 */
class SocketStreambuf : public std::streambuf
{
public:
    SocketStreambuf(int newfd)
        : fd(newfd)
    { 
        setp(writebuffer, writebuffer + sizeof(writebuffer)-1);
        setg(readbuffer, readbuffer, readbuffer);
    }

    ~SocketStreambuf()
    {
        // write remaining buffer to socket
        sync();
        close(fd);
    }

protected:
    virtual int sync()
    {
        overflow(traits_type::eof());
        return 0;
    }
    
    virtual int underflow()
    {
        int res = recv(fd, readbuffer, sizeof(readbuffer), 0);
        if(res < 0) {
            return traits_type::eof();
        }
        setg(readbuffer, readbuffer, readbuffer+res);
        return readbuffer[0];
    }

    virtual int overflow(int c)
    {
        size_t len = size_t(pptr() - pbase());
        if(c != traits_type::eof()) {
            *pptr() = c;
            ++len;
        }
        // anything to send out
        if(len > 0) {
            const void* data = pbase();
            int res = send(fd, data, len, 0);
            if(res <= 0) {
                return traits_type::eof();
            }
            setp(writebuffer, writebuffer+sizeof(writebuffer)-1);
        }

        return 0;
    }

private:
    char readbuffer[1024];
    char writebuffer[1024];
    int fd;
};

class SocketStream : public std::iostream
{
public:
    SocketStream(int fd)
        : std::iostream(new SocketStreambuf(fd))
    { }

    ~SocketStream()
    {
        delete rdbuf();
    }
};

#endif

