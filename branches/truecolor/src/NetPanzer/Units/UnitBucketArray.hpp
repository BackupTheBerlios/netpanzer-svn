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
#ifndef _UNIT_BUCKET_ARRAY_HPP
#define _UNIT_BUCKET_ARRAY_HPP

#include <vector>
#include "Core/CoreTypes.hpp"

class Unit;
class iXY;
class iRect;

typedef std::vector<Unit *> UnitList;

class UnitBucketArray
{
public:
    static void queryPlayerUnitsAt(std::vector<UnitID>& working_list,
                                   const iXY& point,
                                   const Uint16 player_id);

    static void queryUnitsInWorldRect(std::vector<Unit *>& working_list,
                                      const iRect& rect);

    static void queryUnitsInWorldRectBuckets(std::vector<Unit *>& working_list,
                                             const iRect& rect);

    static void queryPlayerUnitsInWorldRect(std::vector<UnitID>& working_list,
                                            const iRect& rect,
                                            const Uint16 player_id);

    static void queryNonPlayerUnitsInWorldRect(std::vector<Unit *>& working_list,
                                               const iRect& rect,
                                               const Uint16 player_id);

    static void queryUnitsAtWorldLocBucket(std::vector<Unit *>& working_list,
                                           const iXY& world_loc);

    static Unit * queryUnitAtMapLoc(const iXY& map_loc);

    static Unit * queryNonPlayerUnitAtWorld(const iXY & world_loc,
                                            const Uint16 player_id);

    static bool queryClosestEnemyUnitInRange(Unit **closest_unit_ptr,
                                             const iXY &loc,
                                             unsigned long range,
                                             const Uint16 player_id);

    static void initialize(const unsigned int x_sample, const unsigned int y_sample);
    static void initialize()
    {
        initialize(10, 10);
    }

    static void cleanUp();

    static void addUnit(Unit *unit);
    static void removeUnit(const Unit *unit);
    static void moveUnit(Unit * unit, const iXY& prev_unit_loc);
    static void sort();
};

#endif // ** _UNIT_BUCKET_ARRAY_HPP

