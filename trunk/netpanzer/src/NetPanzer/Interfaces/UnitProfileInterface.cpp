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
#include <config.h>
#include "UnitProfileInterface.hpp"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>



typedef
struct
{
    char params[16][64];
    short param_count;
}
parameter_list;

#define _MAX_KEY_LENGTH 64

char field_headers[19][64] = { "[TURRET_PLACEMENT]", "[MOVE_OFFSET]", "HITPOINTS",
                               "ATTACK_FACTOR", "RELOAD_TIME", "REACTION_TIME", "RANGE_MAX", "REGEN_TIME",
                               "DEFEND_RANGE", "SPEED_RATE", "SPEED_FACTOR", "REPATH_TIME", "FUEL_CAPACITY",
                               "TRACKING_RATE", "JAMM_ABLE", "JAMMING_TIME", "JAMMING_RANGE", "FUELING_RANGE", "END" };

enum { _turret_place, _move_offset, _hit_points, _attack_x, _reload_time,
       _react_time, _range_max, _regen_time, _defend_range, _speed_rate, _speed_factor, _repath_time, _fuel_capacity,
       _tracking_rate, _jamm_able, _jamming_time, _jamming_range, _fueling_range, _end };

short max_field_key = 19;


//****************************************************************************************

char * extract_spaces(char *string,int index)

{
    int i=0;
    char *stroffset;

    stroffset = string+index;

    while( *(stroffset+i) == ' ' )/* extract leading spaces just encase */
        i++;

    return(stroffset+i);
}

//****************************************************************************************

void find_keyword(char *string, int *command, char *key_list, char max_key_list )
{
    int comindex;    /* index to config commands */
    short int notfound=1;
    char *str;

    str = extract_spaces(string,0);

    for(comindex=0; ( (comindex < max_key_list )  && notfound); comindex++ ) {
        notfound = strncasecmp( str, &key_list[ comindex * _MAX_KEY_LENGTH ], strlen( &key_list[ comindex * _MAX_KEY_LENGTH ]) );
    }

    comindex = comindex-1;

    if ( (comindex == max_key_list - 1 ) && (notfound != 0) ) /* Checks if no commands found */
        *command = max_key_list;
    else
        *command = comindex;

}
//****************************************************************************************
void get_line( char *string, long count, FILE *infile )
{
    int  inchar;
    char tempchar;
    short string_count = 0;

    inchar = fgetc( infile );
    tempchar = (char ) inchar;

    while( !feof(infile) && (tempchar != ';' ) ) {
        if ( (tempchar != '\n') && (tempchar != '\r') ) {
            string[ string_count ] = tempchar;
            string_count++;
        }

        inchar = fgetc( infile );
        tempchar = (char ) inchar;
    }

    string[ string_count ] = 0;
}


//****************************************************************************************

short extract_token( char *token, char **string )
{
    char token_char = ' ';
    char *str;
    short index = 0;
    short token_index = 0;

    str = *string;

    str = extract_spaces( str, 0 );

    while( (str[index] != 0) && (str[ index ] != token_char) ) {
        if (str[index] == '"')
            token_char = '"';
        else
            if ( (str[index] != '\n') && (str[index] != '\r') ) {
                token[ token_index ] = str[index];
                token_index++;
            }

        index++;
    }

    token[ token_index ] = 0;

    *string = (str+index);

    if(str[index] == 0)
        *string = 0;

    if ( token_index == 0 )
        return (false);

    return (true);

}

void string_to_params( char *string, parameter_list *params )
{
    params->param_count = 0;

    if( extract_token( params->params[ params->param_count ], &string ) )
        params->param_count++;


    while ( string != 0 ) {
        if( extract_token( params->params[ params->param_count ], &string ) )
            params->param_count++;
    }


}


void read_vehicle_profile( char *file_path, UnitProfile *profile)
{
    int field;
    char temp_str[80];
    parameter_list param_list;
    int temp_int;
    short not_done = true;

    FILE *file_ptr = 0;

    file_ptr = fopen ( file_path, "rt" );
    assert( file_ptr != 0 );

    while( not_done ) {

        get_line( temp_str, 80, file_ptr );
        string_to_params( temp_str, &param_list );
        find_keyword( param_list.params[0], &field, (char * ) field_headers, max_field_key );

        switch(field) {

        case _hit_points : {
                sscanf( param_list.params[1], "%d", &temp_int );
                profile->hit_points = (short) temp_int;
            }
            break;

        case _attack_x : {
                sscanf( param_list.params[1], "%d", &temp_int );
                profile->attack_factor = (short) temp_int;
            }
            break;

        case _reload_time : {
                sscanf( param_list.params[1], "%d", &temp_int );
                profile->reload_time = (char) temp_int;
            }
            break;

        case _react_time : {
                sscanf( param_list.params[1], "%d", &temp_int );
                profile->reaction_time = (char) temp_int;
            }
            break;

        case _range_max : {
                sscanf( param_list.params[1], "%d", &temp_int );
                profile->attack_range = (long) temp_int;
            }
            break;

        case _regen_time: {
                sscanf( param_list.params[1], "%d", &temp_int );
                profile->regen_time = (short) temp_int;
            }
            break;

        case _defend_range: {
                sscanf( param_list.params[1], "%d", &temp_int );
                profile->defend_range = (long) temp_int;
            }
            break;

        case _speed_rate: {
                sscanf( param_list.params[1], "%d", &temp_int );
                profile->speed_rate = (char) temp_int;
            }
            break;

        case _speed_factor: {
                sscanf( param_list.params[1], "%d", &temp_int );
                profile->speed_factor = (char) temp_int;
            }
            break;

        case _repath_time: {
                sscanf( param_list.params[1], "%d", &temp_int );
                profile->repath_time = (char) temp_int;
            }
            break;

        case _fuel_capacity: {
                sscanf( param_list.params[1], "%d", &temp_int );
                profile->fuel_capacity = (short ) temp_int;
            }
            break;

        case _tracking_rate: {
                sscanf( param_list.params[1], "%d", &temp_int );
                profile->tracking_rate = (char) temp_int;
            }
            break;

        case _jamm_able: {
                sscanf( param_list.params[1], "%d", &temp_int );
                profile->jammable = (char) temp_int;
            }
            break;

        case _jamming_time: {
                sscanf( param_list.params[1], "%d", &temp_int );
                profile->jamming_time = (char) temp_int;
            }
            break;

        case _jamming_range: {
                sscanf( param_list.params[1], "%d", &temp_int );
                profile->jamming_range = (long) temp_int;
            }
            break;

        case _fueling_range: {
                sscanf( param_list.params[1], "%d", &temp_int );
                profile->fueling_range = (long) temp_int;
            }
            break;

        case _end : {
                not_done = false;
            }
            break;
        } // ** switch
    } // ** while ( !feof )
} // function

UnitProfile UnitProfileInterface::profile_table[ _MAX_UNIT_TYPES ];

void UnitProfileInterface::loadUnitProfiles( void )
{
    read_vehicle_profile( "units/profiles/Titan.pfl",
                          &profile_table[ _unit_type_abrams ]  );

    read_vehicle_profile( "units/profiles/Manta.pfl",
                          &profile_table[ _unit_type_valentine ]  );

    read_vehicle_profile( "units/profiles/Panther1.pfl",
                          &profile_table[ _unit_type_leopard ]  );

    read_vehicle_profile( "units/profiles/Stinger.pfl",
                          &profile_table[ _unit_type_hammerhead ]  );

    read_vehicle_profile( "units/profiles/SPanzer.pfl",
                          &profile_table[ _unit_type_humvee ]  );

    read_vehicle_profile( "units/profiles/Bobcat.pfl",
                          &profile_table[ _unit_type_lynx ]  );

    read_vehicle_profile( "units/profiles/Wolf.pfl",
                          &profile_table[ _unit_type_scorpion ]  );

    read_vehicle_profile( "units/profiles/Bear.pfl",
                          &profile_table[ _unit_type_spahpanzer ]  );

    read_vehicle_profile( "units/profiles/Drake.pfl",
                          &profile_table[ _unit_type_m109 ]  );

    read_vehicle_profile( "units/profiles/Archer.pfl",
                          &profile_table[ _unit_type_archer ]  );

}

UnitProfile * UnitProfileInterface::getUnitProfile( unsigned short unit_type )
{
    assert( unit_type < _MAX_UNIT_TYPES );
    return( &profile_table[ unit_type ] );
}
