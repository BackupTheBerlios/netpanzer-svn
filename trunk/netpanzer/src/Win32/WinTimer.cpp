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
#include "WinTimer.hpp"

#include "codewiz.hpp"


unsigned long WinTimer::MMTimerDelay;
unsigned long WinTimer::MMTimerResolution;

unsigned long WinTimer::TimerType;
double WinTimer::TimerResolution; 
double WinTimer::TimerFrequency; 
double WinTimer::MasterClock;

WinTimer WINTIMER;

void CALLBACK MMClock(UINT wTimerID, UINT msg, DWORD dwUser, 
                      DWORD dw1, DWORD dw2) 
{ 
 WinTimer::MasterClock++;  
} 


#if _MSC_VER > 1000
 #pragma optimize( "", off )
#endif

bool WinTimer::DetectRDTSC( void )
 { 
#ifdef MSVC
	volatile bool retval = true;
	volatile DWORD RegEDX;

    __try {
		_asm {
			mov eax, 1		// set up CPUID to return processor version and features
							//	0 = vendor string, 1 = version info, 2 = cache info
			_emit 0x0F 
            _emit 0xA2      // code bytes = 0fh,  0a2h
            mov RegEDX, edx	// features returned in edx
	   	}
   	} __except(EXCEPTION_EXECUTE_HANDLER)
	{
        retval = 0;
	}

	if (retval == 0)
	{
		return false;        	// processor does not support CPUID
	}

	if (RegEDX & 0x10 ) 		// bit 4 is set for RDTSC technology
	{
	   __try { _asm { 
                     _emit 0fh
		             _emit 31h
                    } 
             } 		
	   __except(EXCEPTION_EXECUTE_HANDLER) { retval = false; }

		return retval;
	}
   	else
	 {
      return false;        	// processor supports CPUID but does not support RDTSC technology
     }


	return retval;
#else
	// XXX
	printf ("No RTDSC for gcc...(What is this anyway?)\n");
	return false;
#endif
 }

#if _MSC_VER > 1000
 #pragma optimize( "", on )
#endif  


bool WinTimer::SetupHighResTimer( void )
 {
  bool result;
  LARGE_INTEGER frequency;

  result = QueryPerformanceFrequency( &frequency );

  if ( result == false )
   return ( false );

  TimerFrequency = ( double ) frequency.QuadPart;

  TimerResolution = 1;
  TimerType = _hires_timer;

  return ( true );

 }

bool WinTimer::SetupMMTimer( void )
 {
  MMRESULT result;
  TIMECAPS TimeCap;
    
  result = timeGetDevCaps( &TimeCap, sizeof(TIMECAPS) ); 
  
  assert( result == TIMERR_NOERROR );  
  if ( result != TIMERR_NOERROR )
   return ( false );
    
  MMTimerResolution = MIN( MAX (DWORD(TimeCap.wPeriodMin), DWORD(MMTimerTargerRes) ), DWORD(TimeCap.wPeriodMax));
  TimerResolution = MMTimerResolution;

  timeBeginPeriod( MMTimerResolution ); 
 
  MMTimerDelay = MMTimerTargetDelay;   
  
  MMTimerID = timeSetEvent( MMTimerDelay, MMTimerResolution, MMClock, 0, TIME_PERIODIC );                // single timer event

  assert( MMTimerID != 0 );
  if ( MMTimerID == 0 )
   return ( false );
 
  TimerFrequency = _MMTIMER_FREQUENCY / MMTimerDelay;

  return( true );
  
 }

bool WinTimer::SetupRDTSC( void )
 {
  //TimerFrequency = ( double ) 1000000.0;
  TimerFrequency = ( double ) 2500.0;

  TimerResolution = 1;
  TimerType = _TSC_timer;
  return( true );
 }

WinTimer::WinTimer()
{
   	MMTimerTargerRes = 2;
	MMTimerTargetDelay = 10;

	if ( DetectRDTSC() == true  )
	{ 
		SetupRDTSC();
	} 
	else
	{
		if ( SetupHighResTimer( ) == false )
			SetupMMTimer();
	}
}

WinTimer::~WinTimer()
{
	if ( TimerType == _multimedia_timer )
	{
	 	timeKillEvent( MMTimerID );   
		timeEndPeriod( MMTimerResolution );
	}
}

#ifdef MSVC
__declspec( naked ) long double WinTimer::RDTSC() 
{ _asm {
	_emit 0fh
	_emit 31h
	sub  esp, 8
	mov  [esp + 4], edx
	mov  [esp], eax
	fild qword ptr [esp]
	add  esp, 8
	ret
  }
}
#endif
