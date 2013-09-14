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

#include "MapThumbnailComponent.hpp"
#include "2D/Color.hpp"


MapThumbnailComponent::MapThumbnailComponent(int w, int h)
{
//        setLocation(x, y);
    setSize(w, h);
    mapimage.create(w - 2, h - 2);
    setEmpty();
}

MapThumbnailComponent::~MapThumbnailComponent()
{
    
}

void MapThumbnailComponent::draw(Surface &dest) const
{
    dest.drawRect(rect, Color::lightGray);
    mapimage.blt(dest, rect.getLocationX()+1, rect.getLocationY()+1); // full blit
}

void MapThumbnailComponent::setImage(const Surface& from)
{
    iRect r(0,0, rect.getWidth()-2, rect.getHeight()-2);
    mapimage.bltScale(from, r);
}

void MapThumbnailComponent::setEmpty()
{
    mapimage.fill(0);
}

