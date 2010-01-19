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

#include "Classes/PlacementMatrix.hpp"
#include "Types/iXY.hpp"
#include "Interfaces/MapInterface.hpp"
#include "Units/UnitInterface.hpp"

enum { _placement_state_base_case,
       _placement_state_top_run,
       _placement_state_right_run,
       _placement_state_bottom_run,
       _placement_state_left_run
     };

static unsigned char placement_state = 0;
static long run_length = 0;
static long run_counter = 0;
static iXY* current_offset = 0;
static iXY* current_loc = 0;
static iXY* ini_loc = 0;

void PlacementMatrix::reset( const iXY& ini_map_loc )
{
    if ( ! ini_loc )
    {
        ini_loc = new iXY();
    }
    *ini_loc = ini_map_loc;

    if ( ! current_offset )
    {
        current_offset = new iXY();
    }
    current_offset->zero();

    if ( ! current_loc )
    {
        current_loc = new iXY();
    }
    *current_loc = ini_map_loc;

    run_length = 0;
    run_counter = 0;
    placement_state = _placement_state_base_case;
}

bool verifyLocation( iXY &loc )
{
    long x_offset, y_offset;
    iXY succ;
    unsigned long direction_index;

    if ( MapInterface::getMovementValue(loc) >= 0xFF
        || UnitInterface::unitOccupiesLoc(loc) == true )
    {
        return(false);
    }


    for ( direction_index = 0; direction_index < 9; direction_index++ )
    {
        switch( direction_index )
        {
            case 0: {
                    x_offset =  0; y_offset =  0;
                }
                break;
            case 1: {
                    x_offset =  1; y_offset =  0;
                }
                break;
            case 2: {
                    x_offset =  1; y_offset = -1;
                }
                break;
            case 3: {
                    x_offset =  0; y_offset = -1;
                }
                break;
            case 4: {
                    x_offset = -1; y_offset = -1;
                }
                break;
            case 5: {
                    x_offset = -1; y_offset =  0;
                }
                break;
            case 6: {
                    x_offset = -1; y_offset =  1;
                }
                break;
            case 7: {
                    x_offset =  0; y_offset =  1;
                }
                break;
            case 8: {
                    x_offset =  1; y_offset =  1;
                }
                break;
            default:
                assert(false);
                return false;
        } // ** switch

        succ.x = loc.x + (x_offset);
        succ.y = loc.y + (y_offset);

        if ( MapInterface::getMovementValue(succ) < 0xFF
             && UnitInterface::unitOccupiesLoc(succ) == false )
        {
            return true;
        }

    }

    return false;
}

bool PlacementMatrix::getNextEmptyLoc( iXY *loc )
{
    bool end_cycle = false;

    do
    {
        switch( placement_state )
        {
            case _placement_state_base_case : {
                    placement_state = _placement_state_top_run;
                    current_offset->x -= 2;
                    current_offset->y -= 2;
                    *current_loc = *ini_loc + *current_offset;
                    run_length += 4;

                    if ( verifyLocation( *ini_loc ) == true ) {
                        *loc = *ini_loc;
                        return( true );
                    }

                }
                break;

            case _placement_state_top_run :
            {
                if ( run_counter >= run_length )
                {
                    run_counter = 0;
                    placement_state = _placement_state_right_run;
                }
                else if ( verifyLocation( *current_loc ) == true )
                {
                    *loc = *current_loc;

                    run_counter += 2;
                    current_loc->x += 2;

                    return( true );
                }
                else
                {
                    current_loc->x += 2;
                    run_counter += 2;
                }
                break;
            }

            case _placement_state_right_run :
            {
                if ( run_counter >= run_length )
                {
                    run_counter = 0;
                    placement_state = _placement_state_bottom_run;
                }
                else if ( verifyLocation( *current_loc ) == true )
                {
                    *loc = *current_loc;

                    run_counter += 2;
                    current_loc->y += 2;

                    return( true );

                }
                else
                {
                    current_loc->y += 2;
                    run_counter += 2;
                }
                break;
            }

            case _placement_state_bottom_run :
            {
                if ( run_counter >= run_length )
                {
                    run_counter = 0;
                    placement_state = _placement_state_left_run;
                }
                else if ( verifyLocation( *current_loc ) == true )
                {
                    *loc = *current_loc;

                    run_counter += 2;
                    current_loc->x -= 2;

                    return( true );

                }
                else
                {
                    current_loc->x -= 2;
                    run_counter += 2;
                }
                break;
            }

            case _placement_state_left_run :
            {
                if ( run_counter >= run_length )
                {
                    if ( current_offset->x > -50 )
                    {
                        run_counter = 0;
                        current_offset->x -= 2;
                        current_offset->y -= 2;
                        *current_loc = *ini_loc + *current_offset;
                        run_length += 4;
                        placement_state = _placement_state_top_run;
                    }
                    else
                    {
                        return( false );
                    }
                }
                else if ( verifyLocation( *current_loc ) == true )
                {
                    *loc = *current_loc;

                    run_counter += 2;
                    current_loc->y -= 2;

                    return( true );

                } else {
                    current_loc->y -= 2;
                    run_counter += 2;
                }
                break;
            }
        } // ** switch
    } while( end_cycle == false );

    return( false );
}
