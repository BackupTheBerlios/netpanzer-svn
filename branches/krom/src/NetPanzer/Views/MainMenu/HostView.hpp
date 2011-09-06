/*
Copyright (C) 2011 by Aaron Perez <aaronps@gmail.com>

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
#ifndef HOSTVIEW_HPP
#define HOSTVIEW_HPP

#include "Views/Components/View.hpp"

class Label;
class InputField;
class Slider;

class XHostView : public View
{
public:
    XHostView();
    virtual ~XHostView();
private:
    Label       * player_name_label;
    InputField  * player_name_field;

    Label       * server_name_label;
    InputField  * server_name_field;

    Label       * motd_label;
    InputField  * motd_field;

    Label       * max_players_label;
    Label       * max_players_num;
    Slider      * max_players_slider;
    void onMaxPlayersSliderChanged();

    Label       * units_per_player_label;
    Label       * units_per_player_num;
    Slider      * units_per_player_slider;
    void onUnitsPerPlayerSliderChanged();

};


#endif // HOSTVIEW_HPP
