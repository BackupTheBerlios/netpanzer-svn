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

#ifndef __ChatView_hpp__
#define __ChatView_hpp__

#include "View.hpp"
#include "Surface.hpp"
#include "GameTemplateView.hpp"
#include "ScrollBar.hpp"


//---------------------------------------------------------------------------
void sendChatStringToServer();

class ChatView : public GameTemplateView
{
private:
    void drawMessages(const Surface &dest);

    int  messageCount;         // How many messages do we currently have?
    int  viewableMessageCount; // How many items are viewable.

    enum { MAX_MESSAGE_COUNT = 50 }; // The max number of displayable messages.
    enum { MAX_STRING_LENGTH = 36 }; // Max chars in a single message.

    char messages[MAX_MESSAGE_COUNT][MAX_STRING_LENGTH + 1];

    ScrollBar *scrollBar;

public:
    ChatView();
    virtual ~ChatView()
    {
        delete scrollBar;
    }

    virtual void doDraw(const Surface &windowArea, const Surface &clientArea);

    void addMessage(const char *newMessage);

    void init();

}
; // end ChatView

extern ChatView chatView;

#endif // end __ChatView_hpp__
