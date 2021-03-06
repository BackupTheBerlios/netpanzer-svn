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
#ifndef BOT_H
#define BOT_H

class Unit;
class ObjectiveState;
class iXY;

#include "BotTaskList.hpp"

class Bot
{
public:
    Bot(Uint16 playerid);

    virtual ~Bot() {}
    virtual void processEvents() = 0;

    void moveUnit(Unit *unit, iXY map_pos);
    void attackUnit(Unit *unit, Unit *enemyUnit);
    void manualFire(Unit *unit, iXY world_pos);
    void produceUnit(ObjectiveID outpostID, int selectedProduce);
protected:
    BotTaskList m_tasks;
    Uint16 botPlayerId;
private:
    friend class BotManager;
};

#endif
