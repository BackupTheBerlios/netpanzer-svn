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

#include "UnitID.hpp"
#include "Util/Endian.hpp"

UnitID::UnitID()
    : player(0xff), index(0xffff), unique(0xffff)
{ }

void UnitID::set(uint8_t player, uint16_t index, uint16_t unique)
{
    UnitID::player = player;
    UnitID::index = htol16(index);
    UnitID::unique = htol16(unique);
}

void UnitID::set(uint8_t player, uint16_t index)
{
    UnitID::player = player;
    UnitID::index = htol16(index);
}

uint8_t UnitID::getPlayer() const
{
    return player;
}

uint16_t UnitID::getIndex() const
{
    return ltoh16(index);
}

uint16_t UnitID::getUnique() const
{
    return ltoh16(unique);
}

bool UnitID::operator==( const UnitID& Uid ) const
{
    if ( (player == Uid.player) && (index == Uid.index)
            && (unique == Uid.unique)
       )
        return true;

    return false;
}

bool UnitID::operator!=( const UnitID& Uid ) const
{
    if ( (player != Uid.player) || (index != Uid.index)
            || (unique != Uid.unique)
       )
        return true;

    return false;
}

/* operator< is only used to fit into an std::map, so it's ok that the
 * comparisons are potentially wrong-endian, as long as they're
 * consistent. */
bool UnitID::operator<( const UnitID& Uid ) const
{
    return (player < Uid.player
            || (player == Uid.player && index < Uid.index)
            || (player == Uid.player && index == Uid.index
                && unique < Uid.unique));
}

bool UnitID::playerEqual( const UnitID& Uid ) const
{
    if ( (player == Uid.player) )
        return true;

    return false;
}

void UnitID::operator=( const UnitID& Uid )
{
    player = Uid.player;
    index  = Uid.index;
    unique = Uid.unique;
}
