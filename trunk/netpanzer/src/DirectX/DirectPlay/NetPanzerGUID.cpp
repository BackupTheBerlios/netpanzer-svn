/*
Copyright (C) 2003 Matthias Braun

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

// XXX Matze: I had to move the definition of the GUID outside, because mingw
// gives lots of double defines when using INITGUID in combination with
// dplay.h

#define INITGUID
#include <config.h>
#include <windows.h>
#include <cguid.h>

DEFINE_GUID(NETPANZER_GUID, 
0x2d996661, 0x5c51, 0x11d2, 0x86, 0x6f, 0x0, 0x40, 0x5, 0x16, 0xef, 0x60);
