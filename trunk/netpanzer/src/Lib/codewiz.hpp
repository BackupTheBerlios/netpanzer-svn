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

//***************************************************************************
// includes
//***************************************************************************

#include <assert.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <float.h>
#include <limits.h>

#include "Types/LibTypes.hpp"

//***************************************************************************
// favorite consts
//***************************************************************************
const bool GOTTA_HAVE_IT   = true;
const bool LIVE_WITHOUT_IT = false;

//***************************************************************************
// Persional favorite logging macros
//***************************************************************************

#define DONOTHING ((void)0)

#ifdef _LOGGING_ON

	extern char     const *__logFile;
	extern unsigned        __logLine;
	extern unsigned        __logDepth;

	extern void __log(const char *msg, ...);
	extern void __vlog(const char *leader, const char *trailer, const char *msg, va_list ap);
	extern void __logClose(const char *msg, ...);
	extern void __logOpen(const char *msg, ...);

	class CALL_STACK {
	public:

		static const CALL_STACK *top;

		char              myBuf[200];     // If using a formatted buffer, this holds the formatted string
		char       const *const srcFile;  // filename of the function
		unsigned          const srcLine;  // line number of the function (approx)
		char       const *const strPtr;   // pointer to the description.
		CALL_STACK const *const nextDown; // pointer up the call stack

		// Create a call stack
		inline CALL_STACK(const char *nSrcFile, unsigned nSrcLine,
			const char *nStrPtr) :
			srcFile (nSrcFile),
			srcLine (nSrcLine),
			strPtr  (nStrPtr ),
			nextDown(top     )
		{
			top = this;
		}

		inline CALL_STACK(const char *nSrcFile, unsigned nSrcLine) :
			srcFile (nSrcFile),
			srcLine (nSrcLine),
			strPtr  (myBuf   ),
			nextDown(top     )
		{
			myBuf[0] = '\0';
			top = this;
		}

		inline ~CALL_STACK() {
			// pop this guy off the stack
			top = nextDown;
		}

		// For formatted buffers
		void setMyBuf(const char *fmt, ...);
	};


	class __LOG_PLACEHOLDER {
	private:
		unsigned savedDepth;
		bool     closed;

	public:
		// Create a placeholder for log depth info, so log depth will
		// automatically be restored
		inline __LOG_PLACEHOLDER() {
			savedDepth = __logDepth;
			closed     = false;
		}

		// Restore the log depth
		inline ~__LOG_PLACEHOLDER() {
			__logFile  = 0;
			__logLine  = 0;
			__logDepth = savedDepth;
			if (!closed) __log("}");
		}

		// Log a return value from a function
		void logReturn(const char *file, unsigned line, const char *fmt, ...);
	};

	// insert function into call stack info
	#define FUNC(funcName) CALL_STACK __callStack(__FILE__, __LINE__, funcName)
	#define FUNCF(x)       CALL_STACK __callStack(__FILE__, __LINE__); __callStack.setMyBuf x

	#define LOG(x)             (__logFile = __FILE__, __logLine = __LINE__, (__log x))
	#define LOG_FUNC(funcName) FUNC(funcName); __LOG_PLACEHOLDER __func_placeHolder; (__logFile = __FILE__, __logLine = __LINE__, __logOpen(funcName))
	#define LOG_FUNCF(x)       FUNCF(x); __LOG_PLACEHOLDER __func_placeHolder; (__logFile = __FILE__, __logLine = __LINE__, __logOpen x)
	#define LOG_BLOCK(x)       __LOG_PLACEHOLDER __block_placeHolder; (__logFile = __FILE__, __logLine = __LINE__, (__logOpen x))
	#define RETURN(x);         { __func_placeHolder.logReturn(__FILE__, __LINE__, #x ); return(x); }
	#define RETURNF(x, fmt);   { __func_placeHolder.logReturn(__FILE__, __LINE__,  #x " = " fmt, (x)); return(x); }

#else
	#define FUNCF(x)           DONOTHING
    #define FUNC(funcName)     DONOTHING
    #define LOG(x)             DONOTHING
	#define LOG_BLOCK(s)       DONOTHING
	#define LOG_FUNC(funcName) DONOTHING
	#define RETURN(x)          return(x)
	#define RETURNF(x, fmt)    return(x)
#endif

//***************************************************************************
// Standard program control/cleanup functions
//***************************************************************************

void cleanup(bool fubed);
void term(int returnCode, bool fubed, const char *msg, ...);
void vterm(int returnCode, bool fubed, const char *msg, va_list ap);
void quitOK(const char *msg, ...);
void FUBAR(const char *msg, ...);
void vQuitOK(const char *msg, va_list ap);
void vFUBAR(const char *msg, va_list ap);
void _addCleanupFunc(void (*func)(), bool doEvenOnFubed,
	const char *sourceFile, unsigned lineNum);

#define addCleanupFunc(f, d) _addCleanupFunc((f), (d), __FILE__, __LINE__)

template <class T> inline T align4(T x) {
	return (x + T(3)) & T(~3U);
}

template <class T, class C> inline T rol(T a, C count) {
	return T((DWORD(a) << unsigned(count)) | (DWORD(a) >> unsigned(sizeof(T)*8 - count)));
}

template <class T, class C> inline T ror(T a, C count) {
	return T((DWORD(a) >> unsigned(count)) | (DWORD(a) << unsigned(sizeof(T)*8 - count)));
}

inline bool isValidPtr(const void *ptr) {
	return (ptr > 0) && (ptr < (void *)0x90000000);
}

template <class T> inline T squared(T a) { return a*a; }

//***************************************************************************
// Mathmatical constants and conversions
//***************************************************************************

#define _E           (2.71828182845904523536)

#ifndef PI
#define PI          (3.14159265358979323846)
#endif 

#define RAD_PER_DEG (PI / 180.0)
#define DEG_PER_RAD (180.0 / PI)

inline float RAD2DEG(float radians) { return radians * DEG_PER_RAD; }
inline float DEG2RAD(float radians) { return radians * RAD_PER_DEG; }

//***************************************************************************
// Other standard prototypes
//***************************************************************************

void trimLeadingWhitespace (char *str);
void trimTrailingWhitespace(char *str);
void concatenateWhitespace (char *str);

void swapMem(void *buf1, void *buf2, size_t numBytes);

void reverseBytes(void *buf, size_t numBytes);   //    A    B    C    D <->    D    C    B    A
void reverseWords(void *buf, size_t numWords);   //   AB   CD   EF   GH <->   GH   EF   CD   AB
void reverseDWords(void *buf, size_t numDWords); // ABCD EFGH IJKL MNOP <-> MNOP IJKL EFGH ABCD

//***************************************************************************
#endif // #ifndef __CODEWIZ_H_INCLUDED__
