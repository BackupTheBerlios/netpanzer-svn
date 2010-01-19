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


#include "Util/Exception.hpp"
#include "FlameParticle2D.hpp"
#include "lua/lua.hpp"
#include "2D/ImageArray.hpp"
#include "Util/Log.hpp"
#include "tolua++.h"

typedef struct s_FlameArray
{
    unsigned int num_flames;
    ImageArray* flameImageArray;
} FlameArray;

static FlameArray* flame_array = 0;

const int explosionFPS = 18;

// FlameParticle2D
//---------------------------------------------------------------------------
FlameParticle2D::FlameParticle2D(	const fXYZ  &pos,
                                  float        scaleMin,
                                  float        scaleRand,
                                  float        lifetime,
                                  int          layer) : Particle2D(pos)
{
    if (lifetime <= 0.0f) {
        isAlive = false;
    }

    FlameParticle2D::layer = layer;

    scale = getScale(scaleMin, scaleRand);

    // There are 2 explosion images to choose from.
    int picNum = rand() % flame_array->num_flames;

    index = getPakIndex(scale, flame_array->flameImageArray[picNum].size());
    packedSurface.setData(* (flame_array->flameImageArray[picNum].getImage(index)) );

    // Check for accelerated flames.
    packedSurface.setFPS(getFPS(explosionFPS, 0));
} // end FlameParticle2D::FlameParticle2D

// init
//---------------------------------------------------------------------------
int FlameParticle2D::loadFlames(lua_State *L, void *v)
{
    if ( ! lua_istable(L, -1) )
    {
        LOGGER.warning("BAD explosions configuration, not loaded.");
        return 0;
    }

    if ( ! flame_array )
    {
        flame_array = new FlameArray;
        flame_array->num_flames = 0;
        flame_array->flameImageArray = 0;
    }


    if ( flame_array->num_flames != lua_objlen(L, -1) )
    {
        if ( flame_array->flameImageArray )
        {
            delete[] flame_array->flameImageArray;
        }

        flame_array->num_flames = lua_objlen(L,-1);
        flame_array->flameImageArray = new ImageArray[flame_array->num_flames]();
    }

    for ( unsigned int n = 1; n <= flame_array->num_flames; ++n )
    {
        lua_rawgeti(L, -1, n);
        flame_array->flameImageArray[n-1].loadImageSheetArray(L);
        lua_pop(L, 1);
    }

    return 0;
} // end FlameParticle2D::init

// draw
//---------------------------------------------------------------------------
void FlameParticle2D::draw(SpriteSorter& sorter)
{
    if (!isAlive) {
        return;
    }

    if (!packedSurface.nextFrame()) {
        isAlive = false;
        return;
    }

    packedSurface.setAttrib(iXY((int)pos.x, (int)pos.z), layer);
    sorter.addSprite(&packedSurface);

} // end FlameParticle2D::draw
