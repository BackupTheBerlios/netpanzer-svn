#ifndef __FILESTREAM_HPP__
#define __FILESTREAM_HPP__

#include "FileSystem.hpp"
#include <streambuf>
#include <iostream>

/** This class implements a C++ streambuf object for physfs files.
 * So that you can use normal istream operations on them
 */
class IFileStreambuf : public std::streambuf
{
public:
    IFileStreambuf(ReadFile* newfile)
        : file(newfile)
    {
        // start reading
        underflow();
    }

    ~IFileStreambuf()
    {
        delete file;
    }
    
protected:
    virtual int underflow()
    {
        if(file->isEOF())
            return traits_type::eof();
        
        size_t bytesread = sizeof(buf);
        try {
            file->read(buf, 1, sizeof(buf));
        } catch(FileReadException& e) {
            bytesread = e.getReadCount();
        }
        setg(buf, buf, buf+sizeof(buf));

        return buf[0];
    }

private:
    ReadFile* file;
    char buf[1024];
};

class IFileStream : public std::istream
{
public:
    IFileStream(ReadFile* file)
        : std::istream(new IFileStreambuf(file))
    { }

    IFileStream(const std::string& filename)
        : std::istream(new IFileStreambuf(FileSystem::openRead(filename)))
    { }
};

#endif

