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
////////////////////////////////////////////////////////////////////////////
// Includes.
////////////////////////////////////////////////////////////////////////////

#include <config.h>
#include "Surface.hpp"
#include "loadPics.hpp"


////////////////////////////////////////////////////////////////////////////
// Externs.
////////////////////////////////////////////////////////////////////////////

Surface mouseArrow;

/*
Surface mouseMoveLeft;
Surface mouseMoveRight;
Surface mouseMoveUp;
Surface mouseMoveDown;
Surface mouseMoveLeftStop;
Surface mouseMoveRightStop;
Surface mouseMoveUpStop;
Surface mouseMoveDownStop;

Surface mouseMoveUpLeft;
Surface mouseMoveUpRight;
Surface mouseMoveDownLeft;
Surface mouseMoveDownRight;
Surface mouseMoveUpLeftStop;
Surface mouseMoveUpRightStop;
Surface mouseMoveDownLeftStop;
Surface mouseMoveDownRightStop;

Surface mouseResizeUD;
Surface mouseResizeLR;
Surface mouseResizeBR;
Surface mouseResizeBL;
Surface mouseResizeTR;
Surface mouseResizeTL;
*/

////////////////////////////////////////////////////////////////////////////
// Functions.
////////////////////////////////////////////////////////////////////////////

void loadPics()
{
	//mouseResizeUD.create(40, 40, 40, 29);
	//mouseResizeLR.create(12, 30, 12, 15);
	//mouseResizeBR.create(40, 30, 40, 30);
	//mouseResizeBL.create(40, 30, 40, 30);
	//mouseResizeTR.create(40, 30, 40, 30);
	//mouseResizeTL.create(40, 30, 40, 30);

	//mouseArrow.loadRAW("pics\\cursors\\arrowPointer.raw");
	mouseArrow.create(20, 20, 20, 1);
	mouseArrow.fill(0);
	mouseArrow.drawLine(0, 0, mouseArrow.getCenterX(), 0, 241);
	mouseArrow.drawLine(0, 0, 0, mouseArrow.getCenterY(), 241);

	mouseArrow.drawLine(0, 0, mouseArrow.getPixX(), mouseArrow.getPixY() - 5, 241);
	mouseArrow.drawLine(0, 0, mouseArrow.getPixX() - 5, mouseArrow.getPixY(), 241);
	mouseArrow.drawLine(mouseArrow.getPixX() - 5, mouseArrow.getPixY() - 1, mouseArrow.getPixX(), mouseArrow.getPixY() - 1, 241);
	mouseArrow.drawLine(mouseArrow.getPixX() - 1, mouseArrow.getPixY() - 5, mouseArrow.getPixX() - 1, mouseArrow.getPixY() - 1, 241);

	//mouseMoveLeft.loadRAW("pics\\cursors\\arrowMoveLeft.raw");
	//mouseMoveUp.loadRAW("pics\\cursors\\arrowMoveUp.raw");

	//mouseMoveLeftStop.loadRAW("pics\\cursors\\arrowMoveLeftStop.raw");
	//mouseMoveUpStop.loadRAW("pics\\cursors\\arrowMoveUpStop.raw");

	// Up/Down
	//mouseResizeUD.loadRAW("pics\\cursors\\resizeud.raw");
	// Left/Right
	//mouseResizeLR.extractPCX("pics\\cursors\\resizetb.pcx", 15, 1);

	// Diagonals
	//mouseResizeBR.loadRAW("pics\\cursors\\resizebr.raw");

	//mouseMoveUpLeft.loadRAW("pics\\cursors\\arrowMoveUpperLeft.raw");
	//mouseMoveUpLeftStop.loadRAW("pics\\cursors\\arrowMoveUpperLeftStop.raw");

/*
	mouseResizeBL.copy(mouseResizeBR);
	mouseResizeTL.copy(mouseResizeBR);
	mouseResizeTR.copy(mouseResizeBR);

	// The cardinal mouse move pointers.
	mouseMoveRight.copy(mouseMoveLeft);
	mouseMoveRight.flipHorizontal();
	mouseMoveRight.setOffsetX(-mouseMoveRight.getPix().x);

	mouseMoveRightStop.copy(mouseMoveLeftStop);
	mouseMoveRightStop.flipHorizontal();
	mouseMoveRightStop.setOffsetX(-mouseMoveRightStop.getPix().x);
						  
	mouseMoveDown.copy(mouseMoveUp);
	mouseMoveDown.flipVertical();
	mouseMoveDown.setOffsetY(-mouseMoveDown.getPix().y);

	mouseMoveDownStop.copy(mouseMoveUpStop);
	mouseMoveDownStop.flipVertical();
	mouseMoveDownStop.setOffsetY(-mouseMoveDownStop.getPix().y);

	// Diagonal mouse move pointers.
	mouseMoveUpRight.copy(mouseMoveUpLeft);
	mouseMoveUpRight.flipHorizontal();
	mouseMoveUpRight.setOffsetX(-mouseMoveUpRight.getPix().x);
	
	mouseMoveUpRightStop.copy(mouseMoveUpLeftStop);
	mouseMoveUpRightStop.flipHorizontal();
	mouseMoveUpRightStop.setOffsetX(-mouseMoveUpRightStop.getPix().x);
	
	mouseMoveDownLeft.copy(mouseMoveUpLeft);
	mouseMoveDownLeft.flipVertical();
	mouseMoveDownLeft.setOffsetY(-mouseMoveDownLeft.getPix().y);
	
	mouseMoveDownLeftStop.copy(mouseMoveUpLeftStop);
	mouseMoveDownLeftStop.flipVertical();
	mouseMoveDownLeftStop.setOffsetY(-mouseMoveDownLeftStop.getPix().y);
	
	mouseMoveDownRight.copy(mouseMoveUpLeft);
	mouseMoveDownRight.flipHorizontal();
	mouseMoveDownRight.flipVertical();
	mouseMoveDownRight.setOffset(iXY(-mouseMoveDownRight.getPix().x, -mouseMoveDownRight.getPix().y));

	mouseMoveDownRightStop.copy(mouseMoveUpLeftStop);
	mouseMoveDownRightStop.flipHorizontal();
	mouseMoveDownRightStop.flipVertical();
	mouseMoveDownRightStop.setOffset(iXY(-mouseMoveDownRightStop.getPix().x, -mouseMoveDownRightStop.getPix().y));

	// Make the diagonal resize cursors from the bottom right.
	//mouseResizeTR.flipVertical();
	//mouseResizeBL.flipHorizontal();
	//mouseResizeTL.flipHorizontal();
	//mouseResizeTL.flipVertical();
*/
	int fps = 60;

	mouseArrow.setFPS(fps);
/*	
	mouseMoveLeft.setFPS(fps);
	mouseMoveRight.setFPS(fps);
	mouseMoveUp.setFPS(fps);
	mouseMoveDown.setFPS(fps);
	
	mouseResizeTR.setFPS(fps);
	mouseResizeTR.setFPS(fps);
	mouseResizeBR.setFPS(fps);
	mouseResizeBL.setFPS(fps);
	mouseResizeUD.setFPS(fps);
	mouseResizeLR.setFPS(fps);

	mouseMoveUpLeft.setFPS(fps);
	mouseMoveUpRight.setFPS(fps);
	mouseMoveDownLeft.setFPS(fps);
	mouseMoveDownRight.setFPS(fps);

	fps = 30;

	mouseMoveLeftStop.setFPS(fps);
	mouseMoveRightStop.setFPS(fps);
	mouseMoveUpStop.setFPS(fps);
	mouseMoveDownStop.setFPS(fps);
	mouseMoveUpLeftStop.setFPS(fps);
	mouseMoveUpRightStop.setFPS(fps);
	mouseMoveDownLeftStop.setFPS(fps);
	mouseMoveDownRightStop.setFPS(fps);
*/
} // end loadPics
