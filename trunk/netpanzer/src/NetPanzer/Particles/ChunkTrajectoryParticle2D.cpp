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


#include "ChunkTrajectoryParticle2D.hpp"
#include "Util/TimerInterface.hpp"
#include "PuffParticle2D.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Util/Math.hpp"
#include "Particles/ParticleInterface.hpp"
#include "Units/UnitProfileInterface.hpp"

#include "lua/lua.hpp"
#include "Util/Log.hpp"

Surface* staticPackedGroundChunks = 0;

// ChunkTrajectoryParticle2D
//---------------------------------------------------------------------------
ChunkTrajectoryParticle2D::ChunkTrajectoryParticle2D(	const fXYZ &pos,
        int         maxSpeed,
        float       scaleMin,
        float       scaleRand,
        float       waitMin,
        float       waitRand,
        int         minTrajectoryAngle,
        PUFF_TYPE   particleType,
        int         dieAtMidFlight /* = 0 */,
        int         isFarAway      /* = 0 */,
        int         canHaveSmoke   /* = 1 */) : TrajectoryParticle2D(pos, maxSpeed, (rand() % (90 - minTrajectoryAngle)) + minTrajectoryAngle, dieAtMidFlight)
{
    assert(this != 0);
    assert(minTrajectoryAngle >= 0 && minTrajectoryAngle <= 90);

    ChunkTrajectoryParticle2D::isFarAway    = isFarAway;

    ChunkTrajectoryParticle2D::scaleMin     = scaleMin;
    ChunkTrajectoryParticle2D::scaleRand    = scaleRand;

    ChunkTrajectoryParticle2D::waitMin      = waitMin;
    ChunkTrajectoryParticle2D::waitRand     = waitRand;

    if (isFarAway) {
        // Decrease the amount of smoke on particle.
        ChunkTrajectoryParticle2D::waitMin  *= 2.0f;
        ChunkTrajectoryParticle2D::waitRand *= 2.0f;
    }

    ChunkTrajectoryParticle2D::particleType = particleType;

    // int randChunk = rand() % staticPackedGroundChunks.getFrameCount();

    packedSurface.setData(*staticPackedGroundChunks);
    packedSurface.setDrawModeSolid();
    //packedSurface.setDrawModeBlend(&Palette::colorTableBrighten);

    // Get particle color.
    if (ParticleInterface::gSolidColorExplosionParticles) {
        index = 253;

    } else {
        index = rand() % staticPackedGroundChunks->getNumFrames();
    }

    //int randFrame = rand() % staticPackedChunks[randChunk].getFrameCount();
    staticPackedGroundChunks->setFrame(index);

    packedSurfaceShadow.setData(*staticPackedGroundChunks);
    packedSurfaceShadow.setDrawModeBlend(128); // dark a little

    if (ParticleInterface::gParticlesCanHaveSmoke && canHaveSmoke) {
        hasSmoke = rand() % 3;
    } else {
        hasSmoke = 0;
    }

    smokeCurWait  = 0.0f;
    smokeWaitTime = 0.0f;
    arcYPix       = 0.0f;

} // end ChunkTrajectoryParticle2D::ChunkTrajectoryParticle2D

// init
//---------------------------------------------------------------------------
void ChunkTrajectoryParticle2D::init(lua_State *L)
{
    if ( staticPackedGroundChunks )
    {
        return; // already loaded
    }

    int luatop = lua_gettop(L);

    lua_getfield(L, -1, "chunks");
    if ( ! lua_istable(L, -1) )
    {
        LOGGER.warning("chunks configuration not found.");
        lua_settop(L, luatop);
        return;
    }

//    lua_rawgeti(L, -1, 1);

    lua_rawgeti(L, -1, 1); // file_name
    lua_rawgeti(L, -2, 2); // width
    lua_rawgeti(L, -3, 3); // height
    lua_rawgeti(L, -4, 4); // nframes

    Surface* s = new Surface();
    s->loadPNGSheet( lua_tostring(L, -4),
                    lua_tointeger(L, -3),
                    lua_tointeger(L, -2),
                    lua_tointeger(L, -1));

//    lua_pop(L, 5);

    s->setColorkey();
    s->setOffsetCenter();

    staticPackedGroundChunks = s;

    lua_settop(L, luatop);
} // end ChunkTrajectoryParticle2D::init

// draw
//---------------------------------------------------------------------------
void ChunkTrajectoryParticle2D::draw(SpriteSorter &sorter)
{
    assert(this != 0);

    packedSurface.setAttrib(iXY((int) pos.x, (int) (pos.z - arcYPix)), layer);
    sorter.addSprite(&packedSurface);

    if (gameconfig->displayshadows) {
        packedSurfaceShadow.setAttrib(iXY((int) (pos.x - arcYPix), (int) pos.z), shadowLayer);
        sorter.addSprite(&packedSurfaceShadow);
    }

} // end ChunkTrajectoryParticle2D::draw

// ChunkTrajectoryParticle2D::sim
//---------------------------------------------------------------------------
// Purpose: Handles the particle simulation, no drawing.
//---------------------------------------------------------------------------
void ChunkTrajectoryParticle2D::sim()
{
    assert(this != 0);

    arcYPix = pos.y * arcScale;

    if (hasSmoke == 1) {
        smokeCurWait += TimerInterface::getTimeSlice();

        if (smokeCurWait > smokeWaitTime) {
            try {
                new PuffParticle2D(fXYZ(pos.x, pos.y, pos.z - arcYPix), fXYZ(pos.x - arcYPix, pos.y, pos.z), particleType, scaleMin, scaleRand, smokeFPSMin, smokeFPSRand, smokeLayer, smokeShadowLayer, smokeWindScale, isFarAway);
            } catch(...) {}

            smokeWaitTime = (float(rand()) / float(RAND_MAX)) * waitRand + waitMin;

            smokeCurWait = 0.0f;
        }
    }

    TrajectoryParticle2D::sim();

} // end ChunkTrajectoryParticle2D::sim
