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

#ifndef __GetSessionHostView_hpp__
#define __GetSessionHostView_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif

#include "View.hpp"
#include "Surface.hpp"
#include "cGrowList.hpp"
#include "ScrollBar.hpp"


// Used for sorting a shape.
int cHostCompareName(const void *elem1, const void *elem2);

//---------------------------------------------------------------------------
class cHost
{
public:
	cHost()
	{
	}
	~cHost() {}

	inline const char *getName() { return name; }

public:
	char name[64];
    char map[64];
    char game_type[64];
    int  current_players;
    int  max_players;
}; // end cHost


//---------------------------------------------------------------------------
class GetSessionHostView : public View
{
private:
	cGrowList<cHost> hosts;

	int viewableItemCount;
	int highlightedItem;
	int selectedItem;
	enum { ITEM_GAP_SPACE = 4 };
	
	int findItemContaining(const iXY &pos);

	int  maxYOffset;

	ScrollBar *scrollBar;

	enum { TEXT_GAP_SPACE = 2 };

	Button upButton;
	Button downButton;

	int maxViewableItems;
	int topViewableItem;

public:
	GetSessionHostView();
	virtual ~GetSessionHostView()
	{
		delete scrollBar;
	}


	void drawHostList(const Surface &dest);
	void updateHostList();

	virtual void doDraw(const Surface &windowArea, const Surface &clientArea);
	virtual void lMouseDown(const iXY &pos);
	virtual void actionPerformed(mMouseEvent me);
	virtual void mouseMove(const iXY & prevPos, const iXY &newPos);

}; // end GetSessionHostView

#endif // end __GetSessionHostView_hpp__
