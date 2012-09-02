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

#include "Views/Game/ChatView.hpp"
#include "Classes/WorldInputCmdProcessor.hpp"
#include "Classes/ScreenSurface.hpp"
#include "Interfaces/KeyboardInterface.hpp"
#include "Interfaces/ChatInterface.hpp"
#include "Views/Components/Desktop.hpp"
#include "Views/Components/View.hpp"
#include "Views/Theme.hpp"
#include "Views/Components/Button.hpp"
#ifdef WIN32
#include <windows.h>
#endif

static Button* createTitlebarButton(int x, int y, const Surface& button_images)
{
    Surface bimage(15,15,4);
    
    bimage.setFrame(0);
    button_images.blt(bimage, 0, 0);
    bimage.bltLookup(bimage.getRect(), Palette::darkGray256.getColorArray());
    
    bimage.setFrame(1);
    button_images.blt(bimage, 0, 0);
    
    bimage.setFrame(2);
//    bimage.fill(0);
    button_images.blt(bimage, 0, 0);
    
    bimage.setFrame(3);
    button_images.blt(bimage, 0, 0);
    bimage.bltLookup(bimage.getRect(), Palette::darkGray256.getColorArray());
    
    Button *b = new Button();
    b->setImage(bimage);
    b->setLocation( x, y);
    b->setStateOffset(Button::BPRESSED, 1, 1);
    
    return b;
}


ChatView::ChatView() : GameTemplateView()
{
    setSearchName("ChatView");
    setTitle("Chat View");
    setSubTitle("");

    setAllowResize(false);
    setVisible(false);
    setAllowMove(true);
    setBordered(false);
    resize(500, 150);
    moveTo(screen->getWidth()-getSizeX(), screen->getHeight()-getSizeY());

    Surface button_images;
    button_images.loadBMP(itScroll);
    
    bHideWindow = createTitlebarButton( 0, 0, button_images);
    add(bHideWindow);
    
    button_images.setOffsetX(-30);
    bShowWindow = createTitlebarButton( 15, 0, button_images);
    bShowWindow->disable();
    add(bShowWindow);
    
    HideWindow = false;

    iRect r(0, 15, getSizeX()-15, getSizeY()-31);
    ChatList = new tChatBox(r, 0);
    vsbChat = new tVScrollBar();
    hsbChat = new tHScrollBar();
    add(vsbChat);
    add(hsbChat);
    ChatList->setVscrollBar(vsbChat);
    ChatList->setHscrollBar(hsbChat);
    ChatList->setColor(0);
    ChatList->setAutoScroll(true);
    add(ChatList);
    ChatString.init("", 100, getSizeX()-5);
    input = addInputField(iXY(2, getSizeY()-16), &ChatString, "", true, 100);
    input->setExcludedCharacters("\\�`�");
}

void ChatView::doDraw(Surface &viewArea, Surface &clientArea)
{
    clientArea.bltLookup(viewArea.getRect(), Palette::brightness256.getColorArray());
    clientArea.bltString(35, 4, title, ctTexteNormal);
    View::doDraw(viewArea, clientArea);
}

void ChatView::processEvents()
{
    if (KeyboardInterface::getKeyPressed(SDLK_RETURN)||
            KeyboardInterface::getKeyPressed(SDLK_KP_ENTER))
    {
        if (strlen(input->getDestString())!= 0)
        {
            std::string msg = input->getDestString();
            if (msg[0] == '/')
            {
                if (!msg.compare(0,5,"/all "))
                {
                    ChatInterface::say(msg.substr(5));
                }
            }
            else
            {
                ChatInterface::teamsay(msg);
            }
            input->clearString();
        }
    }
#ifdef WIN32
    else if ( (KeyboardInterface::getKeyState(SDLK_LCTRL)
               || KeyboardInterface::getKeyState(SDLK_RCTRL))
              && (KeyboardInterface::getKeyPressed(SDLK_v) ))
    {
        OpenClipboard(NULL);
        HANDLE clip = GetClipboardData(CF_TEXT);
        CloseClipboard();
        if (clip)
        {
            char* pntchr = (char*)clip;
            int count = 0;
            while ((*pntchr != 0) && (count < 150))
            {
                if (isprint(*pntchr))
                {
                    input->addChar(*pntchr);
                    count++;
                }
                pntchr++;
            }
        }
    }
    else if ( (KeyboardInterface::getKeyState(SDLK_LCTRL)
               || KeyboardInterface::getKeyState(SDLK_RCTRL))
              && (KeyboardInterface::getKeyPressed(SDLK_c) ))
    {
        std::string str = ChatList->getTextItem();
        if (str != "")
        {
            if(OpenClipboard(NULL))
            {
                int pos = str.find_first_of(':')+2;
                std::string chaine = str.substr(pos);
                HGLOBAL hText = GlobalAlloc(GMEM_DDESHARE, chaine.length()+1);
                char * pText = (char*)GlobalLock(hText);
                strcpy(pText, chaine.c_str());
                GlobalUnlock(hText);
                EmptyClipboard();
                SetClipboardData(CF_TEXT, hText);
                CloseClipboard();
            }
        }
    }
#endif
    else
    {
        View::processEvents();
    }
}

void ChatView::onComponentClicked(Component* c)
{
    if ( c == bHideWindow )
    {
        HideWindow = true;
        bHideWindow->disable();
        bShowWindow->enable();
        
        resize(500, 17);
        moveTo(screen->getWidth()-getSizeX(), screen->getHeight()-getSizeY());
    }
    if ( c == bShowWindow )
    {
        HideWindow = false;
        bHideWindow->enable();
        bShowWindow->disable();
        
        resize(500, 150);
        moveTo(screen->getWidth()-getSizeX(), screen->getHeight()-getSizeY());
    }
    View::onComponentClicked(c);
}

void ChatView::postMessage(PIX msgcolor, bool hasFlag, FlagID flag, const char *format, ...)
{
    char temp_str[256];
    va_list vap;

    va_start( vap, format );
    vsnprintf( temp_str, 256, format, vap );
    va_end( vap );

    ChatList->AddChat(temp_str, msgcolor, hasFlag, flag);
}

void ChatView::checkResolution(iXY oldResolution, iXY newResolution)
{
    moveTo(screen->getWidth()-getSizeX(), screen->getHeight()-getSizeY());
}

void ChatView::doActivate()
{
    Desktop::setActiveView(this);
    View::selectedInputField = View::findInputFieldContaining(input->getPos());
}

void ChatView::clear()
{
    ChatList->Clear();
}

