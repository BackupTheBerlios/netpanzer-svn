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
#include "ConfigGetter.hpp"

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

void VideoConfig::load(const JSONNode& node)
{
    ConfigGetter cg(node);
    
    setWidth( cg.getInt("width", 800, 4096, 800) );
    setHeight( cg.getInt("height", 480, 4096, 600) );
    setFullScreen( cg.getBool("fullscreen", false) ); // @todo set to true
    setHardwareSurface( cg.getBool("hwsurface", false) );
    setDoubleBuffer( cg.getBool("doublebuffer", false) );
    setShadows( cg.getBool("shadows", true) );
    setBlendSmoke( cg.getBool("blendsmoke", true) );
#ifdef _WIN32
    setDirectX( cg.getBool("directx", false) );
#endif
    
}

void VideoConfig::save(JSONNode& node) const
{
    node.push_back(JSONNode("width", getWidth()));
    node.push_back(JSONNode("height", getHeight()));
    node.push_back(JSONNode("fullscreen", useFullScreen()));
    node.push_back(JSONNode("hwsurface", useHardwareSurface()));
    node.push_back(JSONNode("doublebuffer", useDoubleBuffer()));
    node.push_back(JSONNode("shadows", useShadows()));
    node.push_back(JSONNode("blendsmoke", useBlendSmoke()));
    
#ifdef _WIN32
    node.push_back(JSONNode("directx", useDirectX()));
#endif
}

