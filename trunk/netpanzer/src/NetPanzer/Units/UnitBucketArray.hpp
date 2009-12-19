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
    void queryPlayerUnitsAt(std::vector<UnitID>& working_list,
                            const iXY& point,
                            const Uint16 player_id);

    void queryUnitsInWorldRect(std::vector<Unit *>& working_list,
                               const iRect& rect);

    void queryPlayerUnitsInWorldRect(std::vector<UnitID>& working_list,
                                     const iRect& rect,
                                     const Uint16 player_id);

    void queryNonPlayerUnitsInWorldRect(std::vector<Unit *>& working_list,
                                        const iRect& rect,
                                        const Uint16 player_id);

    Unit * queryUnitAtMapLoc(const iXY& map_loc);

    Unit * queryNonPlayerUnitAtWorld(const iXY & world_loc,
                                     const Uint16 player_id);

    bool queryClosestEnemyUnitInRange(Unit **closest_unit_ptr,
                                      const iXY &loc,
                                      unsigned long range,
                                      const Uint16 player_id);

private:
    friend class UnitInterface;
    UnitBucketArray( ) {}
    ~UnitBucketArray( ) {}

    void sort();

    void initialize( const iXY & map_size, const iXY & tile_size,
                     const unsigned int x_sample, const unsigned int y_sample );
    void initialize( const iXY & map_size, const iXY & tile_size)
    {
        initialize( map_size, tile_size, 10, 10 );
    }
    
    void moveUnit( Unit * unit, const unsigned int from, const unsigned int to )
    {
        UnitList & uli = getBucket(from);
        uli.erase(std::remove(uli.begin(), uli.end(), unit), uli.end());
        getBucket(to).push_back(unit);
    }

    void cleanUp()
    {
        buckets.clear();
    }

    Unit * getUnitAtWorldLoc( const UnitID unit_id, const iXY & world_loc)
    {
        UnitList & uli = getBucketAssocWorldLoc(world_loc);
        UnitList::iterator iter;
        iter = std::find_if( uli.begin(), uli.end(), FindUnit(unit_id));
        return (iter != uli.end())?*iter:0;
    }

    Unit * getUnitAtMapLoc( const UnitID unit_id, const iXY & map_loc )
    {
        UnitList & uli = getBucketAssocMapLoc(map_loc);
        UnitList::iterator iter;
        iter = std::find_if( uli.begin(), uli.end(), FindUnit(unit_id));
        return (iter != uli.end())?*iter:0;
    }

    void addUnit( Unit *unit )
    {
        getBucketAssocWorldLoc(unit->unit_state.location).push_back(unit);
    }

    void removeUnit( const Unit *unit )
    {
        UnitList & uli = getBucketAssocWorldLoc(unit->unit_state.location);
        uli.erase(std::remove(uli.begin(), uli.end(), unit), uli.end());
    }

    unsigned int getSize() const
    {
        return buckets.size();
    }

    UnitList & getBucket(const unsigned int bucket_index)
    {
//        assert( bucket_index < getSize() );
        return( buckets[ bucket_index ] );
    }

    UnitList & getBucket( const unsigned int row, const unsigned int column )
    {
        return ( getBucket((row * column_size) + column) );
    }

    unsigned int mapLocToBucketIndex( const iXY & map_loc ) const
    {
        return ((map_loc.y / map_y_sample_factor) * column_size)
                + (map_loc.x / map_x_sample_factor);
    }

    unsigned int worldLocToBucketIndex( const iXY & world_loc ) const
    {
        return ((world_loc.y / pixel_y_sample_factor) * column_size)
                + (world_loc.x / pixel_x_sample_factor);
    }

    void worldLocToBucketLoc( const iXY & world_loc, iXY & bucket_loc ) const
    {
        bucket_loc.x = (world_loc.x-1) / pixel_x_sample_factor;
        bucket_loc.y = (world_loc.y-1) / pixel_y_sample_factor;
    }

    void worldRectToBucketRect( const iRect & world_rect, iRect &bucket_rect ) const
    {
        worldLocToBucketLoc(world_rect.min, bucket_rect.min);
        worldLocToBucketLoc(world_rect.max, bucket_rect.max);
    }

    void unitRangeToBucketRect( const iXY& world_loc, unsigned long range, iRect &bucket_rect ) const
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

        if ( (size_t)bucket_rect.max.x >= column_size )
        {
            bucket_rect.max.x = column_size-1;
        }

        if ( (size_t)bucket_rect.max.y >= row_size )
        {
            bucket_rect.max.y = row_size-1;
        }
    }

    void mapLocToBucketLoc( const iXY & map_loc, iXY & bucket_loc) const
    {
        bucket_loc.x = map_loc.x / map_x_sample_factor;
        bucket_loc.y = map_loc.y / map_y_sample_factor;
    }

    void mapRectToBucketRect( const iRect & map_rect, iRect &bucket_rect ) const
    {
        mapLocToBucketLoc(map_rect.min, bucket_rect.min);
        mapLocToBucketLoc(map_rect.max, bucket_rect.max);
    }

    UnitList & getBucketAssocWorldLoc( const iXY & world_loc )
    {
        return getBucket(worldLocToBucketIndex(world_loc));
    }

    UnitList & getBucketAssocMapLoc( const iXY & map_loc )
    {
        return getBucket(mapLocToBucketIndex(map_loc));
    }

    BucketList buckets;
    long map_x_sample_factor;
    long map_y_sample_factor;
    long pixel_x_sample_factor;
    long pixel_y_sample_factor;
    size_t row_size;
    size_t column_size;

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

