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

#include "ResourceManager.hpp"
#include "2D/Surface.hpp"
#include "Util/FileSystem.hpp"

using namespace std;

#define DEFAULT_FLAGS_PATH "pics/flags/"



Surface noimage;

class _RMan
{
public:
    _RMan()
    {
        for (int n = 0; n < 256; n++)
        {
            flagList[n]=&noimage;
        }
    }
    
    Surface * flagList[256];
};

_RMan RMan;

Surface *
ResourceManager::getEmptyImage()
{
    return &noimage;
}
        
// actually loads all the flags.
int
ResourceManager::loadDefaultFlags()
{
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
}

Surface *
ResourceManager::getFlag(FlagID flag)
{
    return RMan.flagList[flag];
}

bool
ResourceManager::isFlagActive(FlagID flag)
{
    return RMan.flagList[flag] != &noimage;
}
