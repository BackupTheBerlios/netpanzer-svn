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

#include "KeyScanCodeDefs.h"
#include "ParticleInterface.hpp"
#include "PuffParticle2D.hpp"
#include "SmolderParticleSystem2D.hpp"
#include "FlameParticle2D.hpp"
#include "FlashParticle2D.hpp"
#include "TemplateExplosionSystem.hpp"
//#include "GroundExplosionParticle2D.hpp"
//#include "GroundExplosionParticleSystem2D.hpp"
#include "CloudParticle2D.hpp"
#include "MapInterface.hpp"
#include "KeyboardInterface.hpp"
#include "Physics.hpp"
//#include "DSound.hpp"
//#include "DirtKickParticle2D.hpp"
#include "SparkParticle2D.hpp"
#include "FireParticleSystem2D.hpp"
//#include "RadarPingParticle2D.hpp"
//#include "MovementDirtPuffParticle2D.hpp"
#include "VectorPuffParticle2D.hpp"
#include "UnitTypes.hpp"
#include "WorldViewInterface.hpp"
#include "ConsoleInterface.hpp"
#include "ChunkTrajectoryParticle2D.hpp"
//#include "SmokingTrajectoryParticle2D.hpp"
#include "UnitProfileInterface.hpp"
#include "TemplateMuzzleSystem.hpp"
#include "UnitGlobals.hpp"
#include "CraterParticle2D.hpp"


cGrowList <UnitParticleInfo> ParticleInterface::unitParticleInfo;
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
{
}

//--------------------------------------------------------------------------
void ParticleInterface::addDirtPuffParticle(const iXY &worldPos)
{
//	addPuffParticle(worldPos, DIRT_PUFF, smolderMinScale, smolderRandScale, smolderMinFPS, smolderRandFPS, smolderLayer);
}

//--------------------------------------------------------------------------
void ParticleInterface::addDirtPuffSystem(const iXY &worldPos, const iRect &bounds)
{
//	addPuffSystem(worldPos, bounds, dirtPuffMaxParticleCount, DIRT_PUFF, smolderMinScale, smolderRandScale, smolderMinFPS, smolderRandFPS, smolderLayer);
}
        
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

	if (particleType == 0)
	{
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
	}
	else if (particleType == 1)
	{
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

	} else { assert(false); }

} // end ParticleInterface::addUnitDamagePuffParticle

//--------------------------------------------------------------------------
void ParticleInterface::addSmokePuffParticle(const iXY &worldPos)
{
	int particleType = rand() % 2;

	if (particleType == 0)
	{
		addSmokePuffParticle(worldPos, LIGHT);
	}
	else if (particleType == 1)
	{
		addSmokePuffParticle(worldPos, DARK);

	} else { assert(false); }

}
        
//--------------------------------------------------------------------------
void ParticleInterface::addSmokePuffParticle(const iXY &worldPos, float minScale, float randScale, int minFPS, int randFPS, int layer, float windScale /* = 1.0f */, float incrementScale /* = 0.0f */)
{
	int particleType = rand() % 2;

	if (particleType == 0)
	{
		addPuffParticle(worldPos, LIGHT, minScale, randScale, minFPS, randFPS, layer, windScale, incrementScale);
	}
	else if (particleType == 1)
	{
		addPuffParticle(worldPos, DARK, minScale, randScale, minFPS, randFPS, layer, windScale, incrementScale);

	} else { assert(false); }

}

//--------------------------------------------------------------------------
void ParticleInterface::addSmokePuffParticle(const iXY &worldPos, PUFF_TYPE type)
{
//	addPuffParticle(worldPos, type, smolderMinScale, smolderRandScale, smolderMinFPS, smolderRandFPS, smolderLayer, smolderWindScale, smolderIncrementScale);
}

//--------------------------------------------------------------------------
void ParticleInterface::addPuffParticle(const iXY &worldPos, PUFF_TYPE type, float minScale, float randScale, int minFPS, int randFPS, int layer, float windScale /* = 1.0f */, float incrementScale /* = 0.0f */)
{
//	new PuffParticle2D(fXYZ(worldPos.x, 0, worldPos.y), type, minScale, randScale, minFPS, randFPS, layer, layer - 1, windScale);
}

//--------------------------------------------------------------------------
void ParticleInterface::addSmokePuffSystem(const iXY &worldPos, const iRect &bounds, int maxParticleCount)
{
//    int particleType = rand() % 2;
//    
//    if      (particleType == 0)
//    {
//		addPuffSystem(worldPos, bounds, maxParticleCount, LIGHT, smolderMinScale, smolderRandScale, smolderMinFPS, smolderRandFPS, smolderLayer);
//    }
//    else if (particleType == 1)
//    {
//		addPuffSystem(worldPos, bounds, maxParticleCount, DARK, smolderMinScale, smolderRandScale, smolderMinFPS, smolderRandFPS, smolderLayer);
//    }
}

//--------------------------------------------------------------------------
void ParticleInterface::addPuffSystem(const iXY &worldPos, const iRect &bounds, int maxParticleCount, PUFF_TYPE particleType, float minScale, float randScale, int minFPS, int randFPS, int layer)
{
//    // Make sure the the bounding box is not negative.
//    assert(bounds.getSize().x >= 0);
//    assert(bounds.getSize().y >= 0);
//
//	iRect r(bounds);
//
//    // If all 0's were entered, give the bounds a size of 1.
//    if(r.getSize().x == 0)
//    {
//        r.max.x = 1;
//        r.min.x = 0;
//    }
//    if(r.getSize().y == 0)
//    {
//        r.max.y = 1;
//        r.min.y = 0;
//    }
//
//    assert(maxParticleCount >= 1);
//    
//    int particleCount = (rand() % maxParticleCount) + 1; // max maxParticleCount, min 1
//
//    // Generate the explosion particles
//    fXYZ offset;
//
//    for (int i = 0; i < particleCount; i++)
//    {
//        // Set the particle somewhere in the bounds of the explosion.
//        offset.x = worldPos.x + rand() % r.getSize().x;
//        offset.y = 0;
//        offset.z = worldPos.y + rand() % r.getSize().y;
//        
//        // These particles have a hardcoded waitMax and waitMin.  They are the last 2 variables
//        // in the following statement.
//        new PuffParticle2D(offset, particleType, minScale, randScale, minFPS, randFPS, layer, layer - 1, smolderWindScale);
//    }
}

//--------------------------------------------------------------------------
void ParticleInterface::addVehicleExplosionFlameParticle(const iXY &worldPos)
{
//	addExplosionFlameParticle(worldPos, vehicleExplosionFlameMinScale, vehicleExplosionFlameRandScale, vehicleExplosionFlameLayer);
}

//--------------------------------------------------------------------------
void ParticleInterface::addVehicleExplosionFlashParticle(const iXY &worldPos)
{
//	addFlashParticle(worldPos, vehicleExplosionFlashScaleMin, vehicleExplosionFlashScaleRand, vehicleExplosionFlashLifetime, vehicleExplosionFlashLayer);
}

//--------------------------------------------------------------------------
void ParticleInterface::addMissleExplosionFlameParticle(const iXY &worldPos)
{
//	addExplosionFlameParticle(worldPos, missleExplosionFlameMinScale, missleExplosionFlameRandScale, missleExplosionFlameLayer);
}

//--------------------------------------------------------------------------
void ParticleInterface::addMissleExplosionFlashParticle(const iXY &worldPos)
{
//	addFlashParticle(worldPos, missleExplosionFlashScaleMin, missleExplosionFlashScaleRand, missleExplosionFlashLifetime, missleExplosionFlashLayer);
}

//--------------------------------------------------------------------------
void ParticleInterface::addFlashParticle(const iXY &worldPos, float minScale, float randScale, float lifetime, int layer, bool singleFrame /* = false */)
{
	new FlashParticle2D(fXYZ(worldPos.x, 0, worldPos.y), minScale, randScale, lifetime, layer, singleFrame);
}

//--------------------------------------------------------------------------
void ParticleInterface::addExplosionFlameParticle(const iXY &worldPos, const float &minScale, const float &randScale, const int &layer)
{
//	new ExplosionFireParticle2D(fXYZ(worldPos.x, 0, worldPos.y), minScale, randScale, layer);
}

//--------------------------------------------------------------------------
void ParticleInterface::addMissleBurnParticle(const iXY &worldPos)
{
//	addFlashParticle(worldPos, missleExplosionFlashScaleMin, missleExplosionFlashScaleRand, missleExplosionFlashLifetime, missleExplosionFlashLayer);
}

//--------------------------------------------------------------------------
void ParticleInterface::addExplosionSmokeParticle(const iXY &worldPos, int maxParticleSpeed)
{
    // Chooses light or dark smoke.
    int particleType = rand() % 2;

    fXYZ  pos(worldPos.x, 0, worldPos.y);
    iRect r(0, 0, 0, 0);

    if      (particleType == 0)
    {
		//new ExplosionParticleSystem2D(pos, r, 1, maxParticleSpeed, LIGHT);
    }
    else if (particleType == 1)
    {
		//new ExplosionParticleSystem2D(pos, r, 1, maxParticleSpeed, DARK);
    }
}

//--------------------------------------------------------------------------
void ParticleInterface::addExplosionDirtSystem(const iXY &worldPos, const iRect &bounds)
{
    fXYZ  pos(worldPos.x, 0, worldPos.y);

    //new ExplosionParticleSystem2D(pos, bounds, dirtExplosionMaxParticleCount / 2, float(explosionParticleMaxSpeed) * 0.75f, DIRT_PUFF);
}

//--------------------------------------------------------------------------
void ParticleInterface::addGroundExplosionSystem(const iXY &worldPos, const iRect &bounds, int maxParticleCount, int maxParticleSpeed)
{
    fXYZ  pos(worldPos.x, 0, worldPos.y);

    //new GroundExplosionParticleSystem2D(pos, bounds, maxParticleCount, maxParticleSpeed);
}

//--------------------------------------------------------------------------
void ParticleInterface::addExplosionSmokeSystem(const iXY &worldPos, const iRect &bounds, int maxParticleCount, int maxParticleSpeed)
{
	// Chooses light or dark smoke.
	int particleNum = rand() % 2;
        
	PUFF_TYPE particleType;

	if      (particleNum == 0) { particleType = LIGHT; }
	else if (particleNum == 1) { particleType = DARK; }
	//if (particleNum == 2) { particleType = DIRT_PUFF; }

	fXYZ  pos(worldPos.x, 0, worldPos.y);

	//new ExplosionParticleSystem2D(pos, bounds, maxParticleCount, maxParticleSpeed, particleType);

	// Add an additional set of particles in the center of the larger explosion which
	// move at a slower initialVelocity, since real explosions have a lot of smoke lingering
	// in the middle of the smoke cloud.
	int slowerParticleSpeed = maxParticleSpeed - 1;

	if (slowerParticleSpeed > 0)
	{
		//new ExplosionParticleSystem2D(pos, bounds, maxParticleCount / 2, slowerParticleSpeed, particleType);
	}
}

//--------------------------------------------------------------------------
void ParticleInterface::addSmolderSystem(const iXY &worldPos, const iRect &bounds, float lifetime, float percent)
{
	assert(percent > 0.0 && percent <= 100.0);

	float waitMin = ((100 - percent) / 100.0) * 1.0;
	float waitMax = ((100 - percent) / 100.0) * 0.3;

	// Chooses light or dark smoke.
	int particleType = rand() % 2;

	fXYZ  pos(worldPos.x, 0, worldPos.y);

	if      (particleType == 0)
	{
		new SmolderParticleSystem2D(pos, bounds, lifetime, waitMin, waitMax, LIGHT);
	}
	else if (particleType == 1)
	{
		new SmolderParticleSystem2D(pos, bounds, lifetime, waitMin, waitMax, DARK);
	} else
	{
			assert(false);
	}
}

//--------------------------------------------------------------------------
void ParticleInterface::addCloudParticle(const iXY &worldPos, const iXY &worldSize)
{
	PointXYi shit;
	MapInterface::getMapPointSize(&shit);

	fXYZ  pos(worldPos.x, 0, worldPos.y);

	new CloudParticle2D(pos, iXY(shit.x, shit.y), cloudWindMinPercent, cloudWindRandPercent);
}

//--------------------------------------------------------------------------
void ParticleInterface::addCloudParticleSystem(int maxParticleCount)
{
    PointXYi shit;
    MapInterface::getMapPointSize(&shit);

    for (int i = 0; i < maxParticleCount; i++)
    {
		addCloudParticle(iXY(rand() % shit.x, rand() % shit.y), iXY(shit.x, shit.y));
    }
}

//--------------------------------------------------------------------------
void ParticleInterface::testSim()
{
    static float groupTime0 = 0.0f;
    static float groupTime1 = 0.0f;

    groupTime0 += TimerInterface::getTimeSlice();
    groupTime1 += TimerInterface::getTimeSlice();

	if (groupTime0 > 1.0f)
    {
        groupTime0 = 0.0f;

        //SFX
		// XXX
        // dsound.PlayAmbientSound( _not_applicable, _blow_up_tank, 0 );

		Recti gameViewRect;
		WorldViewInterface::getViewWindow(&gameViewRect);

		UnitState unitState;

		unitState.location.x = gameViewRect.min.x + 100 + (rand() % (gameViewRect.getSizeX() - 200));
		unitState.location.y = gameViewRect.min.y + 100 + (rand() % (gameViewRect.getSizeY() - 200));
		
		// Hack until all the units are actually used.
		unitState.unit_type = rand() % _MAX_UNIT_TYPES;
		//unitState.unit_type = rand() % 7;

		addHit(unitState);
	}

	if (groupTime1 > 0.5f)
    {
        groupTime1 = 0.0f;

        //SFX
		// XXX
        // dsound.PlayAmbientSound( _not_applicable, _blow_up_tank, 0 );

		Recti gameViewRect;
		WorldViewInterface::getViewWindow(&gameViewRect);

		iXY location;

		location.x = gameViewRect.min.x + 100 + (rand() % (gameViewRect.getSizeX() - 200));
		location.y = gameViewRect.min.y + 100 + (rand() % (gameViewRect.getSizeY() - 200));
		
		addMiss(location, rand() % _MAX_UNIT_TYPES);
	}
}

//--------------------------------------------------------------------------
void ParticleInterface::testSimText(const Surface &dest)
{
    char strBuf[256];

    int y       = 50;
    int yOffset = 10;

    sprintf(strBuf, "particleSystemCount: %d", ParticleSystem2D::getParticleSystemCount());
    dest.bltString5x5(iXY(2, y), strBuf, Color::white);
    y += yOffset;

    sprintf(strBuf, "particleCount:       %d", Particle2D::getFrameCount());
    dest.bltString5x5(iXY(2, y), strBuf, Color::white);
}

//--------------------------------------------------------------------------
void ParticleInterface::addHit(const UnitState &unitState)
{
    ExplosionSystem e;

	float randomness = 1.0f; //Particle2D::getScale(0.8f, 0.4f);
	float frameRateAdjustment = getFrameRateAdjustment();

	e.particleCount = 8 * (frameRateAdjustment * float(unitHitPointTable[unitState.unit_type]) * randomness);

	if (gDrawExplosionParticleCount)
	{
		char strBuf[256];
		sprintf(strBuf, "%d", e.particleCount);
		ConsoleInterface::postMessage(strBuf);
	}

	float hitPointScale = 1.0f;
	
	UnitProfile *p = UnitProfileInterface::getUnitProfile(unitState.unit_type);
	if (p != 0)
	{
		hitPointScale = float(unitHitPointTable[unitState.unit_type]) / 18.0f;
	} else
	{
		assert(false);
	}

	e.percentScaleMin    = 0.8f;
	e.percentScaleRand   = 0.6f;
	e.speedSmall         = 400 * hitPointScale;
	e.speedMedium        = 250 * hitPointScale;
	e.speedLarge         = 150 * hitPointScale;
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
	e.flashCount         = 3 * float(unitParticleInfo[unitState.unit_type].minBounds.getArea()) / float(unitBodyMaxArea);
	e.flashBounds        = unitParticleInfo[unitState.unit_type].minBounds;

	int randTrajectoryAngleMin = rand() % 3;

	if      (randTrajectoryAngleMin == 0) { e.minTrajectoryAngle = 86; }
	else if (randTrajectoryAngleMin == 1) { e.minTrajectoryAngle = 82; }
	else if (randTrajectoryAngleMin == 2) { e.minTrajectoryAngle = 77; }

	if (rand() % 2)
	{
		e.puffType = LIGHT;
	} else
	{
		e.puffType = DARK;
	}
	//explosionType.

	
	fXYZ pos(unitState.location.x, 0, unitState.location.y);

	new TemplateExplosionSystem(	pos,
									unitParticleInfo[unitState.unit_type].minBounds,
									e,
									Particle2D::getFarAway(pos));

	
	// Create some smoke in the middle of the explosion.
	//iXY   size(unitParticleInfo[unitState.unit_type].minBounds.getSize() / 2);
	//iRect bounds(-size, size);

	//addSmolderSystem(unitState.location, bounds, 3.0f, 75.0f);
//	int num = rand() % 25;
//    
//    // For fun, make a big explosion about every 25 explosions.
//	float minScale  = 0.9f;
//	float randScale = 0.5f;
//
//	float percentScale = (float(rand()) / float(RAND_MAX)) * randScale + minScale;
//
//	PUFF_TYPE puffType;
//
//	if (rand() % 2)
//	{
//		puffType = LIGHT;
//	} else
//	{
//		puffType = DARK;
//	}
//
//	new ExplosionParticleSystem2D(	fXYZ(worldPos.x, 0, worldPos.y),
//									iRect(-5, -5, 5, 5),
//									percentScale,
//									puffType);
//
//	addVehicleExplosionFlameParticle(worldPos);
//	addVehicleExplosionFlashParticle(worldPos);
//
//    num = rand() % 3;
//
//    if (num == 0)
//    {
//        num = rand() % 3;
//
//        if (num == 0)
//        {
//            int smolderLife    = rand() % 11 + 5;  // [5..15]
//            int smolderPercent = rand() % 51 + 50; // [50..100]
//
//            //addSmolderSystem(worldPos, iRect(-5, -5, 5, 5), smolderLife, smolderPercent);
//			//ConsoleInterface::postMessage("addSmolderSystem");
//        }
//    }
}

float ParticleInterface::getFrameRateAdjustment()
{
	static float frameMaxbase    = 60.0f;
	static float frameMinbase    = 20.0f;
	static float frameDifference = frameMaxbase - frameMinbase;

	if (TimerInterface::getFPS() > 60.0f)
	{
		return 1.0f;
	}

	float adjustment = (TimerInterface::getFPS() - frameMinbase) / frameDifference;

	if (adjustment < 0.2f)
	{
		adjustment = 0.2f;
	}

	return adjustment;
}

//--------------------------------------------------------------------------
void ParticleInterface::addMiss(const iXY &worldPos, BYTE unitType)
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

	e.particleCount = 5 * (frameRateAdjustment * float(unitAttackFactorTable[unitType]) * randomness);

	if (gDrawExplosionParticleCount)
	{
		char strBuf[256];
		sprintf(strBuf, "%d", e.particleCount);
		ConsoleInterface::postMessage(strBuf);
	}

	float attackScale = 1.0f;
	
	UnitProfile *p = UnitProfileInterface::getUnitProfile(unitType);
	if (p != 0)
	{
		attackScale = float(unitAttackFactorTable[unitType]) / 15.0f;
	} else
	{
		assert(false);
	}

	e.percentScaleMin    = 0.8f;
	e.percentScaleRand   = 0.4f;
	e.speedSmall         = 400 * attackScale;
	e.speedMedium        = 250 * attackScale;
	e.speedLarge         = 150 * attackScale;
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

	if      (randTrajectoryAngleMin == 0) { e.minTrajectoryAngle = 86; }
	else if (randTrajectoryAngleMin == 1) { e.minTrajectoryAngle = 82; }
	else if (randTrajectoryAngleMin == 2) { e.minTrajectoryAngle = 77; }

	e.puffType = DIRT;

	fXYZ pos(worldPos.x, 0, worldPos.y);

	new TemplateExplosionSystem(	pos,
									iRect(-5, -5, 5, 5),
									e,
									Particle2D::getFarAway(pos));
}

void ParticleInterface::buildUnitTables()
{
	for (int i = 0; i < _MAX_UNIT_TYPES; i++)
	{
		UnitProfile *p = UnitProfileInterface::getUnitProfile(i);
		
		unitHitPointTable[i]     = sqrt(p->hit_points);
		unitAttackFactorTable[i] = sqrt(p->attack_factor * 2);
	}
}


// UnitInterface.cpp
/*
                iXY loc = iXY( unit->unit_state.location.x, unit->unit_state.location.y );
                ParticleInterface::addHit(loc, iRect(loc - 5, loc + 5));
*/

// Weapon.cpp
/*
        iXY loc = iXY( location.x, location.y );
                ParticleInterface::addMiss(loc);
*/

//--------------------------------------------------------------------------
void ParticleInterface::addMissleFlightPuff(const iXY &worldPos, const fXY &direction, float &curWait, float &totalWait, BYTE unitType)
{
	iXY thrustOffset(-10.0f * direction.x, -10.0f * direction.y);
	fXYZ loc(worldPos.x + thrustOffset.x, 0, worldPos.y + thrustOffset.y);

	curWait += TimerInterface::getTimeSlice();

	if (curWait > totalWait)
	{
		curWait   = 0.0f;
		totalWait = (float(rand()) / float(RAND_MAX)) * 0.03f + 0.02f;

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
	}
}

//--------------------------------------------------------------------------
void ParticleInterface::addMissleLaunchPuff(const iXY &worldPos, const fXY &direction, BYTE unitType)
{
//	//addMissleFlashParticle(pos);
//
//	//missleLaunchPuffWaitGroup += TimerInterface::getTimeSlice();
//
//	//if (missleLaunchPuffWaitGroup > missleLaunchPuffWaitTotal)
//	//{
//		//missleLaunchPuffWaitGroup = 0.0f;
//		//missleLaunchPuffWaitTotal = MISSLE_FLIGHT_PUFF_RANDNUM;
//	fXYZ pos(worldPos.x, 0, worldPos.y);
//
//	new VectorPuffParticle2D(pos, -direction, missleLaunchPuffMinScale, missleLaunchPuffRandScale, missleLaunchPuffSpeedMin, missleLaunchPuffSpeedRand, missleLaunchPuffWaitMin, missleLaunchPuffWaitRand, missleLaunchPuffLifetimeMin, missleLaunchPuffLifetimeRand, missleLaunchPuffWindScale);
//	//for (int i = 0; i < 10; i++)
//	//{
//		//const int radius = 10;
//
//		//pos.x += (rand() % (radius << 1)) - radius;
//		//pos.y += (rand() % (radius << 1)) - radius;
//
//		//addSmokePuffParticle(pos);
//		//addSmokePuffParticle(pos, missleLaunchPuffMinScale, missleLaunchPuffRandScale, missleLaunchPuffMinFPS, missleLaunchPuffRandFPS, missleLaunchPuffLayer);
//	//}
//	//}
}

//--------------------------------------------------------------------------
void ParticleInterface::addMissleExplosion(const iXY &worldPos)
{
	addMissleExplosionFlameParticle(worldPos);
	addMissleExplosionFlashParticle(worldPos);
}
	
//--------------------------------------------------------------------------
void ParticleInterface::addMuzzlePuff(const fXYZ &worldPos, const fXYZ &direction, int frame, BYTE unitType)
{
	// Cull out any muzzle puffs which are far away.
	if (Particle2D::getFarAway(worldPos))
	{
		gMuzzleSystemCullHitCount++;
		return;
	} else
	{
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
	
	if (unitType == _unit_type_m109)
	{
		m.muzzleType = MuzzleSystem::TRIPLE;
	} else
	{
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
	new TemplateMuzzleSystem(muzzlePos, direction, m);
}

//--------------------------------------------------------------------------
void ParticleInterface::addDirtKick(const iXY &worldPos)
{
	//new DirtKickParticle2D(fXYZ(worldPos.x, 0, worldPos.y));
}

//--------------------------------------------------------------------------
void ParticleInterface::addCloudParticle(int count /* = 1 */)
{
    assert(count >= 0);

	PointXYi shit;
    MapInterface::getMapPointSize(&shit);

    for (int i = 0; i < count; i++)
    {
		new CloudParticle2D(fXYZ(rand() % shit.x, 0, rand() % shit.y), iXY(shit.x, shit.y), cloudWindMinPercent, cloudWindRandPercent);
    }
}

void ParticleInterface::initParticleSystems()
{
        randomize();

        PuffParticle2D::init();
        //SparkParticle2D::init();
        CloudParticle2D::init();
        FlameParticle2D::init();
        FlashParticle2D::init();
        //FireParticleSystem2D::initColors();
        //GroundExplosionParticle2D::init();
        ChunkTrajectoryParticle2D::init();
        CraterParticle2D::init();
        //DirtKickParticle2D::init();
        //RadarPingParticle2D::init();

        //PointXYi shit;
        //MapInterface::getMapPointSize(&shit);
        //addCloudParticle(iXY(400, 400), iXY(shit.x, shit.y));

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
	if (isFarAway)
	{
		if ((rand() % 3) == 1)
		{
			return;
		}
	}

	static float movePuffWaitGroup = 0.0f;
	static float movePuffWaitTotal = 0.0f;

	movePuffWaitGroup += TimerInterface::getTimeSlice();

	if (TileInterface::getWorldPixMovementValue(unitState.location.x, unitState.location.y) == 0)
	{
		if (movePuffWaitGroup >= movePuffWaitTotal)
		{
			iXY size     = unitParticleInfo[unitState.unit_type].minBounds.getSize();

			assert(size > 0);

			iXY halfSize = size / 2;

			pos.x += (rand() % size.x) - halfSize.x;
			pos.z += (rand() % size.y) - halfSize.y;

			movePuffWaitGroup = 0.0f;
			movePuffWaitTotal = (float(rand()) / float(RAND_MAX)) * 0.05f + 0.05f;

			new PuffParticle2D(	pos,
								DIRT,
								0.0f,
								0.15f,
								smokeFPSMin * 0.75f,
								smokeFPSRand * 0.75f,
								1,
								0,
								MuzzleSystem::windScale,
								isFarAway);
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
	assert(gValentineTurret.getFrameCount() > 0);

	// Create the correct number of unit information slots.
	unitParticleInfo.setNum(_MAX_UNIT_TYPES);
	
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

	// _unit_type_hover_craft
	//getMuzzleTips(gHoverCraftBody, unitParticleInfo[_unit_type_hover_craft].muzzleTip);
	//getMinBounds(gHoverCraftBody, unitParticleInfo[_unit_type_hover_craft].minBounds);

	// _unit_type_comm_killer
	//getMuzzleTips(gCommKillerBody, unitParticleInfo[_unit_type_comm_killer].muzzleTip);
	//getMinBounds(gCommKillerBody, unitParticleInfo[_unit_type_comm_killer].minBounds);

	// _unit_type_refueler
	//getMuzzleTips(gRefuelerBody, unitParticleInfo[_unit_type_refueler].muzzleTip);
	//getMinBounds(gRefuelerBody, unitParticleInfo[_unit_type_refueler].minBounds);

	// _unit_type_archer
	getMuzzleTips(gArcherTurret, unitParticleInfo[_unit_type_archer].muzzleTip);
	getMinBounds(gArcherBody, unitParticleInfo[_unit_type_archer].minBounds);

}

//--------------------------------------------------------------------------
void ParticleInterface::getMuzzleTips(PackedSurface &packedSource, iXY muzzleTips[36])
{
	// Copy all the packed images onto regular surfaces.
	Surface source(packedSource.getPix(), packedSource.getPixX(), packedSource.getFrameCount());
	{for (int i = 0; i < packedSource.getFrameCount(); i++)
	{
		source.setFrame(i);
		source.fill(0);

		packedSource.setFrame(i);
		packedSource.blt(source, 0, 0);
	}}

	bool done = false;

	memset(muzzleTips, 0, sizeof(iXY) * 36);

	// Frame 0 - 8
	{for (int  i = 0; i < 9; i++)
	{
		source.setFrame(i);

		for (int offset = 0; offset < source.getCenterX(); offset++)
		{
			int yy = offset;
			for (int x = source.getPixX() - offset; x >= 0; x--)
			{
				if (source.getPixel(x, yy) != 0)
				{
					muzzleTips[i] = iXY(x, yy) - source.getCenter();
					done = true;
				}
			}

			if (done) { break; }

			int xx = source.getPixX() - 1 - offset;
			for (int y = offset; y < source.getPixY(); y++)
			{
				if (source.getPixel(xx, y) != 0)
				{
					muzzleTips[i] = iXY(xx, y) - source.getCenter();
					done = true;
				}
			}

			if (done) { break; }
		}

		done = false;
	}}

	done = false;

	// Frame 9 - 17
	{for (int i = 9; i < 18; i++)
	{
		source.setFrame(i);

		for (int offset = 0; offset < source.getCenterX(); offset++)
		{
			int yy = offset;
			for (int x = offset; x < source.getPixX(); x++)
			{
				if (source.getPixel(x, yy) != 0)
				{
					muzzleTips[i] = iXY(x, yy) - source.getCenter();
					done = true;
				}
			}

			if (done) { break; }

			int xx = offset;
			for (int y = offset; y < source.getPixY(); y++)
			{
				if (source.getPixel(xx, y) != 0)
				{
					muzzleTips[i] = iXY(xx, y) - source.getCenter();
					done = true;
				}
			}

			if (done) { break; }
		}

		done = false;
	}}

	done = false;

	// Frame 18 - 26
	{for (int i = 18; i < 27; i++)
	{
		source.setFrame(i);

		for (int offset = 0; offset < source.getCenterX(); offset++)
		{
			int yy = source.getPixY() - 1 - offset;
			for (int x = source.getPixX() - 1 - offset; x > 0; x--)
			{
				if (source.getPixel(x, yy) != 0)
				{
					muzzleTips[i] = iXY(x, yy) - source.getCenter();
					done = true;
				}
			}

			if (done) { break; }

			int xx = offset;
			for (int y = source.getPixY() - 1; y >= 0; y--)
			{
				if (source.getPixel(xx, y) != 0)
				{
					muzzleTips[i] = iXY(xx, y) - source.getCenter();
					done = true;
				}
			}

			if (done) { break; }
		}

		done = false;
	}}

	done = false;

	// Frame 27 - 35
	{for (int i = 27; i < 36; i++)
	{
		source.setFrame(i);

		for (int offset = 0; offset < source.getCenterX(); offset++)
		{
			int yy = source.getPixY() - 1 - offset;
			for (int x = offset; x < source.getPixX(); x++)
			{
				if (source.getPixel(x, yy) != 0)
				{
					muzzleTips[i] = iXY(x, yy) - source.getCenter();
					done = true;
				}
			}

			if (done) { break; }

			int xx = source.getPixX() - 1 - offset;
			for (int y = source.getPixY() - 1; y >= 0; y--)
			{
				if (source.getPixel(xx, y) != 0)
				{
					muzzleTips[i] = iXY(xx, y) - source.getCenter();
					done = true;
				}
			}

			if (done) { break; }
		}

		done = false;
	}}

}

// getMinBounds
//--------------------------------------------------------------------------
void ParticleInterface::getMinBounds(PackedSurface &packedSource, iRect &minBounds)
{
	// Copy all the packed images onto regular surfaces.
	Surface source(packedSource.getPix(), packedSource.getPixX(), packedSource.getFrameCount());
	for (int i = 0; i < packedSource.getFrameCount(); i++)
	{
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
	while (source.getPixel(x, y) == 0)
	{
		y++;
		minBounds.min.y = y;
	}

	// Get the maxY.
	x = source.getCenterX();
	y = source.getPixY() - 1;
	source.setFrame(0);
	while (source.getPixel(x, y) == 0)
	{
		y--;
		minBounds.max.y = y;
	}

	// Get the minX.
	x = 0;
	y = source.getCenterY();
	source.setFrame(9);
	while (source.getPixel(x, y) == 0)
	{
		x++;
		minBounds.min.x = x;
	}

	// Get the maxX.
	x = source.getPixX() - 1;
	y = source.getCenterY();
	source.setFrame(9);
	while (source.getPixel(x, y) == 0)
	{
		x--;
		minBounds.max.x = x;
	}

	if (minBounds.getArea() > unitBodyMaxArea)
	{
		unitBodyMaxArea = minBounds.getArea();
	}

} // end ParticleInterface::getMinBounds
