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
#include <config.h>

#include <stdio.h>
#include <string.h>
#include <physfs.h>
#include "Exception.hpp"
#include "Log.hpp"
#include "FileSystem.hpp"

void FileSystem::initialize(const char* argv0, const char* company,
                            const char* application)
{
    if(!PHYSFS_init(argv0))
        throw Exception("failure while initialising physfs: %s",
                        PHYSFS_getLastError());

    const char* basedir = PHYSFS_getBaseDir();
    const char* userdir = PHYSFS_getUserDir();
    const char* dirsep = PHYSFS_getDirSeparator();
    char* writedir = new char[strlen(userdir) + strlen(application) + 2];

    sprintf(writedir, "%s.%s", userdir, application);
    if(!PHYSFS_setWriteDir(writedir)) {
        // try to create the directory...
        char* mkdir = new char[strlen(application)+2];
        sprintf(mkdir, ".%s", application);
        if(!PHYSFS_setWriteDir(userdir) || ! PHYSFS_mkdir(mkdir)) {
            delete[] writedir;
            delete[] mkdir;
            throw Exception("failed creating configuration directory: '%s': %s",
                            writedir, PHYSFS_getLastError());
        }
        delete[] mkdir;

        if (!PHYSFS_setWriteDir(writedir)) {
            throw Exception("couldn't set configuration directory to '%s': %s",
                            writedir, PHYSFS_getLastError());
        }
    }

    PHYSFS_addToSearchPath(writedir, 0);
    PHYSFS_addToSearchPath(basedir, 1);

    delete[] writedir;

    /* Root out archives, and add them to search path... */
    char* archiveExt = "zip";
    if (archiveExt != NULL) {
        char **rc = PHYSFS_enumerateFiles("/");
        char **i;
        size_t extlen = strlen(archiveExt);
        char *ext;

        for (i = rc; *i != NULL; i++) {
            size_t l = strlen(*i);
            if ((l > extlen) && ((*i)[l - extlen - 1] == '.')) {
                ext = (*i) + (l - extlen);
                if (strcasecmp(ext, archiveExt) == 0) {
                    const char *d = PHYSFS_getRealDir(*i);
                    char* str = new char[strlen(d) + strlen(dirsep) + l + 1];
                    sprintf(str, "%s%s%s", d, dirsep, *i);
                    PHYSFS_addToSearchPath(str, 1);
                    delete[] str;
                } /* if */
            } /* if */
        } /* for */

        PHYSFS_freeList(rc);
    } /* if */
}

void FileSystem::shutdown()
{
    PHYSFS_deinit();
}

void FileSystem::addToSearchPath(const char* directory, bool append)
{
    if(!PHYSFS_addToSearchPath(directory, append))
        throw Exception("Couldn't add '%s' to searchpath: %s", directory,
                        PHYSFS_getLastError());
}

void FileSystem::removeFromSearchPath(const char* directory)
{
    if(!PHYSFS_removeFromSearchPath(directory))
        throw Exception("Couldn't remove '%s' from searchpath: %s", directory,
                        PHYSFS_getLastError());
}

const char* FileSystem::getRealDir(const char* filename)
{
    return PHYSFS_getRealDir(filename);
}

std::string FileSystem::getRealName(const char* filename)
{
    const char* dir = PHYSFS_getRealDir(filename);
    if (dir == 0) {
        throw Exception("no such path '%s'", filename);
    }
    std::string realname = dir;
    realname += PHYSFS_getDirSeparator();
    realname += filename;
    return realname;
}

char** FileSystem::enumerateFiles(const char* directory)
{
    return PHYSFS_enumerateFiles(directory);
}

void FileSystem::freeList(char** list)
{
    PHYSFS_freeList(list);
}

WriteFile* FileSystem::openWrite(const char* filename)
{
    PHYSFS_file* file = PHYSFS_openWrite(filename);
    if(!file)
        throw Exception("couldn't open file '%s' for writing: %s", filename,
                        PHYSFS_getLastError());

    return new WriteFile(file);
}

ReadFile* FileSystem::openRead(const char* filename)
{
    PHYSFS_file* file = PHYSFS_openRead(filename);
    if(!file)
        throw Exception("couldn't open file '%s' for reading: %s", filename,
                        PHYSFS_getLastError());

    return new ReadFile(file);
}

WriteFile* FileSystem::openAppend(const char* filename)
{
    PHYSFS_file* file = PHYSFS_openAppend(filename);
    if(!file)
        throw Exception("couldn't open file '%s' for writing(append): %s",
                        filename, PHYSFS_getLastError());

    return new WriteFile(file);
}

void FileSystem::mkdir(const char* directory)
{
    if(!PHYSFS_mkdir(directory))
        throw Exception("couldn't create directory '%s': %s", directory,
                        PHYSFS_getLastError());
}

void FileSystem::remove(const char* filename)
{
    if(!PHYSFS_delete(filename))
        throw Exception("couldn't remove file '%s': %s", filename,
                        PHYSFS_getLastError());
}

bool FileSystem::exists(const char* filename)
{
    return PHYSFS_exists(filename);
}

bool FileSystem::isDirectory(const char* filename)
{
    return PHYSFS_isDirectory(filename);
}

bool FileSystem::isSymbolicLink(const char* filename)
{
    return PHYSFS_isSymbolicLink(filename);
}

int64_t FileSystem::getLastModTime(const char* filename)
{
    int64_t modtime = PHYSFS_getLastModTime(filename);
    if(modtime < 0)
        throw Exception("couldn't determine modification time of '%s': %s",
                        filename, PHYSFS_getLastError());

    return modtime;
}

//---------------------------------------------------------------------------

File::File(PHYSFS_file* newfile)
        : file(newfile)
{}

File::~File()
{
    PHYSFS_close(file);
}

bool File::eof()
{
    return PHYSFS_eof(file);
}

int64_t File::tell()
{
    return PHYSFS_tell(file);
}

void File::seek(uint64_t position)
{
    if(!PHYSFS_seek(file, position))
        throw Exception("Seek operation failed: %s", PHYSFS_getLastError());
}

int64_t File::fileLength()
{
    return PHYSFS_fileLength(file);
}

void File::setBuffer(uint64_t bufsize)
{
    if(!PHYSFS_setBuffer(file, bufsize))
        throw Exception("couldn't adjust buffer size: %s",
                        PHYSFS_getLastError());
}

void File::flush()
{
    PHYSFS_flush(file); // no exception - what should an app do if flush fails?
}

//---------------------------------------------------------------------------

ReadFile::ReadFile(PHYSFS_file* file)
        : File(file)
{}

int64_t ReadFile::read(void* buffer, size_t objsize, size_t objcount)
{
    return PHYSFS_read(file, buffer, objsize, objcount);
}

bool ReadFile::isEOF()
{
    return PHYSFS_eof(file);
}

SDL_RWops* ReadFile::getSDLRWOps()
{
    SDL_RWops* rwops = (SDL_RWops*) malloc(sizeof(SDL_RWops));
    memset(rwops, 0, sizeof(SDL_RWops));
    rwops->read = RWOps_Read;
    rwops->seek = RWOps_Seek;
    rwops->close = RWOps_Close;
    rwops->hidden.unknown.data1 = this;

    return rwops;
}

int ReadFile::RWOps_Read(SDL_RWops* context, void* ptr, int size, int maxnum)
{
    ReadFile* file = (ReadFile*) context->hidden.unknown.data1;
    return file->read(ptr, size, maxnum);
}

int ReadFile::RWOps_Seek(SDL_RWops* context, int offset, int whence)
{
    ReadFile* file = (ReadFile*) context->hidden.unknown.data1;
    try { // catch exceptions
        switch(whence) {
        case SEEK_SET: file->seek(offset); break;
        case SEEK_CUR: file->seek(file->tell() + offset); break;
        case SEEK_END: file->seek(file->fileLength() + offset); break;
        }
    } catch(...) {
        LOG(("Unexpected exception while seeking in file."));
        return -1;
    }

    return file->tell();
}

int ReadFile::RWOps_Close(SDL_RWops* context)
{
    ReadFile* file = (ReadFile*) context->hidden.unknown.data1;
    delete file;
    context->hidden.unknown.data1 = 0;
    return 1;
}

int8_t ReadFile::read8()
{
    int8_t val;
    if(PHYSFS_read(file, &val, 1, 1) != 1)
        throw Exception("read error: %s", PHYSFS_getLastError());
    return val;
}

int16_t ReadFile::readSLE16()
{
    int16_t val;
    if(!PHYSFS_readSLE16(file, &val))
        throw Exception("read error: %s", PHYSFS_getLastError());
    return val;
}

uint16_t ReadFile::readULE16()
{
    uint16_t val;
    if(!PHYSFS_readULE16(file, &val))
        throw Exception("read error: %s", PHYSFS_getLastError());
    return val;
}

int16_t ReadFile::readSBE16()
{
    int16_t val;
    if(!PHYSFS_readSBE16(file, &val))
        throw Exception("read error: %s", PHYSFS_getLastError());
    return val;
}

uint16_t ReadFile::readUBE16()
{
    uint16_t val;
    if(!PHYSFS_readUBE16(file, &val))
        throw Exception("read error: %s", PHYSFS_getLastError());
    return val;
}

int32_t ReadFile::readSLE32()
{
    int32_t val;
    if(!PHYSFS_readSLE32(file, &val))
        throw Exception("read error: %s", PHYSFS_getLastError());
    return val;
}

uint32_t ReadFile::readULE32()
{
    uint32_t val;
    if(!PHYSFS_readULE32(file, &val))
        throw Exception("read error: %s", PHYSFS_getLastError());
    return val;
}

int32_t ReadFile::readSBE32()
{
    int32_t val;
    if(!PHYSFS_readSBE32(file, &val))
        throw Exception("read error: %s", PHYSFS_getLastError());
    return val;
}

uint32_t ReadFile::readUBE32()
{
    uint32_t val;
    if(!PHYSFS_readUBE32(file, &val))
        throw Exception("read error: %s", PHYSFS_getLastError());
    return val;
}

int64_t ReadFile::readSLE64()
{
    int64_t val;
    if(!PHYSFS_readSLE64(file, &val))
        throw Exception("read error: %s", PHYSFS_getLastError());
    return val;
}

uint64_t ReadFile::readULE64()
{
    uint64_t val;
    if(!PHYSFS_readULE64(file, &val))
        throw Exception("read error: %s", PHYSFS_getLastError());
    return val;
}

int64_t ReadFile::readSBE64()
{
    int64_t val;
    if(!PHYSFS_readSBE64(file, &val))
        throw Exception("read error: %s", PHYSFS_getLastError());
    return val;
}

uint64_t ReadFile::readUBE64()
{
    uint64_t val;
    if(!PHYSFS_readUBE64(file, &val))
        throw Exception("read error: %s", PHYSFS_getLastError());
    return val;
}

void ReadFile::readLine(std::string& buffer)
{
    char c;
    buffer = "";

    while(!isEOF() && (c = read8()) != '\n') {
        buffer += c;
    }
}

//---------------------------------------------------------------------------

WriteFile::WriteFile(PHYSFS_file* file)
        : File(file)
{}

int64_t WriteFile::write(const void* buffer, size_t objsize, size_t objcount)
{
    return PHYSFS_write(file, buffer, objsize, objcount);
}

void WriteFile::write8(int8_t val)
{
    if(PHYSFS_write(file, &val, 1, 1) != 1)
        throw Exception("couldn't write: %s", PHYSFS_getLastError());
}

void WriteFile::writeSLE16(int16_t val)
{
    if(!PHYSFS_writeSLE16(file, val))
        throw Exception("couldn't write: %s", PHYSFS_getLastError());
}

void WriteFile::writeULE16(uint16_t val)
{
    if(!PHYSFS_writeULE16(file, val))
        throw Exception("couldn't write: %s", PHYSFS_getLastError());
}

void WriteFile::writeSBE16(int16_t val)
{
    if(!PHYSFS_writeSBE16(file, val))
        throw Exception("couldn't write: %s", PHYSFS_getLastError());
}

void WriteFile::writeUBE16(uint16_t val)
{
    if(!PHYSFS_writeUBE16(file, val))
        throw Exception("couldn't write: %s", PHYSFS_getLastError());
}

void WriteFile::writeSLE32(int32_t val)
{
    if(!PHYSFS_writeSLE32(file, val))
        throw Exception("couldn't write: %s", PHYSFS_getLastError());
}

void WriteFile::writeULE32(uint32_t val)
{
    if(!PHYSFS_writeULE32(file, val))
        throw Exception("couldn't write: %s", PHYSFS_getLastError());
}

void WriteFile::writeSBE32(int32_t val)
{
    if(!PHYSFS_writeSBE32(file, val))
        throw Exception("couldn't write: %s", PHYSFS_getLastError());
}

void WriteFile::writeUBE32(uint32_t val)
{
    if(!PHYSFS_writeUBE32(file, val))
        throw Exception("couldn't write: %s", PHYSFS_getLastError());
}

void WriteFile::writeSLE64(int64_t val)
{
    if(!PHYSFS_writeSLE64(file, val))
        throw Exception("couldn't write: %s", PHYSFS_getLastError());
}

void WriteFile::writeULE64(uint64_t val)
{
    if(!PHYSFS_writeULE64(file, val))
        throw Exception("couldn't write: %s", PHYSFS_getLastError());
}

void WriteFile::writeSBE64(int64_t val)
{
    if(!PHYSFS_writeSBE64(file, val))
        throw Exception("couldn't write: %s", PHYSFS_getLastError());
}

void WriteFile::writeUBE64(uint64_t val)
{
    if(!PHYSFS_writeUBE64(file, val))
        throw Exception("couldn't write: %s", PHYSFS_getLastError());
}

void WriteFile::writeLine(const std::string& buffer)
{
    if(write(buffer.c_str(), buffer.size(), 1) != 1)
        throw Exception("Couldn't write line: %s", PHYSFS_getLastError());
	
    write8('\n');
}
