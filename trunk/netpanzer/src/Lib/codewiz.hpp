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

#if _MSC_VER > 1000
	#pragma once
#endif

#ifndef __cplusplus
	#error This file requiers c++ Compilation
#endif

//***************************************************************************
// includes
//***************************************************************************

#include <assert.h>

#if !defined( __STDDEF_H ) && !defined( _STDDEF_H_INCLUDED ) && !defined( _INC_STDDEF )
	#include <stddef.h>
#endif

#if !defined( __STDIO_H ) && !defined( _STDIO_H_INCLUDED ) && !defined( _INC_STDIO )
	#include <stdio.h>
#endif

#if !defined( __STDLIB_H ) && !defined( _STDLIB_H_INCLUDED ) && !defined( _INC_STDLIB )
	#include <stdlib.h>
#endif

#if !defined( __STDARG_H ) && !defined( _STDARG_H_INCLUDED ) && !defined( _INC_STDARG )
	#include <stdarg.h>
#endif

#if !defined( __STRING_H ) && !defined( _STRING_H_INCLUDED ) && !defined( _INC_STRING )
	#include <string.h>
#endif

#if !defined( __MATH_H ) && !defined( _MATH_H_INCLUDED ) && !defined( _INC_MATH )
	#include <math.h>
#endif

#if !defined( __NEW_H ) && !defined ( _NEW_H_INCLUDED ) && !defined( _INC_NEW )
	#include <new.h>
#endif

#if !defined( __IOSTREAM_H ) && !defined ( _IOSTREAM_H_INCLUDED ) && !defined( _INC_IOSTREAM )
	#include <iostream.h>
#endif

#if !defined( __FSTREAM_H ) && !defined ( _FSTREAM_H_INCLUDED ) && !defined( _INC_FSTREAM )
	#include <fstream.h>
#endif

#if !defined( __CTYPE_H ) && !defined ( _CTYPE_H_INCLUDED ) && !defined( _INC_CTYPE )
	#include <ctype.h>
#endif

#if !defined( __FLOAT_H ) && !defined ( _FLOAT_H_INCLUDED ) && !defined( _INC_FLOAT )
	#include <float.h>
#endif

#if !defined( __LIMITS_H ) && !defined ( _LIMITS_H_INCLUDED ) && !defined( _INC_LIMITS )
	#include <limits.h>
#endif

#include <io.h>


//***************************************************************************
// detect build
//***************************************************************************

#if defined( WIN32 ) || defined( _WIN32 )
	#define DOS_BUILD 0
	#define WIN_BUILD 1
#else
	#define DOS_BUILD 1
	#define WIN_BUILD 0
#endif

//***************************************************************************
// detect compiler
//***************************************************************************

#ifdef _MSC_VER
	#define COMPILER_MICROSOFT 1
#else
	#define COMPILER_MICROSOFT 0
#endif

#ifdef __WATCOMC__
	#define COMPILER_WATCOM 1
#else
	#define COMPILER_WATCOM 0
#endif

//FIXME - figure out how to detect
#define COMPILER_GNU 0

#if (COMPILER_MICROSOFT + COMPILER_WATCOM + COMPILER_GNU) != 1
	#error Can't detect compiler
#endif

//***************************************************************************
// compiler specific options
//***************************************************************************

#if COMPILER_MICROSOFT
	#pragma warning(disable: 4244) // conversion from '???' to '???', possible loss of data
	#pragma warning(disable: 4514) // unreferenced inline function has been removed

	#define ASMPROC __fastcall
#endif

#if COMPILER_WATCOM
	#pragma warning 604 9; // must look ahead to determine whether construct is a declaration/type or an expression
	#pragma warning 594 9; // construct resolved as a declaration/type
	#pragma warning 849 9; // base class virtual function has not been overridden


	#pragma aux _ASMPROC "*" parm [ecx][edx] routine modify exact [eax ecx edx] value [eax];

	#define ASMPROC

#endif

//***************************************************************************
// types
//***************************************************************************

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef int            BOOL;

//***************************************************************************
// favorite consts
//***************************************************************************

#ifndef TRUE
	#define TRUE  (1==1)
#endif

#ifndef FALSE
	#define FALSE (1!=1)
#endif

const BOOL NOPROB = TRUE;
const BOOL YIKES  = FALSE;

const BOOL GOTTA_HAVE_IT   = TRUE;
const BOOL LIVE_WITHOUT_IT = FALSE;

const DWORD KILO = (1UL << 10);
const DWORD MEGA = (1UL << 20);
const DWORD GIGA = (1UL << 30);

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
		BOOL     closed;

	public:
		// Create a placeholder for log depth info, so log depth will
		// automatically be restored
		inline __LOG_PLACEHOLDER() {
			savedDepth = __logDepth;
			closed     = FALSE;
		}

		// Restore the log depth
		inline ~__LOG_PLACEHOLDER() {
			__logFile  = NULL;
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
// Dos extender issues (mostly curtesy of Paul Nettle)
//***************************************************************************
#if DOS_BUILD

	//#if !defined( __I86_H ) && !defined ( _I86_H_INCLUDED ) && !defined( _INC_I86 )
		//#include <i86.h>
	//#endif

	#if COMPILER_GNU
		#define CONVENTIONAL_BASE	__djgpp_conventional_base
		#define DOS_MEM_START()		(__djgpp_nearptr_enable() ? DONOTHING : FUBAR("Unable to enable nearptr access") )
		#define DOS_MEM_END()		  __djgpp_nearptr_disable()
	#else
		#define CONVENTIONAL_BASE	0UL
		#define DOS_MEM_START()		DONOTHING
		#define DOS_MEM_END()		  DONOTHING
	#endif

	#define MK_REAL(s, o)     (void *)((DWORD(WORD(s)) << 16) + WORD(o))
	#define MK_PROT(s, o)     (void *)((DWORD(WORD(s)) << 4) + WORD(o) + CONVENTIONAL_BASE)
	#define REAL_SEG(ptr)     WORD(DWORD(ptr) >> 16)
	#define REAL_OFF(ptr)     WORD(DWORD(ptr))
	#define PROT_SEG(ptr)     WORD((DWORD(ptr) - CONVENTIONAL_BASE) >> 4)
	#define PROT_OFF(ptr)     WORD((DWORD(ptr) - CONVENTIONAL_BASE) & 0xf)
	#define REAL_TO_PROT(ptr) MK_PROT(REAL_SEG(ptr), REAL_OFF(ptr))
	#define PROT_TO_REAL(ptr) MK_REAL(PROT_SEG(ptr), PROT_OFF(ptr))

	void realModeInt86X(BYTE intNo, const REGS *inRegs, REGS *outRegs, SREGS *segRegs);

	inline void realModeInt86(BYTE intNo, const REGS *inRegs, REGS *outRegs) {
  	realModeInt86X(intNo, inRegs, outRegs, NULL);
	}

	void *mapPhysicalToLinear(DWORD physicalAddress, DWORD sizeLimit);

#endif // #if DOS_BUILD

//***************************************************************************
// My version of the assert macro
//***************************************************************************

#if COMPILER_WATCOM
	extern "C" void hardBreak(void);
	#pragma aux hardBreak = "int 03";
#endif

#if COMPILER_BORLAND
  #define hardBreak() __emit__(0xcc)
#endif

#if COMPILER_MICROSOFT
  inline void hardBreak() { __asm int 3 }
#endif

/* OLD ASSERT
#undef assert
#ifdef _DEBUG
	#define Assert(x) ((x) ? ((void)0) : (void)(hardBreak(), FUBAR("\n\nAssert failed: %s, file %s, line %u\n\n", #x, __FILE__, __LINE__) ))
	#define Verify(x) ((x) ? ((void)0) : (void)(hardBreak(), FUBAR("\n\nVerify failed: %s, file %s, line %u\n\n", #x, __FILE__, __LINE__) ))
#else
	#define Assert(x) ((void)0)
	#define Verify(x) ((void)(x))
#endif

#define assert Assert
*/


#undef assert
#ifdef _DEBUG
	#define assert(x) ((x) ? ((void)0) : (void)( ( LOG(("assert failed: %s, file %s, line %u", #x, __FILE__, __LINE__ )) ), (_assert(#x, __FILE__, __LINE__), 0) ) )
#else
	#define assert(x) ((void)0)
#endif

//***************************************************************************
// Assembler routines and data
//***************************************************************************

extern "C" ASMPROC zeroMemAsm(void *dest, size_t numBytes);
extern "C" ASMPROC memsetBAsm(void *dest, BYTE  fillVal, size_t numBytes );
extern "C" ASMPROC memsetWAsm(void *dest, WORD  fillVal, size_t numWords );
extern "C" ASMPROC memset3Asm(void *dest, DWORD fillVal, size_t numTrips );
extern "C" ASMPROC memsetDAsm(void *dest, DWORD fillVal, size_t numDWords);

union BYTE_FILL {
	BYTE   byte;
	WORD   word;
	DWORD  dword;
	double qword;
};

extern "C" BYTE_FILL byteFill[0x100];

#if COMPILER_WATCOM
	#pragma aux (_ASMPROC) zeroMemAsm;
#endif

//***************************************************************************
// Standard program control/cleanup functions
//***************************************************************************

void cleanup(BOOL fubed);
void term(int returnCode, BOOL fubed, const char *msg, ...);
void vterm(int returnCode, BOOL fubed, const char *msg, va_list ap);
void quitOK(const char *msg, ...);
void FUBAR(const char *msg, ...);
void vQuitOK(const char *msg, va_list ap);
void vFUBAR(const char *msg, va_list ap);
void _addCleanupFunc(void (*func)(), BOOL doEvenOnFubed,
	const char *sourceFile, unsigned lineNum);

#define addCleanupFunc(f, d) _addCleanupFunc((f), (d), __FILE__, __LINE__)

#if COMPILER_WATCOM
	#pragma aux term    aborts;
	#pragma aux vterm   aborts;
	#pragma aux quitOK  aborts;
	#pragma aux FUBAR   aborts;
	#pragma aux vQuitOK aborts;
	#pragma aux vFUBAR  aborts;
#endif

const size_t __MAX_FILL_SIZE = 5000000;

inline void zeroMem(void *dest, size_t numBytes) {
	#ifdef _DEBUG
		if (numBytes <= 0) return;
		assert(dest != NULL);
		assert(numBytes < __MAX_FILL_SIZE/sizeof(BYTE));
	#endif
	//FIXME - get assembler version working
	memset(dest, 0, numBytes);
}

inline void memsetB(void *dest, BYTE fillVal, size_t numBytes) {
	#ifdef _DEBUG
		if (numBytes <= 0) return;
		assert(dest != NULL);
		assert(numBytes < __MAX_FILL_SIZE/sizeof(BYTE));
	#endif
	//FIXME - supply assembler version
	memset(dest, fillVal, numBytes);
}

inline void memsetW(void *dest, WORD fillVal, size_t numWords) {
	#ifdef _DEBUG
		if (numWords <= 0) return;
		assert(dest != NULL);
		assert(numWords < __MAX_FILL_SIZE/sizeof(WORD));
	#endif
	memsetWAsm(dest, fillVal, numWords);
}

inline void memset3(void *dest, DWORD fillVal, size_t numTrips) {
	#ifdef _DEBUG
		if (numTrips <= 0) return;
		assert(dest != NULL);
		assert(numTrips < __MAX_FILL_SIZE/3);
	#endif
	memset3Asm(dest, fillVal, numTrips);
}

inline void memsetD(void *dest, DWORD fillVal, size_t numDWords) {
	#ifdef _DEBUG
		if (numDWords <= 0) return;
		assert(dest != NULL);
		assert(numDWords < __MAX_FILL_SIZE/sizeof(DWORD));
	#endif
	memsetDAsm(dest, fillVal, numDWords);
}

inline void memcpyB(void *dest, const void *src, size_t numBytes) {
	#ifdef _DEBUG
		if (numBytes <= 0) return;
		assert(dest != NULL);
		assert(src  != NULL);
	#endif
	//FIXME - supply assembler version
	memcpy(dest, src, numBytes);
}

inline void memcpyW(void *dest, const void *src, size_t numWords) {
	#ifdef _DEBUG
		if (numWords <= 0) return;
		assert(dest != NULL);
		assert(src  != NULL);
	#endif
	//FIXME - supply assembler version
	memcpy(dest, src, numWords * sizeof(WORD));
}

inline void memcpyD(void *dest, const void *src, size_t numDWords) {
	#ifdef _DEBUG
		if (numDWords <= 0) return;
		assert(dest != NULL);
		assert(src  != NULL);
	#endif
	//FIXME - supply assembler version
	memcpy(dest, src, numDWords * sizeof(DWORD));
}

template <class T> inline void zeroStruct(T &a) {
	assert(&a != NULL);
	//FIXME - figure out why this doesn't work!
	//zeroMemAsm(&a, sizeof(a));
	memset(&a, 0, sizeof(a));
}

//#define memset HEY_USE_memsetB
//#define memcpy HEY_USE_memcpyB

template <class T> inline T align4(T x) {
	return (x + T(3)) & T(~3U);
}

template <class T, class C> inline T rol(T a, C count) {
	return T((DWORD(a) << unsigned(count)) | (DWORD(a) >> unsigned(sizeof(T)*8 - count)));
}

template <class T, class C> inline T ror(T a, C count) {
	return T((DWORD(a) >> unsigned(count)) | (DWORD(a) << unsigned(sizeof(T)*8 - count)));
}

inline BOOL isValidPtr(const void *ptr) {
	return (ptr > NULL) && (ptr < (void *)0x90000000);
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

void safeStrCpy  (char *dest, size_t destSize, const char *src);
void safeSprintf (char *dest, size_t destSize, const char *fmt, ...);
void safeVsprintf(char *dest, size_t destSize, const char *fmt, va_list ap);

void trimLeadingWhitespace (char *str);
void trimTrailingWhitespace(char *str);
void concatenateWhitespace (char *str);

void swapMem(void *buf1, void *buf2, size_t numBytes);

void reverseBytes(void *buf, size_t numBytes);   //    A    B    C    D <->    D    C    B    A
void reverseWords(void *buf, size_t numWords);   //   AB   CD   EF   GH <->   GH   EF   CD   AB
void reverseDWords(void *buf, size_t numDWords); // ABCD EFGH IJKL MNOP <-> MNOP IJKL EFGH ABCD


//***************************************************************************
#endif // #ifndef __CODEWIZ_H_INCLUDED__
