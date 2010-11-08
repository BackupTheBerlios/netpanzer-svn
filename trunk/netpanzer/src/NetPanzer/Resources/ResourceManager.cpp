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

class _RMan
{
public:
    _RMan()
    {
        flagList = new Surface*[256];
        for (int n = 0; n < 256; n++)
        {
            flagList[n]=new Surface(20,14,1);
            flagList[n]->fill(0);
        }
    }

    ~_RMan()
    {
        if ( flagList )
        {
            for (int n = 0; n < 256; n++)
            {
                delete flagList[n];
            }
            delete[] flagList;
            flagList = 0;
        }
    }
    
    Surface ** flagList;
};

_RMan *RMan = 0;

void
ResourceManager::initialize()
{
    if ( ! RMan )
    {
        RMan = new _RMan();
    }
}

void
ResourceManager::finalize()
{
    if ( RMan )
    {
        delete RMan;
        RMan = 0;
    }
}

// actually loads all the flags.
int
ResourceManager::loadAllFlags(Surface& flags, vector<string>& names)
{
    char** list = filesystem::enumerateFiles(DEFAULT_FLAGS_PATH);
    string flagname;
    
    for(char** file = list; *file != 0; file++)
    {
        flagname = *file;
        if ( flagname.find(".bmp") != string::npos )
        {
            names.push_back(flagname);
        }
    }

    filesystem::freeList(list);

    flags.create( FLAG_WIDTH, FLAG_HEIGHT, names.size());

    sort(names.begin(), names.end());
    string path(DEFAULT_FLAGS_PATH);

    for (vector<string>::size_type i = 0; i < names.size(); i++)
    {
        flags.setFrame(i);
        flags.loadBMP((path+names[i]).c_str(), false);
    }
    
    return names.size();
}

bool
ResourceManager::loadFlag(Surface* dest, string name)
{
    dest->loadBMP((DEFAULT_FLAGS_PATH + name).c_str());
    return true;
}

Surface *
ResourceManager::getFlag(FlagID flag)
{
    return RMan->flagList[flag];
}
