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

#include <stdint.h>
#include <stdlib.h>
#include <physfs.h>

//---------------------------------------------------------------------------

class File
{
public:
	~File();

	bool eof();
	int64_t tell();
	void seek(uint64_t position);
	int64_t fileLength();
	// conveniance function, since I think this name is more c++ typic
	int64_t length()
	{ return fileLength(); }

	void setBuffer(uint64_t bufsize);
	void flush();

protected:
	File(PHYSFS_file* file);

	PHYSFS_file* file;
};

//---------------------------------------------------------------------------

class ReadFile : public File
{
public:
	int64_t read(void* buffer, size_t objsize, size_t objcount);

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

protected:
	ReadFile(PHYSFS_file* file);
	friend class FileSystem;
};

//---------------------------------------------------------------------------

class WriteFile : public File
{
public:
	int64_t write(const void* buffer, size_t objsize, size_t objcount);

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

protected:
	WriteFile(PHYSFS_file* file);
	friend class FileSystem;
};

//---------------------------------------------------------------------------

class FileSystem
{
public:
	static void initialize(const char* argv0, const char* company,
						   const char* applicationname);
	static void shutdown();
	
	static ReadFile* openRead(const char* filename);
	static WriteFile* openAppend(const char* filename);
	static WriteFile* openWrite(const char* filename);

	static void mkdir(const char* dirname);
	static void remove(const char* filename);
	
	static bool exists(const char* filename);
	static bool isDirectory(const char* filename);
	static bool isSymbolicLink(const char* filename);
	static int64_t getLastModTime(const char* filename);
};

#endif

