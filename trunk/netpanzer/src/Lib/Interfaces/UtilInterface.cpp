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
#include <io.h>
#endif
#include <string.h>
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

// getDrive
//---------------------------------------------------------------------------
String UtilInterface::getDrive(String path)
{
	char strBuf[256];

	// XXX
#ifdef WIN32
	//void _splitpath( const char *path, char *drive, char *dir, char *fname, char *ext );
	_splitpath(path, strBuf, 0, 0, 0);
#endif

	return strBuf;

} // end UtilInterface::getDrive

// getDirectory
//---------------------------------------------------------------------------
String UtilInterface::getDirectory(String path)
{
	char strBuf[256];

	// XXX
#ifdef WIN32
	//void _splitpath( const char *path, char *drive, char *dir, char *fname, char *ext );
	_splitpath(path, 0, strBuf, 0, 0);
#endif

	return strBuf;

} // end UtilInterface::getDirectory

// getFilename
//---------------------------------------------------------------------------
String UtilInterface::getFilename(String path)
{
	char strBuf[256];

	// XXX
#ifdef WIN32
	//void _splitpath( const char *path, char *drive, char *dir, char *fname, char *ext );
	_splitpath(path, 0, 0, strBuf, 0);
#endif

	return strBuf;

} // end UtilInterface::getFilename

// getExtension
//---------------------------------------------------------------------------
String UtilInterface::getExtension(String path)
{
	char strBuf[256];

	// XXX
#ifdef WIN32
	//void _splitpath( const char *path, char *drive, char *dir, char *fname, char *ext );
	_splitpath(path, 0, 0, 0, strBuf);
#endif

	return strBuf;

} // end UtilInterface::getExtension

// getFileSize
//---------------------------------------------------------------------------
// Purpose: Returns file size, in bytes, or -1 if file not found.
//---------------------------------------------------------------------------
DWORD UtilInterface::getFileSize(String filename)
{
	// XXX
#ifdef WIN32
	struct _finddata_t myFile;

	if (_findfirst((const char *) filename, &myFile) == -1)
	{
		return 0;
	}

	return myFile.size;
#endif
	return 0;

} // end UtilInterface::getFileSize

// getNumFilesInDirectory
//---------------------------------------------------------------------------
// Purpose: Returns the number of files in the specified directory.
//---------------------------------------------------------------------------
DWORD UtilInterface::getNumFilesInDirectory(String path)
{
	// XXX
#ifdef WIN32
	struct _finddata_t myFile;
	long               hFile;

	DWORD numFiles = 0;

	// Figure out how many files are in the directory.
    if ((hFile = _findfirst((const char *) path, &myFile)) != -1)
	{
		do
		{
			numFiles++;

		} while (_findnext(hFile, &myFile) == 0);
	} 

	_findclose(hFile);

	return numFiles;
#endif
	return 0;

} // end UtilInterface::getNumFilesInDirectory

// deleteFile
//---------------------------------------------------------------------------
// Purpose: Deletes an individual file, or if wild cards are used, multiple
//          files can be saved.
//---------------------------------------------------------------------------
void UtilInterface::deleteFile(String path)
{
	// XXX
#ifdef WIN32
	struct _finddata_t myFile;
	long               hFile;

	char strBuf[256];

    if ((hFile = _findfirst((const char *) path, &myFile)) == -1L) return;
	else
	{
		do
		{
			_splitpath(myFile.name, 0, 0, strBuf, 0);
			
			remove(strBuf);

		} while (_findnext(hFile, &myFile) == 0);
	}

	_findclose(hFile);
#endif

} // end UtilInterface::deleteFile

// checkFileError
//---------------------------------------------------------------------------
void UtilInterface::checkError(FILE *fp)
{
	if (ferror(fp))
	{
		FUBAR("ERROR: Possible corrupt file.");
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
