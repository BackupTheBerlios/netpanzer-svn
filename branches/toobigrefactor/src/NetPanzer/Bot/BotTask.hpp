/*
Copyright (C) 2003 Ivo Danihelka <ivo@danihelka.net>

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
#ifndef BOTTASKS_H
#define BOTTASKS_H

#include "Types/iXY.hpp"

class BotTask {
    private:
        iXY m_lastLoc;
        int m_task;
    public:
        BotTask(const iXY& loc, int task) { m_lastLoc = loc; m_task = task; }

        void setLastLoc(const iXY& loc) { m_lastLoc = loc; }
        const iXY& getLastLoc() { return m_lastLoc; }

        void setTask(int task) { m_task = task; }
        int getTask() { return m_task; }
};

#endif
