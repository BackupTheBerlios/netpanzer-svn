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

#include <iostream>

#include <SDL.h>
#include <stdlib.h>
#include <assert.h>

#include "BotPlayer.hpp"

#include "PlayerInterface.hpp"
#include "PlayerID.hpp"
#include "UnitInterface.hpp"
#include "MapInterface.hpp"
#include "UnitBase.hpp"
#include "ObjectiveInterface.hpp"
#include "Objective.hpp"
#include "Vehicle.hpp"
#include "Util/Log.hpp"

//-----------------------------------------------------------------
BotPlayer::BotPlayer()
    : m_timer(1)
{
    /* empty */
}
//-----------------------------------------------------------------
    void
BotPlayer::processEvents()
{
    int playerIndex = isReady();
    if (playerIndex != NONE_PLAYER) {
        UnitBase *unit = getRandomUnit(playerIndex);
        if (unit) {
            std::cout << "Unit:" << unit->id;
            int unitTask = m_tasks.queryUnitTask(unit);
            if (unitTask != BotTaskList::TASK_MOVE) {
                std::cout << " Occupy";
                unitOccupyOupost(unit);
            }

            // manual fire on closest enemy
            UnitBase *enemyUnit;
            if (UnitInterface::queryClosestEnemyUnit(&enemyUnit,
                        unit->unit_state.location, playerIndex))
            {
                std::cout << " Fire(" << enemyUnit->id << ")";
                manualFire(unit, enemyUnit->unit_state.location);
            }
            std::cout << std::endl;
        }
        else {
            LOGGER.debug("bot: empty unit list");
        }

        outpostProduce();
    }
}
//-----------------------------------------------------------------
/**
 * Whether is time to make action.
 * @return playerIndex or NONE_PLAYER
 */
   int 
BotPlayer::isReady()
{
    int playerIndex = NONE_PLAYER;
    if (m_timer.count()) {
        playerIndex = PlayerInterface::getLocalPlayerIndex();
        if (playerIndex != NONE_PLAYER) {
            int unitCount = UnitInterface::getUnitCount(playerIndex);
            if (unitCount > 0) {
                m_timer.changePeriod(5.0 / unitCount);
            }
        }
    }

    return playerIndex;
}
//-----------------------------------------------------------------
/**
 * @return unit which belong to playerIndex
 */
UnitBase *
BotPlayer::getRandomUnit(int playerIndex)
{
    const std::vector<UnitBase*>& units 
        = UnitInterface::getPlayerUnits(playerIndex);

    if(units.size() == 0)
        return 0;
    
    size_t unitIndex = rand() % units.size();
    return units[unitIndex];
}

//-----------------------------------------------------------------
/**
 * @return list of active enemy players
 */
playerList_t
BotPlayer::getEnemyPlayers()
{
    playerList_t players;
    int localIndex = PlayerInterface::getLocalPlayerIndex();
    int max_players = PlayerInterface::getMaxPlayers();

    for (int player_index = 0; player_index < max_players; player_index++) {
        if (PlayerInterface::getPlayer(player_index)->getStatus() ==
                _player_state_active
                && localIndex != player_index
                && !PlayerInterface::isAllied(localIndex, player_index))
        {
            players.push_back(player_index);
        }
    }
    return players;
}
//-----------------------------------------------------------------
/**
 * @return playerIndex or NONE_PLAYER
 */
int
BotPlayer::getRandomEnemyPlayer()
{
    int result = NONE_PLAYER;
    playerList_t players = getEnemyPlayers();
    if (players.size() > 0) {
        result = players[rand() % players.size()];
    }
    return result;
}
//-----------------------------------------------------------------
/**
 * @return enemy unit
 */
UnitBase *
BotPlayer::getRandomEnemy()
{
    UnitBase *enemyUnit = 0;
    int enemyPlayer = getRandomEnemyPlayer();
    if (enemyPlayer != NONE_PLAYER) {
        enemyUnit = getRandomUnit(enemyPlayer);
    }
    return enemyUnit;
}
//-----------------------------------------------------------------
/**
 *  Dispositions:
 *  _objective_disposition_unoccupied,
 *  _objective_disposition_player,
 *  _objective_disposition_allie,
 *  _objective_disposition_enemy
 *
 * @return list of outposts with this disposition
 */
outpostList_t
BotPlayer::getOutposts(int disposition)
{
    outpostList_t outposts;
    iRect objRect;
    unsigned char objectiveDisposition;
    int objectiveID;

    ObjectiveInterface::startObjectivePositionEnumeration();
    while (ObjectiveInterface::objectivePositionEnumeration(&objRect,
            &objectiveDisposition, &objectiveID))
    {
        if (disposition == objectiveDisposition) {
            outposts.push_back(objectiveID);
        }
    }
    return outposts;
}
//-----------------------------------------------------------------
/**
 * @return objective with this disposition or 0
 */
ObjectiveState *
BotPlayer::getRandomOutpost(int disposition)
{
    ObjectiveState *result = 0;
    outpostList_t outposts = getOutposts(disposition);
    if (outposts.size() > 0) {
        int outpostID = outposts[rand() % outposts.size()];
        result = ObjectiveInterface::getObjectiveState(outpostID);
    }
    return result;
}
//-----------------------------------------------------------------
/**
 * Occupy free or enemy oupost.
 */
void
BotPlayer::unitOccupyOupost(UnitBase *unit)
{
    ObjectiveState *outpost =
        getRandomOutpost(_objective_disposition_unoccupied);
    if (!outpost) {
        outpost = getRandomOutpost(_objective_disposition_enemy);
    }
    if (outpost) {
        // XXX hack, const occupation_pad_offset
        static const iXY occupation_pad_offset(224, 48);

        iXY map_loc;
        MapInterface::pointXYtoMapXY(
                outpost->location + occupation_pad_offset,
                &map_loc);
        moveUnit(unit, map_loc);
    }
}
//-----------------------------------------------------------------
/**
 * Produce in factories
 */
void
BotPlayer::outpostProduce()
{
    outpostList_t outposts = getOutposts(_objective_disposition_player);
    for (outpostList_t::iterator i = outposts.begin();
            i != outposts.end(); i++)
    {
        OutpostStatus outpostStatus =
            ObjectiveInterface::getOutpostStatus(*i);
        if (outpostStatus.unit_generation_on_off == false) {
            // XXX hack, _unit_type_humvee is expected to be last accesible
            produceUnit(*i, rand() % (_unit_type_humvee + 1));
        }
    }
}
