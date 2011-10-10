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


#include "Units/UnitProfileInterface.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Util/Exception.hpp"
#include "Util/FileSystem.hpp"
#include "Util/FileStream.hpp"
#include "Util/Log.hpp"
#include <ctype.h>
#include <memory>
#include <string.h>

#include <vector>

#include "Classes/Network/NetMessage.hpp"
#include "Classes/Network/NetPacket.hpp"

enum
{
    _profile_msg_profile_desc = 0
};

class ByteBufferWriter
{
private:
    unsigned char * buffer;
    size_t len;
    size_t pos;

public:
    ByteBufferWriter(unsigned char * buffer, int len)
    {
        this->buffer = buffer;
        this->len = len;
        this->pos = 0;
    }

    int writedBytesCount() const { return pos; }

    bool writeInt8( Uint8 c )
    {
        if ( len >= pos+1 )
        {
            buffer[pos++] = c;
            return true;
        }
        return false;
    }

    bool writeInt16( Uint16 c )
    {
        if ( len >= pos+2 )
        {
            buffer[pos++] = c & 0xff;
            buffer[pos++] = (c >> 8) & 0xff;
            return true;
        }
        return false;
    }

    bool writeInt32( Uint32 c )
    {
        if ( len >= pos+4 )
        {
            buffer[pos++] = c & 0xff;
            buffer[pos++] = (c >> 8) & 0xff;
            buffer[pos++] = (c >> 16) & 0xff;
            buffer[pos++] = (c >> 24) & 0xff;
            return true;
        }
        return false;
    }

    bool writeString( const NPString& str)
    {
        if ( len >= pos + 2 + str.length() )
        {
            writeInt16(str.length());
            str.copy((char *)&buffer[pos], str.length());
            pos += str.length();
            return true;
        }
        return false;
    }
};

class ByteBufferReader
{
private:
    const unsigned char * buffer;
    size_t len;
    size_t pos;

public:
    ByteBufferReader(const unsigned char * buffer, int len)
    {
        this->buffer = buffer;
        this->len = len;
        this->pos = 0;
    }

    int readedBytesCount() const { return pos; }

    bool readInt8( Uint8* c )
    {
        if ( pos < len )
        {
            *c = buffer[pos++];
            return true;
        }
        return false;
    }

    bool readInt16( Uint16* c )
    {
        if ( len >= pos+2 )
        {
            *c = buffer[pos] | (buffer[pos+1] << 8);
            pos+=2;
            return true;
        }
        return false;
    }

    bool readInt32( Uint32* c )
    {
        if ( len >= pos+4 )
        {
            *c = buffer[pos]
               | (buffer[pos+1] << 8)
               | (buffer[pos+2] << 16)
               | (buffer[pos+3] << 24);
            pos += 4;
            return true;
        }
        return false;
    }

    bool readString( NPString& str)
    {
        Uint16 slen = 0;
        if ( readInt16(&slen) )
        {
            if ( len >= pos+slen )
            {
                if ( slen > 0 )
                {
                    str.assign((char *)&buffer[pos], slen);
                    pos += str.length();
                }
                else
                {
                    str.clear();
                }
                return true;
            }
            else
            {
                pos -= 2;
            }
        }
        else
        {
            pos -= 2;
        }
        return false;
    }
};

void string_to_params( const NPString& str, vector<NPString>& parameters )
{
    parameters.clear();

    static char* limiters[] = { ", \t", "\"" };

    NPString::size_type start = 0;
    NPString::size_type end = 0;

    do
    {
        start = str.find_first_not_of(" \t", start);
        if ( start != NPString::npos )
        {
            int limit_index = 0;
            if ( str[start] == '"' && start < str.length()-1 )
            {
                limit_index = 1;
                ++start;
            }

            end = str.find_first_of(limiters[limit_index], start);

            parameters.push_back( str.substr(start, end != NPString::npos ? end-start : end) );

            if ( end != NPString::npos )
            {
                start = str.find_first_not_of(limiters[limit_index], end);
            }
        }
    } while ( start != NPString::npos && end != NPString::npos );
}

bool read_vehicle_profile(const NPString& unitName, UnitProfile *profile)
{
    int temp_int;
    bool not_done = true;

    NPString file_path = "units/profiles/";
    file_path += unitName;
    file_path += ".pfl";

    profile->unitname = unitName;
    
    try
    {
        IFileStream in(file_path);
        NPString line;
        std::vector<NPString> parameters;

        while( not_done && !in.eof() )
        {
            std::getline(in,line);
            string_to_params( line, parameters );

            if ( parameters.size() == 0 )
            {
                continue;
            }

            if ( parameters[0] == "HITPOINTS" )
            {
                sscanf( parameters[1].c_str(), "%d", &temp_int );
                profile->hit_points = (short) temp_int;
            }
            else if ( parameters[0] == "ATTACK_FACTOR" )
            {
                sscanf( parameters[1].c_str(), "%d", &temp_int );
                profile->attack_factor = (short) temp_int;
            }
            else if ( parameters[0] == "RELOAD_TIME" )
            {
                sscanf( parameters[1].c_str(), "%d", &temp_int );
                profile->reload_time = (char) temp_int;
            }
            else if ( parameters[0] == "RANGE_MAX" )
            {
                sscanf( parameters[1].c_str(), "%d", &temp_int );
                profile->cfg_attack_range = temp_int;
                temp_int *= 32;         // adjust new values
                temp_int *= temp_int;
                profile->attack_range = (long) temp_int;
            }
            else if ( parameters[0] == "REGEN_TIME" )
            {
                sscanf( parameters[1].c_str(), "%d", &temp_int );
                profile->regen_time = (short) temp_int;
            }
            else if ( parameters[0] == "DEFEND_RANGE" )
            {
                sscanf( parameters[1].c_str(), "%d", &temp_int );
                profile->cfg_defend_range = temp_int;
                temp_int *= 32;         // adjust new values
                temp_int *= temp_int;
                profile->defend_range = (long) temp_int;
            }
            else if ( parameters[0] == "SPEED_RATE" )
            {
                sscanf( parameters[1].c_str(), "%d", &temp_int );
                profile->speed_rate = (char) temp_int;
            }
            else if ( parameters[0] == "SPEED_FACTOR" )
            {
                sscanf( parameters[1].c_str(), "%d", &temp_int );
                profile->speed_factor = (char) temp_int;
            }
            else if ( parameters[0] == "IMAGE" )
            {
                profile->imagefile = parameters[1];
            }
            else if ( parameters[0] == "BODYSPRITE" )
            {
                profile->bodySprite_name = parameters[1];
                profile->bodySprite.load(parameters[1]);
            }
            else if ( parameters[0] == "BODYSHADOW" )
            {
                profile->bodyShadow_name = parameters[1];
                profile->bodyShadow.load(parameters[1]);
            }
            else if ( parameters[0] == "TURRETSPRITE" )
            {
                profile->turretSprite_name = parameters[1];
                profile->turretSprite.load(parameters[1]);
            }
            else if ( parameters[0] == "TURRETSHADOW" )
            {
                profile->turretShadow_name = parameters[1];
                profile->turretShadow.load(parameters[1]);
            }
            else if ( parameters[0] == "SOUNDSELECTED" )
            {
                profile->soundSelected = parameters[1];
            }
            else if ( parameters[0] == "FIRESOUND" )
            {
                profile->fireSound = parameters[1];
            }
            else if ( parameters[0] == "WEAPON" )
            {
                profile->weaponType = parameters[1];
            }
            else if ( parameters[0] == "BOUNDBOX" )
            {
                sscanf( parameters[1].c_str(), "%d", &temp_int );
                profile->boundBox = (short) temp_int;
            }
            else if ( parameters[0] == "END" || parameters[0] == "END;" )
            {
                not_done = false;
            }
	} // ** while ( !feof )
    } catch(std::exception& e) {
        LOGGER.warning("Error while reading unitprofile '%s': %s",
                       file_path.c_str(), e.what() );
        return false;
    }

    return true;
} // function

vector<UnitProfile *> UnitProfileInterface::profiles;

void
UnitProfileInterface::clearProfiles()
{
    vector<UnitProfile *>::iterator i = profiles.begin();
    while ( i != profiles.end() )
    {
        delete *i;
        i++;
    }
    profiles.clear();
}

void UnitProfileInterface::loadUnitProfiles( void )
{
    clearProfiles();

    std::vector<NPString> plist;
    NPString pl = *GameConfig::game_unit_profiles;

    string_to_params(pl, plist);

    for ( int n = 0; n < plist.size(); ++n )
    {
        addLocalProfile(plist[n]);
    }

    if ( profiles.size() == 0 )
    {
        LOGGER.warning("Didn't load ANY PROFILE!!! I should die.");
    }
//    addLocalProfile("Manta");
//    addLocalProfile("Panther1");
//    addLocalProfile("Titan");
//    addLocalProfile("Stinger");
//    addLocalProfile("Bobcat");
//    addLocalProfile("Bear");
//    addLocalProfile("Archer");
//    addLocalProfile("Wolf");
//    addLocalProfile("Drake");
//    addLocalProfile("Spanzer");
}

bool UnitProfileInterface::addLocalProfile(const NPString& name)
{
    UnitProfile * p = new UnitProfile();

    bool isok = read_vehicle_profile(name, p);
    if ( isok )
    {
        p->unit_type = profiles.size();
        profiles.push_back(p);
    }

    return isok;
}

UnitProfile * UnitProfileInterface::getUnitProfile( unsigned short unit_type )
{
    if ( unit_type < profiles.size() )
        return profiles[unit_type];
    return 0;
}

UnitProfile *
UnitProfileInterface::getProfileByName( const NPString& name )
{
    vector<UnitProfile *>::iterator i = profiles.begin();
    while ( i != profiles.end() )
    {
        if ( name.length() != (*i)->unitname.length() )
        {
            i++;
            continue; // continue if size !=
        }

        std::string::size_type s = 0;
        while ( s < name.length()
               && toupper(name[s]) == toupper((*i)->unitname[s]) )
        {
            s++;
        }
        
        if ( s == name.length() )
            return *i;
        
        i++;
    }
    return 0; // null pointer warning
}

int
UnitProfileInterface::fillProfileSyncMessage(NetMessage* message, int profile_id)
{
    UnitProfile *p = getUnitProfile(profile_id);
    ByteBufferWriter bb((unsigned char *)message, _MAX_NET_PACKET_SIZE);

    bb.writeInt8( _net_message_class_unit_profile );
    bb.writeInt8( _profile_msg_profile_desc );

    bb.writeString( p->unitname );
    bb.writeInt16(  p->hit_points );
    bb.writeInt16(  p->attack_factor );
    bb.writeInt16(  p->cfg_attack_range );
    bb.writeInt16(  p->cfg_defend_range );
    bb.writeInt8(   p->speed_factor );
    bb.writeInt8(   p->speed_rate );
    bb.writeInt8(   p->reload_time );
    bb.writeInt16(  p->regen_time );
    bb.writeString( p->imagefile );
    bb.writeString( p->bodySprite_name );
    bb.writeString( p->bodyShadow_name );
    bb.writeString( p->turretSprite_name );
    bb.writeString( p->turretShadow_name );
    bb.writeInt16(  p->boundBox );

    return bb.writedBytesCount();
}

UnitProfile *
UnitProfileInterface::loadProfileFromMessage(const NetMessage *message, size_t size)
{
    UnitProfile *p = new UnitProfile();
    ByteBufferReader br((const unsigned char *)message, size);

    Uint8 unused;
    br.readInt8( &unused );
    br.readInt8( &unused );

    br.readString( p->unitname );
    br.readInt16( &p->hit_points );
    br.readInt16( &p->attack_factor );
    br.readInt16( &p->cfg_attack_range );
    br.readInt16( &p->cfg_defend_range );
    br.readInt8(  &p->speed_factor );
    br.readInt8(  &p->speed_rate );
    br.readInt8(  &p->reload_time );
    br.readInt16( &p->regen_time );
    br.readString( p->imagefile );
    br.readString( p->bodySprite_name );
    br.readString( p->bodyShadow_name );
    br.readString( p->turretSprite_name );
    br.readString( p->turretShadow_name );
    br.readInt16( &p->boundBox );

    p->bodySprite.load(p->bodySprite_name);
    p->bodyShadow.load(p->bodyShadow_name);
    p->turretSprite.load(p->turretSprite_name);
    p->turretShadow.load(p->turretShadow_name);

    Uint32 i = p->cfg_attack_range * 32;
    p->attack_range = i*i;

    i = p->cfg_defend_range * 32;
    p->defend_range = i*i;

    return p;
}

void
UnitProfileInterface::processNetMessage(const NetMessage* net_message, size_t size)
{
    switch ( net_message->message_id )
    {
        case _profile_msg_profile_desc:
            handleProfileDescMessage(net_message, size);
            break;

        default:
            LOGGER.warning("Unknown message id in UnitProfileMessage (%d)",
                           net_message->message_id);
    }
}

void
UnitProfileInterface::handleProfileDescMessage(const NetMessage *net_message, size_t size)
{
    UnitProfile* p = loadProfileFromMessage(net_message, size);
    if ( p )
    {
        p->unit_type = profiles.size();
        profiles.push_back(p);
    }
}
