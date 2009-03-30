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

#include <algorithm>

#include "Resources/ResourceManager.hpp"

#include "FlagSelectionView.hpp"
#include "Views/Game/GameView.hpp"
#include "Views/GameViewGlobals.hpp"
#include "Util/Exception.hpp"
#include "Interfaces/GameConfig.hpp"
#include "Views/Components/Button.hpp"
#include "Views/Components/MouseEvent.hpp"
#include "2D/Color.hpp"
#include "2D/Surface.hpp"
#include "Util/FileSystem.hpp"

using namespace std;

string FlagSelectionView::playerFlagSelected = "";
Surface FlagSelectionView::playerFlag;

#define FLAGBUTTONCODE 0xabba

// FlagSelectionView
//---------------------------------------------------------------------------
FlagSelectionView::FlagSelectionView() : RMouseHackView()
{
    setSearchName("FlagSelectionView");
    setTitle("Flag Selection");
    setSubTitle("");

    setAllowResize(false);
    setAllowMove(false);
    setVisible(false);

    // The thirty is to give more room to the map selectior view.
    moveTo(bodyTextRect.min.x + bodyTextRect.getSizeX() / 2 + 10 + 30,
           bodyTextRect.min.y + 50);

    resize(bodyTextRect.getSizeX() / 2 - 10 - 30, 108);

    init();

} // end FlagSelectionView::FlagSelectionView

// init
//---------------------------------------------------------------------------
void FlagSelectionView::init()
{
    iXY flagStartOffset(BORDER_SPACE, BORDER_SPACE * 2 + 14); // 14 is the flag height

    int yOffset = 14 + 2; // 14 is the flag height

    int x = flagStartOffset.x;
    int y = flagStartOffset.y;

    char** list = filesystem::enumerateFiles(DEFAULT_FLAGS_PATH);
    
    vector<string> filenames;
    string flagname;
    
    for(char** file = list; *file != 0; file++)
    {
        flagname = *file;
        if(flagname.find(".bmp") != string::npos)
        {
            filenames.push_back(flagname);
        }
    }

    filesystem::freeList(list);

    sort(filenames.begin(), filenames.end());
    Surface buttonimg;

    // Now load in the sorted BMP names.
    for (unsigned int i = 0; i < filenames.size(); i++)
    {
        LOGGER.warning("Flag '%s'", filenames[i].c_str());
        //flag.loadBMP(filenames[i].c_str());
        //flag.mapFromPalette("netp");
        try
        {
            string fullname(DEFAULT_FLAGS_PATH);
            fullname += filenames[i];
            buttonimg.loadBMP(fullname.c_str());

            Button * newButton = new Button(filenames[i]);
            newButton->setCustomCode(FLAGBUTTONCODE);
            newButton->setLocation(x, y);
            newButton->setImage(buttonimg);
            newButton->setRedGreenBorder();

            add(newButton);

            x += 20 + 2; // 20 is the with of flag

            if (x > flagStartOffset.x + getClientRect().getSizeX() - BORDER_SPACE - 20) // 20 is the width of flag
            {
                x = flagStartOffset.x;
                y += yOffset;
            }
        }
        catch (Exception e)
        {
            LOGGER.warning("Error loading flag '%s'", filenames[i].c_str());
        }
    }
    
    playerFlagSelected = gameconfig->playerflag;
    
    setSelectedFlag(playerFlagSelected);
    
} // end FlagSelectionView::init

// doDraw
//---------------------------------------------------------------------------
void FlagSelectionView::doDraw()
{
    //iRect r(getViewRect());
    //viewArea.bltLookup(r, Palette::darkGray256.getColorArray());

    char strBuf[256];
    sprintf(strBuf, "Current:");
    int CHAR_XPIX = 8; // XXX hardcoded
    
    drawStringShadowed(
            BORDER_SPACE,
            BORDER_SPACE + (playerFlag.getHeight() - Surface::getFontHeight()) / 2,
            strBuf, windowTextColor, windowTextColorShadow);
    
    drawImage(playerFlag, strlen(strBuf) * CHAR_XPIX + (BORDER_SPACE*2), BORDER_SPACE);


    View::doDraw();

} // end FlagSelectionView::doDraw

void
FlagSelectionView::setSelectedFlag(string &fname)
{
    string fullfile(DEFAULT_FLAGS_PATH);
    fullfile += fname;
    try
    {
        playerFlag.loadBMP(fullfile.c_str());
        playerFlagSelected = fname;
    }
    catch (Exception &e)
    {
        LOGGER.warning("Error loading flag '%s', using blank flag", fname.c_str());
        playerFlag.copy(*ResourceManager::getEmptyImage());
    }
}

void
FlagSelectionView::onComponentClicked(Component* c)
{
    if ( c->getCustomCode() == FLAGBUTTONCODE )
    {
        string n = c->getName();
        string fname( n.substr( (n.find('.'))+1 ) );
        setSelectedFlag( fname );
    }
}
