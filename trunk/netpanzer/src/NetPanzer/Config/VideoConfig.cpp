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
 * Created on September 15, 2013, 2:45 PM
 */

#include "VideoConfig.hpp"
#include "json/json.h"

VideoConfig::VideoConfig()
 :  width(800),
    height(600),
    fullscreen(false),
    hardwaresurface(false),
    doublebuffer(false),
    shadows(true),
    blendsmoke(true)
#ifdef _WIN32
    ,usedirectx(false)
#endif
{
    
}

VideoConfig::~VideoConfig()
{
    
}

void VideoConfig::load(const Json::Value& node)
{
    if ( node.isMember("size") )
    {
        const Json::Value& size = node["size"];
        if ( size.isArray() && size.size() > 1 )
        {
            setWidth(size[0].asInt());
            setHeight(size[1].asInt());
        }
    }

    setFullScreen( node.get("fullscreen", false).asBool() ); // @todo set to true
    setHardwareSurface( node.get("hwsurface", false).asBool() );
    setDoubleBuffer( node.get("doublebuffer", false).asBool() );
    setShadows( node.get("shadows", true).asBool() );
    setBlendSmoke( node.get("blendsmoke", true).asBool() );
#ifdef _WIN32
    setDirectX( node.get("directx", false).asBool() );
#endif
}

void VideoConfig::save(Json::Value& node) const
{
    Json::Value size(Json::arrayValue);
    size[0] = getWidth();
    size[1] = getHeight();
    node["size"] = size;
    node["fullscreen"] = useFullScreen();
    node["hwsurface"] = useHardwareSurface();
    node["doublebuffer"] = useDoubleBuffer();
    node["shadows"] = useShadows();
    node["blendsmoke"] = useBlendSmoke();
#ifdef _WIN32
    node["directx"] = useDirectX();
#endif
}

