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
#ifndef _ANGLE_HPP
#define _ANGLE_HPP

#include "math.h"
#include "PObject.hpp"
#include "Point.hpp"

class Angle : public PObject
 {
   protected: 
    static double deg_const; 
    static double pi_const;

   public:
    double angle;
      
   public:
         
   Angle() { }

   Angle( double nAngle )
    {
     angle = nAngle;
        }

   Angle( long x, long y) 
    { 
     angle = (double) atan2( (double) -y, (double) x );                 
        }
   
   Angle( PointXYi &vec )
    {
     set( vec );
    }
 
   inline void set( PointXYi &vec  )
    {
     angle = (double) atan( ((double) -vec.y / (double) vec.x) ); 
     
     if ( angle > 0 )
      {
       if ( vec.x < 0 )
        {
         angle -= pi_const;
        }
      }
    else    
     if ( angle < 0 )
      {
        if ( ((vec.y) < 0) && (vec.x < 0) )
         {
          angle += pi_const;
         }
      }
     else
      if ( angle == 0 )
       {
        if ( vec.x < 0 )
         angle += pi_const;
       }
    }
   
   double DegreesFloat( void )
    {
     double degrees;
     degrees = deg_const * angle;
     if ( degrees < 0 )
          degrees = 360 - degrees;
          return( degrees );
        } 
 
    long DegreesInt( void )
     {
      double degrees;
      degrees = deg_const * angle;
      if ( degrees < 0 )
       degrees = 360 + degrees;
      return( (unsigned long ) degrees );
     } 
  
    long DegreesInt( unsigned long granularity )
     {
          double degrees;
          degrees = deg_const * angle;
      if ( degrees < 0 )
           degrees = 360 + degrees;
      
          degrees = degrees / (double) granularity; 
          return( (unsigned long ) degrees );
         } 

 };


class AngleInt : public Angle
 {
  public:
   long angle_int;
   unsigned long grain;
   long angle_limit;

   AngleInt() { }

   AngleInt( long nAngle, unsigned long granularity )
     : Angle( (double) nAngle ) 
        {
     angle_int = nAngle;
         grain = granularity;
         angle_limit = (360 / grain);    
        }

   AngleInt( long x, long y) 
     : Angle( x, y )    
        { 
     angle_int = DegreesInt();         
        }
   
   AngleInt( PointXYi &vec )
     : Angle( vec )
        {
     angle_int = DegreesInt();     
        }
   
   inline void set( long nAngle, unsigned long granularity )
    {
     angle_int = nAngle;
         grain = granularity;
         angle_limit = (360 / grain);    
        }
       
   inline long operator++( )
    {
     angle_int++;
         if (angle_int == angle_limit )
          angle_int = 0; 
         return( angle_int );
        }
 
   inline long operator--( )
    {
     angle_int--;
         if (angle_int < 0 )
          angle_int = angle_limit - 1; 
     return( angle_int );
        }
 
   inline long Degrees()
    {
     return( angle_int * grain );
    }
 };

#endif
