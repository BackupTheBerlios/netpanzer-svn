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

#include "HostView.hpp"

#include <sstream>

#include "Views/Components/Button.hpp"
#include "Views/Components/CheckBox.hpp"
#include "Views/Components/Choice.hpp"
#include "Views/Components/Label.hpp"
#include "Views/Components/InputField.hpp"
#include "Views/Components/Slider.hpp"
#include "Util/Log.hpp"


XHostView::XHostView()
{
    setSearchName("XHostView");
    setTitle("XHostView");
    setSubTitle("");

    setAllowResize(false);
    setAllowMove(false);

    setBackgroundColor(Color::black);

    moveTo(iXY(242,60));
    resize(iXY(400,400));

    int y_pos = 12;
    int x_pos = 10;

    player_name_label = new Label(x_pos,y_pos,"Player Name:",Color::white);
    add(player_name_label);

    player_name_field = new InputField();
    player_name_field->setLocation(x_pos + 100, y_pos-2);
    player_name_field->setSize(100, 12);
    add(player_name_field);

    y_pos += 14;
    server_name_label = new Label(x_pos, y_pos, "Server Name:", Color::white);
    add(server_name_label);

    server_name_field = new InputField();
    server_name_field->setLocation(x_pos + 100, y_pos - 2);
    server_name_field->setSize(100, 12);
    add(server_name_field);

    y_pos += 14;
    motd_label = new Label(x_pos, y_pos, "MOTD:", Color::white);
    add(motd_label);

    motd_field = new InputField();
    motd_field->setLocation(x_pos + 100, y_pos - 2);
    motd_field->setSize(200, 12);
    add(motd_field);

    y_pos += 14;
    max_players_label = new Label(x_pos, y_pos, "Max players:", Color::white);
    add(max_players_label);

    max_players_num = new Label(x_pos + 100, y_pos, "8", Color::white);
    add(max_players_num);

    max_players_slider = new Slider();
    max_players_slider->setLocation(x_pos + 120, y_pos);
    max_players_slider->setSize(250, 8);
    max_players_slider->setMin(2);
    max_players_slider->setMax(16);
    max_players_slider->setValue(8);
    max_players_slider->changed.connect(this, &XHostView::onMaxPlayersSliderChanged);
    add(max_players_slider);

    y_pos += 14;
    units_per_player_label = new Label(x_pos, y_pos, "Units per player:", Color::white);
    add(units_per_player_label);

    units_per_player_num = new Label(x_pos + 150, y_pos, "40", Color::white);
    add(units_per_player_num);

    units_per_player_slider = new Slider();
    units_per_player_slider->setLocation(x_pos + 180, y_pos);
    units_per_player_slider->setSize(200, 8);
    units_per_player_slider->setMin(1);
    units_per_player_slider->setMax(255);
    units_per_player_slider->setValue(40);
    units_per_player_slider->changed.connect(this, &XHostView::onUnitsPerPlayerSliderChanged);
    add(units_per_player_slider);


}

XHostView::~XHostView()
{
}

void XHostView::onMaxPlayersSliderChanged()
{
    stringstream ss;
    ss << max_players_slider->getValue();
    max_players_num->setText(ss.str());
}

void XHostView::onUnitsPerPlayerSliderChanged()
{
    stringstream ss;
    ss << units_per_player_slider->getValue();
    units_per_player_num->setText(ss.str());
}

