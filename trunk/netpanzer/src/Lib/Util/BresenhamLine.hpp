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
#ifndef _BRESENHAMLINE_HPP
#define _BRESENHAMLINE_HPP

#include "Types/iXY.hpp"

class BresenhamLine
{
protected:

    long  x;
    long  y;

    long  DeltaX;
    long  DeltaY;

    long  Deltax2;
    long  DeltaAx2minusDeltaBx2;
    long  ErrorTerm;
    short XDirection;
    short Octant;

    short finished;

    void IniOctant_0_3( void );
    void IniOctant_1_2( void );
    void IniOctant_4_7( void );
    void IniOctant_5_6( void );

    short Octant_0_3( void );
    short Octant_1_2( void );
    short Octant_4_7( void );
    short Octant_5_6( void );


public:

    BresenhamLine( void )
    { }

    void set_path( iXY &start, iXY &end );

    short increment( iXY *update, short times );


};


#endif // ** _BRESENHAMLINE_HPP
