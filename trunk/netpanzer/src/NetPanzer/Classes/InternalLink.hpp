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
#ifndef _INTERNALLINK_HPP
#define _INTERNALLINK_HPP

#include "PObject.hpp"
#include "Stdlib.h"

class InternalLink : public PObject
 {
  public:

  inline void addToList( InternalLink  **start, InternalLink  **end )
   {
    next_ptr = NULL;
    prev_ptr = NULL;

    if(*start == NULL)
     {
      *start = this;
      *end   = this;
     }
    else
     {
      (*end)->next_ptr = this;
      prev_ptr = *end;
      *end = this;
     }

    }


  inline void removeFromList( InternalLink  **start, InternalLink  **end )
   {
    if ( (this == *start) && (this == *end) )
     {
      *start = NULL;
      *end = NULL;
     } 
    else
     if(this == *start)
      {
       *start = next_ptr;
       next_ptr->prev_ptr = NULL;
      }
     else  
      if(this == *end)
       {
        *end = prev_ptr;
        prev_ptr->next_ptr = NULL;
       } 
      else
       {
        prev_ptr->next_ptr = next_ptr;
        next_ptr->prev_ptr = prev_ptr;
       }
   }



  InternalLink *next_ptr, *prev_ptr;

 };

#endif // ** _INTERNALLINK_HPP
