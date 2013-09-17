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

#ifndef VIDEOCONFIG_HPP
#define	VIDEOCONFIG_HPP

#include "Core/CoreTypes.hpp"
#include "libjson/libjson.h"

class VideoConfig
{
public:
    VideoConfig();
    ~VideoConfig();
    
    void load(const JSONNode& node);
    void save(JSONNode& node) const;
    
    inline unsigned getWidth() const { return width; }
    inline unsigned getHeight() const { return height; }
    inline bool useFullScreen() const { return fullscreen; }
    inline bool useHardwareSurface() const { return hardwaresurface; }
    inline bool useDoubleBuffer() const { return doublebuffer; }
    inline bool useShadows() const { return shadows; }
    inline bool useBlendSmoke() const { return blendsmoke; }
#ifdef _WIN32
    inline bool useDirectX() const { return usedirectx; }
#endif
    
    inline void setWidth(const unsigned w) { width = w; }
    inline void setHeight(const unsigned h) { height = h; }
    inline void setFullScreen(const bool b) { fullscreen = b; }
    inline void setHardwareSurface(const bool b) { hardwaresurface = b; }
    inline void setDoubleBuffer(const bool b) { doublebuffer = b; }
    inline void setShadows(const bool b) { shadows = b; }
    inline void setBlendSmoke(const bool b) { blendsmoke = b; }
#ifdef _WIN32
    inline void setDirectX(const bool b) { usedirectx = b; }
#endif
    
private:
    VideoConfig(const VideoConfig& );
    
    unsigned width;
    unsigned height;
    bool     fullscreen;
    bool     hardwaresurface;
    bool     doublebuffer;
    bool     shadows;
    bool     blendsmoke;
#ifdef _WIN32
    bool     usedirectx;
#endif


};

#endif	/* VIDEOCONFIG_HPP */

