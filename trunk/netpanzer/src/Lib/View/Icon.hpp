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

#ifndef __Icon_hpp__
#define __Icon_hpp__


#if _MSC_VER > 1000
	#pragma once
#endif


#include "String.hpp"
#include "Surface.hpp"


//--------------------------------------------------------------------------
class Icon : public Surface
{
protected:
	Surface image;
	String  description;

public:
	Icon(const Surface &image, const String &description) {}
	Icon(const Surface &image) {}
	Icon() {}
	virtual ~Icon() {}

	const String  &getDescription() const { return description; }

	int getIconHeight() const { return image.getPixY(); }
	int getIconWidth() const { return image.getPixX(); }

	const Surface &getImage() const { return image; }

	void setDescription(const String &decription) { Icon::description = description; }
	void setImage(const Surface &image) {}

}; // end Icon

#endif // end __Icon_hpp__
