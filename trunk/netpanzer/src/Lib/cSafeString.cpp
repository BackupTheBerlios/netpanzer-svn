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

//***************************************************************************
// code
//***************************************************************************

//---------------------------------------------------------------------------
void safeStrcpy(char *dest, size_t destSize, const char *src) {
	if (dest == NULL || destSize <= 0) return;

	if (src == NULL) {
		zeroMem(dest, destSize);
		return;
	}

	if (destSize > 1) strncpy(dest, src, destSize-1);
	dest[destSize-1] = '\0';

}

//---------------------------------------------------------------------------
void safeSprintf(char *dest, size_t destSize, const char *fmt, ...) {

	va_list ap;
	va_start(ap, fmt);
	safeVsprintf(dest, destSize, fmt, ap);
	va_end(ap);

}

//---------------------------------------------------------------------------
void safeVsprintf(char *dest, size_t destSize, const char *fmt, va_list ap) {

	if (dest == NULL || destSize <= 0) return;

	if (fmt == NULL) {
		zeroMem(dest, destSize);
		return;
	}

	char buf[1024];

	unsigned charsPrinted = vsprintf(buf, fmt, ap);
	assert(charsPrinted < sizeof(buf));

	if (charsPrinted >= destSize) {

		memcpyB(dest, buf, destSize-1);
		dest[destSize-1] = '\0';

	} else {

		memcpyB(dest, buf, charsPrinted);
		zeroMem(dest + charsPrinted, destSize - charsPrinted);

	}

}

//---------------------------------------------------------------------------
void trimLeadingWhitespace (char *str) {

	// Safety first!
	if (str == NULL) return;

	// Check one special case where the first character isn't whitespace.
	// In this instance, let's just bail here to avoid any redundant copying
	if (!isspace(*str)) return;

	// Scan forward until we hit a character that isn't whitespace.  Remember
	// the null terminator isn't considered whitespace.
	char *firstNonWhiteSpace = str;
	while (isspace(*++firstNonWhiteSpace)) {}

	// Now move the rest of the string backward to the beginning.
	while ((*(str++) = *(firstNonWhiteSpace++)) != '\0') {}

}

//---------------------------------------------------------------------------
void trimTrailingWhitespace(char *str) {

	// Safety first!
	if (str == NULL) return;

	// Find the null terminator
	char *lastNonWhiteSpace = strchr(str, '\0');

	// Now scan backward until we hit some whitespace or we reach the
	// beginning of the string.
	do {
		if (!isspace(*lastNonWhiteSpace)) break;
		--lastNonWhiteSpace;
	} while(lastNonWhiteSpace >= str);


	// Everything after this until the end of the string is whitespace.  Chop
	// it off.
	lastNonWhiteSpace[1] = '\0';
}

//---------------------------------------------------------------------------
void concatenateWhitespace(char *str) {

	// Safety first!
	if (str == NULL) return;

	// First skip to the first whitespace in the string
	while (!isspace(*str)) {

		// End of string here?  Then we're done
		if (*str == '\0') return;

		++str;
	}

	char *dest = str;

	// OK.  Keep concatonating whitespace until we hit the end of the string
	for (;;) {

		// We're on a stretch of whitespace here.  Replace the whole thing with
		// one single space character.
		*(dest++) = ' ';
		while (isspace(*str)) ++str;

		// This char isn't whitespace.  Copy it until we hit the end of the
		// string or more whitespace.
		do {
			*(dest++) = *str;
			if (*str == '\0') return;
		} while (!isspace(*++str));
	}

}

//---------------------------------------------------------------------------
void swapMem(void *buf1, void *buf2, size_t numBytes) {

	// Swap as much memory as possibly a dword at a time
	DWORD *d1 = (DWORD *)buf1;
	DWORD *d2 = (DWORD *)buf2;

	while (numBytes >= 4) {
		DWORD t;
		SWAP(*d1, *d2, t);
		++d1;
		++d2;
		numBytes -= 4;
	}

	// Now swap the rest a byte at a time
	BYTE *b1 = (BYTE *)d1;
	BYTE *b2 = (BYTE *)d2;

	while (numBytes > 0) {
		BYTE t;
		SWAP(*b1, *b2, t);
		++b1;
		++b2;
		--numBytes;
	}

}

//---------------------------------------------------------------------------
void reverseBytes(void *buf, size_t numBytes) {

	BYTE *p1 = (BYTE *)buf;
	BYTE *p2 = (BYTE *)buf + numBytes - 1;

	while (p1 < p2) {
		BYTE t;
		SWAP(*p1, *p2, t);
		++p1;
		--p2;
	}

}

//---------------------------------------------------------------------------
void reverseWords(void *buf, size_t numWords) {

	WORD *p1 = (WORD *)buf;
	WORD *p2 = (WORD *)buf + numWords - 1;

	while (p1 < p2) {
		WORD t;
		SWAP(*p1, *p2, t);
		++p1;
		--p2;
	}

}

//---------------------------------------------------------------------------
void reverseDWords(void *buf, size_t numDWords) {

	DWORD *p1 = (DWORD *)buf;
	DWORD *p2 = (DWORD *)buf + numDWords - 1;

	while (p1 < p2) {
		DWORD t;
		SWAP(*p1, *p2, t);
		++p1;
		--p2;
	}

}
