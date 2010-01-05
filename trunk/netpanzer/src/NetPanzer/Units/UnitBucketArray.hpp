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
#include <list>
#include <algorithm>
#include "Units/Unit.hpp"
#include "Types/iRect.hpp"

typedef std::vector<Unit *> UnitList;
typedef std::vector<UnitList> BucketList;

class UnitBucketArray
{
public:
    static void queryPlayerUnitsAt(std::vector<UnitID>& working_list,
                            const iXY& point,
                            const Uint16 player_id);

    static void queryUnitsInWorldRect(std::vector<Unit *>& working_list,
                               const iRect& rect);

    static void queryPlayerUnitsInWorldRect(std::vector<UnitID>& working_list,
                                     const iRect& rect,
                                     const Uint16 player_id);

    static void queryNonPlayerUnitsInWorldRect(std::vector<Unit *>& working_list,
                                        const iRect& rect,
                                        const Uint16 player_id);

    static Unit * queryUnitAtMapLoc(const iXY& map_loc);

    static Unit * queryNonPlayerUnitAtWorld(const iXY & world_loc,
                                     const Uint16 player_id);

    static bool queryClosestEnemyUnitInRange(Unit **closest_unit_ptr,
                                      const iXY &loc,
                                      unsigned long range,
                                      const Uint16 player_id);

    static void initialize()
    {
        initialize(10, 10);
    }


private:
    friend class UnitInterface;
    
    static void initialize(const unsigned int x_sample, const unsigned int y_sample);


    static void cleanUp()
    {
        if ( buckets )
        {
            delete buckets;
            buckets = 0;
        }
    }

    static void sort();
    
    static void moveUnit(Unit * unit, const unsigned int from, const unsigned int to)
    {
        UnitList & uli = getBucket(from);
        uli.erase(std::remove(uli.begin(), uli.end(), unit), uli.end());
        getBucket(to).push_back(unit);
    }

    static Unit * getUnitAtWorldLoc(const UnitID unit_id, const iXY & world_loc)
    {
        UnitList & uli = getBucketAssocWorldLoc(world_loc);
        UnitList::iterator iter;
        iter = std::find_if( uli.begin(), uli.end(), FindUnit(unit_id));
        return (iter != uli.end())?*iter:0;
    }

    static Unit * getUnitAtMapLoc(const UnitID unit_id, const iXY & map_loc)
    {
        UnitList & uli = getBucketAssocMapLoc(map_loc);
        UnitList::iterator iter;
        iter = std::find_if( uli.begin(), uli.end(), FindUnit(unit_id));
        return (iter != uli.end())?*iter:0;
    }

    static void addUnit(Unit *unit)
    {
        getBucketAssocWorldLoc(unit->unit_state.location).push_back(unit);
    }

    static void removeUnit(const Unit *unit)
    {
        UnitList & uli = getBucketAssocWorldLoc(unit->unit_state.location);
        uli.erase(std::remove(uli.begin(), uli.end(), unit), uli.end());
    }

    static unsigned int getSize()
    {
        return buckets->size();
    }

    static UnitList & getBucket(const unsigned int bucket_index)
    {
//        assert( bucket_index < getSize() );
        return( (*buckets)[ bucket_index ] );
    }

    static UnitList & getBucket(const unsigned int row, const unsigned int col)
    {
        return ( getBucket((row * column_size) + col) );
    }

    static unsigned int mapLocToBucketIndex(const iXY & map_loc)
    {
        return ((map_loc.y / map_y_sample_factor) * column_size)
                + (map_loc.x / map_x_sample_factor);
    }

    static unsigned int worldLocToBucketIndex(const iXY & world_loc)
    {
        return ((world_loc.y / pixel_y_sample_factor) * column_size)
                + (world_loc.x / pixel_x_sample_factor);
    }

    static void worldLocToBucketLoc(const iXY & world_loc, iXY & bucket_loc)
    {
        bucket_loc.x = (world_loc.x-1) / pixel_x_sample_factor;
        bucket_loc.y = (world_loc.y-1) / pixel_y_sample_factor;
        if ( bucket_loc.x < 0 )
        {
            bucket_loc.x = 0;
        }

        if ( (size_t)bucket_loc.x >= column_size)
        {
            bucket_loc.x = column_size-1;
        }

        if ( bucket_loc.y < 0 )
        {
            bucket_loc.y = 0;
        }

        if ( (size_t)bucket_loc.y >= row_size )
        {
            bucket_loc.y = row_size -1;
        }
    }

    static void worldRectToBucketRect(const iRect & world_rect, iRect &bucket_rect)
    {
        worldLocToBucketLoc(world_rect.min, bucket_rect.min);
        worldLocToBucketLoc(world_rect.max, bucket_rect.max);
    }

    static void unitRangeToBucketRect(const iXY& world_loc,
                                      unsigned long range,
                                      iRect &bucket_rect)
    {
        worldLocToBucketLoc(world_loc, bucket_rect.min);
        worldLocToBucketLoc(world_loc, bucket_rect.max);
        // add range
        unsigned long range_buckets = range / ( (pixel_x_sample_factor*pixel_x_sample_factor)
                                              + (pixel_y_sample_factor*pixel_y_sample_factor));
        bucket_rect.min.x -= range_buckets;
        bucket_rect.min.y -= range_buckets;
        bucket_rect.max.x += range_buckets;
        bucket_rect.max.y += range_buckets;

        if ( bucket_rect.min.x < 0 )
        {
            bucket_rect.min.x = 0;
        }

        if ( (size_t)bucket_rect.max.x >= column_size )
        {
            bucket_rect.max.x = column_size-1;
        }

        if ( bucket_rect.min.y < 0 )
        {
            bucket_rect.min.y = 0;
        }

        if ( (size_t)bucket_rect.max.y >= row_size )
        {
            bucket_rect.max.y = row_size-1;
        }
    }

    static void mapLocToBucketLoc(const iXY & map_loc, iXY & bucket_loc)
    {
        bucket_loc.x = map_loc.x / map_x_sample_factor;
        bucket_loc.y = map_loc.y / map_y_sample_factor;
    }

    static void mapRectToBucketRect(const iRect & map_rect, iRect &bucket_rect)
    {
        mapLocToBucketLoc(map_rect.min, bucket_rect.min);
        mapLocToBucketLoc(map_rect.max, bucket_rect.max);
    }

    static UnitList & getBucketAssocWorldLoc(const iXY & world_loc)
    {
        return getBucket(worldLocToBucketIndex(world_loc));
    }

    static UnitList & getBucketAssocMapLoc(const iXY & map_loc)
    {
        return getBucket(mapLocToBucketIndex(map_loc));
    }

    static BucketList* buckets;
    static long map_x_sample_factor;
    static long map_y_sample_factor;
    static long pixel_x_sample_factor;
    static long pixel_y_sample_factor;
    static size_t row_size;
    static size_t column_size;

    struct FindUnit
    {
        const UnitID toFind;
        FindUnit(const UnitID u) : toFind(u) {}
        bool operator()(Unit* ub) const
        {
            return ub->id == toFind;
        }
    };

};

#endif // ** _UNIT_BUCKET_ARRAY_HPP

