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
// Original source 03/18/97
//
//---------------------------------------------------------------------------

#ifdef _LOGGING_ON

//***************************************************************************
// includes
//***************************************************************************

#include "stdafx.hpp"

//***************************************************************************
// global data
//***************************************************************************

const CALL_STACK *CALL_STACK::top = NULL;

char     const *__logFile     = NULL;
unsigned        __logLine     = 0;
unsigned        __logDepth    = 0;

//***************************************************************************
// global code
//***************************************************************************

//---------------------------------------------------------------------------
void __log(const char *msg, ...) {
	va_list ap;
	va_start(ap, msg);
	__vlog(NULL, NULL, msg, ap);
	va_end(ap);
}

//---------------------------------------------------------------------------
void __logClose(const char *msg, ...) {
	--__logDepth;
	va_list ap;
	va_start(ap, msg);
	__vlog("} ", NULL, msg, ap);
	va_end(ap);
}

//---------------------------------------------------------------------------
void __logOpen(const char *msg, ...) {
	va_list ap;
	va_start(ap, msg);
	__vlog(NULL, " {", msg, ap);
	va_end(ap);
	++__logDepth;
}

//---------------------------------------------------------------------------
void __vlog(const char *leader, const char *trailer, const char *msg,
	va_list ap) {

	static const char FILENAME[] = "DEBUG.LOG";
	static BOOL initted = FALSE;

	if (!initted) {
		#if COMPILER_MICROSOFT
			_unlink(FILENAME);
    #elif COMPILER_WATCOM
			remove(FILENAME);
		#else
			#error
		#endif
		initted = TRUE;
	}

	FILE *fp = fopen(FILENAME, "a+b");
	if (fp == NULL) return;

	char buf[1024];

	const unsigned TAB_EXPAND = 2;

  //assert(signed(__logDepth) >= 0);
  //assert(signed(__logDepth) <  50);
  size_t len = __logDepth * TAB_EXPAND;
	memsetB(buf, ' ', len);

	if (leader  != NULL) len +=  sprintf(buf + len, leader);
	if (msg     != NULL) len += vsprintf(buf + len, msg, ap);
	if (trailer != NULL) len +=  sprintf(buf + len, trailer);

	if (__logFile != NULL || __logLine > 0 || CALL_STACK::top != NULL) {
		buf[len++] = ' ';
		buf[len++] = ' ';
		buf[len++] = ' ';
		while (len < 60 + __logDepth) {
			buf[len++] = ' ';
		}
		buf[len++] = '[';
		if (CALL_STACK::top != NULL) len += sprintf(buf + len, "%s%s", CALL_STACK::top->strPtr, (__logFile != NULL || __logLine > 0) ? ", " : "");
		if (__logFile       != NULL) len += sprintf(buf + len, "%s", __logFile);
		if (__logLine       >  0   ) len += sprintf(buf + len, "%s%u", (__logFile != NULL) ? ":" : "line ", __logLine);
		buf[len++] = ']';
	}

	buf[len++] = '\r';
	buf[len++] = '\n';

	fwrite(buf, len, 1, fp);
	fflush(fp);
	fclose(fp);
}

//---------------------------------------------------------------------------
void __LOG_PLACEHOLDER::logReturn(const char *file, unsigned line, const char *fmt, ...) {
	__logFile = file;
	__logLine = line;
	__logDepth = savedDepth;
	va_list ap;
	va_start(ap, fmt);
	__vlog("} returning ", NULL, fmt, ap);
	va_end(ap);
	closed = TRUE;
}

//---------------------------------------------------------------------------
void CALL_STACK::setMyBuf(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	safeVsprintf(myBuf, sizeof(myBuf), fmt, ap);
	va_end(ap);
}

//***************************************************************************
#endif // #ifdef _LOGGING_ON
