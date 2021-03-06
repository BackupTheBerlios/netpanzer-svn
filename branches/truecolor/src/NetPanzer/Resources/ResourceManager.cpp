/* 
Copyright (C) 2008 by Aaron Perez <aaronps@gmail.com>

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
 *
 * Created on September 11, 2008, 7:05 PM
 */

#include <vector>
#include <algorithm>
#include <cstring>

#include "ResourceManager.hpp"
#include "Util/Log.hpp"
#include "2D/Surface.hpp"
#include "Util/FileSystem.hpp"
#include "Classes/Network/NetMessage.hpp"
#include "ResourceManagerMessages.hpp"
#include "Util/Exception.hpp"

using namespace std;

Surface noimage;

class _RMan
{
public:
    _RMan()
    {
        for (int n = 0; n < 256; n++)
        {
            flagList[n]=&noimage;
            usedList[n]=false;
            flagUsedCount[n] = 0;
        }
    }
    
    Surface * flagList[256];
    bool usedList[256];
    unsigned char flagUsedCount[256];
};

_RMan RMan;

Surface *
ResourceManager::getEmptyImage()
{
    return &noimage;
}
        
int
ResourceManager::loadDefaultFlags()
{
    noimage.create(20,14,1);
    noimage.fill(0);
    for (int n = 0; n < 256; n++ )
    {
        if ( RMan.flagList[n] == &noimage)
        {
            RMan.flagList[n] = new Surface(20, 14, 1, 8);
        }
        RMan.flagList[n]->fill(0);
        RMan.usedList[n] = false;
    }
/*
    char** list = filesystem::enumerateFiles(DEFAULT_FLAGS_PATH);
    
    vector<string> filenames;
    string flagname;
    
    for(char** file = list; *file != 0; file++)
    {
        flagname = DEFAULT_FLAGS_PATH;
        flagname += *file;
        if(flagname.find(".bmp") != string::npos)
        {
            filenames.push_back(flagname);
        }
    }

    filesystem::freeList(list);

    sort(filenames.begin(), filenames.end());

    // Now load in the sorted BMP names.
    for (unsigned int i = 0; i < filenames.size(); i++)
    {
        if ( RMan.flagList[i] != &noimage )
            delete RMan.flagList[i];
        
        RMan.flagList[i] = new Surface();
        // XXX check for null man
        RMan.flagList[i]->loadBMP(filenames[i].c_str());
    }
    
    return filenames.size();
 */
    return 1;
}

Surface *
ResourceManager::getFlag(FlagID flag)
{
    return RMan.flagList[flag];
}

bool
ResourceManager::isFlagActive(FlagID flag)
{
    return RMan.usedList[flag];
}

void
ResourceManager::getFlagData(const char * flagname, Uint8 *dest)
{
    char fname[512];
    snprintf(fname, sizeof(fname), DEFAULT_FLAGS_PATH "%s", flagname);
    fname[sizeof(fname)-1] = 0;
    
    Surface t;
    try
    {
        t.loadBMP(fname);
    }
    catch (Exception e)
    {
        LOGGER.warning("Error loading flag '%s'", flagname);
        t.copy(*ResourceManager::getEmptyImage());
    }
    
    t.getRawDataFromPixels(dest, 20*14); // size of a flag
}

FlagID
ResourceManager::registerFlagFromData(Uint8 *flagdata)
{
    // Asume always has free flags.
    int i=0;
    for (i = 0; i<256; i++)
    {
        if ( !isFlagActive(i) )
        {
            LOGGER.warning("Flag '%d' is free", i);
            syncFlagFromData(i,flagdata);
            break;
        }
        else
        {
            LOGGER.warning("Flag '%d' is used", i);
        }
    }
    LOGGER.warning("Using flag '%d'", i);
    return i;
}

void
ResourceManager::getFlagSyncData(FlagID flag, Uint8 *dest)
{
    Surface * flagsurf = getFlag(flag);
    flagsurf->getRawDataFromPixels(dest, 20*14); // size of flag;
}

int
ResourceManager::getFlagUsedCount(FlagID flag)
{
    return RMan.flagUsedCount[flag];
}

void
ResourceManager::syncFlagFromData(FlagID flag, Uint8 *flagdata)
{
    RMan.flagList[flag]->setPixelsFromRawData(flagdata, 20*14);
    RMan.usedList[flag]=true;
    ++RMan.flagUsedCount[flag];
}

void
ResourceManager::releaseFlag(FlagID flag)
{
    RMan.usedList[flag]=false;
}

void
ResourceManager::processResourceMessage(const NetMessage* message)
{
    switch ( message->message_id )
    {
        case _resource_message_id_sync_flag:
        {
            ResourceManagerSyncFlagMessage *msg = (ResourceManagerSyncFlagMessage*)message;
            LOGGER.warning("Received flag %d synchronization", msg->getFlagID());
            syncFlagFromData(msg->getFlagID(), msg->flagdata);
            break;
        }
        case _resource_message_id_release_flag:
            releaseFlag(((ResourceManagerReleaseFlagMessage*)message)->getFlagID());
            break;
    }
}

