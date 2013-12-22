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
 * Created on September 14, 2013, 5:29 PM
 */

#ifndef MAPTHUMBNAILCOMPONENT_HPP
#define	MAPTHUMBNAILCOMPONENT_HPP

#include "2D/Surface.hpp"
#include "Views/Components/Component.hpp"

class MapThumbnailComponent : public Component
{
public:
    MapThumbnailComponent(int w, int h);
    ~MapThumbnailComponent();
    
    void draw(Surface &dest) const;
    
    void setImage(const Surface& from);
    
    void setEmpty();
    
private:
    MapThumbnailComponent(const MapThumbnailComponent& );

    Surface mapimage;
};

#endif	/* MAPTHUMBNAILCOMPONENT_HPP */

