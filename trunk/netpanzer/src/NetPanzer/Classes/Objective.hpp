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
#ifndef _OBJECTIVE_HPP
#define _OBJECTIVE_HPP

#include "ArrayUtil/BoundBox.hpp"
#include "SpriteSorter.hpp"
#include "PlayerState.hpp"
#include "ObjectiveMessageTypes.hpp"

enum { _objective_status_null };

enum { _occupation_status_unoccupied,
       _occupation_status_occupied
     };


class ObjectiveState
{
public:
    short ID;
    bool  selection_state;
    iRect    selection_box;
    unsigned char outpost_type;
    char name[64];
    iXY location;
    BoundBox capture_area;
    BoundBox area;
    unsigned char objective_status;
    unsigned char occupation_status;
    PlayerID occupying_player;

    inline bool isBounded( iXY &test )
    {
        return( capture_area.bounds( location, test ) );
    }

};


class OutpostStatus
{
public:
    unsigned short unit_generation_type;
    bool        unit_generation_on_off;
    float          unit_generation_time;
    float          unit_generation_time_remaining;
    iXY unit_collection_loc;
};

class Objective
{
protected:
    void objectiveMesgUpdateOccupation( ObjectiveMessage *message );
    void objectiveMesgSync( ObjectiveMessage *message );

public:
    ObjectiveState objective_state;

    Objective( short ID, iXY location, BoundBox area );

    void getSyncData( SyncObjective &objective_sync_mesg );

    virtual void processMessage( ObjectiveMessage *message );

    virtual void updateStatus( void )
    { }

    virtual void offloadGraphics( SpriteSorter &sorter )
    { }

}
;

#endif // ** _OBJECTIVE_HPP
