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
#ifndef _MINIMAPINTERFACE_HPP
#define _MINIMAPINTERFACE_HPP

#include "MapInterface.hpp"
#include "ArrayUtil/Timer.hpp"
#include "ArrayUtil/BitArray.hpp"
#include "Types/fXY.hpp"

class MiniMapInterface
{
protected:
    static fXY            scale_factor;
    static iXY       mini_map_size;
    static bool        pathing_debug_mode;

    static TimerFrameBase radar_blink_timer;
    static bool        radar_blink_flag;

    static TimerFrameBase show_enemy_radar_timer;
    static bool        show_enemy_radar_flag;

    static PIX player_unit_color;
    static PIX selected_unit_color;
    static PIX allie_unit_color;

    static PIX player_objective_color;
    static PIX allie_objective_color;
    static PIX enemy_objective_color;

    static void annotateUnits( Surface &map_surface );
    static void annotateObjectives( Surface &map_surface );
    static void drawLargeUnitDot(Surface &dest, const iXY &location,
            unsigned char color );
    static void drawSmallUnitDot(Surface &dest, const iXY &location,
            unsigned char color );

public:

    static inline Surface * getMiniMap( void )
    {
        return( MapInterface::getMiniMapSurface() );
    }

    static iRect getWorldWindow( void );

    static void setMapScale(const iXY &mini_map_size);
    static const iXY &getMapScale()
    {
        return mini_map_size;
    }

    static void setWorldWindowPosition( iXY world_loc );

    static void annotateMiniMap( Surface &map_surface );

    static bool isUnitSelected( void );
    static void deselectUnits( void );
    static bool selectUnits( iRect bound_box );
    static bool isValidUnitMove( iXY location );
    static bool moveUnits( iXY location );


    static void setPathingDebugMode( bool on_off );

    static void setShowEnemyRadar( float time );

    static void setProperties( PIX player_unit_color,
                               PIX selected_unit_color,
                               PIX allie_unit_color,
                               PIX player_objective_color,
                               PIX allie_objective_color,
                               PIX enemy_objective_color
                             );

};

#endif // ** _MINIMAPINTERFACE_HPP
