#ifndef __SOCKETSTREAM_HPP__
#define __SOCKETSTREAM_HPP__

#include <SDL_net.h>
#include <streambuf>
#include <iostream>

namespace masterserver
{

/** streambuf implementation for tcp network sockets. This makes them usable as
 * C++ istreams
 */
class SDLNetSocketStreambuf : public std::streambuf
{
public:
    SDLNetSocketStreambuf(TCPsocket newsocket)
        : socket(newsocket)
    { 
        setp(writebuffer, writebuffer + sizeof(writebuffer)-1);
        setg(readbuffer, readbuffer, readbuffer);
    }

    ~SDLNetSocketStreambuf()
    {
        // write remaining buffer to socket
        sync();
        SDLNet_TCP_Close(socket);
    }

protected:
    virtual int sync()
    {
        overflow(traits_type::eof());
        return 0;
    }
    
    virtual int underflow()
    {
        int res = SDLNet_TCP_Recv(socket, readbuffer, sizeof(readbuffer));
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
            int res = SDLNet_TCP_Send(socket, const_cast<void*> (data), len);
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
    TCPsocket socket;
};

class SDLNetSocketStream : public std::iostream
{
public:
    SDLNetSocketStream(TCPsocket socket)
        : std::iostream(new SDLNetSocketStreambuf(socket))
    { }

    ~SDLNetSocketStream()
    {
        delete rdbuf();
    }
};

} // masterserver

#endif

