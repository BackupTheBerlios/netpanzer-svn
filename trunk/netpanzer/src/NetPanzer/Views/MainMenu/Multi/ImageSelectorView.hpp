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


#ifndef __ImageSelectorView_hpp__
#define __ImageSelectorView_hpp__


// old code?!?
#if 0

#include "cView.hpp"
#include "Surface.hpp"
#include "iXY.hpp"


//---------------------------------------------------------------------------
class ImageSelectorView : public cView
{
protected:
    Surface images;             // All the images.
    int     topImage;           // The top image in the view.
    int     selectedImage;      // The currently selected image, -1 none.
    int     flagNoImagesLoaded; // Flag for no images loaded case.
    int     viewableImageCount; // How many images can we see on the screen?

    enum { GAP_YSPACE = 2 };  // Space between thumbnails.

    void reset();

    virtual void doDraw    (const Surface &windowArea, const Surface &clientArea);
    virtual void drawImages(const Surface &clientArea);
    virtual void lMouseDown(const iXY &pos);

public:
    ImageSelectorView();
    virtual ~ImageSelectorView()
    {}

    virtual void init(const iXY pos);

    int  getSelectedImage()
    {
        return selectedImage;
    }
    int  findImageContaining(const iXY pos);
    void setSelectedImageNone()
    {
        selectedImage = -1;
    }

    virtual void loadImages()
    {}

}
; // end ImageSelectorView
#endif

#endif // end __ImageSelectorView_hpp__
