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
#include <map>
#include <algorithm>

#include "ResourceManager.hpp"
#include "2D/Surface.hpp"
#include "Util/FileSystem.hpp"
#include "Util/FileStream.hpp"
#include "Util/Log.hpp"
#include "Util/StringUtil.hpp"
#include "ArrayUtil/PtrArray.hpp"
#include "Classes/MapFile.hpp"

#define DEFAULT_FLAGS_PATH "pics/flags/"
#define DEFAULT_MAPS_PATH "maps/"

typedef std::map<NPString, MapFile*> MapList;

class _RMan
{
public:
    _RMan()
    {
        for (int n = 0; n < 256; n++)
        {
            flagList[n]=new Surface(20,14);
            flagList[n]->fill(0);
            flagUsedCount[n] = 0;
        }
    }

    ~_RMan()
    {
        if ( flagList )
        {
            for (int n = 0; n < 256; n++)
            {
                delete flagList[n];
                flagList[n] = 0;
                flagUsedCount[n] = 0;
            }
        }
    }
    
    Surface * flagList[256];
    unsigned char flagUsedCount[256];
    
    MapList mapList;
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
ResourceManager::loadAllFlags( PtrArray<Surface>& flags )
{
    PtrArray<char> fnames;
    
    char** list = filesystem::enumerateFiles(DEFAULT_FLAGS_PATH);
    NPString flagname;
    
    for(char** file = list; *file != 0; file++)
    {
        int flen = strlen(*file) - 4;
        if ( (flen > 0) && (strcmp((*file) + flen, ".bmp") == 0) )
        {
            fnames.push_back(*file);
        }
    }


    std::sort(fnames.begin(), fnames.end(), StringUtil::cstr_sorter());
    
    Surface * tmp;
    char fil[512];
    int len = strlen(DEFAULT_FLAGS_PATH);
    memcpy(fil, DEFAULT_FLAGS_PATH, len);

    for (size_t i = 0; i < fnames.size(); i++)
    {
        tmp = new Surface();
        strcpy(fil+len, fnames[i]);
        tmp->loadBMP(fil);
        flags.push_back(tmp);
    }
    
    filesystem::freeList(list);
    return flags.size();
}

bool
ResourceManager::loadFlag(Surface* dest, const NPString& name)
{
    dest->loadBMP((DEFAULT_FLAGS_PATH + name).c_str());
    return true;
}

Surface *
ResourceManager::getFlag(const FlagID flag)
{
    return RMan->flagList[flag];
}

void
ResourceManager::getFlagData(const FlagID flag, Uint8 * dest, const size_t dest_len)
{
    RMan->flagList[flag]->frameToBuffer(dest, dest_len);
}

void
ResourceManager::updateFlagData(const FlagID flag, const Uint8 * src, const size_t src_len)
{
    RMan->flagUsedCount[flag] += 1;
    RMan->flagList[flag]->bufferToFrame(src, src_len);
}

int
ResourceManager::getFlagUsedCount(const FlagID flag)
{
    return RMan->flagUsedCount[flag];
}

// ----------------------- MAPS ---------------------

#define MAP_HEADER_SIZE ( sizeof(MapFile::netp_id_header) \
                        + sizeof(MapFile::id) \
                        + sizeof(MapFile::name) \
                        + sizeof(MapFile::description) \
                        + sizeof(MapFile::width) \
                        + sizeof(MapFile::height) \
                        + sizeof(MapFile::tile_set) \
                        )

static MapFile* loadMapInfo(const NPString& name)
{
    MapFile * mapfile = 0;
    NPString fullname(DEFAULT_MAPS_PATH);
    fullname += name;
    fullname += ".npm";
        
    filesystem::ReadFile file(fullname);
    if ( file.isOpen() )
    {
        mapfile = new MapFile();
        mapfile->filename = name;
        mapfile->fullfilename = fullname;
        
        file.read(&mapfile->netp_id_header, sizeof(mapfile->netp_id_header), 1);
        mapfile->id = file.readULE16();
        file.read(&mapfile->name, sizeof(mapfile->name), 1);
        file.read(&mapfile->description, sizeof(mapfile->description), 1);
        mapfile->width = file.readULE16();
        mapfile->height = file.readULE16();
        file.read(&mapfile->tile_set, sizeof(mapfile->tile_set), 1);
        mapfile->thumbnail_width = file.readULE16();
        mapfile->thumbnail_height = file.readULE16();
        
        mapfile->data = 0;
        mapfile->thumbnail = 0;
        
        int objectiveCount = 0;
        NPString outpost_filename = DEFAULT_MAPS_PATH + name + ".opt";
        IFileStream in(outpost_filename);
  
        std::string dummy;
        in >> dummy >> objectiveCount;
        if(!in.good())
        {
            objectiveCount = 0;
        }
        
        mapfile->outpost_count = objectiveCount;

        RMan->mapList[name] = mapfile;
    }
    
    return mapfile;
}

static int listMapFiles(std::vector<NPString>& dest)
{
    NPString suffix = ".npm";
    char **list = filesystem::enumerateFiles(DEFAULT_MAPS_PATH);

    for (char **i = list; *i != NULL; i++)
    {
        NPString filename = DEFAULT_MAPS_PATH;
        filename.append(*i);

        if ( !filesystem::isDirectory(filename.c_str()) )
        {
            if ( filename.size() >= suffix.size()
                && (filename.compare( filename.size() - suffix.size(),
                                      suffix.size(), suffix) == 0) )
            {
                NPString mapname;
                size_t p = 0;
                char c;
                while( (c = (*i)[p++]) != 0)
                {
                    if(c == '.')
                        break;
                    mapname += c;
                }
                
                dest.push_back(mapname);
            }
        }
    }
    filesystem::freeList(list);

    return dest.size();
}


void ResourceManager::refreshMapList()
{
    LOGGER.info("Refreshing map list");
    for ( MapList::iterator i = RMan->mapList.begin(), e = RMan->mapList.end(); i != e; i++ )
    {
        delete i->second;
    }
    RMan->mapList.clear();
    
    std::vector<NPString> mapnames;
    int nmaps = listMapFiles(mapnames);
    
    for ( int n = 0; n <nmaps; n++ )
    {
        MapFile * mf = loadMapInfo( mapnames[n] );
        if ( mf )
        {
            RMan->mapList[mapnames[n]] = mf;
            LOGGER.info("Map '%s' data", mapnames[n].c_str());
            
            LOGGER.info("filename: '%s'", mf->filename.c_str());
            LOGGER.info("full filename: '%s'", mf->fullfilename.c_str());
            LOGGER.info("netp_id_header: '%s'", mf->netp_id_header);
            LOGGER.info("id: %d", mf->id);
            LOGGER.info("name: '%s'", mf->name);
            LOGGER.info("description: '%s'", mf->description);
            LOGGER.info("size: %dx%d", mf->width, mf->height);
            LOGGER.info("tile set: '%s'", mf->tile_set);
            
            LOGGER.info("thumbnail size:%dx%d", mf->thumbnail_width, mf->thumbnail_height);
            
        }
    }
}

int ResourceManager::listMaps(std::vector<NPString>& dest)
{
    for ( MapList::iterator i = RMan->mapList.begin(), e = RMan->mapList.end(); i != e; i++ )
    {
        dest.push_back(i->first);
    }
    return dest.size();
}

bool ResourceManager::mapExists(const NPString& name)
{
    NPString basefilename = DEFAULT_MAPS_PATH;
    basefilename += name;

    NPString filename = basefilename + ".npm";
    if(!filesystem::exists(filename))
        return false;
        
    filename = basefilename + ".opt";
    if(!filesystem::exists(filename))
        return false;
        
    filename = basefilename + ".spn";
    if(!filesystem::exists(filename))
        return false;

    return true;
}

const MapFile* ResourceManager::getMap(const NPString& name, const int flags)
{
    MapFile * mapfile = 0;
    MapList::iterator i = RMan->mapList.find(name);
    if ( i != RMan->mapList.end() )
    {
        mapfile = i->second;
    }
        
    if ( mapfile && flags )
    {
        filesystem::ReadFile file(mapfile->fullfilename);
        
        if ( (flags & ResourceManager::MAP_DATA) && ! mapfile->hasData() )
        {
            mapfile->data_length = mapfile->width * mapfile->height;
            mapfile->data = new MapFile::ElementType [ mapfile->data_length ];
            MapFile::ElementType * d = mapfile->data;
            
            file.seek(MAP_HEADER_SIZE);
            
            for ( size_t i = 0; i < mapfile->data_length; ++i )
            {
                d[i] = file.readULE16();
            }
        }
        
        if ( ( flags & ResourceManager::MAP_THUMBNAIL) && ! mapfile->hasThumbnail() )
        {
            file.seek(MAP_HEADER_SIZE+(mapfile->width*mapfile->height*sizeof(MapFile::ElementType)));
            
            size_t thumbnail_size = mapfile->thumbnail_width*mapfile->thumbnail_height;
            uint8_t * thumbnail_data = new uint8_t[thumbnail_size];
            file.read(thumbnail_data, thumbnail_size, 1);
            
            Surface * th = new Surface(mapfile->thumbnail_width, mapfile->thumbnail_height);
            th->bufferToFrame(thumbnail_data, thumbnail_size);
            delete[] thumbnail_data;
            mapfile->thumbnail = th;
        }
    }
    
    return mapfile;
}
