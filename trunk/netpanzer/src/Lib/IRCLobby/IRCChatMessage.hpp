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
#ifndef __IRCChatMessage_h__
#define __IRCChatMessage_h__

#include <string>

//---------------------------------------------------------------------------
class IRCChatMessage
{
public:
    IRCChatMessage(const std::string& newuser, const std::string& newmessage) 
        : user(newuser), message(newmessage)
    {
    }

    IRCChatMessage(const IRCChatMessage& other)
        : user(other.user), message(other.message)
    {
    }

    const std::string getUser() const
    { return user; }

    const std::string getMessage() const
    { return message; }

private:
    std::string user;
    std::string message;
};

#endif
