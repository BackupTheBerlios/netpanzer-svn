#ifndef __SOCKETSTREAM_HPP__
#define __SOCKETSTREAM_HPP__

#include <streambuf>
#include <iostream>
#include "SocketHeaders.hpp"
#include "TCPSocket.hpp"

namespace network
{

/** streambuf implementation for tcp network sockets. This makes them usable as
 * C++ istreams
 */
class SocketStreamBuf : public std::streambuf
{
public:
    SocketStreamBuf(TCPSocket& newsocket)
        : socket(newsocket)
    { 
        setp(writebuffer, writebuffer + sizeof(writebuffer)-1);
        setg(readbuffer, readbuffer, readbuffer);
    }

    ~SocketStreamBuf()
    {
        // write remaining buffer to socket
        sync();
    }

protected:
    virtual int sync()
    {
        overflow(traits_type::eof());
        return 0;
    }
    
    virtual int underflow()
    {
        int res = recv(socket.sockfd,(char*) readbuffer, sizeof(readbuffer), 0);
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
            int res = send(socket.sockfd, (const char*) data, len, 0);
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
    TCPSocket& socket;
};

class SocketStream : public std::iostream
{
public:
    SocketStream(TCPSocket& socket)
        : std::iostream(new SocketStreamBuf(socket))
    { }

    ~SocketStream()
    {
        delete rdbuf();
    }
};

}

#endif

