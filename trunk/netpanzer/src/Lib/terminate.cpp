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

//***************************************************************************
// includes
//***************************************************************************

#include "stdafx.hpp"

#include <process.h>

//***************************************************************************
// constants
//***************************************************************************

const size_t MAX_CLEANUP_FUNCS = 30;

//***************************************************************************
// local types
//***************************************************************************

typedef struct {
	void (*func)();
	BOOL   doEvenOnFubed;
} CLEANUP_FUNC;

//***************************************************************************
// local prototypes
//***************************************************************************

static void atexit_cleanup();

//***************************************************************************
// local data
//***************************************************************************

static CLEANUP_FUNC cleanupFunc[MAX_CLEANUP_FUNCS];
static size_t       numCleanupFuncs = 0;

//***************************************************************************
// global code
//***************************************************************************

//---------------------------------------------------------------------------
void cleanup(BOOL fubed) {
	while (numCleanupFuncs > 0) {
		--numCleanupFuncs;
		if (!fubed || cleanupFunc[numCleanupFuncs].doEvenOnFubed &&
			cleanupFunc[numCleanupFuncs].func) {
			cleanupFunc[numCleanupFuncs].func();
		}
	}
	zeroMem(cleanupFunc, sizeof(cleanupFunc));
}

//---------------------------------------------------------------------------
void term(int returnCode, BOOL fubed, const char *msg, ...) {
	va_list ap;
	va_start(ap, msg);
	vterm(returnCode, fubed, msg, ap);
}

//---------------------------------------------------------------------------
void vterm(int returnCode, BOOL fubed, const char *msg, va_list ap) {
	cleanup(fubed);

	#if COMPILER_WATCOM
	    flushall();
	#endif
//	const CALL_STACK *s = CALL_STACK::top;
//	CALL_STACK::top = NULL;
//
//	__logDepth = 0;
//	__logFile  = NULL;
//	__logLine  = 0;

	if (msg != NULL) {
		static char buf[4096];
		vsprintf(buf, msg, ap);
		#ifdef _LOGGING_ON
			__log("\nFUBAR:\n");
			//CALL_STACK::top = s;
			__log(buf);
			//CALL_STACK::top = NULL;
		#endif

		#if WIN_BUILD
			#ifdef _DEBUG
				if (fubed) {
					static char prompt[sizeof(buf) + 200];
					sprintf(prompt, "%s\n\nPress RETRY to debug the application.", buf);
					if (MessageBox(NULL, prompt, "GTFO!", MB_RETRYCANCEL) == IDRETRY) {
            hardBreak();
					}
				} else
			#endif

			MessageBox(NULL, buf, "GTFO!", MB_OK);

		#else
			fprintf(fubed ? stderr : stdout, "\n%s\n", buf);
			//if (fubed && s != NULL) {
			//	//fprintf(stderr, "\n\nCall stack:\n");
			//	__log("\nCall stack:");
			//	do {
			//		//fprintf(stderr, "  %s  [%s:%u]\n", s->strPtr, s->srcFile, s->srcLine);
			//		__log("  %s  [%s:%u]", s->strPtr, s->srcFile, s->srcLine);
			//		s = s->nextDown;
			//	} while (s != NULL);
			//	fprintf(stderr, "\n");
			//}
		#endif
	}
	exit(returnCode);

}

//---------------------------------------------------------------------------
void quitOK(const char *msg, ...) {
	va_list ap;
	va_start(ap, msg);
	vterm(0, FALSE, msg, ap);
}

//---------------------------------------------------------------------------
void FUBAR(const char *msg, ...) {
	va_list ap;
	va_start(ap, msg);
	vterm(255, TRUE, msg, ap);
}

//---------------------------------------------------------------------------
void vQuitOK(const char *msg, va_list ap) {
	vterm(0, FALSE, msg, ap);
}

//---------------------------------------------------------------------------
void vFUBAR(const char *msg, va_list ap) {
	vterm(255, TRUE, msg, ap);
}

//---------------------------------------------------------------------------
void _addCleanupFunc(void (*func)(), BOOL doEvenOnFubed,
	const char *sourceFile, unsigned lineNumber) {

	FUNCF(("_addCleanupFunc(doEvenOnFubed=%d, sourceFile=%s, lineNumber=%u)", doEvenOnFubed, sourceFile, lineNumber));
	static BOOL initted = FALSE;
	if (!initted) {
		if (atexit(atexit_cleanup))
			FUBAR("Cannot register cleanup function with atexit");
		initted = TRUE;
	}
	if (numCleanupFuncs >= MAX_CLEANUP_FUNCS) {
		FUBAR("Attempted to add more than %u cleanup functions in %s, line %u",
			MAX_CLEANUP_FUNCS, sourceFile, lineNumber);
	}
	cleanupFunc[numCleanupFuncs].doEvenOnFubed = doEvenOnFubed;
	cleanupFunc[numCleanupFuncs].func          = func;
	++numCleanupFuncs;
}

//***************************************************************************
// local code
//***************************************************************************

void atexit_cleanup() {
	cleanup(FALSE);
}
