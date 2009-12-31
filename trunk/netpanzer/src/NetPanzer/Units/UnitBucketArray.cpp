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
#include "Classes/PlayerState.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Classes/TileSet.hpp"

BucketList* UnitBucketArray::buckets = 0;
long        UnitBucketArray::map_x_sample_factor = 0;
long        UnitBucketArray::map_y_sample_factor = 0;
long        UnitBucketArray::pixel_x_sample_factor = 0;
long        UnitBucketArray::pixel_y_sample_factor = 0;
size_t      UnitBucketArray::row_size = 0;
size_t      UnitBucketArray::column_size = 0;

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

    rows = (unsigned long) map_width / map_y_sample_factor;
    columns = (unsigned long) map_height / map_x_sample_factor;

    row_size = rows;
    column_size = columns;
    buckets = new BucketList();
    buckets->resize(rows*columns);
}

void
UnitBucketArray::sort()
{
    unsigned int index;
    unsigned int real_index;
    UnitList::iterator iter;

    for( index = 0; index < getSize(); ++index )
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
    UnitList & ubl = getBucketAssocWorldLoc(point);
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
            UnitList & bucket_list = getBucket(row, col);

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
            UnitList & bucket_list = getBucket(row, col);

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
            UnitList & bucket_list = getBucket(row, col);

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
            UnitList &bucket_list = getBucket( row_index, column_index );

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

Unit *
UnitBucketArray::queryUnitAtMapLoc(const iXY & map_loc)
{
    iXY world_loc;
    MapInterface::mapXYtoPointXY(map_loc, &world_loc);

    UnitList & ubl = getBucketAssocMapLoc(map_loc);
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
    UnitList & ubl = getBucketAssocWorldLoc(world_loc);
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
