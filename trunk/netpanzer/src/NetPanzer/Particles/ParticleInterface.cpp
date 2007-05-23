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
#include <config.h>

#include "ParticleInterface.hpp"
#include "PuffParticle2D.hpp"
#include "FlameParticle2D.hpp"
#include "FlashParticle2D.hpp"
#include "TemplateExplosionSystem.hpp"
#include "CloudParticle2D.hpp"
#include "MapInterface.hpp"
#include "KeyboardInterface.hpp"
#include "Physics.hpp"
#include "VectorPuffParticle2D.hpp"
#include "UnitTypes.hpp"
#include "WorldViewInterface.hpp"
#include "ConsoleInterface.hpp"
#include "ChunkTrajectoryParticle2D.hpp"
#include "UnitProfileInterface.hpp"
#include "TemplateMuzzleSystem.hpp"
#include "UnitGlobals.hpp"
#include "CraterParticle2D.hpp"
#include "Util/Exception.hpp"
#include "System/Sound.hpp"
#include "2D/Color.hpp"
#include "2D/Palette.hpp"


std::vector<UnitParticleInfo> ParticleInterface::unitParticleInfo;
int ParticleInterface::unitBodyMaxArea                   = 0;
int ParticleInterface::gParticlesCanHaveSmoke            = 1;
int ParticleInterface::gSolidColorExplosionParticles     = 0;
int ParticleInterface::gTestSim                          = 0;
int ParticleInterface::gDrawExplosionParticleCount       = 0;
int ParticleInterface::gExplosionFlameFlashCullMissCount = 1;
int ParticleInterface::gExplosionFlameFlashCullHitCount  = 1;
int ParticleInterface::gMuzzleSystemCullMissCount        = 1;
int ParticleInterface::gMuzzleSystemCullHitCount         = 1;

int ParticleInterface::unitHitPointTable[_MAX_UNIT_TYPES];
int ParticleInterface::unitAttackFactorTable[_MAX_UNIT_TYPES];


//--------------------------------------------------------------------------
ParticleInterface::ParticleInterface()
{}

// addUnitDamagePuffParticle
//--------------------------------------------------------------------------
void ParticleInterface::addUnitDamagePuffParticle(const UnitState &unitState)
{
    iXY pos(unitState.location);

    iXY size     = unitParticleInfo[unitState.unit_type].minBounds.getSize();
    iXY halfSize = size / 2;

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

        unitState.location.x = gameViewRect.min.x + 100 + (rand() % (gameViewRect.getSizeX() - 200));
        unitState.location.y = gameViewRect.min.y + 100 + (rand() % (gameViewRect.getSizeY() - 200));

        // Hack until all the units are actually used.
        unitState.unit_type = rand() % _MAX_UNIT_TYPES;
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

        location.x = gameViewRect.min.x + 100 + (rand() % (gameViewRect.getSizeX() - 200));
        location.y = gameViewRect.min.y + 100 + (rand() % (gameViewRect.getSizeY() - 200));

        addMiss(location, rand() % _MAX_UNIT_TYPES);
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

    if (gDrawExplosionParticleCount) {
        char strBuf[256];
        sprintf(strBuf, "%d", e.particleCount);
        ConsoleInterface::postMessage(strBuf);
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
      char  reaction_time;
      char  speed_factor;
      char  speed_rate;
      char  tracking_rate;
      short fuel_capacity;
      char  reload_time;
      char  repath_time; 
      short regen_time;
      char  jammable;
      char  jamming_time;
      // ** for fuel truck only
      long  fueling_range;
      // ** for ecm truck only ** 
      long  jamming_range; 
    */

    ExplosionSystem e;

    float randomness = 1.0f; //Particle2D::getScale(0.8f, 0.4f);
    float frameRateAdjustment = getFrameRateAdjustment();

    e.particleCount = int(5 * (frameRateAdjustment * float(unitAttackFactorTable[unitType]) * randomness));

    if (gDrawExplosionParticleCount) {
        char strBuf[256];
        sprintf(strBuf, "%d", e.particleCount);
        ConsoleInterface::postMessage(strBuf);
    }

    float attackScale = 1.0f;

    UnitProfile *p = UnitProfileInterface::getUnitProfile(unitType);
    assert(p != 0);
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
    for (int i = 0; i < _MAX_UNIT_TYPES; i++) {
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

    if (unitType == _unit_type_m109) {
        m.muzzleType = MuzzleSystem::TRIPLE;
    } else {
        m.muzzleType = MuzzleSystem::SINGLE;
    }

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
    assert(count >= 0);

    iXY shit;
    MapInterface::getMapPointSize(&shit);

    for (int i = 0; i < count; i++) {
        try {
            new CloudParticle2D(fXYZ(rand() % shit.x, 0, rand() % shit.y), iXY(shit.x, shit.y), cloudWindMinPercent, cloudWindRandPercent);
        } catch(...) {}
    }
}

void ParticleInterface::initParticleSystems()
{
    Palette::init("netp");
    PuffParticle2D::init();
    CloudParticle2D::init();
    FlameParticle2D::init();
    FlashParticle2D::init();
    ChunkTrajectoryParticle2D::init();
    CraterParticle2D::init();
	
    buildUnitTables();
    getUnitParticleInfo();
    Palette::init("netpmenu");
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

    if (TileInterface::getWorldPixMovementValue(unitState.location.x, unitState.location.y) == 0) {
        if (movePuffWaitGroup >= movePuffWaitTotal) {
            iXY size     = unitParticleInfo[unitState.unit_type].minBounds.getSize();

            assert(size.x > 0 && size.y > 0);

            iXY halfSize = size / 2;

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

//enum  { _unit_type_valentine,
//        _unit_type_leopard,
//        _unit_type_abrams,
//        _unit_type_hammerhead,
//        _unit_type_lynx,
//        _unit_type_m109,
//        _unit_type_spahpanzer,
//        _unit_type_scorpion,
//        _unit_type_humvee,
//        _unit_type_hover_craft,
//        _unit_type_comm_killer,
//        _unit_type_refueler,
//        _unit_type_null
//      };

//--------------------------------------------------------------------------
void ParticleInterface::getUnitParticleInfo()
{
    assert(gValentineTurret.getNumFrames() > 0);

    // Create the correct number of unit information slots.
    unitParticleInfo.resize(_MAX_UNIT_TYPES);

    // _unit_type_valentine
    getMuzzleTips(gValentineTurret, unitParticleInfo[_unit_type_valentine].muzzleTip);
    getMinBounds(gValentineBody, unitParticleInfo[_unit_type_valentine].minBounds);

    // _unit_type_leopard
    getMuzzleTips(gLeopardTurret, unitParticleInfo[_unit_type_leopard].muzzleTip);
    getMinBounds(gLeopardBody, unitParticleInfo[_unit_type_leopard].minBounds);

    // _unit_type_abrams
    getMuzzleTips(gAbramsTurret, unitParticleInfo[_unit_type_abrams].muzzleTip);
    getMinBounds(gAbramsBody, unitParticleInfo[_unit_type_abrams].minBounds);

    // _unit_type_hammerhead
    getMuzzleTips(gHammerheadTurret, unitParticleInfo[_unit_type_hammerhead].muzzleTip);
    getMinBounds(gHammerheadBody, unitParticleInfo[_unit_type_hammerhead].minBounds);

    // _unit_type_humvee
    getMuzzleTips(gSpahPanzerBody, unitParticleInfo[_unit_type_humvee].muzzleTip);
    getMinBounds(gSpahPanzerBody, unitParticleInfo[_unit_type_humvee].minBounds);

    // _unit_type_lynx
    getMuzzleTips(gLynxTurret, unitParticleInfo[_unit_type_lynx].muzzleTip);
    getMinBounds(gLynxBody, unitParticleInfo[_unit_type_lynx].minBounds);

    // _unit_type_m109
    getMuzzleTips(gM109Turret, unitParticleInfo[_unit_type_m109].muzzleTip);
    getMinBounds(gM109Body, unitParticleInfo[_unit_type_m109].minBounds);

    // _unit_type_spahpanzer
    getMuzzleTips(gBearTurret, unitParticleInfo[_unit_type_spahpanzer].muzzleTip);
    getMinBounds(gBearBody, unitParticleInfo[_unit_type_spahpanzer].minBounds);

    // _unit_type_scorpion
    getMuzzleTips(gScorpionTurret, unitParticleInfo[_unit_type_scorpion].muzzleTip);
    getMinBounds(gScorpionBody, unitParticleInfo[_unit_type_scorpion].minBounds);

    // _unit_type_archer
    getMuzzleTips(gArcherTurret, unitParticleInfo[_unit_type_archer].muzzleTip);
    getMinBounds(gArcherBody, unitParticleInfo[_unit_type_archer].minBounds);

}

//--------------------------------------------------------------------------
void ParticleInterface::getMuzzleTips(Surface &source, iXY muzzleTips[36])
{
    // XXX here may need to lock surfaces
    bool done = false;

    memset(muzzleTips, 0, sizeof(iXY) * 36);
    float saveframe=source.getCurFrame();

    // Frame 0 - 8
    {for (int  i = 0; i < 9; i++)
        {
            source.setFrame(i);

            for (unsigned int offset = 0; offset < source.getCenterX(); offset++) {
                int yy = offset;
                for (int x = source.getWidth() - offset; x >= 0; x--) {
                    if (source.getPixel(x, yy) != 0) {
                        muzzleTips[i] = iXY(x, yy) - source.getCenter();
                        done = true;
                    }
                }

                if (done) {
                    break;
                }

                int xx = source.getWidth() - 1 - offset;
                for (unsigned int y = offset; y < source.getHeight(); y++) {
                    if (source.getPixel(xx, y) != 0) {
                        muzzleTips[i] = iXY(xx, y) - source.getCenter();
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
            source.setFrame(i);

            for (unsigned int offset = 0; offset < source.getCenterX(); offset++) {
                int yy = offset;
                for (unsigned int x = offset; x < source.getWidth(); x++) {
                    if (source.getPixel(x, yy) != 0) {
                        muzzleTips[i] = iXY(x, yy) - source.getCenter();
                        done = true;
                    }
                }

                if (done) {
                    break;
                }

                int xx = offset;
                for (unsigned int y = offset; y < source.getHeight(); y++) {
                    if (source.getPixel(xx, y) != 0) {
                        muzzleTips[i] = iXY(xx, y) - source.getCenter();
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
            source.setFrame(i);

            for (unsigned int offset = 0; offset < source.getCenterX(); offset++) {
                int yy = source.getHeight() - 1 - offset;
                for (int x = source.getWidth() - 1 - offset; x > 0; x--) {
                    if (source.getPixel(x, yy) != 0) {
                        muzzleTips[i] = iXY(x, yy) - source.getCenter();
                        done = true;
                    }
                }

                if (done) {
                    break;
                }

                int xx = offset;
                for (int y = source.getHeight() - 1; y >= 0; y--) {
                    if (source.getPixel(xx, y) != 0) {
                        muzzleTips[i] = iXY(xx, y) - source.getCenter();
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
            source.setFrame(i);

            for (unsigned int offset = 0; offset < source.getCenterX(); offset++) {
                int yy = source.getHeight() - 1 - offset;
                for (unsigned int x = offset; x < source.getWidth(); x++) {
                    if (source.getPixel(x, yy) != 0) {
                        muzzleTips[i] = iXY(x, yy) - source.getCenter();
                        done = true;
                    }
                }

                if (done) {
                    break;
                }

                int xx = source.getWidth() - 1 - offset;
                for (int y = source.getHeight() - 1; y >= 0; y--) {
                    if (source.getPixel(xx, y) != 0) {
                        muzzleTips[i] = iXY(xx, y) - source.getCenter();
                        done = true;
                    }
                }

                if (done) {
                    break;
                }
            }

            done = false;
        }}
        
    
    source.setFrame(saveframe);
}

// getMinBounds
//--------------------------------------------------------------------------
void ParticleInterface::getMinBounds(Surface &packedSource, iRect &minBounds)
{
    // Copy all the packed images onto regular surfaces.
    Surface source(packedSource.getWidth(), packedSource.getHeight(), packedSource.getNumFrames());
    for (unsigned int i = 0; i < packedSource.getNumFrames(); i++) {
        source.setFrame(i);
        source.fill(0);

        packedSource.setFrame(i);
        packedSource.blt(source, 0, 0);
    }

    int x;
    int y;

    // Get the minY.
    x = source.getCenterX();
    y = 0;
    source.setFrame(0);
    while (source.getPixel(x, y) == 0) {
        y++;
        minBounds.min.y = y;
    }

    // Get the maxY.
    x = source.getCenterX();
    y = source.getHeight() - 1;
    source.setFrame(0);
    while (source.getPixel(x, y) == 0) {
        y--;
        minBounds.max.y = y;
    }

    // Get the minX.
    x = 0;
    y = source.getCenterY();
    source.setFrame(9);
    while (source.getPixel(x, y) == 0) {
        x++;
        minBounds.min.x = x;
    }

    // Get the maxX.
    x = source.getWidth() - 1;
    y = source.getCenterY();
    source.setFrame(9);
    while (source.getPixel(x, y) == 0) {
        x--;
        minBounds.max.x = x;
    }

    if (minBounds.getArea() > unitBodyMaxArea) {
        unitBodyMaxArea = minBounds.getArea();
    }

} // end ParticleInterface::getMinBounds
