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
#ifndef _OUTPOST_HPP
#define _OUTPOST_HPP

#include "Classes/Objective.hpp"
#include "Util/Timer.hpp"
#include "Classes/SelectionBoxSprite.hpp"
#include "Classes/UnitBase.hpp"

class Outpost : public Objective
{
private:
    unsigned char outpost_state;
    iXY outpost_map_loc;
    iXY unit_generation_loc;
    iXY unit_collection_loc;
    iXY occupation_pad_offset;
public:
    unsigned short unit_generation_type;
    bool unit_generation_on_flag;

private:
    SelectionBoxSprite select_box;

public:
    Timer occupation_status_timer;
    Timer unit_generation_timer;

private:
    void attemptOccupationChange(UnitID unit_id);

    void checkOccupationStatus( void );

    void generateUnits( void );

    // ** Message Handlers **
    void objectiveMesgChangeUnitGeneration(const ObjectiveMessage* message);
    void objectiveMesgDisownPlayerObjective(const ObjectiveMessage* message);
    void objectiveMesgChangeOutputLocation(const ObjectiveMessage* message);

public:

    Outpost( short ID, iXY location, BoundBox area );

    void getOutpostStatus( OutpostStatus &status );

    virtual void processMessage(const ObjectiveMessage* message);

    virtual void updateStatus();

    virtual void offloadGraphics( SpriteSorter &sorter );

};

#endif // ** _OUTPOST_HPP

