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

#include "HelpScrollView.hpp"
#include "GameView.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Views/Components/Desktop.hpp"
#include "Classes/WorldInputCmdProcessor.hpp"

HelpScrollView::HelpScrollView() : SpecialButtonView()
{
    setSearchName("HelpScrollView");
    setTitle("Help Information");
    setSubTitle("");
    setAllowResize(false);
    setAllowMove(false);

    moveTo(bodyTextRect.min);
    resize(bodyTextRect.getSize());
    iRect r(0, 0, bodyTextRect.getSizeX()-25, bodyTextRect.getSizeY()-20);
    HelpBox = new tStringListBox(r, 0);
    scrollBar = new tVScrollBar();
    add(scrollBar);
    HelpBox->setVscrollBar(scrollBar);
    add(HelpBox);
    HelpBox->Add("Key");
    HelpBox->Add("");
    HelpBox->Add("  LMC = Left Mouse Click");
    HelpBox->Add("  RMC = Right Mouse Click");
    HelpBox->Add("  RMH = Right Mouse Hold");
    HelpBox->Add("");
    HelpBox->Add("");
    HelpBox->Add("Unit Related");
    HelpBox->Add("");
    HelpBox->Add("  LMC                              Selects a unit");
    HelpBox->Add("  LMH                              Makes a bouding box for unit selection");
    HelpBox->Add("  RMC                              De-selects all selected units");
    HelpBox->Add("  LMC + Ctrl                       Directs (force/manual) fire of a unit");
    HelpBox->Add("  LMC + Shift, LMC + Ctrl          Add unit to selection");
    HelpBox->Add("  Ctrl + [0..9]                    Define a group of units");
    HelpBox->Add("  [0..9]                           Recall a group of units");
    HelpBox->Add("  [0..9] (2 times)                 Center view on a group of units");
    //HelpBox->Add("  'E'                              Select all units on screen");

    //HelpBox->Add("  LMC + 'T'............................Track the selected unit");
    //HelpBox->Add("  LMC + 'W'............................Set the selected objective's way point");
    HelpBox->Add("  RMH                              Fast map scroll");
    //HelpBox->Add("  X....................................Scatter units");
    //HelpBox->Add("  S....................................Stop units");
    //HelpBox->Add("  LMC + Ctrl + Alt.....................Escort unit");
    HelpBox->Add("");
    HelpBox->Add("");
    HelpBox->Add("Game Related");
    HelpBox->Add("");
    HelpBox->Add("  LMC + 'A'                        Allies with the clicked unit's team");
    HelpBox->Add("  ESC                              In game menus");
    HelpBox->Add("");
    HelpBox->Add("");
    HelpBox->Add("Outpost Related");
    HelpBox->Add("");
    HelpBox->Add("  'O'                              Toggle outpost names");
    HelpBox->Add("  LMC on captured outpost          Displays the outpost view");
    HelpBox->Add("  mouse drag on captured outpost   Select unit spawn point");
    HelpBox->Add("");
    HelpBox->Add("");
    HelpBox->Add("Chat Related");
    HelpBox->Add("");
    HelpBox->Add("  Enter                            Send message to all");
    HelpBox->Add("  Ctrl + 'A'                       Send message to allies");
    HelpBox->Add("  'C'                              Send count down message");
    HelpBox->Add("");
    HelpBox->Add("");
    HelpBox->Add("View Related");
    HelpBox->Add("");
    HelpBox->Add("  F1                               Help View");
    HelpBox->Add("  'B'                              Toggle flag selection window");
    HelpBox->Add("  'D'                              Damage View");
    HelpBox->Add("  'F'                              Flag View");
    HelpBox->Add("  'M'                              Mini Map View");
    HelpBox->Add("  'N'                              Name View");
    HelpBox->Add("  TAB                              Rank View");
    HelpBox->Add("  RMH + mouse movement             Moves any view on the screen");
    HelpBox->Add("  RMH + [1 - 5]                    Sets the background drawing mode");
    HelpBox->Add(" ");
    HelpBox->Add(" ");
    HelpBox->Add("Mini Map View Related");
    HelpBox->Add("");
    HelpBox->Add("  RMH + [1 - 7]                    Sets the blending level of the minimap");
    HelpBox->Add("  RMH + [+ or -]                   Scales the size of the minimap");
    //HelpBox->Add("  RMH + 'O'                        Toggles outposts");
    HelpBox->Add(" ");
    HelpBox->Add(" ");
    HelpBox->Add("System Related");
    HelpBox->Add("");
    HelpBox->Add("  ALT + Enter                      Toggle Fullscreen/Windowed mode");
    HelpBox->Add("  F9                               BMP screen shot");
    HelpBox->Add(" ");
    HelpBox->Add(" ");
    HelpBox->Add("Scripting");
    HelpBox->Add(" ");
    HelpBox->Add("");
    HelpBox->Add("Client Related");
    HelpBox->Add("");
    HelpBox->Add("  /listcommands                     List of available commands");
    HelpBox->Add("  /countdown [timeout in seconds]   Generate a countdown");
    HelpBox->Add("  /quit                             Quit the game");
    HelpBox->Add("  /help                             Show Help window (same 'F1')");
    HelpBox->Add("  /say [your talk]                  Send message to all (same 'Enter'");
    HelpBox->Add("  /teamsay [your talk]              Send message to allies (same Ctrl + 'A')");
    HelpBox->Add("");
    HelpBox->Add("Server Related");
    HelpBox->Add("");
    HelpBox->Add("  /server listcommands              List of Gameserver available commands");
    HelpBox->Add("  /server adminlogin [password]     Gameserver admin login");
    HelpBox->Add("  /server unitspawnlist [list]      List/set spawn units (number)");
    HelpBox->Add("  /server unitprofiles [list]       List/set spawn units (types)");
    HelpBox->Add("  /server help                      Show Help window (same 'F1')");
    HelpBox->Add("  /server map [map name]            Restart game with the given map");
    HelpBox->Add("  /server autokick [timeout]        Show/set autokick for inactive players");
    HelpBox->Add("  /server flagtimer [timeout]       Show/set flag change timer");
    HelpBox->Add("  /server countdown [timeout]       Generate a countdown (same 'c')");
    HelpBox->Add("  /server say [your talk]           Server say something in chat");
    HelpBox->Add("  /server listplayers               List players with the assigned number");
    HelpBox->Add("  /server listprofiles              List available profiles");
    HelpBox->Add("  /server kick [player number]      Kick a given player");
    HelpBox->Add("  /server baselimit [max bases]     Show/set the max bases number per player");
    HelpBox->Add("  /server gamepass [password]       Show/set game connection password");
    HelpBox->Add(" ");
    HelpBox->Add(" ");
}

void HelpScrollView::doDraw(Surface &viewArea, Surface &clientArea)
{
    if (Desktop::getVisible("GameView")) {
        bltViewBackground(viewArea);
    }

    View::doDraw(viewArea, clientArea);
} // end HelpScrollView::doDraw

void HelpScrollView::doActivate()
{
    /* empty */
}

void HelpScrollView::processEvents()
{
    if ( Desktop::getVisible("GameView") )
        COMMAND_PROCESSOR.process(false);
}

