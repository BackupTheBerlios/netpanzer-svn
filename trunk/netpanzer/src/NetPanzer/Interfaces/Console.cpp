#include <config.h>

#ifdef USE_READLINE
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#endif

#include "Console.hpp"
#include "GameConfig.hpp"

/** A streambuf implementation that wraps around an existing streambuf and
 * outputs the current timestampe after each newline
 */
class FileStampStreamBuf : public std::streambuf
{
public:
    FileStampStreamBuf()
        : needstamp(true)
    {
        stream = &std::cout;
        setp(buf, buf+sizeof(buf));
    }

    virtual int overflow(int c)
    {
        if(pbase() == pptr())
            return 0;
       
        if(needstamp)
            outputdate();
        needstamp = false;

        // check for \n in the buffer
        char* chunkstart = pbase();
        for(char* p = pbase(); p != pptr(); ++p) {
            if(*p == '\n') {
                // output data in buffer so far
                stream->write(chunkstart, p-chunkstart+1);
                redisplay_prompt();
                if(p < pptr()-1)
                    outputdate();
                else
                    needstamp = true;
                chunkstart = p+1;
            }
        }
        // output the rest
        stream->write(chunkstart, pptr() - chunkstart);

        if(c != traits_type::eof()) {
            *stream << (char) c;
            if(c == '\n') {
                needstamp = true;
                redisplay_prompt();
            }
        }
        setp(buf, buf+sizeof(buf));
        return 0;
    }

    virtual int sync()
    {
        overflow(traits_type::eof());
        stream->flush();
        return 0;
    }

private:
    void outputdate()
    {
        char timestamp[64];
        time_t curtime = time(0);
        struct tm* loctime = localtime(&curtime);
        strftime(timestamp, sizeof(timestamp), "<%F %T>", loctime);
        *stream << "\r" << timestamp;
    }

    void redisplay_prompt()
    {
        // XXX doesn't work reliably...
        //*stream << "netpanzer-server: ";
    }

    std::ostream* stream;

    bool needstamp;
    char buf[1024];
};

class OFileStampStream : public std::ostream
{
public:
    OFileStampStream()
        : std::ostream(new FileStampStreamBuf)
    { }

    ~OFileStampStream()
    {
        delete rdbuf();
    }
};

//---------------------------------------------------------------------------

std::ostream* Console::server;

void Console::initialize()
{
    server = new OFileStampStream();
}

void Console::shutdown()
{
    delete server;
}
