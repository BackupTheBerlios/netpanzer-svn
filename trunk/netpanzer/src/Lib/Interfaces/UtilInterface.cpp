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
#include <config.h>
#include <sstream>

#include <sys/stat.h>
#include <string.h>
#include "SplitPath.hpp"
#include "Exception.hpp"
#include "FileSystem.hpp"
#include "GameConfig.hpp"
#include "UtilInterface.hpp"
#include "Log.hpp"

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
size_t UtilInterface::getFileSize(String filename)
{
    ReadFile* file = FileSystem::openRead(filename);
    size_t size = file->length();
    delete file;

    return size;
} // end UtilInterface::getFileSize

// getNumFilesInDirectory
//---------------------------------------------------------------------------
// Purpose: Returns the number of files in the specified directory.
//---------------------------------------------------------------------------
int UtilInterface::getNumFilesInDirectory(String path)
{
    char** list = FileSystem::enumerateFiles(path);
    int numfiles=0;
    for(char** file = list; *file != 0; file++)
	numfiles++;
    FileSystem::freeList(list);

    return numfiles;
} // end UtilInterface::getNumFilesInDirectory

// deleteFile
//---------------------------------------------------------------------------
// Purpose: Deletes an individual file, or if wild cards are used, multiple
//          files can be saved.
//---------------------------------------------------------------------------
void UtilInterface::deleteFile(String path)
{
    if (remove(path) < 0)
        printf("Couldn't remove file '%s'.\n", (const char*) path);
} // end UtilInterface::deleteFile

// checkFileError
//---------------------------------------------------------------------------
void UtilInterface::checkError(FILE *fp)
{
    if (ferror(fp)) {
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
#else
    srand((unsigned)time(0));
#endif
} // end UtilInterface::startRandomNumberGenerator



// split server:port string, doesn't always set the port
void UtilInterface::splitServerPort(const std::string& server,std::string& address,int *port)
{
    unsigned int colon=server.find(':',0);
    if(colon==std::string::npos) {
        address=server;
    }
    else {
        address=server.substr(0,colon);
        colon++;
        std::string port_str(server.substr(colon,server.length()-colon));
        port[0]=atoi(port_str.c_str());
    }
}


void UtilInterface::makeBase64(std::string &base64,std::string &str)
{
    static const char base64_chars[65]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const char *auth_ptr;
    char auth_ptr_dest[5];

    auth_ptr_dest[4]=0;
    base64="";
    for(auth_ptr=str.c_str(); *auth_ptr; auth_ptr+=3) {
        auth_ptr_dest[0]='=';
        auth_ptr_dest[1]='=';
        auth_ptr_dest[2]='=';
        auth_ptr_dest[3]='=';

        auth_ptr_dest[0]=base64_chars[(auth_ptr[0]&0xfc) >>2];
        auth_ptr_dest[1]=base64_chars[((auth_ptr[0]&0x3) <<4)|((auth_ptr[1]&0xf0)>>4)]; 
        if(auth_ptr[1]==0) { base64+=auth_ptr_dest; break; }
        auth_ptr_dest[2]=base64_chars[((auth_ptr[1]&0xf) <<2)|((auth_ptr[2]&0xc0)>>6)];
        if(auth_ptr[2]==0) { base64+=auth_ptr_dest; break; }
        auth_ptr_dest[3]=base64_chars[((auth_ptr[2]&0x3f))];
        base64+=auth_ptr_dest;
    }
}


void UtilInterface::getProxyConnect(std::stringstream &buffer,const std::string &serveraddress) {
    if(((const std::string &)gameconfig->proxyserver).size()>0) {
        buffer << "CONNECT " << serveraddress << " HTTP/1.0\r\n";
        if(((const std::string &)gameconfig->proxyserveruser).size()>0) {
            std::string base64;
            std::string userpass( ((const std::string &)gameconfig->proxyserveruser) +":"+((const std::string &)gameconfig->proxyserverpass) );
            UtilInterface::makeBase64(base64, userpass);
            buffer << "Authorization: Basic " << base64 << "\r\n";
        }
        buffer << "\r\n";
    }
}


void UtilInterface::sendProxyConnect(TCPsocket socket,const std::string &serveraddress)
{
    std::stringstream buffer;

    getProxyConnect(buffer,serveraddress);

    SDLNet_TCP_Send(socket,const_cast<char*> (buffer.str().c_str()),buffer.str().size());
    int lfs=0;
// XXX grab any http error messages
    while(1) {
        char ch;
        if(SDLNet_TCP_Recv(socket,&ch,1)!=1) { break; }
        if(ch=='\r') { continue; }
        if(ch=='\n') { lfs++; }
        else { lfs=0; }
        if(lfs>=2) { break; }
    }
}


