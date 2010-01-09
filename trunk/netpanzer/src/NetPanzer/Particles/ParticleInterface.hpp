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
#ifndef __ParticleInterface_hpp__
#define __ParticleInterface_hpp__

#include <vector>

#include "Types/iXY.hpp"
#include "Types/iRect.hpp"
#include "PuffParticle2D.hpp"
#include "ParticleSystemGlobals.hpp"
#include "Units/UnitTypes.hpp"
#include "Units/UnitState.hpp"

#include <vector>

using namespace std;

class Surface;

class UnitParticleInfo
{
public:
    iXY   muzzleTip[36];  // 36 frames per unit.
    iRect minBounds;      // Body min bounds.
}; // end UnitParticleInfo

// ParticleInterface
class ParticleInterface
{
private:
    friend class ScriptManager;
    static void registerScript(const char * table_name);
    static std::vector<UnitParticleInfo> unitParticleInfo;
    static int unitBodyMaxArea;

    static void getUnitParticleInfo();
    static void getMuzzleTips(Surface &packedSource, iXY muzzleTips [36]);
    static void getMinBounds(Surface &packedSource, iRect &minBounds);

    static vector<int>   unitHitPointTable;
    static vector<int>   unitAttackFactorTable;

    static void  buildUnitTables();
    static float getFrameRateAdjustment();

public:
    static int gParticlesCanHaveSmoke;
    static int gSolidColorExplosionParticles;
    static int gTestSim;
    static int gDrawExplosionParticleCount;

    static int gExplosionFlameFlashCullMissCount;
    static int gExplosionFlameFlashCullHitCount;
    static int gMuzzleSystemCullMissCount;
    static int gMuzzleSystemCullHitCount;

    static int getExplosionFlameFlashCullMissCount()
    {
        return gExplosionFlameFlashCullMissCount;
    }
    static int getExplosionFlameFlashCullHitCount()
    {
        return gExplosionFlameFlashCullHitCount;
    }
    static int getMuzzleSystemCullMissCount()
    {
        return gMuzzleSystemCullMissCount;
    }
    static int getMuzzleSystemCullHitCount()
    {
        return gMuzzleSystemCullHitCount;
    }

    ParticleInterface();

    static void testSim();
    static void testSimText(Surface &dest);

    static void addHit(const UnitState &unitState);
    static void addMiss(const iXY &worldPos, Uint8 unitType);

    static void addMissleFlightPuff(const iXY &worldPos, const fXY &direction, float &curWait, float &totalWait, Uint8 unitType);

    static void addMuzzlePuff(const fXYZ &worldPos, const fXYZ &direction, int frame, Uint8 unitType);

    static void addCloudParticle(int count = 1);

    static void addMoveDirtPuff(const UnitState &unitState);

    static void initParticleSystems();

    static void addUnitDamagePuffParticle(const UnitState &unitState);
}; // end ParticleInterface

#endif // __ParticleInterface_hpp__
