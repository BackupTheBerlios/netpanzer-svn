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
#ifndef __WAVE_INCLUDED__
#define __WAVE_INCLUDED__
#include "windows.h"
#include "dsound.h"

#define WAVEVERSION 1

#ifndef ER_MEM
#define ER_MEM 				0xe000
#endif

#ifndef ER_CANNOTOPEN
#define ER_CANNOTOPEN 		0xe100
#endif

#ifndef ER_NOTWAVEFILE
#define ER_NOTWAVEFILE 		0xe101
#endif

#ifndef ER_CANNOTREAD
#define ER_CANNOTREAD 		0xe102
#endif

#ifndef ER_CORRUPTWAVEFILE
#define ER_CORRUPTWAVEFILE	0xe103
#endif

#ifndef ER_CANNOTWRITE
#define ER_CANNOTWRITE		0xe104
#endif

typedef struct _fileinfo
{
    BYTE                *pbData;        // Pointer to actual data of file.
    UINT                cbSize;         // Size of data.
    WAVEFORMATEX		*pwfx;          // Pointer to waveformatex structure.

    DWORD               dwFreq;         // Frequency.
    DWORD               dwPan;          // Panning info.
    DWORD               dwVol;          // Total volume.
    bool                fLooped;        // Looped?

    bool                fPlaying;       // Is this one playing?
    bool                fLost;          // Is this one lost?
    bool                fHardware;      // Is this a hardware buffer?
    bool                fSticky;        // Is this a sticky buffer?

    int			cox;            // Coordinates of where the structure is
    int			coy;            // printed.

    // HWND's needed.
    HWND                hWndFileName_TXT;   // Filename text string.
    HWND                hWndFreq_TB;        // Trackbar handle.
    HWND                hWndFreq_TXT;       // Text string for freq.
    HWND                hWndPan_TB;         // Trackbar handle.
    HWND                hWndPan_TXT;        // Text string for pan.
    HWND                hWndVol_TXT;        // Text string for volume.
    HWND                hWndVolL_TB;        // Trackbar for volume left.
    HWND                hWndVolR_TB;        // Trackbar for volume right.
    HWND                hWndVolM_TB;        // Main volume.
    HWND                hWndLooped_BN;      // Looped
    HWND                hWndPlay_BN;        // Play
    HWND                hWndRemove_BN;      // Remove.
    
    HWND                hWndFileName_EDGE;  // The line under filename.
    HWND                hWndLooped_EDGE;    // The line under looped.
    HWND                hWndFreq_EDGE;      // The line under freq.
    HWND                hWndPan_EDGE;       // The line under pan.
    HWND                hWndVol_EDGE;       // The line under volume.
    HWND                hWndWhole_EDGE;     // The whole surrounding edge.
    HWND                hWndVolL_TXT;       // For the L
    HWND                hWndVolR_TXT;       // For the R

    #ifdef SHOWSTATUS
    HWND                hWndStatus_TXT;     // For status.
    HWND                hWndStatus_EDGE;
    HWND                hWndPlayPosition_TXT;
    HWND                hWndPlayPosition_EDGE;
    HWND                hWndWritePosition_TXT;
    HWND                hWndWritePosition_EDGE;
    #endif

    LPDIRECTSOUNDBUFFER pDSB;               // Pointer to direct sound buffer.

    int                 nFileName;          // Index to filename, not including dir.
    char                szFileName[MAX_PATH];
    struct _fileinfo    *pNext;             // Pointer to next file.

} FILEINFO;



int WaveOpenFile(char *, HMMIO *, WAVEFORMATEX **, MMCKINFO *);
int WaveStartDataRead(HMMIO *, MMCKINFO *, MMCKINFO *);
int WaveReadFile(HMMIO, UINT, BYTE *, MMCKINFO *, UINT *);
int WaveCloseReadFile(HMMIO *, WAVEFORMATEX **);
//int WaveLoadFile(char *, UINT *, DWORD *, WAVEFORMATEX **, BYTE **);
int WaveLoadFile(
			char *pszFileName,              // (IN)
			UINT *cbSize,                   // (OUT)
			DWORD *pcSamples,               // (OUT)
			WAVEFORMATEX **ppwfxInfo,       // (OUT)
			BYTE **ppbData                  // (OUT)
			)
;

#endif
