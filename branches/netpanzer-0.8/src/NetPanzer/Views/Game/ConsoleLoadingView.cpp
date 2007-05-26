/*
Copyright (C) 2003 Ivo Danihelka <ivo@danihelka.net>
 
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
#include "ConsoleLoadingView.hpp"

#include "Util/Log.hpp"
#include "GameManager.hpp"

// update
//---------------------------------------------------------------------------
// Purpose: Adds the specified text to the current surface, erasing
//          whatever is on the current line.
//---------------------------------------------------------------------------
void ConsoleLoadingView::update(const char *text)
{
    LOGGER.info("Loading: %s", text);
}

// toggleGameView
//---------------------------------------------------------------------------
void ConsoleLoadingView::toggleGameView()
{
    GameManager::setNetPanzerGameOptions();
}

// toggleMainMenu
//---------------------------------------------------------------------------
void ConsoleLoadingView::toggleMainMenu()
{
}

