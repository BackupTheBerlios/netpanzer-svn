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


#include "ImageSelectorView.hpp"
#include "cViewManager.hpp"
#include "iRect.hpp"


// ImageSelectorView
//---------------------------------------------------------------------------
ImageSelectorView::ImageSelectorView() : cView()
{
	setTitle("Image Selector View");
	setAllowResize(false);
	setDisplayStatusBar(false);
	setVisible(false);
	setAllowMove(false);
	setBordered(false);

	reset();

} // end ImageSelectorView::ImageSelectorView

// reset
//---------------------------------------------------------------------------
void ImageSelectorView::reset()
{
	topImage           =  0;
	selectedImage      = -1;
	flagNoImagesLoaded =  0;
} // end ImageSelectorView::reset

// init
//---------------------------------------------------------------------------
void ImageSelectorView::init(const iXY pos)
{
	loadImages();

	// Adjust the cView size (Must be done after we get the images).
	viewableImageCount = (images.getFrameCount() < (SCREEN_YPIX - 23) / images.getPixY()) ? images.getFrameCount() : (SCREEN_YPIX - 23) / images.getPixY();

	// If there are no images to be loaded, display a window which
	// says that there are no images loaded, else display the images.
	if (viewableImageCount == 0)
	{
		iXY size;
		size.x = images.getPixX() + cView::SCROLL_BAR_XSIZE;

		moveTo(pos);
		//resize(iXY(size.x + scrollBar.getXSize(), size.y + 30));
		resize(iXY(size.x, SCREEN_YPIX - 23));

		flagNoImagesLoaded = 1;
		setScrollBar(false);
	} else
	{
		iXY size;
		size.x = images.getPixX() + cView::SCROLL_BAR_XSIZE;

		moveTo(pos);
		//resize(iXY(size.x + scrollBar.getXSize(), size.y + 30));
		resize(iXY(size.x, SCREEN_YPIX - 23));

		//setScrollBar(true);
		setScrollBarViewableCount(viewableImageCount);
		setScrollBarItemCount(images.getFrameCount());
		setScrollBarItemsSize(images.getPix().y);

		flagNoImagesLoaded = 0;
	}

} // end init

// doDraw
//---------------------------------------------------------------------------
void ImageSelectorView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	viewArea.fill(Color::black);
	
	// If no images are loaded, display the background saying that
	// there are no images loaded, else draw the loaded images.
	if (flagNoImagesLoaded)
	{
		viewArea.bltStringCenter("No Images.", Color::white);

	} else
	{
		drawImages(viewArea);
	}

	cView::doDraw(viewArea, clientArea);

} // end doDraw

// drawImages
//---------------------------------------------------------------------------
void ImageSelectorView::drawImages(const Surface &dest)
{
	for (int i = getScrollBarTopItem(); i < getScrollBarTopItem() + viewableImageCount; i++)
	{
		images.setFrame(i);

		// Draw a rectangle over each visible image.
		if (i == selectedImage)
		{
			iRect r(images.getRect());

			images.drawRect(r, Color::green);
			//images.drawRect(r.min + 1, r.max - 1, pal.cRED);
		} else
		{
			//images.drawButtonBorder(Color::gray64, Color::gray64);
		}

		int newY = ((i - getScrollBarTopItem()) * (images.getPixY() + 2));

		images.blt(dest, 0, newY);

		// Display the name of this shape.
		//bltStringShadowed(dest, offset.x, newY, shapes[i].getName(), pal.cRED, pal.cBLUE);
	}
} // end drawImages

// findShapeContaining
//---------------------------------------------------------------------------
int ImageSelectorView::findImageContaining(const iXY pos)
{
	// Go through the viewable images to find the one we are on.
	for (int i = 0; i < viewableImageCount; i++)
	{
		if (pos.y > (images.getPixY() + 1) * i && pos.y < (images.getPixY() + 1) * (i + 1) &&
			pos.x < (images.getPixX() + 1))
		{
			return getScrollBarTopItem() + i;
		}
	}
	
	return -1;

} // end findImageContaining

// lMouseDown
//---------------------------------------------------------------------------
void ImageSelectorView::lMouseDown(const iXY &pos)
{
	cView::lMouseDown(pos);

	if (getPressedButton() >= 0) return;
	
	selectedImage = findImageContaining(pos);

} // end lMouseDown
