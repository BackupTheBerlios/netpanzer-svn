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


#define _MMTIMER_FREQUENCY 1000


enum { _TSC_timer, _hires_timer, _multimedia_timer };

class WinTimer
{
   public:  
   static unsigned long MMTimerDelay;
   static unsigned long MMTimerResolution;
   
   public:
   static unsigned long TimerType;
   static double TimerResolution;
   static double TimerFrequency; 
   static double MasterClock;


   protected:
   bool DetectRDTSC( void );
   bool SetupRDTSC( void );
   bool SetupHighResTimer( void );
   bool SetupMMTimer( void );
   
   public:
   bool Initialize( void );
   void	ShutDown( void );


   inline static double GetClock( void )
    {
	 return 0;
	}
  	
    //static inline __declspec( naked ) long double RDTSC() 
  	static long double RDTSC();

};

#endif
