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
#ifndef _SELECTION_BOX_SPRITE_HPP
#define _SELECTION_BOX_SPRITE_HPP

#include <string>

#include "Core/CoreTypes.hpp"
#include "Resources/ResourceManager.hpp"
#include "ArrayUtil/BoundBox.hpp"
#include "Classes/Sprite.hpp"

class Surface;

class UnitSelectionBox : public Sprite
{
protected:
    BoundBox selection_area;
    IntColor box_color;
    bool box_state;

    float hit_points;
    float max_hit_points;
    IntColor hit_bar_color;

    bool allied_visiblity_state;
    bool allie_state;
    bool flag_visibility_state;
    
    Surface * unit_flag;
    std::string playerName;

public:
    UnitSelectionBox( );

    inline void setBoxAttributes( BoundBox box, IntColor box_color )
    {
        selection_area = box;
        box_color = box_color;
    }

    inline void setBoxState( bool on_off )
    {
        box_state = on_off;
    }

    virtual ~UnitSelectionBox()
    { }

    inline void setHitBarAttributes( int max_points, IntColor bar_color )
    {
        max_hit_points = (float) max_points;
        hit_bar_color =  bar_color;
    }

    inline void setHitPoints( int points )
    {
        hit_points = (float) points;
        if ( hit_points <= 0 )
            hit_points = 0;
    }

    inline void setFlag( FlagID flag_enum )
    {
        unit_flag = ResourceManager::getFlag( flag_enum );
    }

    inline void setFlagIcon( bool flag_state )
    {
        flag_visibility_state = flag_state;
    }

    inline void setAllieIcon( bool allie_state )
    {
        allied_visiblity_state = allie_state;
    }

    inline void setAllieState( bool is_allied )
    {
        allie_state = is_allied;
    }
    
    inline void setName( std::string name )
    {
        playerName = name;
    }

    virtual bool isVisible(const iRect &world_win ) const;

    virtual void blit( Surface *surface, const iRect &world_win );

};

#endif // ** _SELECTION_BOX_SPRITE_HPP
