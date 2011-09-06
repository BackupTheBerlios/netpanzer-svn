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

#include "JoinView.hpp"

#include "Views/Components/Button.hpp"
#include "Views/Components/CheckBox.hpp"
#include "Views/Components/Choice.hpp"
#include "Views/Components/Label.hpp"
#include "Views/Components/InputField.hpp"
#include "Util/Log.hpp"


JoinView::JoinView()
{
    setSearchName("XJoinView");
    setTitle("XJoinView");
    setSubTitle("");

    setAllowResize(false);
    setAllowMove(false);

    setBackgroundColor(Color::black);

    moveTo(iXY(242,60));
    resize(iXY(400,400));

    Label * player_name_label = new Label(10,12,"Name:",Color::white);
    add(player_name_label);

    InputField * infi = new InputField();
    infi->setLocation(50,10);
    infi->setSize(100, 12);
    add(infi);

    Button * test_button = new Button();
    test_button->setLocation(10,30);
    test_button->setNormalBorder();
    test_button->setSize(80,20);
    test_button->setLabel("A Button");
    test_button->clicked.connect(this, &JoinView::onButtonClicked);
    add(test_button);

    cb = new CheckBox("A CheckBox", false);
    cb->stateChanged.connect(this, &JoinView::onCheckBoxStateChanged);
    cb->setLocation(100, 30);
    add(cb);

    choice = new Choice();
    choice->setLocation(10,60);
    choice->addItem("item 1");
    choice->addItem("item 2");
    choice->addItem("item 3");
    choice->addItem("item 4");
    choice->stateChanged.connect(this, &JoinView::onChoiceChanged);
    add(choice);

    Button * other_button = new Button();
    other_button->loadPackedImage("pics/backgrounds/menus/buttons/default/pak/inMenusHelp.pak");
    other_button->setLocation(10, 150);
    add(other_button);

}

JoinView::~JoinView()
{
}

void JoinView::onButtonClicked()
{
    LOGGER.warning("The button was clicked!!!");
}

void JoinView::onCheckBoxStateChanged()
{
    LOGGER.warning("The checkbox now is %s", cb->getState() ? "on" : "off");
}

void JoinView::onChoiceChanged()
{
    LOGGER.warning("The new choice index is %d", choice->getSelectedIndex());
}
