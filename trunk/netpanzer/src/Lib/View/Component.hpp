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
#ifndef __Component_hpp__
#define __Component_hpp__

#include "Cursor.hpp"
#include "Color.hpp"
#include "Surface.hpp"
#include "iXY.hpp"
#include "iRect.hpp"
#include "String.hpp"
#include "NoCopy.hpp"

class mMouseEvent;

//--------------------------------------------------------------------------
class Component : public NoCopy
{
protected:
	BYTE    background;
	BYTE    foreground;
	iXY     size;
	iXY     min;
	Surface surface;
	Cursor  cursor;
	String  name;
	bool    enabled;
	bool    visible;

	static int borderSize;

	void reset();

public:
	Component *next;
	Component *prev;

	void *parent;       // Who is my daddy?

	void setParent(void *parent);

	enum
	{
		BOTTOM_ALIGNMENT,
		CENTER_ALIGNMENT,
		LEFT_ALIGNMENT,
		RIGHT_ALIGNMENT,
		TOP_ALIGNMENT,
	};

	Component()
	{
		reset();
	}
	Component(int i)
	{
		reset();
	}
	virtual ~Component() {}

	// Accessor functions.
	bool contains(int x, int y) const;
	inline bool            contains(iXY p) const { return contains(p.x, p.y); }
	inline        BYTE     getBackground() const { return background; }
	//inline const  iRect   &getBounds() { return iRect(min, min + size); }
	inline void getBounds(iRect &r)
	{
		r.min = min;
		r.max = min + size;
	}
	inline const  Cursor  &getCursor() const { return cursor; }
	inline        BYTE     getForeground() const { return foreground; }
	inline        Surface  getGraphics() const { return surface; }
	inline        String   getName() const { return name; }
	inline const  iXY     &getSize() const { return size; }
	inline int             getSizeX() const { return size.x; }
	inline int             getSizeY() const { return size.y; }
	inline        bool     isEnabled() const { return enabled; }
	//const bool &isValid() {}
	inline        bool     isVisible() const { return visible; }

	void setBackground(BYTE color) {}
	void setBounds(const iRect &r)
	{
		min  = r.min;
		size = r.getSize();
	}

	void setCursor() {}
	void setEnabled(bool enabled);
	void setForeground(PIX foreground);
	void setLocation(int x, int y);
	inline void setLocation(const iXY &p) { setLocation(p.x, p.y); }
	void setName(const String &name) { Component::name = name; }
	void setSize(int width, int height) {}
	inline void setSize(const iXY &d) { setSize(d.x, d.y); }
	void setVisible(bool b) {}

	virtual void draw(const Surface &dest) {}
	virtual void actionPerformed(const mMouseEvent &me) {}

}; // end Component

#endif // end __Component_hpp__
