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
#ifndef __Math_hpp__
#define __Math_hpp__

#include <math.h>

#include "fXY.hpp"
#include "iXY.hpp"

#ifndef M_PI
#define	M_PI 3.14159265358979323846
#endif

const  double PI_DIV_2          = M_PI / 2.0;
const  double deg2RadConversion = M_PI / 180.0;
const  double rad2DegConversion = 180.0 / M_PI;

//--------------------------------------------------------------------------
class Math
{
private:
	static float cosTable[360 + 1];
	static float sinTable[360 + 1];

public:
	static int   rad2Deg(float radian);
	static float deg2Rad(int degree);
	static float radAngle(const iXY &a, const iXY &b);
	static float radAngle(float x, float y);
	static int   degAngle(const iXY &a, const iXY &b);
	static int   degAngle(float x, float y);
	static fXY   unitDirection(const iXY &a, const iXY &b);
	static fXY   unitDirection(float angle);
	static fXY   unitDirectionEast(const fXY &northDirection);
	static fXY   unitDirectionWest(const fXY &northDirection);
	
	static float getCos(int angle);
	static float getSin(int angle);
	
	static void init();

}; // end Math

#endif // end __Math_hpp__