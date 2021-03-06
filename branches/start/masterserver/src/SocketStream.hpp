#ifndef __SOCKETSTREAM_HPP__
#define __SOCKETSTREAM_HPP__

#include <sys/socket.h>
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

