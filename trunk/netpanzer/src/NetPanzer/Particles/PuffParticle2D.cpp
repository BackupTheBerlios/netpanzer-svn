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

#include "PuffParticle2D.hpp"
#include "ParticleSystemGlobals.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Util/Log.hpp"
#include "2D/ImageArray.hpp"

#include "lua/lua.hpp"

typedef struct s_PuffArray
{
    unsigned int num_puff_types;
    ImageArray* puffImageArray;
} PuffArray;

static PuffArray* puff_array = 0;

// PuffParticle2D
//---------------------------------------------------------------------------
PuffParticle2D::PuffParticle2D(	const fXYZ &pos,
                                const fXYZ &shadowPos,
                                unsigned int particleType,
                                float       minScale,
                                float       randScale,
                                int         minFPS,
                                int         randFPS,
                                int         layer,
                                int         shadowLayer,
                                float       windScale /* = 1.0f */,
                                int) : WindParticle2D(pos, windScale)
{
    PuffParticle2D::shadowPos = shadowPos;
    userDefinedShadowPos      = true;

    create(particleType, minScale, randScale, minFPS, randFPS, layer, shadowLayer);

} // end PuffParticle2D::PuffParticle2D

// create
//---------------------------------------------------------------------------
void PuffParticle2D::create( unsigned int particleType,
                             float     scaleMin,
                             float     scaleRand,
                             int       FPSmin,
                             int       FPSrand,
                             int       layer,
                             int       shadowLayer,
                             int       isFarAway /* = 0 */)
{
    assert(scaleMin >= 0.0);
    assert(scaleRand >= 0.0);

    scale = getScale(scaleMin, scaleRand);

    PuffParticle2D::layer       = layer;
    PuffParticle2D::shadowLayer = shadowLayer;
    PuffParticle2D::isFarAway   = isFarAway;

    if ( particleType >= puff_array->num_puff_types )
    {
        particleType = 0;
        LOGGER.warning("Wrong puff_type %d, using 0", particleType);
    }

    index = getPakIndex(scale, puff_array->puffImageArray[particleType].size());

    Surface *s = puff_array->puffImageArray[particleType].getImage(index);

    packedSurface.setData( *s );
    packedSurfaceShadow.setData( *s );

    packedSurfaceShadow.setDrawModeBlend(32); // dark a little

    if (gameconfig->blendsmoke) {
        packedSurface.setDrawModeBlend(128);
    } else {
        packedSurface.setDrawModeSolid();
    }

    packedSurface.setFPS(getFPS(FPSmin, FPSrand));

    // If the particles are far away, speed them up.
    if (isFarAway) {
        assert(packedSurface.getFPS() > 0);

        packedSurface.setFPS(packedSurface.getFPS() * 2.0f);
    }

    // Set the shadow FPS to match the non-shadow particle.
    packedSurfaceShadow.setFPS(packedSurface.getFPS());

} // end PuffParticle2D::create

// draw
//---------------------------------------------------------------------------
// Purpose: Draws a single puff particle, no simulation.
//---------------------------------------------------------------------------
void PuffParticle2D::draw(SpriteSorter &sorter)
{
    if (!packedSurface.nextFrame()) {
        isAlive = false;
        return;
    }

    packedSurfaceShadow.nextFrame();

    packedSurface.setAttrib(iXY((int) pos.x, (int) pos.z), layer);
    sorter.addSprite(&packedSurface);

    if (gameconfig->displayshadows) {
        if (!userDefinedShadowPos) {
            shadowPos.x = pos.x - ((float(index) /
                        float(puff_array->puffImageArray[0].size())) * packedSurfaceShadow.getCurFrame() * 10);
        }

        packedSurfaceShadow.setAttrib(iXY((int) shadowPos.x, (int) shadowPos.z), shadowLayer);
        sorter.addSprite(&packedSurfaceShadow);
    }

} // end PuffParticle2D::draw

static bool loadPuffData(lua_State *L, const char * field_name, int puff_num)
{
    lua_getfield(L, -1, field_name);
    if ( ! lua_istable(L, -1) )
    {
        LOGGER.warning("%s puffs configuration not found.", field_name);
        lua_pop(L, 1);
        return false;
    }

    puff_array->puffImageArray[puff_num].loadImageSheetArray(L);

    lua_pop(L, 1);
}

// init
//---------------------------------------------------------------------------
void PuffParticle2D::init(lua_State* L)
{
    if ( ! puff_array )
    {
        int luatop = lua_gettop(L);

        lua_getfield(L, -1, "puffs");
        if ( ! lua_istable(L, -1) )
        {
            LOGGER.warning("Puffs configuration not found.");
            lua_settop(L, luatop);
            return;
        }

        puff_array = new PuffArray;
        puff_array->num_puff_types = 3;
        puff_array->puffImageArray = new ImageArray[3]();

        loadPuffData(L, "light", 0);
        loadPuffData(L, "dark", 1);
        loadPuffData(L, "dirt", 2);
        
        lua_settop(L, luatop);
    }
} // end PuffParticle2D::init
