/*
Copyright (C) 2012 Netpanzer Team. (www.netpanzer.org), Laurent Jacques

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


#include "Views/Components/tChatBox.hpp"
#include "Views/Components/View.hpp"
#include "Views/Theme.hpp"
#include "Interfaces/PlayerInterface.hpp"
#include "Resources/ResourceManager.hpp"

int tChatBox::getMaxItemWidth(int Index)
{
    return Surface::getTextLength(List[Index].text)+34;
}

void tChatBox::onPaint(Surface &dst, int Index)
{
    int StartX = 3;
    ChatMessage *MsgData = (ChatMessage*)(List[Index].Data);
    if (MsgData)
    {
        if (MsgData->IsFlag)
        {
            StartX = 23;
            Surface * flag = 0;
            flag = ResourceManager::getFlag(MsgData->FlagIndex);
            if (flag)
            {
                flag->blt(dst, 1, 0);
                dst.bltString(StartX , 4, List[Index].text.c_str(), MsgData->Color);
            }
            else dst.bltString(StartX , 4, List[Index].text.c_str(), MsgData->Color);
        } 
        else dst.bltString(StartX , 4, List[Index].text.c_str(), MsgData->Color);
    }
    else dst.bltString(StartX , 4, List[Index].text.c_str(), ctTexteNormal);
}

void tChatBox::AddChat(std::string msg, PIX color, bool isflag, FlagID flagindex)
{
    ChatMessage *Datamsg;
    Datamsg = new ChatMessage();
    Datamsg->Color = color;
    Datamsg->IsFlag = isflag;
    Datamsg->FlagIndex = flagindex;

    AddData(msg, Datamsg);
}

