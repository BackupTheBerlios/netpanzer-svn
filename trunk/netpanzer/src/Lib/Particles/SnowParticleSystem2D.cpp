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

#include "SnowParticleSystem2D.hpp"
#include "TimerInterface.hpp"


int SnowParticleSystem2D::snowParticleCount;


// SnowParticleSystem2D constructor
//---------------------------------------------------------------------------
SnowParticleSystem2D::SnowParticleSystem2D(fXYZ pos) : ParticleSystem2D(pos)
{
	FUNC("SnowParticleSystem2D::SnowParticleSystem2D");

	reset();

	init();

} // end SnowParticleSystem2D

// reset
//---------------------------------------------------------------------------
void SnowParticleSystem2D::reset()
{
	FUNC("SnowParticleSystem2D::reset");

	combinedTime      = 0.0;
	totalTime         = 0.0;
	snowParticleCount = 0;

	init();

} // end SnowParticleSystem2D::reset

// init
//---------------------------------------------------------------------------
void SnowParticleSystem2D::init()
{
	FUNC("SnowParticleSystem2D::init");

	snowParticles = new SnowParticle2D [SNOW_PARTICLE_COUNT];
	assert(snowParticles != 0);

	for (unsigned num = 0; num < SNOW_PARTICLE_COUNT; num++)
	{
		snowParticles[num].init(fXYZ(rand() % SCREEN_XPIX, 0, rand() % SCREEN_YPIX), 2 + rand() % 5);
	}

} // end SnowParticleSystem2D::init

// sim
//---------------------------------------------------------------------------
void SnowParticleSystem2D::sim()
{
	FUNC("SnowParticleSystem2D::sim");

	if (totalTime < 20) 
	{
		totalTime += TimerInterface::getTimeSlice();

		for (int i = 0; i < SNOW_PARTICLE_COUNT; i++)
		{
			snowParticles[i].sim();
		}

	}	else
		{
			delete snowParticles;
			delete this;
			return;
		}

} // end SnowParticleSystem2D::sim

// draw
//---------------------------------------------------------------------------
void SnowParticleSystem2D::draw(const Surface &dest, SpriteSorter &sorter)
{
	//for (unsigned num = 0; num < NUM_SNOW_PARTICLES; num++)
	//	snowParticles[num].draw(dest);

} // end SnowParticleSystem2D::draw
