/*
Copyright (C) 1998 Pyrosoft Inc. (www.pyrosoftgames.com), Matthew Bogue

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
#ifdef WIN32
#include <config.h>
#include <io.h>
#else
#include <sys/stat.h>
#endif
#include <string.h>
#include "SplitPath.hpp"
#include "FindFirst.hpp"
#include "Exception.hpp"
#include "UtilInterface.hpp"

bool gSpanBlittingFlag = false;

// FilenameSortFunction
//---------------------------------------------------------------------------
int FilenameSortFunction(const void *a, const void *b)
{
	const Filename *p1 = (const Filename *)a;
	const Filename *p2 = (const Filename *)b;

	return strcasecmp(p1->name, p2->name);

} // end FilenameSortFunction


/////////////////////////////////////////////////////////////////////////////
// UtilInterface
/////////////////////////////////////////////////////////////////////////////

// getDirectory
//---------------------------------------------------------------------------
String UtilInterface::getDirectory(String path)
{
	char strBuf[256];

	_splitpath(path, 0, strBuf, 0, 0);

	return strBuf;

} // end UtilInterface::getDirectory

// getFilename
//---------------------------------------------------------------------------
String UtilInterface::getFilename(String path)
{
	char strBuf[256];

	_splitpath(path, 0, 0, strBuf, 0);

	return strBuf;

} // end UtilInterface::getFilename

// getExtension
//---------------------------------------------------------------------------
String UtilInterface::getExtension(String path)
{
	char strBuf[256];

	_splitpath(path, 0, 0, 0, strBuf);

	return strBuf;

} // end UtilInterface::getExtension

// getFileSize
//---------------------------------------------------------------------------
// Purpose: Returns file size, in bytes, or 0 if file not found.
//---------------------------------------------------------------------------
DWORD UtilInterface::getFileSize(String filename)
{
#ifdef WIN32
	struct _finddata_t myFile;

	// XXX small memory leak here
	if (_findfirst((const char *) filename, &myFile) == ((int*) -1))
	{
		return 0;
	}

	return myFile.size;
#else
	struct stat filestats;
	if (stat(filename, &filestats) < 0)
		return 0;

	return (DWORD) filestats.st_size;
#endif
} // end UtilInterface::getFileSize

// getNumFilesInDirectory
//---------------------------------------------------------------------------
// Purpose: Returns the number of files in the specified directory.
//---------------------------------------------------------------------------
DWORD UtilInterface::getNumFilesInDirectory(String path)
{
	struct _finddata_t myFile;
	int* hFile;

	DWORD numFiles = 0;

	// Figure out how many files are in the directory.
    if ((hFile = _findfirst((const char *) path, &myFile)) != ((int*) -1))
	{
		do
		{
			numFiles++;

		} while (_findnext(hFile, &myFile) == 0);
		_findclose(hFile);
	} 

	return numFiles;
} // end UtilInterface::getNumFilesInDirectory

// deleteFile
//---------------------------------------------------------------------------
// Purpose: Deletes an individual file, or if wild cards are used, multiple
//          files can be saved.
//---------------------------------------------------------------------------
void UtilInterface::deleteFile(String path)
{
#ifdef WIN32
	struct _finddata_t myFile;
	int* hFile;

	char strBuf[256];

    if ((hFile = _findfirst((const char *) path, &myFile)) == ((int*) -1)) return;
	else
	{
		do
		{
			_splitpath(myFile.name, 0, 0, strBuf, 0);
			
			remove(strBuf);

		} while (_findnext(hFile, &myFile) == 0);
	}

	_findclose(hFile);
#else
	if (remove(path) < 0)
		printf("Couldn't remove file '%s'.\n", (const char*) path);
#endif
} // end UtilInterface::deleteFile

// checkFileError
//---------------------------------------------------------------------------
void UtilInterface::checkError(FILE *fp)
{
	if (ferror(fp))
	{
		throw Exception("ERROR: Possible corrupt file.");
	}

} // end UtilInterface::checkError

// startRandomNumberGenerator
//---------------------------------------------------------------------------
void UtilInterface::startRandomNumberGenerator()
{
#ifdef _DEBUG	
	srand(0);
	return;
#endif 
	srand((unsigned)time(0));
} // end UtilInterface::startRandomNumberGenerator
