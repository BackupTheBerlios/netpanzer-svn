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


#if _MSC_VER > 1000
	#pragma once
#endif


#include "stdafx.hpp"
#include "Desktop.hpp"
#include "KeyboardInterface.hpp"
#include "loadPics.hpp"
#include "TimerInterface.hpp"
#include "MouseInterface.hpp"


float     Desktop::totalMouseDownTime   = 0.0;
float     Desktop::currentMouseDownTime = 0.0;
View     *Desktop::top;
View     *Desktop::focus;
int       Desktop::mouseActions;
iXY       Desktop::lMouseDownPos;
iXY       Desktop::rMouseDownPos;
iXY       Desktop::prevMousePos;
iXY       Desktop::mouseActionOffset;
int       Desktop::prevButton;
int       Desktop::mouseIsDown;
View     *Desktop::mouseView;
View     *Desktop::prevMouseView;
View     *Desktop::lMouseView;
View     *Desktop::rMouseView;
TIMESTAMP Desktop::lDoubleClickDeadline;
TIMESTAMP Desktop::rDoubleClickDeadline;
float     Desktop::doubleClickTime = 0.15f;
int       Desktop::mouseMoveStatus = 0;


/////////////////////////////////////////////////////////////////////////////
// Class definitions.
/////////////////////////////////////////////////////////////////////////////

// Desktop constructor
//--------------------------------------------------------------------------
// Purpose:
//--------------------------------------------------------------------------
Desktop::Desktop()
{
	top          = NULL;
	focus        = NULL;
	mouseActions = 0;
	prevButton   = 0;

	// Set the double click deadlines to 1 minute ago, so we don't
	// double click on the first press
	lDoubleClickDeadline = rDoubleClickDeadline = now() - 60;
} // end Desktop constructor

// manage
//--------------------------------------------------------------------------
// Purpose: Manages the window manager.  Check for button presses, actions
//          occur, which window to occur in, etc.
//--------------------------------------------------------------------------
void Desktop::manage(int mouseX, int mouseY, int curButton)
{
	iXY mousePos(mouseX, mouseY);
	
	MouseInterface::setCursor(_mcursor_default);

	prevMouseView = mouseView;

	mouseView = findViewContaining(mousePos);

	// Check for mouseEnter and mouseExit.
	if (mouseView != prevMouseView)
	{
		if (mouseView != NULL)
		{
			mouseView->mouseEnter(mouseView->getScreenToClientPos(mousePos));
		}
		if (prevMouseView != NULL)
		{
			prevMouseView->mouseExit(prevMouseView->getScreenToClientPos(mousePos));
		}
	}
	
	if (mouseView != NULL)
	{
		mouseView->mouseMove(mouseView->getScreenToClientPos(prevMousePos), mouseView->getScreenToClientPos(mousePos));
	}

  //////////////////////////////////////////////////////////////////////////
  //
  // handle left mouse button
  //
  //////////////////////////////////////////////////////////////////////////

	if (curButton & LMOUSE_BUTTON_MASK)
	{
		//LOG_BLOCK(("Button is down"));

		// The button is down.  See if it just got pressed, or if it was pressed
		// before and is being dragged.
		if (!(prevButton & LMOUSE_BUTTON_MASK))
		{
			//LOG_BLOCK(("Button just got pressed"));

			// The mouse button just got pressed. Remember the initial place
			// where it got pressed.
			lMouseDownPos = mousePos;

			// Is this a double click? **FIX THE DOUBLE CLICK SENSITIVITY**
			if (lMouseView != NULL)
			{
				int shit = 123456789;

			} else
			{
				//LOG_BLOCK(("Not double click"));

				// Not a double click.  See what window we're over
				lMouseView = mouseView;
				if (lMouseView != NULL)
				{

					//LOG_BLOCK(("Setting lMouseView"));

					lMouseView->lMouseDown(lMouseView->getScreenToClientPos(mousePos));
					if (focus != lMouseView)
					{
						activate(lMouseView);
					}
					// We are over a window.  Get the standard mouse actions for that
					// window at this mouse position.

					// mouseActionOffset is relative to the current window
					mouseActionOffset = mousePos - lMouseView->min;
					mouseActions      = lMouseView->getMouseActions(mousePos-lMouseView->min);
					if (mouseActions != 0)
					{
						// A standard mouse action is supposed to happen here.  Remember
						// what that action is, and record the offset of the mouse from
						// the window, so the distance we move the mouse is the distance
						// the window gets resized, etc. exactly.

						if (lMouseView->status & View::STATUS_ALLOW_RESIZE)
						{
							if (mouseActions & View::MA_RESIZE_LEFT)
							{
								mouseActionOffset.x = mousePos.x - lMouseView->min.x;
							}
							else if (mouseActions & View::MA_RESIZE_RIGHT)
							{
								mouseActionOffset.x = lMouseView->max.x - mousePos.x;
							}

							if (mouseActions & View::MA_RESIZE_TOP)
							{
								mouseActionOffset.y = mousePos.y - lMouseView->min.y;
							}
							else if (mouseActions & View::MA_RESIZE_BOTTOM)
							{
								mouseActionOffset.y = lMouseView->max.y - mousePos.y;
							}
						}

						doMouseActions(mousePos);
						
					} else
					{
						// No standard mouse actions.  Tell the window that the mouse
						// just got pressed.
					}
				}
			}
	  	} else
		{
			//LOG_BLOCK(("still down"));

			// The mouse was down before and is still down.  Are we performing
			// and standard window actions?

			if (lMouseView != NULL && mouseActions != 0)
			{
				//FUBAR("Double click!");

				// Yes - it's a double click.  Inform the window as such.
				//lMouseView->lMouseDouble(lMouseView->getScreenToClientPos(mousePos));

				// We're doing some standard mouse actions.  Do them.
				doMouseActions(mousePos);
			} else
			  {
					//currentMouseDownTime += TimerInterface::getTimeSlice();
					//totalMouseDownTime   += TimerInterface::getTimeSlice();
					//
					//if (totalMouseDownTime > 2.0f && totalMouseDownTime < 0.5f)
					//{
					//	if (currentMouseDownTime > 0.4f)
					//	{
					//		currentMouseDownTime = 0.0f;
					//		lMouseView->lMouseUp(lMouseView->getScreenToClientPos(lMouseDownPos), lMouseView->getScreenToClientPos(mousePos));
					//		lMouseView->lMouseDown(lMouseView->getScreenToClientPos(mousePos));
					//	}
					//}
					//else if (totalMouseDownTime >= 5.0f)
					//{
					//	if (currentMouseDownTime > 0.1f)
					//	{
					//		currentMouseDownTime = 0.0f;
					//		lMouseView->lMouseUp(lMouseView->getScreenToClientPos(lMouseDownPos), lMouseView->getScreenToClientPos(mousePos));
					//		lMouseView->lMouseDown(lMouseView->getScreenToClientPos(mousePos));
					//	}
					//}

					//LOG_BLOCK(("Not doing mouse actions - lMouseView=%p, mouseActions=%d", lMouseView, mouseActions));

					// No standard mouse actions.  Tell the window where the mouse
					// was originally clicked that the mouse is being dragged over
					// it.
					if (lMouseView != NULL)
					{
						lMouseView->lMouseDrag(lMouseView->getScreenToClientPos(lMouseDownPos), lMouseView->getScreenToClientPos(prevMousePos), lMouseView->getScreenToClientPos(mousePos));
					}
				}
		}
  } else
	{
		// The mouse button is up.
		//See if it just got released, or if it was up before.
		if (prevButton & LMOUSE_BUTTON_MASK)
		{
			// The mouse button just got released.  If it was on top of a window
			// before, then tell the window that the button was released
			if (lMouseView != NULL)
			{
				lMouseView->lMouseUp(lMouseView->getScreenToClientPos(lMouseDownPos), lMouseView->getScreenToClientPos(mousePos));
			}
			// Set double click deadline
			lDoubleClickDeadline = now() + doubleClickTime;
		}

		//LOG(("Button released - clearing lMouseView and mouseActions"));
		lMouseView = NULL;
		mouseActions = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Handle right mouse button.
	//
	//////////////////////////////////////////////////////////////////////////

	if (curButton & RMOUSE_BUTTON_MASK)
	{
		// The button is down.  See if it just got pressed, or if it was pressed
		// before and is being dragged.
		if (!(prevButton & RMOUSE_BUTTON_MASK))
		{
			// The mouse button just got pressed. Remember the initial place
			// where it got pressed.
			rMouseDownPos = mousePos;

			// Is this a double click? **FIX THE DOUBLE CLICK SENSITIVITY**
			if (rMouseView != NULL && now() <= rDoubleClickDeadline && 0)
			{
				 //FUBAR("Double click!");

				 // Yes - it's a double click.  Inform the window as such.
				 rMouseView->rMouseDouble(rMouseView->getScreenToClientPos(mousePos));
			} else
			{
				// Not a double click.  See what window we're over.
				rMouseView = mouseView;
				if (rMouseView != NULL)
				{
					// No standard mouse actions.  Tell the window that the mouse
					// just got pressed.
					rMouseView->rMouseDown(rMouseView->getScreenToClientPos(mousePos));
				}
			}
  	} else
      {
        // The mouse was down before and is still down.

        // No standard mouse actions.  Tell the window where the mouse
        // was originally clicked that the mouse is being dragged over
        // it.
        if (rMouseView != NULL) {
           rMouseView->rMouseDrag(rMouseView->getScreenToClientPos(rMouseDownPos), rMouseView->getScreenToClientPos(prevMousePos), rMouseView->getScreenToClientPos(mousePos));
      }
    }
	} else
    {
			// The mouse button is up.
      //See if it just got released, or if it was up before.
      if (prevButton & RMOUSE_BUTTON_MASK)
			{
         // The mouse button just got released.  If it was on top of a window
         // before, then tell the window that the button was released
         if (rMouseView != NULL)
         {
           rMouseView->rMouseUp(rMouseView->getScreenToClientPos(rMouseDownPos), rMouseView->getScreenToClientPos(mousePos));
         }

         // Set double click deadline
         rDoubleClickDeadline = now() + doubleClickTime;
      }
    rMouseView = NULL;
  }

  //if (curButton & 2)
  //{
  //  // Set off that a right single click was made
  //  mouseView = findViewContaining(mousePos);
  //  mouseView->rMouseDown(mouseView->getScreenToClientPos(mousePos));
  //} else
  //  {
  //    // The mouse button 2 is up.
  //    mouseView = findViewContaining(mousePos);
  //    mouseView->rMouseUp();
  //  }

	// Send a message to the top (active) window of the position 
	// of the mouse.
	//if (focus != NULL)
		//focus->mouseMove(focus->getScreenToClientPos(prevMousePos), focus->getScreenToClientPos(mousePos));


	//{
	//  LOG_BLOCK(("Window stack:"));
	//	for (View *w = top ; w ; w = w->next)
	//		LOG(("%s", w->title));
	//}

	prevButton   = curButton;
	prevMousePos = mousePos;

	//mouseView = findViewContaining(mousePos);
	unsigned effActions = mouseActions;
	if (mouseView != NULL && effActions == 0) {
		effActions = mouseView->getMouseActions(mousePos-mouseView->min);
	}

        Surface *pointer = &mouseArrow;

	//if (mouseView->status & View::STATUS_ALLOW_RESIZE && mouseView->status & View::STATUS_ACTIVE)
	//{
	//	switch (effActions)
	//	{
	//		case (View::MA_RESIZE_TOP   ): pointer = &mouseResizeUD; break;
	//		case (View::MA_RESIZE_BOTTOM): pointer = &mouseResizeUD; break;
	//		case (View::MA_RESIZE_LEFT  ): pointer = &mouseResizeLR; break;
	//		case (View::MA_RESIZE_RIGHT ): pointer = &mouseResizeLR; break;

	//		case (View::MA_RESIZE_TOP   +View::MA_RESIZE_RIGHT): pointer = &mouseResizeTR; break;
	//		case (View::MA_RESIZE_TOP   +View::MA_RESIZE_LEFT ): pointer = &mouseResizeTL; break;
	//		case (View::MA_RESIZE_BOTTOM+View::MA_RESIZE_LEFT ): pointer = &mouseResizeBL; break;
	//		case (View::MA_RESIZE_BOTTOM+View::MA_RESIZE_RIGHT): pointer = &mouseResizeBR; break;
	//	}
//}

/*	mouseMoveStatus = 0;

	const int moveGapSpace = 1;
	if      (mousePos.x < moveGapSpace)                mouseMoveStatus |= MM_LEFT;
	else if (mousePos.x > SCREEN_XSIZE - moveGapSpace) mouseMoveStatus |= MM_RIGHT;
  	if      (mousePos.y < moveGapSpace)                mouseMoveStatus |= MM_UP;
  	else if (mousePos.y > SCREEN_YSIZE - moveGapSpace) mouseMoveStatus |= MM_DOWN;

	switch(mouseMoveStatus)
	{
		// Check to see if the mouse if at the bounds of the world.
		case (MM_LEFT  + MM_UP):   
		{
			if (mainViewMin.x <= 0 && mainViewMin.y <= 0)
			{
				pointer = &mouseMoveUpLeftStop; 
			} else pointer = &mouseMoveUpLeft; 
		} break;
		case (MM_LEFT  + MM_DOWN):
		{
			if (mainViewMin.x <= 0 && 
				mainViewMin.y + mainViewSize.y >= mapLayers.getTotalPix().y - 1)
			{
				pointer = &mouseMoveDownLeftStop; 
			} else pointer = &mouseMoveDownLeft;
		} break;
		case (MM_RIGHT + MM_UP):
		{
			if (mainViewMin.x + mainViewSize.x >= mapLayers.getTotalPix().x - 1 && 
				mainViewMin.y <= 0)
			{
				pointer = &mouseMoveUpRightStop; 
			} else pointer = &mouseMoveUpRight;
		} break;
		case (MM_RIGHT + MM_DOWN):
		{
			if (mainViewMin.x + mainViewSize.x >= mapLayers.getTotalPix().x - 1 && 
				mainViewMin.y + mainViewSize.y >= mapLayers.getTotalPix().y - 1)
			{
				pointer = &mouseMoveDownRightStop; 
			} else pointer = &mouseMoveDownRight;
		} break;
		case (MM_LEFT):
		{
			if (mainViewMin.x <= 0)
			{
				pointer = &mouseMoveLeftStop;
			}
			else pointer = &mouseMoveLeft;
		} break;
		case (MM_RIGHT):
		{
			if (mainViewMin.x + mainViewSize.x >= mapLayers.getTotalPix().x - 1)
			{
				pointer = &mouseMoveRightStop;
			}
			else pointer = &mouseMoveRight;
		} break;
		case (MM_UP):
		{
			if (mainViewMin.y <= 0)
			{
				pointer = &mouseMoveUpStop;
			}
			else pointer = &mouseMoveUp;
		} break;
		case (MM_DOWN):
		{
			if (mainViewMin.y + mainViewSize.y >= mapLayers.getTotalPix().y - 1)
			{
				pointer = &mouseMoveDownStop;
			}
			else pointer = &mouseMoveDown;
		} break;
		default: break;
	}
*/	
/*	if      (mousePos.x < moveGapSpace)
	{ 
		pointer = &mouseMoveLeft; mouse.setPointer(pointer);
	}
	else if (mousePos.y < moveGapSpace)
	{ 
		pointer = &mouseMoveUp; mouse.setPointer(pointer);
	}
	else if (mousePos.x > SCREEN_XSIZE - moveGapSpace)
	{ 
		pointer = &mouseMoveRight; mouse.setPointer(pointer);
	}
	else if (mousePos.y > SCREEN_YSIZE - moveGapSpace)
	{ 
		pointer = &mouseMoveDown; mouse.setPointer(pointer);
	}
*/
	//mouse.setPointer(pointer);

    if (focus != NULL)
    {
		focus->processEvents();
		KeyboardInterface::flushCharBuffer();
    }

} // end manage

// draw
//--------------------------------------------------------------------------
// Purpose:
//--------------------------------------------------------------------------
void Desktop::draw()
{
	const size_t MAX_WINDOWS = 512;

	size_t viewCount = 0;
	View *list[MAX_WINDOWS];

	for (View *w = top ; w != NULL ; w = w->next)
	{
		list[viewCount++] = w;
	}

	for (int n = viewCount-1 ; n >= 0 ; --n)
	{
		//LOG(("Window Name: %s", list[n]->getTitle()));
		list[n]->draw();
	}

	//if (mouseView != NULL) mouseView->drawToolTip(screen);
} // end draw

// add
//--------------------------------------------------------------------------
// Purpose: Adds a new window to the end of the current window list.
//--------------------------------------------------------------------------
void Desktop::add(View *view, bool autoActivate /* = TRUE */)
{
  assert(isValidPtr(view));

	//printf("Initting Window: %s\n", view->getTitle());

  // First remove it from the list if we already have it somehow
  remove(view);

	// Insert the guy at the end of the list, while checking that the window is
  // not inserted before some window which should always remain on the bottom.
  View **prevLink = &top;
	while (*prevLink != NULL /*&& !((*prevLink)->status & View::STATUS_ALWAYS_ON_BOTTOM)*/)
	  prevLink = &(*prevLink)->next;

	*prevLink = view;
  view->next = NULL;

	if (autoActivate) activate(view);

} // end add

// activate
//--------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
void Desktop::activate(View *view)
{
	assert(view != NULL);

	// If the top window equals the window to activate, then nothing needs to
	// be done.
	if (focus != view)
	{
		if (focus != NULL)
		{
			focus->deactivate();
		}

		if (!(view->getAlwaysOnBottom()))
		{
			// Remove the new to be top window from the window list.
			remove(view);

			// Set the current top window to the second wyindow in the list.
			view->next = top;

			// Set the window to the top window.
			top = view;

			// Activate the new top window.
		}
		focus = view;
		view->activate();
	}
} // end activate

// remove
//---------------------------------------------------------------------------
// Purpose:
//---------------------------------------------------------------------------
void Desktop::remove(View *view)
{
	View **prevLink = &top;

	while (*prevLink != NULL)
	{
		if (*prevLink == view)
		{
			*prevLink = view->next;
			break;
		}

		prevLink = &(*prevLink)->next;
	}
} // end remove

// findViewContaining
//--------------------------------------------------------------------------
View *Desktop::findViewContaining(iXY pos)
{
	for (View *view = top ; view != NULL ; view = view->next)
	{
		if (view->status & View::STATUS_VISIBLE)
		if (view->contains(pos)) return view;
	}
	return NULL;
} // end findViewContaining

// removeView
//--------------------------------------------------------------------------
// Purpose:
//--------------------------------------------------------------------------
bool Desktop::removeView(const char *searchName)
{
	for (View *view = top; view != NULL; view = view->next)
	{
		if (strcmp(view->searchName, searchName) == 0)
		{
			remove(view);
			return TRUE;
		}
	}
	return false;
} // end removeView

// removeAllViewAlwaysOnBottom
//--------------------------------------------------------------------------
// Purpose:
//--------------------------------------------------------------------------
void Desktop::removeAllViewAlwaysOnBottom()
{
	for (View *view = top; view != NULL; view = view->next)
	{
		if (view->getAlwaysOnBottom())
		{
			remove(view);
			//delete view;
		}
	}
} // end removeAllViewAlwaysOnBottom

// toggleVisibility
//--------------------------------------------------------------------------
// Purpose:
//--------------------------------------------------------------------------
void Desktop::toggleVisibility(const char *searchName)
{
	for (View *view = top; view != NULL; view = view->next)
	{
		if (strcmp(view->searchName, searchName) == 0)
		{
			view->status ^= View::STATUS_VISIBLE;

			if (view->status & View::STATUS_VISIBLE)
			{
				view->doActivate();
				//activate(view);
			}
			else
			{
				view->doDeactivate();
			}
			break;
		}
	}
} // end toggleVisibility

// checkViewPositions
//--------------------------------------------------------------------------
// Purpose: Makes sure all the view are on the screen.
//--------------------------------------------------------------------------
void Desktop::checkViewPositions()
{
	for (View *view = top; view != NULL; view = view->next)
	{
		view->moveTo(view->min);
	}

} // end Desktop::checkViewPositions

// toggleVisibilityNoDoAnything
//--------------------------------------------------------------------------
// Purpose: Only changes whether a visibility of the view, no activate or 
//          deactivate calls.
//--------------------------------------------------------------------------
void Desktop::toggleVisibilityNoDoAnything(const char *searchName)
{
	for (View *view = top; view != NULL; view = view->next)
	{
		if (strcmp(view->searchName, searchName) == 0)
		{
			view->status ^= View::STATUS_VISIBLE;
			break;
		}
	}
} // end toggleVisibilityNoDoAnything

// setActiveView
//--------------------------------------------------------------------------
// Purpose:
//--------------------------------------------------------------------------
void Desktop::setActiveView(const char *searchName)
{
	for (View *view = top; view != NULL; view = view->next)
	{
		if (strcmp(view->searchName, searchName) == 0)
		{
			// If the view is not active set the view active.
			if (!view->getActive())
			{
				activate(view);
			}
			
			return;
		}
	}

} // end setActiveView

//--------------------------------------------------------------------------
void Desktop::setActiveView(View *view)
{
	for (View *v = top; v != NULL; v = v->next)
	{
		if (v == view)
		{
			// If the view is not active set the view active.
			if (!view->getActive()) { activate(view); }
			
			return;
		}
	}

}


// getPointerStatus
//--------------------------------------------------------------------------
unsigned Desktop::getPointerStatus(int mouseX, int mouseY)
{
	iXY mouse(mouseX, mouseY);

  View *mouseView = findViewContaining(mouse);
  if (mouseView == NULL) return 0;

	// Only let the top window be resized
  if (mouseView == top) return mouseView->getMouseActions(mouse);
  else return 0;
} // end getPointerStatus

// doMouseActions
//--------------------------------------------------------------------------
// Purpose:
//--------------------------------------------------------------------------
void Desktop::doMouseActions(const iXY &mousePos)
{
	if (lMouseView == NULL) return;

	if (mouseActions & View::MA_MOVE)
	{
		// Move the window
		lMouseView->moveTo(mousePos - mouseActionOffset);
	}
	else if (mouseActions & View::MA_SCROLL_BAR)
	{
		lMouseView->scrollBarMove(lMouseView->getScreenToClientPos(prevMousePos), lMouseView->getScreenToClientPos(mousePos));
	}
	else if (lMouseView->getResize())
	{
		// Resize the window and move it if necessary
		iXY resizeMin(lMouseView->min);
		iXY resizeMax(lMouseView->max);

		if (mouseActions & View::MA_RESIZE_LEFT)
		{
			resizeMin.x = MIN(	mousePos.x - mouseActionOffset.x,
								lMouseView->max.x - View::RESIZE_XMINSIZE);
		}
		if (mouseActions & View::MA_RESIZE_TOP)
		{
			resizeMin.y = MIN(	mousePos.y + mouseActionOffset.y,
								lMouseView->max.y - View::RESIZE_XMINSIZE);
		}
		if (mouseActions & View::MA_RESIZE_RIGHT)
		{
			resizeMax.x = MAX(	mousePos.x + mouseActionOffset.x,
								lMouseView->min.x + View::RESIZE_XMINSIZE);
		}
		if (mouseActions & View::MA_RESIZE_BOTTOM)
		{
			resizeMax.y = MAX(	mousePos.y + mouseActionOffset.y,
								lMouseView->min.y + View::RESIZE_YMINSIZE);
		}

		lMouseView->moveTo(resizeMin);
		lMouseView->resize(resizeMax - resizeMin);
	}

} // end Desktop::doMouseActions

// getTopViewTitle
//--------------------------------------------------------------------------
// Purpose: Returns the title of the top window.
//--------------------------------------------------------------------------
char *Desktop::getTopViewTitle()
{
	if (top != NULL)
	{
		return top->title;
	}
	
	return "noTitle";

} // end getTopViewTitle

// getMouseViewTitle
//--------------------------------------------------------------------------
// Purpose: Returns the title of the window which contains the mouse.
//--------------------------------------------------------------------------
char *Desktop::getMouseViewTitle()
{
	if (mouseView != NULL)
	{
		return mouseView->title;
	}
	
	return "noTitle";

} // end getMouseViewTitle

// getMouseViewStatus
//--------------------------------------------------------------------------
// Purpose: Returns the status of the window which contains the mouse.
//--------------------------------------------------------------------------
unsigned Desktop::getMouseViewStatus()
{
	if (mouseView != NULL) return mouseView->status;
	else return 0;
} // end getMouseViewStatus

// getViewCount
//--------------------------------------------------------------------------
// Purpose: Returns the number of windows in the window manager.
//--------------------------------------------------------------------------
int Desktop::getViewCount()
{
	const int MAX_WINDOWS = 512;

	int viewCount = 0;
	View *list[MAX_WINDOWS];

	for (View *view = top ; view != NULL ; view = view->next)
	{
		if (viewCount > MAX_WINDOWS) assert(false);
		list[viewCount++] = view;
	}

	return viewCount;
} // end getViewCount

// getViewTitle
//--------------------------------------------------------------------------
// Purpose: Returns the title of the specified window.
//--------------------------------------------------------------------------
const char *Desktop::getViewTitle(int winNum)
{
	assert(winNum < getViewCount());

	View *w = top;

	// Goes through the window list till we get the requested window.
	for (int num = 0; num < winNum; num++)
	{
		assert(w != NULL);
		w = w->next;
	}

	// Returnthe requested windows title.
	return w->title;

} // end getViewTitle

// getViewSearchName
//--------------------------------------------------------------------------
// Purpose: Returns the searchName of the specified window.
//--------------------------------------------------------------------------
const char *Desktop::getViewSearchName(int winNum)
{
	assert(winNum < getViewCount());

	View *w = top;

	// Goes through the window list till we get the requested window.
	for (int num = 0; num < winNum; num++)
	{
		assert(w != NULL);
		w = w->next;
	}

	// Returnthe requested windows searchName.
	return w->getSearchName();

} // end getViewSearchName

// getViewStatus
//--------------------------------------------------------------------------
// Purpose: Returns the status of the specified window.
//--------------------------------------------------------------------------
int Desktop::getViewStatus(const char *searchName)
{
	for (View *view = top; view != NULL; view = view->next)
	{
		if (strcmp(view->searchName, searchName) == 0)
		{
			return view->status;
		}
	}

	return 0;
} // end getViewStatus

// closeView
//--------------------------------------------------------------------------
// Purpose: Closes the window of the specified name.
//--------------------------------------------------------------------------
bool Desktop::closeView(const char *searchName)
{
	const int MAX_WINDOWS = 512;

	int   viewCount = 0;
	View *list[MAX_WINDOWS];

	for (View *w = top ; w != NULL ; w = w->next)
	{
		// Bounds checking.
		if (viewCount > MAX_WINDOWS) assert(false);
		
		// If the window searchName is found, disable the windows visibility.
		if (strcmp(w->searchName, searchName) == 0)
		{
			w->status &= ~View::STATUS_VISIBLE;
			
			return TRUE;
		}
		
		list[viewCount++] = w;
	}

	return false;

} // end closeView

// setVisibility
//--------------------------------------------------------------------------
// Purpose:
//--------------------------------------------------------------------------
void Desktop::setVisibility(const char *searchName, int isVisible)
{
	for (View *view = top; view != NULL; view = view->next)
	{
		if (strcmp(view->searchName, searchName) == 0)
		{
			if (isVisible)
			{
				view->status |= View::STATUS_VISIBLE;
				view->doActivate();
				//activate(view);
			} else
			{
				view->status &= ~View::STATUS_VISIBLE;
				view->doDeactivate();
			}
			break;
		}
	}
} // end setVisibility

// setVisibilityNoDoAnything
//--------------------------------------------------------------------------
// Purpose: Only changes whether a visibility of the view, no activate or 
//          deactivate calls.
//--------------------------------------------------------------------------
void Desktop::setVisibilityNoDoAnything(const char *searchName, int isVisible)
{
	for (View *view = top; view != NULL; view = view->next)
	{
		if (strcmp(view->searchName, searchName) == 0)
		{
			if (isVisible)
			{
				view->status |= View::STATUS_VISIBLE;
			} else
			{
				view->status &= ~View::STATUS_VISIBLE;
			}
			break;
		}
	}
} // end setVisibilityNoDoAnything

// DesktopView
//---------------------------------------------------------------------------
DesktopView::DesktopView() : View()
{
	setSearchName("DesktopView");
	setTitle("View Status");
	setSubTitle(" - F3");

	setAllowResize(false);
	setDisplayStatusBar(false);
	setVisible(false);

	resizeClientArea(iXY(320, 400));
	moveTo(iXY(0, 0));

	addLabel(iXY(0, 0),   "Name", Color::white);
	addLabel(iXY(200, 0), "Status", Color::white);

} // end DesktopView::DesktopView

// doDraw
//---------------------------------------------------------------------------
void DesktopView::doDraw(const Surface &viewArea, const Surface &clientArea)
{
	viewArea.fill(Color::black);

	int yOffset = 10;
	char strBuf[256];
	
	for (int i = 0; i < Desktop::getViewCount(); i++)
	{
		clientArea.bltString(0, yOffset, Desktop::getViewSearchName(i), Color::white);
		
		if (Desktop::getViewStatus(Desktop::getViewSearchName(i)) & STATUS_VISIBLE)
		{
			sprintf(strBuf, "STATUS_VISIBLE");

		} else
		{
			sprintf(strBuf, "INVISIBLE");
		}

		clientArea.bltString(200, yOffset, strBuf, Color::white);

		yOffset += CHAR_YPIX;
	}

	View::doDraw(viewArea, clientArea);

} // end doDraw

// rMouseDrag
//---------------------------------------------------------------------------
void DesktopView::rMouseDrag(const iXY &downPos, const iXY &prevPos, const iXY &newPos)
{
	moveTo(min + newPos - prevPos);

} // end DesktopView::rMouseDrag

// doActivate
//---------------------------------------------------------------------------
void DesktopView::doActivate()
{
	Desktop::setActiveView(this);

} // end DesktopView::doActivate
