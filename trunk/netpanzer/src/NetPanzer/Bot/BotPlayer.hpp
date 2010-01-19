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

#ifndef BOTPLAYER_H
#define BOTPLAYER_H

#include "Core/CoreTypes.hpp"
#include "Bot.hpp"
#include "Util/Timer.hpp"

class ObjectiveState;

#include <vector>
typedef std::vector<int> playerList_t;
typedef std::vector<ObjectiveID> outpostList_t;

class BotPlayer : public Bot {
    private:
        static const int NONE_PLAYER = 0xFFFF;
        Timer m_timer;
    public:
        BotPlayer();
        virtual void processEvents();

        int isReady();
        UnitBase *getRandomUnit(int playerIndex);

        playerList_t getEnemyPlayers();
        int getRandomEnemyPlayer();
        UnitBase *getRandomEnemy();

        outpostList_t getOutposts(int disposition);
        ObjectiveState *getRandomOutpost(int disposition);
        void unitOccupyOupost(UnitBase *unit);
        void outpostProduce();
};

#endif

