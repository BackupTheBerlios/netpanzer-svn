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

#include <physfs.h>
#include "Exception.hpp"
#include "FileSystem.hpp"

void FileSystem::initialize(const char* argv0, const char* company,
							const char* application)
{
	if(!PHYSFS_init(argv0) ||
	   !PHYSFS_setSaneConfig(company, application, "zip", 0, 0))
		throw Exception("failure while initialising physfs: %s",
							PHYSFS_getLastError());
}

void FileSystem::shutdown()
{
	PHYSFS_deinit();
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
{
}

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
{
}

int64_t ReadFile::read(void* buffer, size_t objsize, size_t objcount)
{
	return PHYSFS_read(file, buffer, objsize, objcount);
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

//---------------------------------------------------------------------------

WriteFile::WriteFile(PHYSFS_file* file)
	: File(file)
{
}

int64_t WriteFile::write(const void* buffer, size_t objsize, size_t objcount)
{
	return PHYSFS_write(file, buffer, objsize, objcount);
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

