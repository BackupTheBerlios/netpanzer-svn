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
#include "stdafx.hpp"
#include "KeyBinder.hpp"

#include <string.h>


KeyBinder KEY_BINDER;

boolean KeyBinder::locked_key_table[256]; 

KeyBinder::KeyBinder( unsigned long key_list_size )
 {
  initialize( key_list_size );
 }

void KeyBinder::initialize( unsigned long key_list_size )
 {
  unsigned long key_list_index;

  key_list.initialize( key_list_size, 1, 256 );

  for( key_list_index = 0; key_list_index < key_list_size; key_list_index++ )
   {
    memset( &key_list[ key_list_index ], 0, sizeof( KeyBinderElement ) );  
   } // ** for 
 }

KeyBinderElement * KeyBinder::findKeyScanCode( unsigned char key_scan_code )
 {
  unsigned long key_list_size;
  unsigned long key_list_index;

  key_list_size = key_list.getSize();

  for( key_list_index = 0; key_list_index < key_list_size; key_list_index++ )
   {
    if( key_list[ key_list_index ].key_scan_code == key_scan_code )
     {
      return( &key_list[ key_list_index ] );
     } // ** if
   } // ** for 

  return( NULL );
 }

KeyBinderElement * KeyBinder::findActionName( char *action_name )
 {
  unsigned long key_list_size;
  unsigned long key_list_index;

  key_list_size = key_list.getSize();

  for( key_list_index = 0; key_list_index < key_list_size; key_list_index++ )
   {
    if( stricmp( key_list[ key_list_index ].action_name, action_name) == 0 )
     {
      return( &key_list[ key_list_index ] );
     } // ** if
   } // ** for 

  return( NULL );
 }

void KeyBinder::staticInitialize( void )
 {
  memset( locked_key_table, 0, sizeof( boolean ) );
 }

void KeyBinder::lockKey( unsigned char key_scan_code )
 {
  assert( key_scan_code < 256 );
  locked_key_table[ key_scan_code ] = _TRUE;
 }

void KeyBinder::unlockKey( unsigned char key_scan_code )
 {
  assert( key_scan_code < 256 );
  locked_key_table[ key_scan_code ] = _FALSE;
 }

void KeyBinder::bindAction( unsigned short action_enum, char *action_name, unsigned char key_scan_code )
 {
  if ( isBound( key_scan_code ) == _TRUE )
   return;

  key_list[ action_enum ].action_enum = action_enum;
  strcpy( key_list[ action_enum ].action_name, action_name );
  key_list[ action_enum ].key_scan_code = key_scan_code;
 }

unsigned char KeyBinder::getActionKey( unsigned short action_enum )
 {
  return( key_list[ action_enum ].key_scan_code );
 }

unsigned char KeyBinder::getActionKey( char *action_name )
 {
  KeyBinderElement *key_info;

  key_info = findActionName( action_name );
  if( key_info != NULL )
   {
    return( key_info->key_scan_code );
   }
 
  return( 0 );
 }

boolean KeyBinder::isBound( unsigned char key_scan_code )
 {
  KeyBinderElement *key_info;

  key_info = findKeyScanCode( key_scan_code );
  if( key_info != NULL )
   {
    return( _TRUE ); 
   }

  return( _FALSE );
 }
   
unsigned short KeyBinder::getKeyAction( unsigned char key_scan_code )
 {
  KeyBinderElement *key_info;

  key_info = findKeyScanCode( key_scan_code );
  if( key_info != NULL )
   {
    return( key_info->action_enum );
   }
  
  return(0);
 }
 
boolean KeyBinder::getKeyAction( unsigned char key_scan_code, char *action_name )
 {
  KeyBinderElement *key_info;

  key_info = findKeyScanCode( key_scan_code );
  if( key_info != NULL )
   {
    strcpy( action_name, key_info->action_name );
    return( _TRUE );
   }
  
  return( _FALSE );
 }
