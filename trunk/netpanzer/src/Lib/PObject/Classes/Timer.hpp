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
#ifndef _TIMER_HPP
#define _TIMER_HPP

#include "cTimeStamp.hpp"
#include "TimerInterface.hpp"

class Timer
{
protected:
    float period;
    TIMESTAMP last_count;

public:
    Timer()
    { }

    Timer( float rate )
    {
        period = 1/rate;
    }

    inline void changeRate( float rate )
    {
        period = 1/rate;
        last_count = now();
    }

    inline void changePeriod( float period )
    {
        Timer::period = period;
        last_count = now();
    }

    inline void reset( void )
    {
        last_count = now();
    }

    inline void zero( void )
    {
        last_count = now() - period;
    }

    inline bool count( void )
    {
        float difference;
        float remainder;

        difference = now() - last_count;

        if ( difference >= period ) {
            remainder = difference - period;

            if ( remainder >= period )
                remainder = 0;

            last_count = now() - remainder;

            return true;
        }
        return false;
    }

    inline float getPeriod() const
    {
        return period;
    }

    inline float getRate() const
    {
        return 1/period;
    }

    inline float getTime() const
    {
        return( (float) (now() - last_count) );
    }

    inline float getTimeLeft() const
    {
        return( (float) ( period - (now() - last_count) ) );
    }
};

class TimerFrameBase
{
protected:
    float period;
    TIMESTAMP last_count;

public:

    TimerFrameBase()
    { }

    TimerFrameBase( float rate )
    {
        period = 1/rate;
    }

    inline void changeRate( float rate )
    {
        period = 1/rate;
        last_count = TimerInterface::getFrameStartTime();
    }

    inline void changePeriod( float period )
    {
        TimerFrameBase::period = period;
        last_count = TimerInterface::getFrameStartTime();
    }

    inline void reset( void )
    {
        last_count = TimerInterface::getFrameStartTime();
    }

    inline void zero( void )
    {
        last_count = TimerInterface::getFrameStartTime() - period;
    }

    inline bool count( void )
    {
        float difference;
        float remainder;

        difference = TimerInterface::getFrameStartTime() - last_count;

        if ( difference >= period ) {
            remainder = difference - period;

            if ( remainder >= period )
                remainder = 0;

            last_count = TimerInterface::getFrameStartTime() - remainder;

            return( true );
        }
        return( false );
    }

    inline float getPeriod( void )
    {
        return( period );
    }

    inline float getRate( void )
    {
        return( 1/period );
    }

    inline float getTime( void )
    {
        return( (float) (TimerInterface::getFrameStartTime() - last_count) );
    }

    inline float getTimeLeft( void )
    {
        return( (float) ( period - (TimerInterface::getFrameStartTime() - last_count) ) );
    }
};


#endif // ** _TIMER_HPP
