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

#include "IPAddressView.hpp"
#include "Desktop.hpp"
#include "GameViewGlobals.hpp"

#include "Client.hpp"

cInputFieldString IPAddressView::szServer;


static void buttonOkay(void)
{
    ConnectionAddress address;
    char * szServerName;

    szServerName = (char *)IPAddressView::szServer.getString();

    strcpy( address.string_rep, szServerName );

    // XXX No server info view yet
#if 0
    ret_val = CLIENT->startEnumeration( address );
    ret_val = 1;

    if(ret_val == 1) {
        Desktop::setVisibility("IPAddressView", false);

        Desktop::setVisibility("JoinView", true);
        Desktop::setVisibility("GetSessionHostView", true);
        Desktop::setVisibility("UnitSelectionView", true);
        Desktop::setVisibility("FlagSelectionView", true);
        Desktop::setVisibility("PlayerNameView", true);

    } else {
        CLIENT->closeSession();
        Desktop::setVisibilityAllWindows(false);
        Desktop::setVisibility("GetSessionView", true);
    }
#endif
}
////////////////////////////////////////////
//END buttonOkay////////////////////////////


static void buttonCancel(void)
{
    CLIENT->closeSession();
    Desktop::setVisibilityAllWindows(false);
    Desktop::setVisibility("GetSessionView", true);

}
////////////////////////////////////////////
//END buttonCancel//////////////////////////



// IPAddressView
//---------------------------------------------------------------------------
IPAddressView::IPAddressView() : View()
{
    setSearchName("IPAddressView");
    setTitle("Server IP Address");
    setSubTitle("");

    setAllowResize(false);
    setAllowMove(false);
    setVisible(false);

    moveTo(iXY(20,70));

    iXY  area_size = iXY(280, 110);
    resizeClientArea(area_size);

    addButtonCenterText(iXY(185, 10 ), 80,  "Okay", "", buttonOkay);
    addButtonCenterText(iXY(185, 35 ), 80,  "Cancel", "", buttonCancel);

    //addButtonCenterText(iXY(16, 50 ), area_size.x - 40,  "Cancel", "", buttonCancel);

    Init();
} // end IPAddressView constructor



void IPAddressView::Init()
{
    szServer.init("  ", 100);
    addInputField(iXY(10, 80), &szServer, "", true);
} // end PlayerNameView::init




// doDraw
//---------------------------------------------------------------------------
void IPAddressView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
    clientArea.fill(Color::black);
    View::doDraw(viewArea, clientArea);
} // end IPAddressView::doDraw
