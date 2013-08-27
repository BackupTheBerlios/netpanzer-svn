/*
Copyright (C) 2003 Matthias Braun <matze@braunis.de>
                                                                                
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
/** This is a thin C++ wrapper around libphysfs */
#ifndef __LIB_FILESYSTEM_HPP__
#define __LIB_FILESYSTEM_HPP__

#include "Exception.hpp"
#include <stdlib.h>
#include "physfs/physfs.h"
#include <stdint.h>
#include <string>

namespace filesystem
{

class File
{
public:
    File() : file(0) {}
    ~File() { close(); }

    bool eof();
    int64_t tell();
    void seek(uint64_t position);
    int64_t fileLength();
    // conveniance function, since I think this name is more c++ typic
    int64_t length()
    {
        return fileLength();
    }

    void setBuffer(uint64_t bufsize);
    void flush();
    
    bool isOpen() const { return file != 0; }
    
    void close()
    {
        if ( isOpen() )
        {
            PHYSFS_close(file);
            file = 0;
        }
    }

protected:
    File(PHYSFS_file* file) : file(file) {}
    File(const File&);
    void operator=(const File&);

    PHYSFS_file* file;
};

class FileSystem;

class ReadFile : public File
{
public:
    ReadFile(const std::string& name);
    
    bool isEOF();	

    void read(void* buffer, size_t objsize, size_t objcount);
    size_t _read(void* buffer, size_t objsize, size_t objcount);

    int8_t read8();

    int16_t readSLE16();
    uint16_t readULE16();
    int16_t readSBE16();
    uint16_t readUBE16();

    int32_t readSLE32();
    uint32_t readULE32();
    int32_t readSBE32();
    uint32_t readUBE32();

    int64_t readSLE64();
    uint64_t readULE64();
    int64_t readSBE64();
    uint64_t readUBE64();

    void readLine(std::string& buffer);

    /** for internal use only */
    ReadFile(PHYSFS_file* file);
};

//---------------------------------------------------------------------------

class WriteFile : public File
{
public:
    void write(const void* buffer, size_t objsize, size_t objcount);

    void write8(int8_t val);

    void writeSLE16(int16_t val);
    void writeULE16(uint16_t val);
    void writeSBE16(int16_t val);
    void writeUBE16(uint16_t val);

    void writeSLE32(int32_t val);
    void writeULE32(uint32_t val);
    void writeSBE32(int32_t val);
    void writeUBE32(uint32_t val);

    void writeSLE64(int64_t val);
    void writeULE64(uint64_t val);
    void writeSBE64(int64_t val);
    void writeUBE64(uint64_t val);

    /// writes the text in the buffer and an additional newline
    void writeLine(const std::string& line);

    /** for inernal use only */
    WriteFile(PHYSFS_file* file);
};

//---------------------------------------------------------------------------

/** This class is a c++ wrapper for the physfs library. See physfs
 * documentation for details about the functions here. Most function names are
 * exactly the same as in physfs
 */
bool initialize( const char* argv0, const char* applicationname );
void shutdown();

void addToSearchPath(const char* dir, bool append = true);
void removeFromSearchPath(const char* dir);

const char* getRealDir(const char* filename);
std::string getRealName(const char* filename);
std::string getRealWriteName(const char* filename);
/// remember to call freeLisT
char** enumerateFiles(const char* directory);
static inline char** enumerateFiles(const std::string& directory)
{ return enumerateFiles(directory.c_str()); }
void freeList(char** list);

WriteFile* openAppend(const char* filename);
static inline WriteFile* openAppend(const std::string& filename)
{ return openAppend(filename.c_str()); }
WriteFile* openWrite(const char* filename);
static inline WriteFile* openWrite(const std::string& filename)
{ return openWrite(filename.c_str()); }

void mkdir(const char* dirname);
static inline void mkdir(const std::string& dirname)
{ mkdir(dirname.c_str()); }
void remove(const char* filename);
static inline void remove(const std::string& filename)
{ remove(filename.c_str()); }

bool exists(const char* filename);
static inline bool exists(const std::string& filename)
{ return exists(filename.c_str()); }
bool isDirectory(const char* filename);
static inline bool isDirectory(const std::string& filename)
{ return isDirectory(filename.c_str()); }
bool isSymbolicLink(const char* filename);
static inline bool isSymbolicLink(const std::string& filename)
{ return isSymbolicLink(filename.c_str()); }
int64_t getLastModTime(const char* filename);
static inline int64_t getLastModTime(const std::string& filename)
{ return getLastModTime(filename.c_str()); }

//---------------------------------------------------------------------------

class FileReadException : public Exception
{
public:
    FileReadException(size_t objread, size_t objrequested,
	    const std::string& msg) throw();
    virtual ~FileReadException() throw();
    
    size_t getReadCount() const throw()
    { return objread; }
    size_t getRequestCount() const throw()
    { return objrequested; }
private:
    size_t objread, objrequested;
};

}

#endif
