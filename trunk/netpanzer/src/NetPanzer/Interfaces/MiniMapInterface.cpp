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
#include "MiniMapInterface.hpp"

#include "WorldInputCmdProcessor.hpp"

#include "UnitInterface.hpp"
#include "WorldViewInterface.hpp"
#include "MapInterface.hpp"
#include "PathScheduler.hpp"
#include "ObjectiveInterface.hpp"
#include "GameConfig.hpp"

fXY MiniMapInterface::scale_factor;
iXY MiniMapInterface::mini_map_size;
bool MiniMapInterface::pathing_debug_mode = false;

PIX MiniMapInterface::player_unit_color;
PIX MiniMapInterface::allie_unit_color;

PIX MiniMapInterface::player_objective_color;
PIX MiniMapInterface::allie_objective_color;
PIX MiniMapInterface::enemy_objective_color;

TimerFrameBase MiniMapInterface::radar_blink_timer;
bool        MiniMapInterface::radar_blink_flag;

TimerFrameBase MiniMapInterface::show_enemy_radar_timer;
bool        MiniMapInterface::show_enemy_radar_flag;

iRect MiniMapInterface::getWorldWindow()
{
    iRect world_win;
    WorldViewInterface::getViewWindow(&world_win);

    world_win.min.x = int(float(world_win.min.x) / scale_factor.x);
    world_win.min.y = int(float(world_win.min.y) / scale_factor.y);
    world_win.max.x = int(float(world_win.max.x) / scale_factor.x);
    world_win.max.y = int(float(world_win.max.y) / scale_factor.y);

    return( world_win );
}

void MiniMapInterface::setMapScale(const iXY &mini_map_size)
{
    iXY map_size;

    MiniMapInterface::mini_map_size = mini_map_size;

    MapInterface::getMapPointSize( &map_size );
    scale_factor.x = float(map_size.x) / float(mini_map_size.x);
    scale_factor.y = float(map_size.y) / float(mini_map_size.y);
}

void MiniMapInterface::setWorldWindowPosition( iXY world_loc )
{
    world_loc.x = int(float(world_loc.x) * scale_factor.x);
    world_loc.y = int(float(world_loc.y) * scale_factor.y);

    WorldViewInterface::setCameraPosition( world_loc );
}

void MiniMapInterface::annotateUnits( Surface &map_surface )
{
    iXY world_loc, map_loc;
    unsigned char unit_dispostion;
    unsigned char threat_level;

    UnitInterface::startUnitPositionEnumeration( );

    iRect unitRect;

    while( UnitInterface::unitPositionEnumeration( &world_loc, &unit_dispostion, &threat_level) ) {
        map_loc.x = int(float(world_loc.x) / scale_factor.x);
        map_loc.y = int(float(world_loc.y) / scale_factor.y);

        if (gameconfig->radar_unitsize == _mini_map_unit_size_large) {
            unitRect = iRect(map_loc, map_loc + iXY(1,1));
        }

        if ( unit_dispostion == _unit_player ) {
            if ( threat_level == _threat_level_under_attack ) {
                if ( radar_blink_flag == true ) {
                    drawLargeUnitDot( map_surface, map_loc, Color::yellow );
                }
            } else {
                if (gameconfig->radar_unitsize == _mini_map_unit_size_small) {
                    drawSmallUnitDot( map_surface, map_loc, player_unit_color );
                } else if (gameconfig->radar_unitsize == _mini_map_unit_size_large) {
                    drawLargeUnitDot( map_surface, map_loc, player_unit_color );

                } else {
                    assert(false);
                }
            }
        } else
            if ( unit_dispostion == _unit_allied ) {
                if (gameconfig->radar_unitsize == _mini_map_unit_size_small) {
                    drawSmallUnitDot( map_surface, map_loc, allie_unit_color );
                } else if (gameconfig->radar_unitsize == _mini_map_unit_size_large) {
                    drawLargeUnitDot( map_surface, map_loc, allie_unit_color );

                } else {
                    assert(false);
                }
            } else
                if( ( unit_dispostion == _unit_enemy ) && (show_enemy_radar_flag == true) ) {

                    if (gameconfig->radar_unitsize == _mini_map_unit_size_small) {
                        drawSmallUnitDot( map_surface, map_loc, enemy_objective_color );
                    } else if (gameconfig->radar_unitsize == _mini_map_unit_size_large) {
                        drawLargeUnitDot( map_surface, map_loc, enemy_objective_color );

                    } else {
                        assert(false);
                    }

                }

    }
}

void MiniMapInterface::annotateObjectives( Surface &map_surface )
{
    iRect world_rect, map_rect;
    unsigned char objective_disposition;
    PIX color;

    ObjectiveInterface::startObjectivePositionEnumeration();

    while( ObjectiveInterface::objectivePositionEnumeration( &world_rect, &objective_disposition, 0 ) ) {
        switch( objective_disposition ) {
        case _objective_disposition_unoccupied :
            color = Color::white;
            break;

        case _objective_disposition_player :
            color = player_objective_color;
            break;

        case _objective_disposition_allie :
            color = allie_objective_color;
            break;

        case _objective_disposition_enemy :
            color = enemy_objective_color;
            break;
        } // ** switch

        map_rect.min.x = int(float(world_rect.min.x) / scale_factor.x);
        map_rect.min.y = int(float(world_rect.min.y) / scale_factor.y);
        map_rect.max.x = int(float(world_rect.max.x) / scale_factor.x);
        map_rect.max.y = int(float(world_rect.max.y) / scale_factor.y);

        // Removed black borders to the text.
        if (gameconfig->radar_objectivedrawmode == _mini_map_objective_draw_mode_solid_rect) {
            map_surface.fillRect( map_rect, color);
        } else if (gameconfig->radar_objectivedrawmode == _mini_map_objective_draw_mode_outline_rect) {
            map_surface.drawRect( map_rect, color);
            map_surface.drawRect( iRect(map_rect.min.x + 1, map_rect.min.y + 1, map_rect.max.x - 1, map_rect.max.y - 1), color );
            //}
            //else if (gameconfig->radar_objectivedrawmode == _mini_map_objective_draw_mode_player_flag)
            //{
            //map_surface.fillRect( map_rect, color );
        } else {
            assert(false);
        }
    } // ** while
}

void MiniMapInterface::annotateMiniMap( Surface &map_surface )
{

    if ( pathing_debug_mode == true ) {
        BitArray *bit_array;
        unsigned long map_x_size = MapInterface::getMapXsize();
        unsigned long map_y_size = MapInterface::getMapYsize();

        bit_array = PathScheduler::getLongPatherSetArray( );

        for( unsigned long map_x_index = 0; map_x_index < map_x_size; map_x_index++ ) {
            for( unsigned long map_y_index = 0; map_y_index < map_y_size; map_y_index++ ) {
                if( bit_array->getBit( map_x_index, map_y_index ) ) {
                    map_surface.putPixel( iXY( map_x_index, map_y_index), Color::lightYellow );
                }
            } // ** for
        } // ** for

    } else {
        if ( radar_blink_timer.count() ) {
            radar_blink_flag = !radar_blink_flag;
        }

        if( show_enemy_radar_flag == true ) {
            if( show_enemy_radar_timer.count() ) {
                show_enemy_radar_flag = false;
            }
        }

        annotateObjectives( map_surface );
        annotateUnits( map_surface );
    }

}

bool MiniMapInterface::isUnitSelected( void )
{
    return( COMMAND_PROCESSOR.isUnitSelected() );
}

bool MiniMapInterface::selectUnits( iRect bound_box )
{
    bound_box.min.x = int(float(bound_box.min.x) * scale_factor.x);
    bound_box.min.y = int(float(bound_box.min.y) * scale_factor.y);
    bound_box.max.x = int(float(bound_box.max.x) * scale_factor.x);
    bound_box.max.y = int(float(bound_box.max.y) * scale_factor.y);

    return( COMMAND_PROCESSOR.selectUnits( bound_box ) );
}

void MiniMapInterface::deselectUnits( void )
{
    COMMAND_PROCESSOR.deselectUnitsExternal();
}

bool MiniMapInterface::isValidUnitMove( iXY location )
{
    location.x = int(location.x * scale_factor.x);
    location.y = int(location.y * scale_factor.y);

    return( COMMAND_PROCESSOR.isValidMoveLocation( location ) );
}

bool MiniMapInterface::moveUnits( iXY location )
{
    location.x = int(location.x * scale_factor.x);
    location.y = int(location.y * scale_factor.y);

    return( COMMAND_PROCESSOR.sendMoveCommandExternal( location ) );
}

void MiniMapInterface::setPathingDebugMode( bool on_off )
{
    pathing_debug_mode = on_off;
}

void MiniMapInterface::setShowEnemyRadar( float time )
{
    show_enemy_radar_timer.changePeriod( time );
    show_enemy_radar_timer.reset();
    show_enemy_radar_flag = true;
}

void MiniMapInterface::setProperties( PIX player_unit_color,
                                      PIX allie_unit_color,
                                      PIX player_objective_color,
                                      PIX allie_objective_color,
                                      PIX enemy_objective_color
                                    )
{
    MiniMapInterface::player_unit_color = player_unit_color;
    MiniMapInterface::allie_unit_color = allie_unit_color;

    MiniMapInterface::player_objective_color = player_objective_color;
    MiniMapInterface::allie_objective_color = allie_objective_color;
    MiniMapInterface::enemy_objective_color = enemy_objective_color;

    radar_blink_flag = false;
    radar_blink_timer.changeRate( 10 );

    show_enemy_radar_flag = false;
}

void MiniMapInterface::drawLargeUnitDot( const Surface &dest, const iXY &location, unsigned char color )
{
    // Center
    dest.putPixel( location.x,     location.y,     color );
    dest.putPixel( location.x + 1, location.y,     color );
    dest.putPixel( location.x,     location.y + 1, color );
    dest.putPixel( location.x + 1, location.y + 1, color );

    //// Top and Bottom
    //dest.putPixel( location.x,     location.y - 1, Color::black );
    //dest.putPixel( location.x + 1, location.y - 1, Color::black );
    //dest.putPixel( location.x,     location.y + 2, Color::black );
    //dest.putPixel( location.x + 1, location.y + 2, Color::black );
    //
    //// Left and Right
    //dest.putPixel( location.x - 1, location.y,     Color::black );
    //dest.putPixel( location.x - 1, location.y + 1, Color::black );
    //dest.putPixel( location.x + 2, location.y,     Color::black );
    //dest.putPixel( location.x + 2, location.y + 1, Color::black );
}

void MiniMapInterface::drawSmallUnitDot( const Surface &dest, const iXY &location, unsigned char color )
{
    dest.putPixel( location, color );
}
