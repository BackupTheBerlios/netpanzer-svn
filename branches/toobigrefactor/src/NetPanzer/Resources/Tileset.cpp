/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez

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
 * Created on November 16, 2013, 9:42 AM
 */

#include "Tileset.hpp"
#include "json/json.h"
#include "ResourceManager.hpp"
#include "Util/FileStream.hpp"
#include "Util/Log.hpp"

template<> Tileset * ResourceManager::resourceFromJSon<Tileset>(const Json::Value& node)
{
    if ( node.isMember("file") )
    {
        Tileset *t = new Tileset();
        t->file   = node["file"].asString();
        return t;
    }
    
    return 0;
}

bool Tileset::load()
{
    IFileStream ifile(file);
    if ( ifile.good() )
    {
        Json::Value root;
        Json::Reader reader;
        if ( ! reader.parse(ifile, root, true) )
        {
            LOGGER.warning("Error loading Tileset data[%s]:\n%s", file.c_str(), reader.getFormattedErrorMessages().c_str());
            return false;
        }
        
        const Json::Value& js_idheader    = root["idheader"];
        const Json::Value& js_version     = root["version"];
        const Json::Value& js_tile_width  = root["tile_width"];
        const Json::Value& js_tile_height = root["tile_height"];
        const Json::Value& js_tile_count  = root["tile_count"];
        const Json::Value& js_movevals    = root["move"];
        const Json::Value& js_avgcolors   = root["color"];
        
        idheader = js_idheader.asString();
        tile_width  = js_tile_width.asUInt();
        tile_height = js_tile_height.asUInt();
        tile_count  = js_tile_count.asUInt();
        
        move_value = new uint8_t[tile_count];
        avg_color  = new uint8_t[tile_count];
        
        for ( unsigned n = 0; n < tile_count; n++ )
        {
            move_value[n] = js_movevals[n].asUInt();
            avg_color[n] = js_avgcolors[n].asUInt();
        }
        
        loaded = true;
    }

    return loaded;
}
