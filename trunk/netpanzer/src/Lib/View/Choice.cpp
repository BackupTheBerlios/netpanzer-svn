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

#include <algorithm>
#include "Choice.hpp"
#include "View.hpp"

//---------------------------------------------------------------------------
void Choice::reset()
{
	index     = 0;
	minWidth  = 0;
	size.x    = 10;
	size.y    = ChoiceItemHeight;
	isOpen    = 0;
	adjustedY = 0;
}

//---------------------------------------------------------------------------
void Choice::addItem(const String &item)
{
	choiceList.setNum(choiceList.getCount() + 1);

	choiceList[choiceList.getCount() - 1] = item;

	int borderSpace = borderSize * 2;

	size.x = std::max(int(strlen(item) * CHAR_XPIX + borderSpace), size.y);
	size.y = ChoiceItemHeight;
}

//---------------------------------------------------------------------------
void Choice::addItemDefault(const String &item)
{
	choiceList.setNum(choiceList.getCount() + 1);

	choiceList[choiceList.getCount() - 1] = item;

	int borderSpace = borderSize * 2;

	size.x = std::max(int(strlen(item) * CHAR_XPIX + borderSpace), size.y);
	size.y = CHAR_XPIX + borderSpace;

	select(item);
}

//---------------------------------------------------------------------------
void Choice::insert(String item, int index)
{
//	for (int i = 0; i < choiceList.getCount(); i++)
//	{
//		if (strcmp((const char *) item, (const char *) choiceList[i]) == 0)
//		{
//			choiceList.removeByIndex(i);
//			
//			return;
//		}
//	}
}

//---------------------------------------------------------------------------
void Choice::remove(String item)
{
	for (int i = 0; i < choiceList.getCount(); i++)
	{
		if (strcmp((const char *) item, (const char *) choiceList[i]) == 0)
		{
			choiceList.removeByIndex(i);
			
			return;
		}
	}
}

//---------------------------------------------------------------------------
void Choice::remove(int index)
{
	choiceList.removeByIndex(index);
}

//---------------------------------------------------------------------------
void Choice::removeAll()
{
	choiceList.setNum(0);
}

//---------------------------------------------------------------------------
void Choice::select(const String &item)
{
	for (int i = 0; i < choiceList.getCount(); i++)
	{
		if (strcmp((const char *) item, (const char *) choiceList[i]) == 0)
		{
			index = i;
			
			return;
		}
	}
}

//---------------------------------------------------------------------------
void Choice::select(int index)
{
	Choice::index = index;
}

//---------------------------------------------------------------------------
void Choice::actionPerformed(const mMouseEvent &me)
{
	iXY parentDimensions(((View *)parent)->getClientRect().getSize());

	if (	me.getID() == mMouseEvent::MOUSE_EVENT_PRESSED &&
			(me.getModifiers() & InputEvent::BUTTON1_MASK))
	{
		//assert(isOpen == false);
		isOpen = true;

		// Set the size to accomodate all items.
		size.y = choiceList.getCount() * ChoiceItemHeight;

		// Make sure the choice fits on the screen.
		if (min.y + size.y >= parentDimensions.y)
		{
			// Check to see if it will fit above the base position.
			if ((min.y + ChoiceItemHeight) - size.y > 0)
			{
				// Since it fits above, put it there.
				adjustedY =  size.y - ChoiceItemHeight;
				min.y     -= adjustedY;

			} else
			{
				// Since it does not fit above, just put it below and keep it on the screen.
				adjustedY =  min.y + size.y - parentDimensions.y;
				min.y     -= adjustedY + 1;
			}


			// Make sure the choice is still on the screen.
			if (min.y < 0)
			{
				assert(false);
			}
		}
		assert(min + size < parentDimensions);
	}
	else if (	me.getID() == mMouseEvent::MOUSE_EVENT_DRAGGED &&
				(me.getModifiers() & InputEvent::BUTTON1_MASK))
	{
		isOpen = true;

		iRect r(min.x, min.y, min.x + size.x, min.y + ChoiceItemHeight);
		
		for (int i = 0; i < choiceList.getCount(); i++)
		{
			// Find the selected item.
			if (r.contains(iXY(me.getX(), me.getY())))
			{
				index = i;
				break;
			}

			r.translate(iXY(0, ChoiceItemHeight));
		}
		assert(min + size < parentDimensions);
	}
	else if (	me.getID() == mMouseEvent::MOUSE_EVENT_CLICKED &&
				(me.getModifiers() & InputEvent::BUTTON1_MASK))
	{
		//assert(isOpen);
		isOpen = false;
		
		// Set the size back down to a single item.
		size.y = ChoiceItemHeight;
		
		// Move the choice back to its original location.
		min.y += adjustedY;
		adjustedY = 0;

		// Since an item was selected, find which item was selected.
		assert(min + size < parentDimensions);
	}
	else if (	me.getID() == mMouseEvent::MOUSE_EVENT_RELEASED &&
				(me.getModifiers() & InputEvent::BUTTON1_MASK))
	{
		//assert(!isOpen);
		isOpen = false;
		
		// Set the size back down to a single item.
		size.y = ChoiceItemHeight;
		
		// Move the choice back to its original location.
		min.y += adjustedY;
		adjustedY = 0;
		assert(min + size < parentDimensions);
	}

}

// draw
//---------------------------------------------------------------------------
void Choice::draw(const Surface &dest)
{
	iXY parentDimensions(((View *)parent)->getClientRect().getSize());

	Surface s;
	iRect   r;

	// Draw the label.
	iXY pos;

	//pos.x = min.x - strlen((const char *) label) * CHAR_XPIX - CHAR_XPIX;
	pos.x = min.x;
	pos.y = min.y - CHAR_YPIX - 4;

	// Draw the name of the choice.
	dest.bltStringShadowed(min.x, pos.y + adjustedY, (const char *) name, Color::white, Color::black);

	getBounds(r);

	s.setTo(dest, r);
	
	// Draw the border.
	s.drawRect(iRect(0, 0, s.getPixX() - 2, s.getPixY() - 2), Color::gray96);
	s.drawRect(iRect(1, 1, s.getPixX() - 1, s.getPixY() - 1), Color::white);
	s.fillRect(iRect(1, 1, s.getPixX() - 2, s.getPixY() - 2), Color::terreVerte);

	if (!isOpen)
	{
		s.bltStringShadowedCenter(choiceList[index], Color::white, Color::black);
	} else
	{
		r = iRect(min.x, min.y, min.x + size.x, min.y + ChoiceItemHeight);
		
		int count = choiceList.getCount();

		for (int i = 0; i < count; i++)
		{
			s.setTo(dest, r);

			if (i == index)
			{
				// Higlight the selected item.
				s.fill(Color::white);
				s.bltStringCenter(choiceList[i], Color::black);

			} else
			{
				s.bltStringShadowedCenter(choiceList[i], Color::white, Color::black);
			}

			assert(r.max < parentDimensions);
			r.translate(iXY(0, ChoiceItemHeight));
		}
	}

	//isOpen = 0;

} // end Choice::draw

// add
//---------------------------------------------------------------------------
void Choice::add(const String &item)
{
	choiceList.setNum(choiceList.getCount() + 1);

	choiceList[choiceList.getCount() - 1] = item;

	int borderSpace = borderSize * 2;
	int length      = strlen((const char *) item);

	size.x = std::max(int(length * CHAR_XPIX + borderSpace), size.y);
	size.x = std::max(minWidth, size.x);

} // end Choice::add

// setMinWidth
//---------------------------------------------------------------------------
void Choice::setMinWidth(int minWidth)
{
	Choice::size.x   = minWidth;
	Choice::minWidth = minWidth;

} // end Choice::setMinWidth

// copyItems
//---------------------------------------------------------------------------
void Choice::copyItems(const Choice &choice)
{
	choiceList.setNum(choice.choiceList.getCount());

	for (int i = 0; i < choice.choiceList.getCount(); i++)
	{
		choiceList[i] = choice.choiceList[i];
	}

	index = choice.getSelectedIndex();

} // end Choice::copyItems
