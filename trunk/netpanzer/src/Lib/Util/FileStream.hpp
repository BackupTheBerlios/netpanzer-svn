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
        
        size_t bytesread = file->_read(buf, 1, sizeof(buf));
        setg(buf, buf, buf + bytesread);

        return buf[0];
    }

private:
    ReadFile* file;
    char buf[1024];
};

class OFileStreambuf : public std::streambuf
{
public:
    OFileStreambuf(WriteFile* newfile)
        : file(newfile)
    {
        setp(buf, buf+sizeof(buf));
    }

    ~OFileStreambuf()
    {
        sync();
        delete file;
    }

protected:
    virtual int overflow(int c)
    {
        if(pbase() == pptr())
            return 0;

        try {
            size_t size = pptr() - pbase();
            file->write(pbase(), size, 1);
            if(c != traits_type::eof())
                file->write8(c);
        } catch(std::exception& e) {
            return traits_type::eof();
        }

        setp(buf, buf+sizeof(buf));
        return 0;
    }

    virtual int sync()
    {
        return overflow(traits_type::eof());
    }

private:
    WriteFile* file;
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

    ~IFileStream()
    {
        delete rdbuf();
    }
};

class OFileStream : public std::ostream
{
public:
    OFileStream(WriteFile* file)
        : std::ostream(new OFileStreambuf(file))
    { }

    OFileStream(const std::string& filename)
        : std::ostream(new OFileStreambuf(FileSystem::openWrite(filename)))
    { }

    ~OFileStream()
    {
        delete rdbuf();
    }
};

#endif

