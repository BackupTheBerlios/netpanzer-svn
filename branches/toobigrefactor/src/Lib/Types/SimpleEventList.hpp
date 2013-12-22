/*
Copyright (C) 2013 Netpanzer Team. (www.netpanzer.org), Aaron Perez

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
 *
 * Created on March 10, 2013, 1:35 PM
 */

#ifndef SIMPLEEVENTLIST_HPP
#define	SIMPLEEVENTLIST_HPP

template<typename event_type, unsigned max_size>
class SimpleEventList
{
private:
    event_type events[max_size];
    unsigned current;
    unsigned last;

public:
    SimpleEventList() : current(0), last(0) {}

    inline bool empty() const { return current >= last; }

    inline void reset() { current = last = 0; }

    inline event_type nextEvent()
    {
        return current < last ? events[current++] : 0;
    }

    inline void push(event_type a)
    {
        if ( a && (last < max_size) )
        {
            events[last++] = a;
        }
    }
};

#endif	/* SIMPLEEVENTLIST_HPP */

