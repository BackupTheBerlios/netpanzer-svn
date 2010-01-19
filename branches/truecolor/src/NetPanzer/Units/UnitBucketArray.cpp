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

#include "Util/Log.hpp"
#include <list>
#include <algorithm>

#include "Units/UnitBucketArray.hpp"
#include "Units/Unit.hpp"

#include "Classes/PlayerState.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Classes/TileSet.hpp"

typedef std::vector<UnitList> BucketList;

BucketList*   buckets = 0;
static long   map_x_sample_factor = 0;
static long   map_y_sample_factor = 0;
static long   pixel_x_sample_factor = 0;
static long   pixel_y_sample_factor = 0;
static size_t row_size = 0;
static size_t column_size = 0;

static UnitList& getBucket(const unsigned int bucket_index)
{
//    assert( bucket_index < getSize() );
    return( (*buckets)[ bucket_index ] );
}

// conversion functions

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

//static void mapLocToBucketLoc(const iXY & map_loc, iXY & bucket_loc)
//{
//    bucket_loc.x = map_loc.x / map_x_sample_factor;
//    bucket_loc.y = map_loc.y / map_y_sample_factor;
//}

static unsigned int bucketLocToBucketIndex(const unsigned int row, const unsigned int col)
{
    return (row * column_size) + col;
}

//static void mapRectToBucketRect(const iRect & map_rect, iRect &bucket_rect)
//{
//    mapLocToBucketLoc(map_rect.min, bucket_rect.min);
//    mapLocToBucketLoc(map_rect.max, bucket_rect.max);
//}


// UnitBucketArray methods

void
UnitBucketArray::initialize(const unsigned int x_sample, const unsigned int y_sample)
{
    cleanUp();

    unsigned int map_width = MapInterface::getWidth();
    unsigned int map_height = MapInterface::getHeight();

    unsigned long rows, columns;

    assert( x_sample >= 1 );
    assert( y_sample >= 1 );

    map_x_sample_factor = x_sample;
    map_y_sample_factor = y_sample;

    while( (map_width % map_x_sample_factor) > 0 )
    {
        map_x_sample_factor++;
    }

    while( (map_height % map_y_sample_factor) > 0 )
    {
        map_y_sample_factor++;
    }

    pixel_x_sample_factor = TileSet::getTileXsize() * map_x_sample_factor;
    pixel_y_sample_factor = TileSet::getTileYsize() * map_y_sample_factor;

    rows = (unsigned long) map_height / map_y_sample_factor;
    columns = (unsigned long) map_width / map_x_sample_factor;

    row_size = rows;
    column_size = columns;
    buckets = new BucketList();
    buckets->resize(rows*columns);
}

void
UnitBucketArray::cleanUp()
{
    if ( buckets )
    {
        delete buckets;
        buckets = 0;
    }
}

void UnitBucketArray::addUnit(Unit *unit)
{
    getBucket(worldLocToBucketIndex(unit->unit_state.location)).push_back(unit);
}

void UnitBucketArray::removeUnit(const Unit *unit)
{
    UnitList& uli = getBucket(worldLocToBucketIndex(unit->unit_state.location));
    uli.erase(std::remove(uli.begin(), uli.end(), unit), uli.end());
}

void
UnitBucketArray::moveUnit(Unit * unit, const iXY& prev_unit_loc)
{
    unsigned int from_bucket = worldLocToBucketIndex(prev_unit_loc);
    unsigned int to_bucket = worldLocToBucketIndex(unit->unit_state.location);
    if ( from_bucket != to_bucket )
    {
        UnitList & uli = getBucket(from_bucket);
        uli.erase(std::remove(uli.begin(), uli.end(), unit), uli.end());
        getBucket(to_bucket).push_back(unit);
    }
}

void
UnitBucketArray::sort()
{
    unsigned int index;
    unsigned int real_index;
    UnitList::iterator iter;

    for( index = 0; index < buckets->size(); ++index )
    {
        UnitList & uli = getBucket(index);

        for (iter = uli.begin(); iter != uli.end(); /* nothing */)
        {
            real_index = worldLocToBucketIndex( (*iter)->unit_state.location );

            if( real_index != index )
            {
                getBucket(real_index).push_back(*iter);
                iter = uli.erase(iter);
            }
            else
            {
                ++iter;
            }
        }
    }
}

void
UnitBucketArray::queryPlayerUnitsAt(std::vector<UnitID>& working_list,
                                    const iXY& point,
                                    const Uint16 player_id)
{
    UnitList & ubl = getBucket(worldLocToBucketIndex(point));
    for(UnitList::iterator i = ubl.begin(); i != ubl.end(); ++i)
    {
        Unit* unit = *i;
        if( (*i)->unit_state.bounds(point)
            && (*i)->player->getID() == player_id )
        {
            working_list.push_back(unit->id);
        }
    }
}

void
UnitBucketArray::queryUnitsInWorldRect(std::vector<Unit *>& working_list,
                                       const iRect& rect)
{
    UnitList::iterator iter;
    iRect bucket_rect;
    worldRectToBucketRect( rect, bucket_rect);

    for( int row = bucket_rect.min.y; row <= bucket_rect.max.y; ++row )
    {
        for( int col = bucket_rect.min.x; col <= bucket_rect.max.x; ++col )
        {
            UnitList & bucket_list = getBucket(bucketLocToBucketIndex(row, col));

            for( iter = bucket_list.begin(); iter != bucket_list.end(); ++iter )
            {
                if( rect.contains((*iter)->unit_state.location) )
                {
                    working_list.push_back(*iter);
                }
            }
        }
    }
}

void
UnitBucketArray::queryUnitsInWorldRectBuckets(std::vector<Unit *>& working_list,
                                              const iRect& rect)
{
    UnitList::iterator iter;
    iRect bucket_rect;
    worldRectToBucketRect( rect, bucket_rect);

    for( int row = bucket_rect.min.y; row <= bucket_rect.max.y; ++row )
    {
        for( int col = bucket_rect.min.x; col <= bucket_rect.max.x; ++col )
        {
            UnitList & bucket_list = getBucket(bucketLocToBucketIndex(row, col));

            for( iter = bucket_list.begin(); iter != bucket_list.end(); ++iter )
            {
                working_list.push_back(*iter);
            }
        }
    }
}

void
UnitBucketArray::queryPlayerUnitsInWorldRect(std::vector<UnitID>& working_list,
                                             const iRect& rect,
                                             const Uint16 player_id)
{
    UnitList::iterator iter;
    iRect bucket_rect;
    worldRectToBucketRect( rect, bucket_rect);

    for( int row = bucket_rect.min.y; row <= bucket_rect.max.y; ++row )
    {
        for( int col = bucket_rect.min.x; col <= bucket_rect.max.x; ++col )
        {
            UnitList & bucket_list = getBucket(bucketLocToBucketIndex(row, col));

            for( iter = bucket_list.begin(); iter != bucket_list.end(); ++iter )
            {
                if(    rect.contains((*iter)->unit_state.location)
                    && (*iter)->player->getID() == player_id )
                {
                    working_list.push_back((*iter)->id);
                }

            }
        }
    }
}

void
UnitBucketArray::queryNonPlayerUnitsInWorldRect(std::vector<Unit *>& working_list,
                                                const iRect& rect,
                                                const Uint16 player_id)
{
    UnitList::iterator iter;
    iRect bucket_rect;
    worldRectToBucketRect( rect, bucket_rect);

    for( int row = bucket_rect.min.y; row <= bucket_rect.max.y; ++row )
    {
        for( int col = bucket_rect.min.x; col <= bucket_rect.max.x; ++col )
        {
            UnitList & bucket_list = getBucket(bucketLocToBucketIndex(row, col));

            for( iter = bucket_list.begin(); iter != bucket_list.end(); ++iter )
            {
                if(    rect.contains((*iter)->unit_state.location)
                    && (*iter)->player->getID() != player_id )
                {
                    working_list.push_back(*iter);
                }

            }
        }
    }
}

bool
UnitBucketArray::queryClosestEnemyUnitInRange(Unit **closest_unit_ptr,
                                              const iXY &loc,
                                              unsigned long range,
                                              const Uint16 player_id)
{
    Unit *closest_unit = 0;
    long closest_magnitude = 0;
    iRect bucket_rect;
    UnitList::iterator bucket_iter;

    unitRangeToBucketRect( loc, range, bucket_rect );

    for( long row_index = bucket_rect.min.y; row_index <= bucket_rect.max.y; row_index++ )
    {
        for( long column_index = bucket_rect.min.x; column_index <= bucket_rect.max.x; column_index++ )
        {
            UnitList &bucket_list = getBucket(bucketLocToBucketIndex(row_index, column_index));

            for ( bucket_iter = bucket_list.begin();
                    bucket_iter != bucket_list.end(); ++bucket_iter)
            {
                iXY delta;
                long temp_mag;

                Uint16 unitPlayerID = (*bucket_iter)->player->getID();

                if (   unitPlayerID == player_id
                    || PlayerInterface::isAllied(player_id, unitPlayerID) )
                {
                    continue;
                }

                if ( closest_unit == 0 )
                {
                    closest_unit = *bucket_iter;
                    delta  = loc - (*bucket_iter)->unit_state.location;
                    closest_magnitude = long(delta.mag2());
                }
                else
                {
                    delta  = loc - (*bucket_iter)->unit_state.location;
                    temp_mag = long(delta.mag2());

                    if ( closest_magnitude > temp_mag )
                    {
                        closest_unit = *bucket_iter;
                        closest_magnitude = temp_mag;
                    }
                }
            }
        }
    }

    if( closest_unit != 0 )
    {
        *closest_unit_ptr = closest_unit;
        return true;
    }

    *closest_unit_ptr = 0;
    return false;
}

void
UnitBucketArray::queryUnitsAtWorldLocBucket(std::vector<Unit *>& working_list,
                                            const iXY& world_loc)
{
    UnitList & ubl = getBucket(worldLocToBucketIndex(world_loc));
    for(UnitList::iterator i = ubl.begin(); i != ubl.end(); ++i)
    {
        working_list.push_back(*i);
    }
}

Unit *
UnitBucketArray::queryUnitAtMapLoc(const iXY & map_loc)
{
    iXY world_loc;
    MapInterface::mapXYtoPointXY(map_loc, &world_loc);

    UnitList & ubl = getBucket(mapLocToBucketIndex(map_loc));
    for(UnitList::iterator i = ubl.begin(); i != ubl.end(); ++i)
    {
        if ( (*i)->unit_state.bounds(world_loc) )
        {
            return *i;
        }
    }

    return 0;
}

Unit *
UnitBucketArray::queryNonPlayerUnitAtWorld(const iXY & world_loc,
                                           const Uint16 player_id)
{
    UnitList & ubl = getBucket(worldLocToBucketIndex(world_loc));
    for(UnitList::iterator i = ubl.begin(); i != ubl.end(); ++i)
    {
        if ( (*i)->unit_state.bounds(world_loc)
              && (*i)->player->getID() != player_id )
        {
            return *i;
        }
    }

    return 0;
}
