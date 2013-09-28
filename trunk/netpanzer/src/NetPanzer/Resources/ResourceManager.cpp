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

#include "MouseCursor.hpp"

#include "json/json.h"

#define DEFAULT_FLAGS_PATH "pics/flags/"
#define DEFAULT_MAPS_PATH "maps/"

typedef std::map<NPString, MapFile*> MapList;
typedef std::map<NPString, MouseCursor*> MouseCursors;
typedef std::map<NPString, Image*> Images;
typedef std::map<NPString, MImage*> MImages;
typedef std::map<NPString, BlendTable*> BlendTables;
typedef std::map<NPString, ImageFilter*> ImageFilters;

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
    
    MouseCursors mcursors;
    Images images;
    MImages mimages;
    BlendTables blendtables;
    ImageFilters imagefilters;
};

_RMan *RMan = 0;

void
ResourceManager::initialize()
{
    if ( ! RMan )
    {
        RMan = new _RMan();
        loadResourceDescriptions("data/resources.json");
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
        if ( (flen > 0) && (strcmp((*file) + flen, ".png") == 0) )
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
        tmp->loadPNG(fil);
        flags.push_back(tmp);
    }
    
    filesystem::freeList(list);
    return flags.size();
}

bool
ResourceManager::loadFlag(Surface* dest, const NPString& name)
{
    dest->loadPNG((DEFAULT_FLAGS_PATH + name).c_str());
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

static bool loadMouseCursor(const Json::Value& node, MouseCursor * mc)
{
    if ( node.isMember("name")
      && node.isMember("file")
      && node.isMember("offset") )
    {
        const Json::Value& off = node["offset"];
        if ( off.isArray() && off.size() >= 2 )
        {
            mc->name = node["name"].asString();
            mc->file = node["file"].asString();
            mc->offset_x = off[0].asInt();
            mc->offset_y = off[1].asInt();
            return true;
        }
    }
    
    return false;
}

static bool loadImage(const Json::Value& node, Image * i)
{
    if ( node.isMember("name")
      && node.isMember("file")
      && node.isMember("size") )
    {
        const Json::Value& siz = node["size"];
        if ( siz.isArray() && siz.size() >= 2 )
        {
            i->name   = node["name"].asString();
            i->file   = node["file"].asString();
            i->width  = siz[0].asInt();
            i->height = siz[1].asInt();
            return true;
        }
    }
    
    return false;
}

static bool loadMImage(const Json::Value& node, MImage * i)
{
    // offset and fps are optional
    if ( node.isMember("name")
      && node.isMember("file")
      && node.isMember("size")
      && node.isMember("frames") )
    {
        const Json::Value& siz = node["size"];
        if ( siz.isArray() && siz.size() >= 2 )
        {
            i->name   = node["name"].asString();
            i->file   = node["file"].asString();
            i->width  = siz[0].asInt();
            i->height = siz[1].asInt();
            i->frames = node["frames"].asInt();
            
            const Json::Value& off = node["offset"];
            if ( off.isArray() && off.size() >= 2 )
            {
                i->offset_x = off[0].asInt();
                i->offset_y = off[1].asInt();
            }
            else
            {
                i->offset_x = 0;
                i->offset_y = 0;
            }
            
            const Json::Value& fps = node["fps"];
            if ( fps.isInt() )
            {
                i->fps = fps.asInt();
            }
            else
            {
                i->fps = 0;
            }
            
            return true;
        }
    }
    
    return false;
}

static bool loadBlendTable(const Json::Value& node, BlendTable * b)
{
    if ( node.isMember("name")
      && node.isMember("file") )
    {
        b->name   = node["name"].asString();
        b->file   = node["file"].asString();
        return true;
    }
    
    return false;
}

static bool loadImageFilter(const Json::Value& node, ImageFilter * b)
{
    if ( node.isMember("name")
      && node.isMember("file") )
    {
        b->name   = node["name"].asString();
        b->file   = node["file"].asString();
        return true;
    }
    
    return false;
}

bool ResourceManager::loadResourceDescriptions(const NPString& filename)
{
    bool wasGood = false;
    IFileStream ifile(filename);
    if ( ifile.good() )
    {
        wasGood = true;
        Json::Value root;
        Json::Reader reader;
        if ( ! reader.parse(ifile, root, true) )
        {
            LOGGER.warning("Error loading resource descriptions:\n%s", reader.getFormattedErrorMessages().c_str());
            return false;
        }
        
        {
            const Json::Value& v = root["mcursors"];
            if ( v.isArray() && v.size() > 0 )
            {
                for ( int n = 0, e = v.size(); n < e; n++ )
                {
                    MouseCursor * c = new MouseCursor();
                    if ( loadMouseCursor(v[n], c) )
                    {
                        MouseCursors::iterator i = RMan->mcursors.find(c->name);
                        if ( i == RMan->mcursors.end() )
                        {
                            LOGGER.warning("Adding cursor: '%s'", c->name.c_str());
                            RMan->mcursors.insert(std::make_pair(c->name, c));
                        }
                        else
                        {
                            // @todo replace cursor with new one or do something
                        }
                    }
                    else
                    {
                        delete c;
                    }
                }
            }
        }
        
        {
            const Json::Value& v = root["images"];
            if ( v.isArray() && v.size() > 0 )
            {
                for ( int n = 0, e = v.size(); n < e; n++ )
                {
                    Image * c = new Image();
                    if ( loadImage(v[n], c) )
                    {
                        Images::iterator i = RMan->images.find(c->name);
                        if ( i == RMan->images.end() )
                        {
                            LOGGER.warning("Adding image: '%s'", c->name.c_str());
                            RMan->images.insert(std::make_pair(c->name, c));
                        }
                        else
                        {
                            // @todo replace cursor with new one or do something
                        }
                    }
                    else
                    {
                        delete c;
                    }
                }
            }
        }
        
        {
            const Json::Value& v = root["mimages"];
            if ( v.isArray() && v.size() > 0 )
            {
                for ( int n = 0, e = v.size(); n < e; n++ )
                {
                    MImage * c = new MImage();
                    if ( loadMImage(v[n], c) )
                    {
                        MImages::iterator i = RMan->mimages.find(c->name);
                        if ( i == RMan->mimages.end() )
                        {
                            LOGGER.warning("Adding mimage: '%s'", c->name.c_str());
                            RMan->mimages.insert(std::make_pair(c->name, c));
                        }
                        else
                        {
                            // @todo replace cursor with new one or do something
                        }
                    }
                    else
                    {
                        delete c;
                    }
                }
            }
        }
        
        {
            const Json::Value& v = root["blendtables"];
            if ( v.isArray() && v.size() > 0 )
            {
                for ( int n = 0, e = v.size(); n < e; n++ )
                {
                    BlendTable * c = new BlendTable();
                    if ( loadBlendTable(v[n], c) )
                    {
                        BlendTables::iterator i = RMan->blendtables.find(c->name);
                        if ( i == RMan->blendtables.end() )
                        {
                            LOGGER.warning("Adding blendtable: '%s'", c->name.c_str());
                            RMan->blendtables.insert(std::make_pair(c->name, c));
                        }
                        else
                        {
                            // @todo replace cursor with new one or do something
                        }
                    }
                    else
                    {
                        delete c;
                    }
                }
            }
        }
        
        {
            const Json::Value& v = root["imagefilters"];
            if ( v.isArray() && v.size() > 0 )
            {
                for ( int n = 0, e = v.size(); n < e; n++ )
                {
                    ImageFilter * c = new ImageFilter();
                    if ( loadImageFilter(v[n], c) )
                    {
                        ImageFilters::iterator i = RMan->imagefilters.find(c->name);
                        if ( i == RMan->imagefilters.end() )
                        {
                            LOGGER.warning("Adding imagefilter: '%s'", c->name.c_str());
                            RMan->imagefilters.insert(std::make_pair(c->name, c));
                        }
                        else
                        {
                            // @todo replace cursor with new one or do something
                        }
                    }
                    else
                    {
                        delete c;
                    }
                }
            }
        }
    }
    return wasGood;
}

MouseCursorResource ResourceManager::getMouseCursor(const NPString& name)
{
    MouseCursors::iterator i = RMan->mcursors.find(name);
    if ( i == RMan->mcursors.end())
    {
        // @todo return default thing
        LOGGER.warning("Resource not found (cursor): '%s'", name.c_str());
        return MouseCursorResource(0); // will die!!!!
    }
    
    if ( ! i->second->isLoaded() )
    {
        i->second->image.loadPNG(i->second->file.c_str());
        i->second->loaded = true;
    }
    
    return MouseCursorResource(i->second);
}

ImageResource ResourceManager::getImage(const NPString& name)
{
    Images::iterator i = RMan->images.find(name);
    if ( i == RMan->images.end())
    {
        // @todo return default thing
        LOGGER.warning("Resource not found (image): '%s'", name.c_str());
        return ImageResource(0); // will die!!!!
    }
    
    if ( ! i->second->isLoaded() )
    {
        i->second->image.loadPNG(i->second->file.c_str());
        i->second->loaded = true;
    }
    
    return ImageResource(i->second);
}

MImageResource ResourceManager::getMImage(const NPString& name)
{
    MImages::iterator i = RMan->mimages.find(name);
    if ( i == RMan->mimages.end())
    {
        // @todo return default thing
        LOGGER.warning("Resource not found (mimage): '%s'", name.c_str());
        return MImageResource(0); // will die!!!!
    }
    
    if ( ! i->second->isLoaded() )
    {
        MImage * mimage = i->second;
        
        Surface surf;
        surf.loadPNG(mimage->file.c_str());

        iRect bounds(0, 0, mimage->width, mimage->height);
            
        PtrArray<Surface> ps(mimage->frames);
        for ( int n = 0; n < mimage->frames; n++ )
        {
            // @todo replace grab with create and blit
            Surface * s = new Surface();
            s->grab(surf, bounds);
            ps.push_back(s);

            bounds.translate(mimage->width, 0);
            if ( bounds.getLocationX() >= surf.getWidth() )
            {
                bounds.setLocationX(0);
                bounds.translate(0, mimage->height);
            }
        }
        
        mimage->image.pack(ps);
        ps.deleteAll();
        
        mimage->loaded = true;
    }
    
    return MImageResource(i->second);
}

BlendTableResource ResourceManager::getBlendTable(const NPString& name)
{
    BlendTables::iterator i = RMan->blendtables.find(name);
    if ( i == RMan->blendtables.end())
    {
        // @todo return default thing
        LOGGER.warning("Resource not found (blendtable): '%s'", name.c_str());
        return BlendTableResource(0); // will die!!!!
    }
    
    if ( ! i->second->isLoaded() )
    {
        filesystem::ReadFile f(i->second->file);
        if ( f.isOpen() )
        {
            const unsigned blendtablesize = 256*256;
            uint8_t * buffer = new uint8_t[blendtablesize];
            f.read(buffer, blendtablesize, 1);
            i->second->data = buffer;
            i->second->loaded = true;
        }
    }
    
    return BlendTableResource(i->second);
}

ImageFilterResource ResourceManager::getImageFilter(const NPString& name)
{
    ImageFilters::iterator i = RMan->imagefilters.find(name);
    if ( i == RMan->imagefilters.end())
    {
        // @todo return default thing
        LOGGER.warning("Resource not found (imagefilter): '%s'", name.c_str());
        return ImageFilterResource(0); // will die!!!!
    }
    
    if ( ! i->second->isLoaded() )
    {
        filesystem::ReadFile f(i->second->file);
        if ( f.isOpen() )
        {
            const unsigned imagefiltersize = 256;
            uint8_t * buffer = new uint8_t[imagefiltersize];
            f.read(buffer, imagefiltersize, 1);
            i->second->data = buffer;
            i->second->loaded = true;
        }
    }
    
    return ImageFilterResource(i->second);
}
