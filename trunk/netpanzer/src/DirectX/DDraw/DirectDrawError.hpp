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


#ifndef __ERROR_H__
#define __ERROR_H__

#include "stdafx.hpp"


#ifdef __cplusplus
extern "C" {
#endif

/* Msg
 * Displays a message box containing the given formatted string.
 */
void __cdecl
Msg( LPSTR fmt, ... );

/*
 * D3DRMErrorToString
 * Returns a pointer to a string describing the given DD, D3D or D3DRM error code.
 */
char*
D3DRMErrorToString(HRESULT error);

void DDErrorBox( char *location, HRESULT error );


#ifdef __cplusplus
};
#endif
#endif // __ERROR_H__
