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

#include "Exception.hpp"
#include "ChatView.hpp"
#include "Desktop.hpp"
#include "KeyboardInterface.hpp"
#include "ChatInterface.hpp"
#include "GameViewGlobals.hpp"


ChatView chatView;

void addNewChatStringCallBack( const char *message_text )
{
    chatView.addMessage( message_text );
}

// ChatView
//---------------------------------------------------------------------------
ChatView::ChatView() : GameTemplateView()
{} // end ChatView::ChatView

// init
//---------------------------------------------------------------------------
void ChatView::init()
{
    setSearchName("ChatView");
    setTitle("Message Archive");
    setSubTitle(" - F7");

    setAllowResize(false);
    setDisplayStatusBar(false);
    setVisible(false);

    moveTo(0, 0);
    resize(400, 200);

    int x = 0;
    int y = 0;
    //int buttonXSize = 60;

    x = 0;
    y = getClientRect().getSize().y - 12;

    messageCount         = 0;
    //viewableMessageCount = 	(getClientRect().getSize().y - 20) / (CHAR_YPIX + ITEM_GAP_SPACE);

    {for (int i = 0; i < MAX_MESSAGE_COUNT; i++)
        {
            memset(messages[i], 0, sizeof(messages[0]));
        }}

    ChatInterface::setNewMessageCallBack( addNewChatStringCallBack );

    // Define the scrollBar fot this view.
    scrollBar = new ScrollBar(VERTICAL, 0, 1, 0, 100);
    if (scrollBar == 0) {
        throw Exception("ERROR: Unable to allocate the scrollBar.");
    }

    //iRect clientRect(getClientRect());
    //scrollBar->setViewableAmount(clientRect.getSizeY() / (TEXT_GAP_SPACE + CHAR_YPIX) - 1);
    //scrollBar->setBlockIncrement(1);

    //add(scrollBar);

} // end ChatView::init

// addMessage
//---------------------------------------------------------------------------
void ChatView::addMessage(const char *newMessage)
{
    /*
    	// Since the message list is not full, just add it in the list.
    	if (messageCount < MAX_MESSAGE_COUNT)
    	{
         sprintf(messages[messageCount], "%s", newMessage);
         messageCount++;
    	} else
    	{
         // Since the message list is full, scoot all the messages up 1 and insert the new message.
         memcpy(messages[0], messages[1], sizeof(messages[0]) * (MAX_MESSAGE_COUNT - 1));
         sprintf(messages[messageCount - 1], "%s", newMessage);	  
        }
    	
    	setScrollBarViewableCount(viewableMessageCount);
    	setScrollBarItemCount(messageCount);
    	setScrollBarItemsSize(CHAR_YPIX + ITEM_GAP_SPACE / 2);
    	
    	if(messageCount < viewableMessageCount)
    	{
    		setScrollBarTopItem(messageCount);
    	} else
    	{
    		setScrollBarTopItem(messageCount - viewableMessageCount);
    	}
    */
} // end ChatView::addMessage

// drawMessages
//---------------------------------------------------------------------------
void ChatView::drawMessages(const Surface &dest)
{
    /*
    	iXY offset;
    	int index;
     
    	for (int i = 0; i < viewableMessageCount; i++)
    	{
    		offset.x = 2;
    		offset.y = 16 + i * (CHAR_YPIX + ITEM_GAP_SPACE / 2);
     
    		index = getScrollBarTopItem() + i;
    		assert(index < MAX_MESSAGE_COUNT);
     
    		dest.bltString(offset.x, offset.y, messages[index], Color::white);
    	}
    */
} // end ChatView::drawMessages

// doDraw
//---------------------------------------------------------------------------
void ChatView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
    bltViewBackground(viewArea);

    drawMessages(clientArea);
    //viewArea.drawButtonBorder(Color::white, pal.cGRAY64);
    //viewArea.blendRect(getViewRect(), pal.gray64);

    // Chat Window
    //clientArea.drawButtonBorder(iRECT(0, 0, 300, 200), pal.cDARKGRAY, Color::white);

    // Recipient Window
    //clientArea.drawButtonBorder(iRECT(302, 45, 380, 216), pal.cDARKGRAY, Color::white);

    View::doDraw(viewArea, clientArea);
    //drawTitle(viewArea);
    //drawDefinedButtons(clientArea);
    //drawInputFields(clientArea);

} // end doDraw
