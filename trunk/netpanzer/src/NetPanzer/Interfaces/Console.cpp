#include <config.h>

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
        std::cout << "New\n";
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
                if(p < epptr()-1)
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
            if(c == '\n')
                needstamp = true;
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
        *stream << timestamp;
    }

    void output(const char* data, size_t len)
    {
        
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
};

//---------------------------------------------------------------------------

class ScreenMessageBuf : public std::streambuf
{
public:
    ScreenMessageBuf();
    virtual ~ScreenMessageBuf();

protected:
    virtual int overflow(int c)
    {
        return 0;
    }

private:
    
};

std::ostream* Console::server;
std::ostream* Console::clientscreen = &std::cout;

void Console::initialize()
{
    server = new OFileStampStream();
    clientscreen = &std::cout;
}

