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
#ifndef _WINTIMER_HPP
#define _WINTIMER_HPP

#include <windows.h>
#include <mmsystem.h>


#define _MMTIMER_FREQUENCY 1000


enum { _TSC_timer, _hires_timer, _multimedia_timer };

class WinTimer
 {
   protected:
   MMRESULT MMTimerID;
   unsigned long MMTimerTargetDelay;
   unsigned long MMTimerTargerRes;
   
   public:  
   static unsigned long MMTimerDelay;
   static unsigned long MMTimerResolution;
   
   public:
   static unsigned long TimerType;
   static double TimerResolution;
   static double TimerFrequency; 
   static double MasterClock;


   protected:
   BOOL DetectRDTSC( void );
   BOOL SetupRDTSC( void );
   BOOL SetupHighResTimer( void );
   BOOL SetupMMTimer( void );
   
   public:
   BOOL Initialize( void );
   void	ShutDown( void );


   inline static double GetClock( void )
    {
     LARGE_INTEGER counter;
			 
	 if ( TimerType == _TSC_timer )
      {
       return( RDTSC() );
      } 
     else
      if ( TimerType == _hires_timer )
	   { 
	    QueryPerformanceCounter( &counter );	  
	    return ( ( double ) counter.QuadPart );
	   }
	  else
	    return ( MasterClock );
    }
   
  	//static inline __declspec( naked ) long double RDTSC() 
  	static long double RDTSC();

  };

extern WinTimer WINTIMER;

#endif
