/*
Copyright (C) 2012 Netpanzer Team. (www.netpanzer.org), Laurent Jacques

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


#include "Views/Components/tPlayerStateBox.hpp"
#include "Views/Components/View.hpp"
#include "Views/Theme.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Resources/ResourceManager.hpp"

tPlayerStateBox::tPlayerStateBox(iRect rect, StateChangedCallback* newcallback)
        : tStringListBox(rect, newcallback)
{
    DrawFlags = true;
    UpdateState(true);
    ShowTeam = false;
    TeamNumber = 0;
}

void tPlayerStateBox::onPaint(int Index, int row)
{
    char statBuf[256];
    int StartX = 3;
    PlayerState *state = reinterpret_cast<PlayerState *>(List[Index].Data);
    if (DrawFlags)
    {
        Surface * flag = 0;
        flag = ResourceManager::getFlag(state->getFlag());
        flag->blt(surface, 2, row-3);
        StartX = flag->getWidth()+4;
    }
    snprintf(statBuf, sizeof(statBuf), "%-20s", state->getName().c_str());
    surface.bltString(StartX , row, statBuf, ctTexteNormal);
}

void tPlayerStateBox::UpdateState(bool ForceUpdate)
{
    if ((PlayerInterface::getActivePlayerCount() > Count()) || ForceUpdate) 
    {
        Clear();
        for ( int i = 0; i < PlayerInterface::getMaxPlayers(); ++i)
        {
            PlayerState* state = PlayerInterface::getPlayer(i);
            if( state->isActive() )
            {
                if (ShowTeam)
                {
                    if (state->getTeamID() == TeamNumber)
                        AddData(state->getName(), state);
                } 
                else AddData(state->getName(), state);
            }
        }
    }
}
void tPlayerStateBox::setDrawFlags(bool df)
{
    DrawFlags = df;
    dirty = true;
}

void tPlayerStateBox::setShowTeam(Uint8 Team_Number)
{
    TeamNumber = Team_Number;
    ShowTeam = true;
    UpdateState(true);
}

