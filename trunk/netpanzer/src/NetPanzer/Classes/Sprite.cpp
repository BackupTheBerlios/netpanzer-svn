/*
Copyright (C) 1998 Pyrosoft Inc. (www.pyrosoftgames.com), Matthew Bogue
 
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
*/

#include "Classes/Sprite.hpp"
#include "2D/Palette.hpp"

Sprite::Sprite( )
{
    height = 0;
    visible = true;
    next = 0;

}

SpritePacked::SpritePacked()
        : Sprite()
{
    blendTable = 0;
}

SpritePacked::SpritePacked( PackedSurface &source )
        : Sprite()
{
    setTo( source );
    blendTable = 0;
}

bool SpritePacked::isVisible(const iRect &world_win ) const
{
    return ! world_win.clip(iRect(world_pos.x - (pix.x>>1), world_pos.y - (pix.y>>1), pix.x, pix.y));
}

void SpritePacked::blit( Surface *surface, const iRect &world_win )
{
    iXY blit_offset;

    blit_offset = (world_pos + attach_offset) - world_win.getLocation();

    if ( blendTable )
    {
        bltBlend(*surface, blit_offset.x, blit_offset.y, blendTable);
    }
    else
    {
        blt(*surface, blit_offset);
    }
}
