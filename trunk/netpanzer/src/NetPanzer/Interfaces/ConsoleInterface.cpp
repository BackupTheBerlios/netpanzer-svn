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
#include "ConsoleInterface.hpp"
#include "GameConfig.hpp"

//GameConfig::getConsoleTextColor()


FILE *  ConsoleInterface::con_file;
bool ConsoleInterface::log_file_active;
bool ConsoleInterface::stdout_pipe;

long ConsoleInterface::console_size; 
ConsoleLineArray ConsoleInterface::line_list;

iXY ConsoleInterface::surface_size;
iRect    ConsoleInterface::bounds;
iXY ConsoleInterface::line_offset;
long     ConsoleInterface::vertical_spacing;
long     ConsoleInterface::horizontal_spacing;
long     ConsoleInterface::max_char_per_line;

long ConsoleInterface::line_index; 

bool ConsoleInterface::input_string_active;

int  ConsoleInterface::maxCharCount;
char ConsoleInterface::inputString[256];
char ConsoleInterface::inputPrompt[256];
int  ConsoleInterface::cursorPos;

void ConsoleInterface::initialize( long size )
 {
  assert( size > 0 );

  console_size = size;
  line_list.initialize( size );

  line_index = console_size - 1;

  surface_size = iXY( 640, 480 );
  bounds = iRect( 5, 5, 640 - 5, 480 - 5 );
 
  max_char_per_line = (bounds.max.x - bounds.min.x) / 8;
    
  vertical_spacing = 2; 

  line_offset.x = 0;
  line_offset.y = (8 + vertical_spacing); 

  input_string_active = false;

  long line_loop;

  for ( line_loop = 0; line_loop < console_size; line_loop++ )
   {
    line_list[ line_loop ].color = GameConfig::getConsoleTextColor();
    line_list[ line_loop ].string[0] = 0;
    line_list[ line_loop ].life_timer.changePeriod( 8 );
   }  

  log_file_active = false;
  stdout_pipe = false;
 }

void ConsoleInterface::setToSurfaceSize( iXY pix )
{
  surface_size = pix;
  
  bounds.min.x = 5;
  bounds.min.y = 5;
  bounds.max = pix - 5; 
  
  max_char_per_line = (bounds.max.x - bounds.min.x) / CHAR_XPIX; 
}

void ConsoleInterface::openLogFile( void )
 {
  con_file = fopen( "console.log", "wt" );
  log_file_active = true;
 }

void ConsoleInterface::closeLogFile( void )
 {
  fclose( con_file );
  log_file_active = false;
 }

void ConsoleInterface::setStdoutPipe( bool on_off )
 {
  stdout_pipe = on_off;
 }

void ConsoleInterface::update( Surface &surface )
 {
  update_overlap( surface );
 }

void ConsoleInterface::postMessage( PIX color, const char *format, ... ) 
 {
  char temp_str[256];
  char format_str[256];
  char *temp_str_ptr;
  long temp_str_length;
  va_list vap;

  va_start( vap, format );
  vsprintf( temp_str, format, vap );
  va_end( vap );

  va_start( vap, format );
  if ( stdout_pipe == true )
   {
    strcpy( format_str, format );
    strcat( format_str, "\n" ); 
    vprintf( format_str, vap );
    fflush(stdout);
   }    
  va_end( vap );
   
  if( log_file_active == true )
   { 
    fprintf( con_file, temp_str );
    fprintf( con_file, "\n");
   }

   
   temp_str_ptr = temp_str;
   temp_str_length = (long) strlen(temp_str);
   
   if( temp_str_length > max_char_per_line )
    {
     long partion_count = temp_str_length / max_char_per_line;
       
     for( int i = 0; i < partion_count; i++ )
      {
       if (line_index == 0)
        line_index = console_size - 1;
       else
        line_index = (line_index - 1) % console_size; 

       strncpy( line_list[ line_index ].string, temp_str_ptr, max_char_per_line);
       line_list[ line_index ].string[ max_char_per_line ] = 0; 

       line_list[ line_index ].color = color;
       line_list[ line_index ].visible = true;
       line_list[ line_index ].life_timer.reset(); 
      
       temp_str_ptr = temp_str_ptr + max_char_per_line;
      }
   
    }
   
   if (line_index == 0)
    line_index = console_size - 1;
   else
    line_index = (line_index - 1) % console_size; 

   strcpy( line_list[ line_index ].string, temp_str_ptr );   

   line_list[ line_index ].color = color;
   line_list[ line_index ].visible = true;
   line_list[ line_index ].life_timer.reset(); 
 }

void ConsoleInterface::postMessage( const char *format, ... ) 
 {
  char temp_str[256];
  char format_str[256];
  char *temp_str_ptr;
  long temp_str_length;
  va_list vap;

  va_start( vap, format );
  vsprintf( temp_str, format, vap );
  va_end( vap );
   
  va_start( vap, format );
  if ( stdout_pipe == true )
   {
    strcpy( format_str, format );
    strcat( format_str, "\n" ); 
    vprintf( format_str, vap );
   }    
  va_end( vap );

  if( log_file_active == true )
   { 
    fprintf( con_file, temp_str );
    fprintf( con_file, "\n");
   }
   
  temp_str_ptr = temp_str;
  temp_str_length = (long) strlen(temp_str);
   
   if( temp_str_length > max_char_per_line )
    {
     long partion_count = temp_str_length / max_char_per_line;
       
     for( int i = 0; i < partion_count; i++ )
      {
       if (line_index == 0)
        line_index = console_size - 1;
       else
        line_index = (line_index - 1) % console_size; 

       strncpy( line_list[ line_index ].string, temp_str_ptr, max_char_per_line);
       line_list[ line_index ].string[ max_char_per_line ] = 0; 

       line_list[ line_index ].color = GameConfig::getConsoleTextColor();
       line_list[ line_index ].visible = true;
       line_list[ line_index ].life_timer.reset(); 
      
       temp_str_ptr = temp_str_ptr + max_char_per_line;
      }
   
    }
   
   if (line_index == 0)
    line_index = console_size - 1;
   else
    line_index = (line_index - 1) % console_size; 

   strcpy( line_list[ line_index ].string, temp_str_ptr );   

   line_list[ line_index ].color = GameConfig::getConsoleTextColor();
   line_list[ line_index ].visible = true;
   line_list[ line_index ].life_timer.reset(); 

 }


 
void ConsoleInterface::update_overlap( Surface &surface )
 {
  iXY current_line;
  long  index; 
  short visible_count;
  
  index = line_index; 

  visible_count = 0;
  
  do
  {
   if ( line_list[ index ].life_timer.count() )
    {
     line_list[ index ].visible = 0;
    }

   if ( line_list[ index ].visible )
    visible_count++;
    
   index = (index + 1) % console_size;

  } while( index != line_index );
 
  if( input_string_active == true )
   {  
    current_line.y = bounds.min.y + (line_offset.y * visible_count ) + (line_offset.y * 2);
    current_line.x = bounds.min.x + line_offset.x;
   }
  else
   {
    current_line.y = bounds.min.y + (line_offset.y * visible_count );
    current_line.x = bounds.min.x + line_offset.x;
   }

  do
  {
   if ( line_list[ index ].visible )
    { 
     surface.bltStringShadowed(current_line, line_list[ index ].string, line_list[ index ].color, Color::black );
 
     current_line.y = current_line.y - line_offset.y;
    }
   
   index = (index + 1) % console_size;

  } while( index != line_index );
 
  
  if( input_string_active == true )
   {  
    iXY input_offset;
    unsigned long max_char_space;
    unsigned long input_string_length;
    char *string_ptr;

    current_line.y = current_line.y - line_offset.y;

    surface.bltStringShadowed(current_line, inputPrompt, GameConfig::getConsoleTextColor(), Color::black );
    
    input_offset.x = current_line.x + ( (long) strlen( inputPrompt ) ) * CHAR_XPIX;
    input_offset.y = current_line.y;

    max_char_space = (bounds.max.x - input_offset.x ) / CHAR_XPIX;
    input_string_length = strlen(inputString); 
    
    if( input_string_length > max_char_space )
     {
      string_ptr = inputString + (input_string_length - (max_char_space - 1) );
     }
    else
     { string_ptr = inputString; }
      
    surface.bltStringShadowed(input_offset, string_ptr , GameConfig::getConsoleTextColor(), Color::black );
    
    surface.bltStringShadowed(input_offset.x + cursorPos * CHAR_XPIX, input_offset.y, "_", GameConfig::getConsoleTextColor(), Color::black );
   }
 }

void ConsoleInterface::setInputStringStatus( bool on_off )
 {
  input_string_active = on_off;
 }

void ConsoleInterface::resetInputString( char *prompt )
 {
  cursorPos = 0;
  inputString[0] = 0;   
  maxCharCount = 256;
  strcpy( inputPrompt, prompt );
 }

void ConsoleInterface::getInputString( char *string )
 {
  strcpy( string, inputString );
 }

void ConsoleInterface::addChar(int newChar)
{
	// Check if the character should be excluded.
	// Add the character.
	int length = strlen(inputString) + 1;
		
	inputString[cursorPos] = newChar;

	inputString[length] = '\0';
		
	if (cursorPos < maxCharCount - 1)
	{
		cursorPos++;
	} 
} // addChar

void ConsoleInterface::addExtendedChar(int newExtendedChar)
{
	// Process the extendedChar accordingly.
	switch (newExtendedChar)
	{
		case SDLK_RETURN:
		{
		} break;
		
		case SDLK_HOME:
		{
			cursorPos = 0;
		} break;
		
		case SDLK_LEFT:
		{
			if(--cursorPos < 0)
			{
				cursorPos = 0;
			}
		} break;
		
		case SDLK_RIGHT:
		{
			int length = strlen(inputString);
			if(++cursorPos > length)
			{
				if (cursorPos > maxCharCount)
				{
					cursorPos = maxCharCount - 1;
				} else
				{
					cursorPos = length;
				}
			}
		} break;
		
		case SDLK_END:
		{
			cursorPos = strlen(inputString);

			if (cursorPos > maxCharCount)
			{
				cursorPos = maxCharCount - 1;
			}
		} break;
		
		case SDLK_INSERT:
		{
		} break;

		case SDLK_DELETE:
		{
			if (cursorPos == (int) strlen(inputString + cursorPos)) { break; }

			memcpy(inputString + cursorPos, inputString + cursorPos + 1, strlen(inputString + cursorPos + 1) + 1);
		} break;
		
		case SDLK_BACKSPACE:
		{
			if (cursorPos >= 1)
			{
				int byteCount = strlen(inputString + cursorPos);
				
				// Only do this if we are not at the end of the string.
				if (byteCount > 0)
				{
					memcpy(inputString + cursorPos - 1, inputString + cursorPos, byteCount);
				}

				cursorPos--;
				inputString[cursorPos + byteCount] = '\0';
			}
			
		} break;

		default: break;
	}

} // addExtendedChar
