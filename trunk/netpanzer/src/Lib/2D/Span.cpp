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
#include "Span.hpp"


int allowSpanBlitting = 1;

//--------------------------------------------------------------------------
void bltTransSpan(PIX *dRow, const PIX *sRow, int pixelsPerRow)
{
	// XXX not needed at the moment
	memcpy(dRow, sRow, pixelsPerRow*sizeof(PIX));
#if 0
		_asm {
			mov edi, [dRow]
			mov esi, [sRow]
			mov ecx, [pixelsPerRow]

alignMe:
			test edi, 3
			je aligned
			mov al, [esi]
			test al, al
			jz skipMe1
			mov [edi], al
skipMe1:
			inc edi
			inc esi
			dec ecx
			jnz alignMe
			jmp spanDone
allOpaque:
			mov [edi-4], edx
aligned:
			sub ecx, 4
			jl leftovers
test4:
			mov edx,7EFEFEFFh
			mov eax,dword ptr [esi]

			add edx,eax
			add esi, 4

			xor eax,0FFFFFFFFh
			add edi, 4

			xor eax,edx
			mov edx,dword ptr [esi-4]

			test eax,81010100h
			je allOpaque

			test dl, dl
			jz done0
			mov [edi-4], dl
done0:

			test dh, dh
			jz done1
			mov [edi-4+1], dh
done1:

			shr edx, 16
			test dl, dl
			jz done2
			mov [edi-4+2], dl
done2:

			test dh, dh
			jz done3
			mov [edi-4+3], dh
done3:

			sub ecx, 4
			jge test4
			
leftovers:
			add ecx, 4
			jle spanDone
leftover:
			mov al, [esi]
			test al, al
			jz skipMe2
			mov [edi], al
skipMe2:
			inc esi
			inc edi
			dec ecx
			jnz leftover

spanDone:
		}
#endif

} // end bltTransSpan

//--------------------------------------------------------------------------
void bltBlendSpan(PIX *dRow, const PIX *sRow, int pixelsPerRow, const BYTE *table)
{
	if (!allowSpanBlitting) { return; } // Remove for release candidate.

	for(size_t idx=0;idx<pixelsPerRow;idx++) {
		dRow[idx] = table[ (dRow[idx]<<8)+sRow[idx] ];
		//dRow[idx] = table[sRow[idx]];
	}

#if 0
		_asm {
			mov edi, [dRow]
			mov esi, [sRow]
			mov ecx, [pixelsPerRow]
			mov ebx, [table]

			xor eax, eax
alignMe:
			test edi, 3
			je aligned
			mov al, [esi]
			mov ah, [edi]
			mov al, [ebx + eax]
			mov [edi], al
			inc esi
			inc edi
			dec ecx
			jnz alignMe
			jmp spanDone

aligned:
			sub ecx, 4
			jl leftovers
			push ebp
			mov ebp, ebx
			xor ebx, ebx
do4:
			mov al, [esi+2]
			mov bl, [esi+3]

			mov ah, [edi+2]
			mov bh, [edi+3]

			add esi, 4
			mov dl, [ebp + eax]

			add edi, 4
			mov dh, [ebp + ebx]

			shl edx, 16
			mov al, [esi-4+0]

			mov bl, [esi-4+1]
			mov ah, [edi-4+0]

			mov bh, [edi-4+1]
			mov dl, [ebp + eax]

			mov dh, [ebp + ebx]
			sub ecx, 4

			mov [edi-4], edx
			jge do4
			
			mov ebx, ebp
			pop ebp

leftovers:
			add ecx, 4
			jz spanDone
leftover:
			mov al, [esi]
			mov ah, [edi]
			mov al, [ebx + eax]
			mov [edi], al
			inc esi
			inc edi
			dec ecx
			jnz leftover

spanDone:

		}
#endif

} // end bltBlendSpan

//--------------------------------------------------------------------------
void bltBlendScaleSpan(PIX *dRow, const PIX *sRow, int srcX1FracWithCount, int stepAndDecCount, int stepWholePart, const BYTE *table)
{
	if (!allowSpanBlitting) { return; } // Remove for release candidate.

	// XXX disabled msvc assembler routines
#ifdef MSVC
		_asm {
			mov edi, [dRow]
			mov esi, [sRow]
			mov ecx, [srcX1FracWithCount];
			mov ebx, [table]

			xor eax, eax
alignMe:
			test edi, 3
			je aligned
			mov al, [esi]
			mov ah, [edi]
			mov al, [ebx + eax]
			mov [edi], al
			inc edi
			add ecx, [stepAndDecCount]
			adc esi, [stepWholePart]
			test ecx, 0ffffh
			jnz alignMe
			jmp spanDone

aligned:
			test ecx, 0fffch
			jz leftovers
do4:
			mov al, [esi]
			add ecx, [stepAndDecCount]

			mov ah, [edi+0]
			adc esi, [stepWholePart]

			mov dl, [ebx + eax]
			mov al, [esi]

			add ecx, [stepAndDecCount]
			mov ah, [edi+1]

			adc esi, [stepWholePart]
			mov dh, [ebx + eax]

			shl edx, 16
			add edi, 4

			mov al, [esi]
			add ecx, [stepAndDecCount]

			mov ah, [edi-4+2]
			adc esi, [stepWholePart]

			mov dl, [ebx + eax]
			mov al, [esi]

			mov ah, [edi-4+3]
			add ecx, [stepAndDecCount]

			mov dh, [ebx + eax]
			adc esi, [stepWholePart]

			rol edx, 16
			test ecx, 0fffch

			mov [edi-4], edx
			jnz do4
			
leftovers:
			test ecx, 0fh
			jz spanDone
leftover:
			mov al, [esi]
			mov ah, [edi]
			mov al, [ebx + eax]
			mov [edi], al
			inc edi
			add ecx, [stepAndDecCount]
			adc esi, [stepWholePart]
			test ecx, 0fh
			jnz leftover

spanDone:

		}
#endif

} // end bltBlendScaleSpan

//--------------------------------------------------------------------------
void bltLookupSpan(PIX *dRow, int pixelsPerRow, const PIX *table)
{
	if (!allowSpanBlitting) { return; } // Remove for release candidate.

	for(size_t idx=0;idx<pixelsPerRow;idx++)
	  	dRow[idx] = table[dRow[idx]];

#if 0
		_asm {
			mov edi, [dRow]
			mov ecx, [pixelsPerRow]
			mov esi, [table]

			xor eax, eax
alignMe:
			test edi, 3
			je aligned
			mov al, [edi]
			mov al, [esi + eax]
			mov [edi], al
			inc edi
			dec ecx
			jnz alignMe
			jmp spanDone

aligned:
			sub ecx, 4
			jl leftovers
			xor ebx, ebx

			mov al, [edi+2]
			mov bl, [edi+3]

do4:
			mov dl, [esi + eax]
			mov al, [edi+0]

			mov dh, [esi + ebx]
			mov bl, [edi+1]

			shl edx, 16
			add edi, 4

			mov dl, [esi + eax]
			sub ecx, 4

			mov al, [edi+2]
			mov dh, [esi + ebx]

			mov bl, [edi+3]
			mov [edi-4], edx
			jge do4
			
leftovers:
			add ecx, 4
			jz spanDone
leftover:
			mov al, [edi]
			mov al, [esi + eax]
			mov [edi], al
			inc edi
			dec ecx
			jnz leftover

spanDone:
		}
#endif

} // end bltLookupSpan

//--------------------------------------------------------------------------
void bltScaleSpan(PIX *dRow, const PIX *sRow, int srcX1FracWithCount,
				  int stepAndDecCount, int stepWholePart)
{
	if (!allowSpanBlitting) { return; } // Remove for release candidate.
	// XXX not needed anymore at the moment...

	// XXX disabled msvc assembler
#ifdef MSVC
		_asm {
			mov edi, [dRow]
			mov esi, [sRow]
			mov ecx, [srcX1FracWithCount];
			mov edx, [stepAndDecCount]
			mov eax, [stepWholePart]
			push ebp
			mov ebp, eax


alignMe:
			test edi, 3
			je aligned
			mov al, [esi]
			mov [edi], al
			inc edi
			add ecx, edx
			adc esi, ebp
			test ecx, 0ffffh
			jnz alignMe
			jmp spanDone

aligned:
			test ecx, 0fffch
			jz leftovers
do4:
			mov al, [esi]
			add ecx, edx

			adc esi, ebp
			add ecx, edx

			mov ah, [esi]
			lea edi, [edi+4]

			adc esi, ebp

			shl eax, 16
			add ecx, edx

			mov al, [esi]
			adc esi, ebp

			mov ah, [esi]
			add ecx, edx

			adc esi, ebp

			rol eax, 16
			test ecx, 0fffch

			mov [edi-4], eax
			jnz do4
			
leftovers:
			test ecx, 0fh
			jz spanDone
leftover:
			mov al, [esi]
			mov [edi], al
			inc edi
			add ecx, edx
			adc esi, ebp
			test ecx, 0fh
			jnz leftover

spanDone:
			pop ebp
		}
#endif

} // end bltScaleSpan

