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
#ifndef _BOUNDBOX_HPP
#define _BOUNDBOX_HPP

#include <assert.h>
#include "Point.hpp"

class BoundBox : public Recti
 {
  public:

   BoundBox( void ) {}

   BoundBox(int x1, int y1, int x2, int y2)
     : Recti( x1, y1, x2, y2 )
        {
     assert( min.x <= 0 && min.y <= 0 && max.x >= 0 && max.y >= 0 );  
    }
  
   BoundBox(const BoundBox &a)
     : Recti( a )
        {
     assert( min.x <= 0 && min.y <= 0 && max.x >= 0 && max.y >= 0 );   
    }
        
   BoundBox(PointXYi &nMin, PointXYi &nMax)
     : Recti( nMin, nMax )
        {
     assert( min.x <= 0 && min.y <= 0 && max.x >= 0 && max.y >= 0 );  
    }
 
   bool bounds( PointXYi &center, PointXYi &test );

   void setBounds(const PointXYi &nMin, const PointXYi &nMax );

   inline Recti getAbsRect( PointXYi &center )
   {
	 return( Recti( center + min, center + max ) );
   }
    
};

#endif
