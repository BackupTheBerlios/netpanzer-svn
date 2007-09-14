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

#ifndef __FlashParticle2D_hpp__
#define __FlashParticle2D_hpp__



#include "ExplosionFireParticle2D.hpp"


// FlashParticle2D
//--------------------------------------------------------------------------
class FlashParticle2D : public ExplosionFireParticle2D
{
public:
    FlashParticle2D(fXYZ pos, int xSize);

    static void init();

protected:
    int xSize;

    Surface static flashSprite;
    //Surface static explosionLightSprite1;

    //SpriteBltBrightness explosionLightSurface0;
    //SpriteBltBrightness explosionLightSurface1;
    SpriteBltBlendScale flashSurface;
    //SpriteBltBlendScale explosionLightSurface1;

    virtual void draw(const Surface &dest, SpriteSorter &sorter);

}
; // end FlashParticle2D

#endif // __FlashParticle2D_hpp__
