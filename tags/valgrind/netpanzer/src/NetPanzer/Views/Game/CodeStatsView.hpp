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
#ifndef __CodeStatsView_hpp__
#define __CodeStatsView_hpp__

#include "GameTemplateView.hpp"
#include "Surface.hpp"


#if _MSC_VER > 1000
	#pragma once
#endif


class CodeStatsView : public GameTemplateView
{
 protected:
  void drawNetworkStats( const Surface &clientArea);
  void drawSorterStats( const Surface &clientArea);
  void drawPathingStats( const Surface &clientArea);
  void drawUnitStats( const Surface &clientArea );

 public:
  CodeStatsView();
    
  virtual void doDraw(const Surface &windowArea, const Surface &clientArea);

}; // end CodeStatsView


#endif // end __CodeStatsView_hpp__
