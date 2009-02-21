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
#include <config.h>

#include "Util/Log.hpp"
#include <list>
#include <algorithm>

#include "2D/Palette.hpp"
#include "Units/UnitBucketArray.hpp"

void
UnitBucketArray::initialize( const iXY & map_size, const iXY & tile_size,
        const unsigned int x_sample, const unsigned int y_sample )
{
    cleanUp();

    unsigned long rows, columns;

    assert( x_sample >= 1 );
    assert( y_sample >= 1 );

    map_x_sample_factor = x_sample;
    map_y_sample_factor = y_sample;

    while( (map_size.x % map_x_sample_factor) > 0 )
    {
        map_x_sample_factor++;
    }

    while( (map_size.y % map_y_sample_factor) > 0 )
    {
        map_y_sample_factor++;
    }

    pixel_x_sample_factor = tile_size.x * map_x_sample_factor;
    pixel_y_sample_factor = tile_size.y * map_y_sample_factor;

    rows = (unsigned long) map_size.y / map_y_sample_factor;
    columns = (unsigned long) map_size.x / map_x_sample_factor;

    row_size = rows;
    column_size = columns;
    buckets.resize(rows*columns);
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
