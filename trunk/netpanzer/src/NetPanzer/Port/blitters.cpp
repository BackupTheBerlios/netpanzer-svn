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
#include "blitters.h"
#include "DirectDrawGlobals.hpp"

void blit_partial_xy( unsigned char *tile_ptr,unsigned char *buffer_ptr,short y_size,short x_size)
{
	assert(isValidPtr(tile_ptr));
	assert(isValidPtr(buffer_ptr));
	assert(y_size > 0);
	assert(x_size > 0);

	// XXX disabled msvc assembler
#ifdef MSVC
  __asm
 {
                        mov  esi,tile_ptr;
                        mov  edi,buffer_ptr;
                        mov  ebx,0
                        mov  ecx,0
                        mov  eax,0
                        mov  bx,y_size;
                        mov  cx,x_size;

                        mov  dl,bl
                        mov  ebx,32
                        sub  ebx,ecx   ;compute next line offset in tile
                        mov  eax,DBUFFER_WIDTH
                        sub  eax,ecx   ;compute next line in d_buffer

                        shr  cx,2
                        mov  dh,cl     ;set x length storage

copy_loop:              mov  cl,dh
                        rep  movsd
                        add  esi,ebx
                        add  edi,eax
                        dec  dl
                        jnz  copy_loop


  } // end asm block
#endif

}


/*
void blit_partial_y( unsigned char *tile_ptr, unsigned char *buffer_ptr, short y_size)
{
  __asm
 { 
                        mov  esi,tile_ptr;
                        mov  edi,buffer_ptr;
                        mov  ebx,0
                        mov  ecx,0
                        mov  eax,0
                        mov  bx,y_size;
        
                        mov  dl,bl
                        mov  dh,8                ;set x length storage
       
                        mov  eax,DBUFFER_WIDTH
                        sub  eax,32              ;compute next line in d_buffer


  copy_loop2:           mov  cl,dh
                        rep  movsd
                        add  edi,eax
                        dec  dl
                        jnz  copy_loop2

 }

}
*/


void blit_partial_y( unsigned char *tile_ptr, unsigned char *buffer_ptr, short y_size)
{
	// XXX disabled msvc assembler
#ifdef MSVC
  __asm
 { 
                        mov  esi,tile_ptr;
                        mov  edi,buffer_ptr;
                        mov  ebx,0
                        mov  ecx,0
                        mov  eax,0
                        mov  bx,y_size;
        
                        mov  dl,bl
                        mov  dh,8                ;set x length storage
       
                        mov  eax,DBUFFER_WIDTH
                        sub  eax,32              ;compute next line in d_buffer


  copy_loop2:           
					   ;mov  cl,dh
                        mov  ebx,[esi]	  ;1
                        add  esi,4
						mov  [edi],ebx
						add  edi,4						          						

                        mov  ebx,[esi]	  ;2
						add  esi,4
						mov  [edi],ebx
                        add  edi,4						          						
						  
                        mov  ebx,[esi]	  ;3
						add  esi,4
                        mov  [edi],ebx
                        add  edi,4						          						

                        mov  ebx,[esi]	  ;4
                        add  esi,4
						mov  [edi],ebx
						add  edi,4						          						

                        mov  ebx,[esi]	  ;5
                        add  esi,4
						mov  [edi],ebx
						add  edi,4						          						
                        
                        mov  ebx,[esi]	  ;6
                        add  esi,4
						mov  [edi],ebx
						add  edi,4						          						
						
                        mov  ebx,[esi]	  ;7
                        add  esi,4
						mov  [edi],ebx
						add  edi,4						          						
						
                        mov  ebx,[esi]	  ;8
                        add  esi,4
						mov  [edi],ebx
						add  edi,4						          						

						add  edi,eax	   
                        dec  dl
                        jnz  copy_loop2

 }
#endif

}

void general_blitter( unsigned char x_size, unsigned char y_size, unsigned long frame_offset, 
                      unsigned char *buffer_ptr, unsigned char *dbuffer_ptr )
 {

	 // XXX disabled msvc assembler
#ifdef MSVC
  __asm
  {
                        mov  ebx,0
                        mov  eax,0
                        mov  ecx,0
                        mov  edx,0
                        mov  bl,x_size
                        mov  dl,y_size
                        mov  eax,frame_offset
                        mov  esi,buffer_ptr
                        mov  edi,dbuffer_ptr
                                                 
                        add  edi,eax

                        mov  eax,DBUFFER_WIDTH
                        sub  ecx,ecx
                        mov  cl,bl
                        sub  eax,ecx

                        mov  bh,dl
row_loop_gb:
                        mov  cl,bl

column_loop_gb:         mov  dl,[esi]

                        cmp  dl,0
                        jz   transparent_gb
                        mov  [edi],dl

transparent_gb:         add  edi,1
                        add  esi,1

                        dec  cl
                        jnz  column_loop_gb


                        add  edi,eax

                        dec  bh
                        jnz  row_loop_gb
  }
#endif
 
 }
                                         

void blit_selector_square( unsigned char x_size, unsigned char y_size,
                           unsigned long frame_offset, unsigned char *dbuffer_ptr )
 {
	 // XXX disabled msvc assembler
#ifdef MSVC
  __asm
   {
                        mov  ebx,0
                        mov  eax,0
                        mov  ecx,0
                        mov  edx,0
                        mov  al,x_size
                        mov  cl,y_size
                        mov  edx,frame_offset
                        mov  edi,dbuffer_ptr
                                                
                        add edi,edx     ;add frame offset to dbuffer

                        mov bh,al       ; eax -> bh   x_size
                        mov bl,cl       ; ecx -> bl   y_size

                        mov edx,DBUFFER_WIDTH
                        sub edx,eax

                        mov ecx,0

                        mov al,0x47
                        mov cl,bh 
                        rep stosb

                        add edi,edx

                        mov cl,bl
                        sub cl,2

                        mov ch,0x47

                        sub eax,eax
                        mov al,bh
                        sub al,1
                        
selector_loop:          mov [edi],ch
                        add edi,eax

                        mov [edi],ch
                        inc edi
                        add edi,edx

                        dec cl
                        jnz selector_loop

                        mov al,0x47
                        sub ecx,ecx
                        mov cl,bh
                        rep stosb

   
   
   }  
#endif

 }

