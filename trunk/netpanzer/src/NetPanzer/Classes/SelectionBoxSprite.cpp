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

#include "Core/CoreTypes.hpp"
#include "Classes/SelectionBoxSprite.hpp"
#include "2D/Palette.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Interfaces/TeamManager.hpp"
#include "Resources/ResourceManager.hpp"
#include "ScreenSurface.hpp"

bool SelectionBoxSprite::isVisible(const iRect &world_win) const
{
    return visible
        && (  world_win.contains(world_pos - selection_area.siz)
           || world_win.contains(world_pos + selection_area.siz) );
}


void SelectionBoxSprite::blit( Surface *surface, const iRect &world_win )
{

    if ( box_state == false )
        return;

    iXY min_abs(world_pos), max_abs;
    
    min_abs = (world_pos - selection_area.siz) - world_win.getLocation();
    max_abs = (world_pos + selection_area.siz) - world_win.getLocation();

    if( (min_abs.x >= 0 ) ) {
        surface->drawVLine(min_abs.x, min_abs.y, max_abs.y, box_color);
    }

    if( ((unsigned int)max_abs.x < surface->getWidth())  ) {
        surface->drawVLine(max_abs.x, min_abs.y, max_abs.y, box_color);
    }

    if( (min_abs.y >= 0 ) ) {
        surface->drawHLine(min_abs.x, min_abs.y, max_abs.x, box_color);
    }

    if ( ((unsigned int)max_abs.y < surface->getHeight()) ) {
        surface->drawHLine(min_abs.x, max_abs.y, max_abs.x+1, box_color);
    }

}

UnitSelectionBox::UnitSelectionBox( )
{
    allied_visiblity_state = false;
    allie_state = false;
    flag_visibility_state = false;
    unit_flag = 0;
}


void UnitSelectionBox::blit( Surface *surface, const iRect &world_win )
{
    iXY min_abs, max_abs;

    min_abs = (world_pos - selection_area.siz) - world_win.getLocation();
    max_abs = (world_pos + selection_area.siz) - world_win.getLocation();

    iRect screen_rect;
    screen_rect.setLocation(min_abs);
    screen_rect.setSize(max_abs - min_abs);
    
    if ( box_state == true )
    {
        PIX selectionBoxColor = gameconfig->gameinterface.getVehicleSelectionColor();

        assert(max_hit_points > 0);

        surface->drawBoxCorners(screen_rect, 7, selectionBoxColor);

    } // ** box_state == true

    // Draw the unit hitpoints.
    if ( gameconfig->gameinterface.showHealth() || (box_state == true) )
    {
        // Draw a color coded hit bar.
        Uint8 hitBarColor;

        float hitPointPercent = float(hit_points) / float(max_hit_points);

        if (hitPointPercent < 0.3f)
        {
            hitBarColor = Color::red;
        }
        else if (hitPointPercent < 0.6f)
        {
            hitBarColor = Color::yellow;
        }
        else
        {
            hitBarColor = Color::green;
        }

        int hit_bar_size = int ( (float) (max_abs.x - min_abs.x - 4) * ( hitPointPercent ) );

        screen_rect.translate(1, screen_rect.getHeight()-6);
        screen_rect.setSize(screen_rect.getWidth()-2, 5);
        surface->bltLookup(screen_rect, Palette::filterDarkGray());

        screen_rect.grow(-1,-1);
        screen_rect.setWidth(hit_bar_size);
        surface->fillRect(screen_rect, hitBarColor);
    }

    if ( gameconfig->gameinterface.showFlags() == true  && unit_flag )
    {
        unit_flag->blt( *surface, min_abs.x, min_abs.y - FLAG_HEIGHT - 1 ); // full blit
    }

    if ( gameconfig->gameinterface.showNames() && (playerName.length() > 0) )
    {
        if (GameConfig::game_teammode == true)
        {
            surface->bltString( min_abs.x + FLAG_WIDTH  + 2,
                                min_abs.y - FLAG_HEIGHT + 2,
                                playerName.c_str(), TeamManager::getTeamColor(team_ID) );
        }
        else
        {
            surface->bltString( min_abs.x + FLAG_WIDTH  + 2,
                                min_abs.y - FLAG_HEIGHT + 2,
                                playerName.c_str(), Color::gray224);
        }
    }

}
