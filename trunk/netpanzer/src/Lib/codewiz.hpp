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

#ifndef __CODEWIZ_H_INCLUDED__
#define __CODEWIZ_H_INCLUDED__

#include "Util.hpp"
#include "Types/LibTypes.hpp"

//***************************************************************************
// favorite consts
//***************************************************************************
const bool GOTTA_HAVE_IT   = true;
const bool LIVE_WITHOUT_IT = false;

//***************************************************************************
// Standard program control/cleanup functions
//***************************************************************************

#include <stdarg.h>

void cleanup(bool fubed);
void term(int returnCode, bool fubed, const char *msg, ...);
void vterm(int returnCode, bool fubed, const char *msg, va_list ap);
void quitOK(const char *msg, ...);
void FUBAR(const char *msg, ...);
void vQuitOK(const char *msg, va_list ap);
void vFUBAR(const char *msg, va_list ap);

// XXX changed this, as it was stupid code (on unix this failed from time to
// time
inline bool isValidPtr(const void *ptr) {
	//return (ptr > 0) && (ptr < (void *)0x90000000);
	return ptr != 0;
}

//***************************************************************************
#endif // #ifndef __CODEWIZ_H_INCLUDED__
