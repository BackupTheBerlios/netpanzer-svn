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


#include "Particles/ParticleInterface.hpp"
#include "PuffParticle2D.hpp"
#include "FlameParticle2D.hpp"
#include "Particles/FlashParticle2D.hpp"
#include "TemplateExplosionSystem.hpp"
#include "CloudParticle2D.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Physics.hpp"
#include "VectorPuffParticle2D.hpp"
#include "Interfaces/WorldViewInterface.hpp"
#include "Interfaces/ConsoleInterface.hpp"
#include "ChunkTrajectoryParticle2D.hpp"
#include "Units/UnitProfileInterface.hpp"
#include "TemplateMuzzleSystem.hpp"
#include "Units/UnitGlobals.hpp"
#include "CraterParticle2D.hpp"
#include "Util/Exception.hpp"
#include "System/Sound.hpp"

#include "Util/Log.hpp"
#include "2D/Color.hpp"


std::vector<UnitParticleInfo> ParticleInterface::unitParticleInfo;
unsigned ParticleInterface::unitBodyMaxArea                   = 0;
int ParticleInterface::gParticlesCanHaveSmoke            = 1;
int ParticleInterface::gSolidColorExplosionParticles     = 0;
int ParticleInterface::gTestSim                          = 0;
int ParticleInterface::gDrawExplosionParticleCount       = 0;
int ParticleInterface::gExplosionFlameFlashCullMissCount = 1;
int ParticleInterface::gExplosionFlameFlashCullHitCount  = 1;
int ParticleInterface::gMuzzleSystemCullMissCount        = 1;
int ParticleInterface::gMuzzleSystemCullHitCount         = 1;

vector<int> ParticleInterface::unitHitPointTable;
vector<int> ParticleInterface::unitAttackFactorTable;


//--------------------------------------------------------------------------
ParticleInterface::ParticleInterface()
{}

// addUnitDamagePuffParticle
//--------------------------------------------------------------------------
void ParticleInterface::addUnitDamagePuffParticle(const UnitState &unitState)
{
    iXY pos(unitState.location);

    iXY size     = unitParticleInfo[unitState.unit_type].minBounds.getSize();
    iXY halfSize(size.x / 2, size.y /2 );

    fXYZ newPos;

    newPos.x = pos.x + (rand() % size.x) - halfSize.x;
    newPos.y = 0;
    newPos.z = pos.y + (rand() % size.y) - halfSize.y;

    int particleType = rand() % 2;

    if (particleType == 0) {
        try {
            new PuffParticle2D(	newPos,
                                LIGHT,
                                0.0f,
                                0.4f,
                                smokeFPSMin,
                                smokeFPSRand,
                                smokeLayer,
                                smokeShadowLayer,
                                smokeWindScale,
                                Particle2D::getFarAway(newPos));
        } catch(...) {
            // ignored for now
        }
    } else if (particleType == 1) {
        try {
            new PuffParticle2D(	newPos,
                                DARK,
                                0.0f,
                                0.4f,
                                smokeFPSMin,
                                smokeFPSRand,
                                smokeLayer,
                                smokeShadowLayer,
                                smokeWindScale,
                                Particle2D::getFarAway(newPos));
        } catch(...) {
            // ignored for now
        }
    } else {
        throw Exception("Unknonw DamagePuffParticle type");
    }
} // end ParticleInterface::addUnitDamagePuffParticle

//--------------------------------------------------------------------------
void ParticleInterface::testSim()
{
    static float groupTime0 = 0.0f;
    static float groupTime1 = 0.0f;

    groupTime0 += TimerInterface::getTimeSlice();
    groupTime1 += TimerInterface::getTimeSlice();

    if (groupTime0 > 1.0f) {
        groupTime0 = 0.0f;

        //SFX
        sound->playSound("expl");

        iRect gameViewRect;
        WorldViewInterface::getViewWindow(&gameViewRect);

        UnitState unitState;

        unitState.location.x = gameViewRect.getLocationX() + 100 + (rand() % (gameViewRect.getWidth() - 200));
        unitState.location.y = gameViewRect.getLocationY() + 100 + (rand() % (gameViewRect.getHeight() - 200));

        // Hack until all the units are actually used.
        unitState.unit_type = rand() % UnitProfileInterface::getNumUnitTypes();
        //unitState.unit_type = rand() % 7;

        addHit(unitState);
    }

    if (groupTime1 > 0.5f) {
        groupTime1 = 0.0f;

        //SFX
        sound->playSound("expl");

        iRect gameViewRect;
        WorldViewInterface::getViewWindow(&gameViewRect);

        iXY location;

        location.x = gameViewRect.getLocationX() + 100 + (rand() % (gameViewRect.getWidth() - 200));
        location.y = gameViewRect.getLocationY() + 100 + (rand() % (gameViewRect.getHeight() - 200));

        addMiss(location, rand() % UnitProfileInterface::getNumUnitTypes());
    }
}

//--------------------------------------------------------------------------
void ParticleInterface::testSimText(Surface &dest)
{
    char strBuf[256];

    int y       = 50;
    int yOffset = 10;

    sprintf(strBuf, "particleSystemCount: %d", ParticleSystem2D::getParticleSystemCount());
    dest.bltString(2, y, strBuf, Color::white);
    y += yOffset;

    sprintf(strBuf, "particleCount:       %d", Particle2D::getFrameCount());
    dest.bltString(2, y, strBuf, Color::white);
}

//--------------------------------------------------------------------------
void ParticleInterface::addHit(const UnitState &unitState)
{
    ExplosionSystem e;

    float randomness = 1.0f; //Particle2D::getScale(0.8f, 0.4f);
    float frameRateAdjustment = getFrameRateAdjustment();

    e.particleCount = (int) (
                          8 * (frameRateAdjustment *
                               float(unitHitPointTable[unitState.unit_type]) * randomness));

    if (gDrawExplosionParticleCount)
    {
        ConsoleInterface::postMessage(Color::white, false, 0, "%d", e.particleCount);
    }

    float hitPointScale = 1.0f;

    UnitProfile *p = UnitProfileInterface::getUnitProfile(unitState.unit_type);
    if (p != 0) {
        hitPointScale = float(unitHitPointTable[unitState.unit_type]) / 18.0f;
    } else {
        assert(false);
    }

    e.percentScaleMin    = 0.8f;
    e.percentScaleRand   = 0.6f;
    e.speedSmall         = int(400 * hitPointScale);
    e.speedMedium        = int(250 * hitPointScale);
    e.speedLarge         = int(150 * hitPointScale);
    e.percentCountSmall  = 0.4f;
    e.percentCountMedium = 0.3f;
    e.percentCountLarge  = 0.3f;
    e.flameMinScale      = 0.2f * hitPointScale;
    e.flameRandScale     = 0.15f * hitPointScale;
    e.flameLifetime      = 0.1f;
    e.flameCount         = 1;
    e.flameBounds.zero();
    e.flashMinScale      = 0.6f * hitPointScale;
    e.flashRandScale     = 0.4f * hitPointScale;
    e.flashLifetime      = 0.5f;
    e.flashCount         = int( 3 * float(unitParticleInfo[unitState.unit_type].minBounds.getArea()) / float(unitBodyMaxArea));
    e.flashBounds        = unitParticleInfo[unitState.unit_type].minBounds;

    int randTrajectoryAngleMin = rand() % 3;

    if      (randTrajectoryAngleMin == 0) {
        e.minTrajectoryAngle = 86;
    } else if (randTrajectoryAngleMin == 1) {
        e.minTrajectoryAngle = 82;
    } else if (randTrajectoryAngleMin == 2) {
        e.minTrajectoryAngle = 77;
    }

    if (rand() % 2) {
        e.puffType = LIGHT;
    } else {
        e.puffType = DARK;
    }
    //explosionType.


    fXYZ pos(unitState.location.x, 0, unitState.location.y);

    try {
        new TemplateExplosionSystem(	pos,
                                     unitParticleInfo[unitState.unit_type].minBounds,
                                     e,
                                     Particle2D::getFarAway(pos));
    } catch(...) {}

}

float ParticleInterface::getFrameRateAdjustment()
{
    static float frameMaxbase    = 60.0f;
    static float frameMinbase    = 20.0f;
    static float frameDifference = frameMaxbase - frameMinbase;

    if (TimerInterface::getFPS() > 60.0f) {
        return 1.0f;
    }

    float adjustment = (TimerInterface::getFPS() - frameMinbase) / frameDifference;

    if (adjustment < 0.2f) {
        adjustment = 0.2f;
    }

    return adjustment;
}

//--------------------------------------------------------------------------
void ParticleInterface::addMiss(const iXY &worldPos, Uint8 unitType)
{
    /*
      short hit_points;
      short attack_factor;
      long  attack_range;
      long  defend_range;
      char  speed_factor;
      char  speed_rate;
      char  reload_time;
      short regen_time;
    */

    ExplosionSystem e;

    float randomness = 1.0f; //Particle2D::getScale(0.8f, 0.4f);
    float frameRateAdjustment = getFrameRateAdjustment();

    e.particleCount = int(5 * (frameRateAdjustment * float(unitAttackFactorTable[unitType]) * randomness));

    if (gDrawExplosionParticleCount)
    {
       ConsoleInterface::postMessage(Color::white, false, 0, "%d", e.particleCount);
    }

    float attackScale = 1.0f;

    attackScale = float(unitAttackFactorTable[unitType]) / 15.0f;

    e.percentScaleMin    = 0.8f;
    e.percentScaleRand   = 0.4f;
    e.speedSmall         = int(400 * attackScale);
    e.speedMedium        = int(250 * attackScale);
    e.speedLarge         = int(150 * attackScale);
    e.percentCountSmall  = 0.5f;
    e.percentCountMedium = 0.3f;
    e.percentCountLarge  = 0.2f;
    e.flameMinScale      = 0.1f * attackScale;
    e.flameRandScale     = 0.1f * attackScale;
    e.flameLifetime      = 0.1f;
    e.flameCount         = 1;
    e.flameBounds.zero();
    e.flashMinScale      = 0.2f * attackScale;
    e.flashRandScale     = 0.3f * attackScale;
    e.flashLifetime      = 0.5f;
    e.flashCount         = 1;
    e.flashBounds.zero();

    int randTrajectoryAngleMin = rand() % 3;

    if      (randTrajectoryAngleMin == 0) {
        e.minTrajectoryAngle = 86;
    } else if (randTrajectoryAngleMin == 1) {
        e.minTrajectoryAngle = 82;
    } else if (randTrajectoryAngleMin == 2) {
        e.minTrajectoryAngle = 77;
    }

    e.puffType = DIRT;

    fXYZ pos(worldPos.x, 0, worldPos.y);

    try {
        new TemplateExplosionSystem( pos,
                                     iRect(-5, -5, 5, 5),
                                     e,
                                     Particle2D::getFarAway(pos));
    } catch(...) {}
}

void ParticleInterface::buildUnitTables()
{
    unitHitPointTable.resize(UnitProfileInterface::getNumUnitTypes());
    unitAttackFactorTable.resize(UnitProfileInterface::getNumUnitTypes());
    
    for (unsigned int i = 0; i < UnitProfileInterface::getNumUnitTypes(); i++) {
        UnitProfile *p = UnitProfileInterface::getUnitProfile(i);

        unitHitPointTable[i]     = int(sqrt(p->hit_points));
        unitAttackFactorTable[i] = int(sqrt(p->attack_factor * 2));
    }
}

//--------------------------------------------------------------------------
void ParticleInterface::addMissleFlightPuff(const iXY &worldPos, const fXY &direction, float &curWait, float &totalWait, Uint8 unitType)
{
    (void) unitType;
    iXY thrustOffset(int(-10.0f * direction.x), int(-10.0f * direction.y));
    fXYZ loc(worldPos.x + thrustOffset.x, 0, worldPos.y + thrustOffset.y);

    curWait += TimerInterface::getTimeSlice();

    if (curWait > totalWait) {
        curWait   = 0.0f;
        totalWait = (float(rand()) / float(RAND_MAX)) * 0.03f + 0.02f;

        try {
            new PuffParticle2D(	loc,
                                LIGHT,
                                0.05f,
                                0.05f,
                                smokeFPSMin,
                                smokeFPSRand,
                                smokeLayer,
                                smokeShadowLayer,
                                MuzzleSystem::windScale,
                                Particle2D::getFarAway(loc));
        } catch(...) {
            // ignored
        }
    }
}

//--------------------------------------------------------------------------
void ParticleInterface::addMuzzlePuff(const fXYZ &worldPos, const fXYZ &direction, int frame, Uint8 unitType)
{
    // Cull out any muzzle puffs which are far away.
    if (Particle2D::getFarAway(worldPos)) {
        gMuzzleSystemCullHitCount++;
        return;
    } else {
        gMuzzleSystemCullMissCount++;
    }

    MuzzleSystem m;

    m.speedMin       = 100;
    m.speedRand      = 50;
    m.waitMin        = 0.05f;
    m.waitRand       = 0.05f;
    m.lifetimeMin    = 0.2f;
    m.lifetimeRand   = 0.1f;
    m.flashMinScale  = 0.3f;
    m.flashRandScale = 0.2f;
    m.puffMinScale   = 0.1f;
    m.puffRandScale  = 0.1f;
    m.puffType       = DARK;

// XXX CHECK
//    if (unitType == _unit_type_m109) {
//        m.muzzleType = MuzzleSystem::TRIPLE;
//    } else {
        m.muzzleType = MuzzleSystem::SINGLE;
//    }

    assert(frame < 36 && frame >= 0);
    fXYZ muzzlePos;

    //char strBuf[256];
    //sprintf(strBuf, "frame: %d", frame);
    //ConsoleInterface::postMessage(strBuf);

    iXY muzzleOffset(unitParticleInfo[unitType].muzzleTip[frame]);

    muzzlePos.x = worldPos.x + muzzleOffset.x;
    muzzlePos.z = worldPos.z + muzzleOffset.y;

    // Start the muzzle smoke.
    try {
        new TemplateMuzzleSystem(muzzlePos, direction, m);
    } catch(...) {}
}

//--------------------------------------------------------------------------
void ParticleInterface::addCloudParticle(int count /* = 1 */)
{
    if ( count < 0 )
    {
        count = 0;
    }

    iXY shit;
    MapInterface::getMapPointSize(&shit);

    LOGGER.warning("Adding %d clouds", count);

    for (int i = 0; i < count; i++)
    {
        try
        {
            int px = rand() % shit.x;
            int py = rand() % shit.y;
//            LOGGER.warning("Adding %d cloud @ %d,%d", i, px, py);
            new CloudParticle2D(fXYZ(px, 0, py), iXY(shit.x, shit.y), cloudWindMinPercent, cloudWindRandPercent);
//            new CloudParticle2D(fXYZ(rand() % shit.x, 0, rand() % shit.y), iXY(shit.x, shit.y), cloudWindMinPercent, cloudWindRandPercent);
        } catch(...) {}
    }
}

void ParticleInterface::initParticleSystems()
{
    PuffParticle2D::init();
    CloudParticle2D::init();
    FlameParticle2D::init();
    FlashParticle2D::init();
    ChunkTrajectoryParticle2D::init();
    CraterParticle2D::init();

    rebuildUnitParticleData();
}

void ParticleInterface::rebuildUnitParticleData()
{
    buildUnitTables();
    getUnitParticleInfo();
}

// Purpose: Add dirt puffs under the units.
//--------------------------------------------------------------------------
void ParticleInterface::addMoveDirtPuff(const UnitState &unitState)
{
    fXYZ pos(unitState.location.x, 0, unitState.location.y);

    int isFarAway = Particle2D::getFarAway(pos);

    // Remove some of the particles if they are far away.
    if (isFarAway) {
        if ((rand() % 3) == 1) {
            return;
        }
    }

    static float movePuffWaitGroup = 0.0f;
    static float movePuffWaitTotal = 0.0f;

    movePuffWaitGroup += TimerInterface::getTimeSlice();

    // @todo this only added the puff if move val was 0, to it right
//    if (TileInterface::getWorldPixMovementValue(unitState.location.x, unitState.location.y) == 0)
    if ( true )
    {
        if (movePuffWaitGroup >= movePuffWaitTotal)
        {
            iXY size     = unitParticleInfo[unitState.unit_type].minBounds.getSize();

            assert(size.x > 0 && size.y > 0);

            iXY halfSize(size.x/2, size.y/2);

            pos.x += (rand() % size.x) - halfSize.x;
            pos.z += (rand() % size.y) - halfSize.y;

            movePuffWaitGroup = 0.0f;
            movePuffWaitTotal = (float(rand()) / float(RAND_MAX)) * 0.05f + 0.05f;

            try {
                new PuffParticle2D( pos,
                                    DIRT,
                                    0.0f,
                                    0.15f,
                                    (int) (smokeFPSMin * 0.75f),
                                    (int) (smokeFPSRand * 0.75f),
                                    1,
                                    0,
                                    MuzzleSystem::windScale,
                                    isFarAway);
            } catch(...) {
                // ignored for now
            }
        }
    }
}

//--------------------------------------------------------------------------
void ParticleInterface::getUnitParticleInfo()
{
    // Create the correct number of unit information slots.
    unitParticleInfo.resize(UnitProfileInterface::getNumUnitTypes());
    
    for ( unsigned int i=0; i< UnitProfileInterface::getNumUnitTypes(); i++ )
    {
        UnitProfile * uprofile = UnitProfileInterface::getUnitProfile(i);
        getMuzzleTips(uprofile->turretSprite, unitParticleInfo[i].muzzleTip);
        getMinBounds(uprofile->bodySprite, unitParticleInfo[i].minBounds);        
    }
}

//--------------------------------------------------------------------------
void ParticleInterface::getMuzzleTips(PackedSurface &packedSource, iXY muzzleTips[36])
{
    PtrArray<Surface> source(36);

    // Copy all the packed images onto regular surfaces.
    for (int i = 0; i < packedSource.getFrameCount(); i++)
    {
        packedSource.setFrame(i);

        Surface *s = new Surface(packedSource.getWidth(), packedSource.getHeight());
        s->fill(0);

        packedSource.blt(*s, 0, 0);
        source.push_back(s);
    }

    bool done = false;

    memset(muzzleTips, 0, sizeof(iXY) * 36);

    // Frame 0 - 8
    {for (int  i = 0; i < 9; i++)
        {
            for (int offset = 0; offset < source[i]->getCenterX(); offset++) {
                int yy = offset;
                for (int x = source[i]->getWidth() - offset; x >= 0; x--) {
                    if (source[i]->getPixel(x, yy) != 0) {
                        muzzleTips[i] = iXY(x, yy) - source[i]->getCenter();
                        done = true;
                    }
                }

                if (done) {
                    break;
                }

                int xx = source[i]->getWidth() - 1 - offset;
                for (unsigned int y = offset; y < source[i]->getHeight(); y++) {
                    if (source[i]->getPixel(xx, y) != 0) {
                        muzzleTips[i] = iXY(xx, y) - source[i]->getCenter();
                        done = true;
                    }
                }

                if (done) {
                    break;
                }
            }

            done = false;
        }}

    done = false;

    // Frame 9 - 17
    {for (int i = 9; i < 18; i++)
        {
            for (int offset = 0; offset < source[i]->getCenterX(); offset++) {
                int yy = offset;
                for (unsigned int x = offset; x < source[i]->getWidth(); x++) {
                    if (source[i]->getPixel(x, yy) != 0) {
                        muzzleTips[i] = iXY(x, yy) - source[i]->getCenter();
                        done = true;
                    }
                }

                if (done) {
                    break;
                }

                int xx = offset;
                for (unsigned int y = offset; y < source[i]->getHeight(); y++) {
                    if (source[i]->getPixel(xx, y) != 0) {
                        muzzleTips[i] = iXY(xx, y) - source[i]->getCenter();
                        done = true;
                    }
                }

                if (done) {
                    break;
                }
            }

            done = false;
        }}

    done = false;

    // Frame 18 - 26
    {for (int i = 18; i < 27; i++)
        {
            for (int offset = 0; offset < source[i]->getCenterX(); offset++) {
                int yy = source[i]->getHeight() - 1 - offset;
                for (int x = source[i]->getWidth() - 1 - offset; x > 0; x--) {
                    if (source[i]->getPixel(x, yy) != 0) {
                        muzzleTips[i] = iXY(x, yy) - source[i]->getCenter();
                        done = true;
                    }
                }

                if (done) {
                    break;
                }

                int xx = offset;
                for (int y = source[i]->getHeight() - 1; y >= 0; y--) {
                    if (source[i]->getPixel(xx, y) != 0) {
                        muzzleTips[i] = iXY(xx, y) - source[i]->getCenter();
                        done = true;
                    }
                }

                if (done) {
                    break;
                }
            }

            done = false;
        }}

    done = false;

    // Frame 27 - 35
    {for (int i = 27; i < 36; i++)
        {
            for (unsigned int offset = 0; offset < (unsigned int)source[i]->getCenterX(); offset++) {
                int yy = source[i]->getHeight() - 1 - offset;
                for (unsigned int x = offset; x < source[i]->getWidth(); x++) {
                    if (source[i]->getPixel(x, yy) != 0) {
                        muzzleTips[i] = iXY(x, yy) - source[i]->getCenter();
                        done = true;
                    }
                }

                if (done) {
                    break;
                }

                int xx = source[i]->getWidth() - 1 - offset;
                for (int y = source[i]->getHeight() - 1; y >= 0; y--) {
                    if (source[i]->getPixel(xx, y) != 0) {
                        muzzleTips[i] = iXY(xx, y) - source[i]->getCenter();
                        done = true;
                    }
                }

                if (done) {
                    break;
                }
            }

            done = false;
        }}

    source.deleteAll();

}

// getMinBounds
//--------------------------------------------------------------------------
void ParticleInterface::getMinBounds(PackedSurface &packedSource, iRect &minBounds)
{
    // Copy all the packed images onto regular surfaces.
    Surface frame0(packedSource.getWidth(), packedSource.getHeight());
    Surface frame9(packedSource.getWidth(), packedSource.getHeight());

    frame0.fill(0);
    packedSource.setFrame(0);
    packedSource.blt(frame0, 0, 0);

    frame9.fill(0);
    packedSource.setFrame(9);
    packedSource.blt(frame9, 0, 0);

    int x;
    int y;

    // Get the minY.
    x = frame0.getCenterX();
    y = 0;
    while (frame0.getPixel(x, y) == 0)
    {
        y++;
        minBounds.setLocationY(y);
    }

    // Get the maxY.
    x = frame0.getCenterX();
    y = frame0.getHeight() - 1;
    while (frame0.getPixel(x, y) == 0)
    {
        y--;
        minBounds.setHeight(y);
    }

    // Get the minX.
    x = 0;
    y = frame9.getCenterY();
    while (frame9.getPixel(x, y) == 0) {
        x++;
        minBounds.setLocationX(x);
    }

    // Get the maxX.
    x = frame9.getWidth() - 1;
    y = frame9.getCenterY();
    while (frame9.getPixel(x, y) == 0) {
        x--;
        minBounds.setWidth(x);
    }

    if (minBounds.getArea() > unitBodyMaxArea) {
        unitBodyMaxArea = minBounds.getArea();
    }

} // end ParticleInterface::getMinBounds
