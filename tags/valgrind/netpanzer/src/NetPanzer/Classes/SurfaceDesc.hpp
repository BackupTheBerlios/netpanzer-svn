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
#ifndef _SURFACEDESC_HPP
#define _SURFACEDESC_HPP

#include "PObject.hpp"
#include "Point.hpp"

enum { _surfdesc_system_mem = 0x01, _surfdesc_hardware_mem = 0x02 };
enum { _surfdesc_double_buffer = 0x01, _surfdesc_page_flipped = 0x02,
       _surfdesc_writeable = 0x04, _surfdesc_readable = 0x08 }; 

class SurfaceDesc : virtual PObject
 {
  public:
   PointXYi size;
   Recti    viewable;
   PointXYi clipOffset;  
   unsigned int      bpp;

   unsigned long offset_to_view;
   unsigned long offset_to_screen_start;

   unsigned char type;
   unsigned char attributes;

 };  // ** end SurfaceDesc

#endif // ** _SURFACEDESC_HPP
