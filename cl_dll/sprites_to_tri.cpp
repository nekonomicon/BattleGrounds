/*
    Copyright 2004. The Battle Grounds Team and Contributors

    This file is part of the Battle Grounds Modification for Half-Life.

    The Battle Grounds Modification for Half-Life is free software;
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser General Public License as published by the Free
    Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    The Battle Grounds Modification for Half-Life is distributed in
    the hope that it will be useful, but WITHOUT ANY WARRANTY; without
    even the implied warranty of MERCHANTABILITY or FITNESS FOR A
    PARTICULAR PURPOSE.  See the GNU Lesser General Public License
    for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with The Battle Grounds Modification for Half-Life;
    if not, write to the Free Software Foundation, Inc., 59 Temple Place,
    Suite 330, Boston, MA  02111-1307  USA

    You must obey the GNU Lesser General Public License in all respects for
    all of the code used other than code distributed with the Half-Life
    SDK developed by Valve.  If you modify this file, you may extend this
    exception to your version of the file, but you are not obligated to do so.
    If you do not wish to do so, delete this exception statement from your
    version.
*/

#include "STDIO.H"
#include "STDLIB.H"

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include <string.h>
#include "triangleapi.h"
#include "com_model.h"

#include "vgui_int.h"
#include "vgui_TeamFortressViewport.h"
#include "vgui_ServerBrowser.h"

// i hate globals, but this is the easiest way to set this up so that
// the change from engine sprite funcs to triapi is as seemless as possible
HSPRITE hTempPic = 0;
int iFrame = 0;
int iClipX = 0;
int iClipY = 0;
int iClipWidth = 0;
int iClipHeight = 0;
bool bClip = false;

// internal function to draw a sprite to the screen. uses the globals set up above
void Draw_Sprite(int x, int y, const wrect_t *prc, bool bScale);

// saves values our clipper will use
void SPR_EnableScissor ( int x, int y, int width, int height ) {
	bClip = true;
	iClipX = x;
	iClipY = y;
	iClipWidth = width;
	iClipHeight = height;
}

// turn off clipping
void SPR_DisableScissor ( void ) { bClip = false; }

// sets the sprite to be drawn, for .spr's only
void SPR_Set (HSPRITE hPic, int r, int g, int b ) {
	// save the sprite to a global
	hTempPic = hPic;

	// do intial setup common to all drawing types
	gEngfuncs.pTriAPI->CullFace( TRI_FRONT );
	gEngfuncs.pTriAPI->Color4f( ((float)r/255.0), ((float)g/255.0), ((float)b/255.0), 1.0 );
}

// draw a normal sprite
void SPR_Draw ( int frame, int x, int y, const wrect_t *prc, bool bScale ) {
	if(hTempPic == 0)
		return;

	iFrame = frame;

	// load the sprite and set the normal render mode
	gEngfuncs.pTriAPI->RenderMode( kRenderNormal );
	gEngfuncs.pTriAPI->SpriteTexture( (model_s*)gEngfuncs.GetSpritePointer(hTempPic), iFrame );

	// draw the sprite on the screen
	Draw_Sprite(x, y, prc, bScale);
}

// draw a sprite which has one colour transparent
void SPR_DrawHoles ( int frame, int x, int y, const wrect_t *prc, bool bScale ) {
	if(hTempPic == 0)
		return;

	iFrame = frame;

	// load the sprite and set the render mode to ignore the indexed colour
	gEngfuncs.pTriAPI->RenderMode( kRenderTransColor );
	gEngfuncs.pTriAPI->SpriteTexture( (model_s*)gEngfuncs.GetSpritePointer(hTempPic), iFrame );

	// draw the sprite on the screen
	Draw_Sprite(x, y, prc, bScale);
}

// draw a sprite on top of the background scene
void SPR_DrawAdditive ( int frame, int x, int y, const wrect_t *prc, bool bScale ) {
	if(hTempPic == 0)
		return;

	iFrame = frame;

	gEngfuncs.pTriAPI->RenderMode( kRenderTransAdd );
	gEngfuncs.pTriAPI->SpriteTexture( (model_s*)gEngfuncs.GetSpritePointer(hTempPic), iFrame );

	// draw the sprite on the screen
	Draw_Sprite(x, y, prc, bScale);
}

// actually draws the sprite
void Draw_Sprite(int x, int y, const wrect_t *prc, bool bScale) {
	int x_org = 0; int y_org = 0;
	int iLeftDiff = 0; int iTopDiff = 0; int iRightDiff = 0; int iBottomDiff = 0;
	int x_length = 0; int y_length = 0; int x_org_length = 0; int y_org_length = 0;
	float flTop = 0.0; float flBottom = 0.0; float flLeft = 0.0; float flRight = 0.0;

	// save the original positions
	x_org = x; y_org = y;
	// calculate the lengths and save them off so we can tell the different after clipping
	if(prc == NULL) {
		x_org_length = x_length = SPR_Width(hTempPic, iFrame);
		y_org_length = y_length = SPR_Height(hTempPic, iFrame);
	} else {
		x_org_length = x_length = prc->right - prc->left;
		y_org_length = y_length = prc->bottom - prc->top;
	}

	// only both with clip calculations if its on
	if(bClip) {
		// the clip rect starts to the right of the sprite box start
		if(iClipX > x) {
			// the clip rect starts further over than the box goes, so show nothing
			if(iClipX >= (x + x_length)) {
				x_length = 0;
			} else {
				// reduce the length by how far across we now start
				x_length -= iClipX - x;
				// the length is now reduced on the left.  if the original sprite goes out further than
				// the clip box, reduce it to the clip width
				if(iClipWidth < x_length)
					x_length = iClipWidth;
			}
			// save the left position now that its moved
			x = iClipX;
		// the clip rect starts outside the sprite box
		} else {
			// the clip rect never goes inside the sprite box, so display nothing
			if(iClipX + iClipWidth < x) {
				x_length = 0;
			// we find out where the clip box ends, then take away where the sprite starts to get the new width
			} else {
				x_length = iClipX + iClipWidth - x;
			}
		}

		// the clip rect starts below the sprite box start
		if(iClipY > y) {
			// the clip rect starts further down than the box goes, so show nothing
			if(iClipY >= (y + y_length)) {
				y_length = 0;
			} else {
				// reduce the length by how far down we now start
				y_length -= iClipY - y;
				// the length is now reduced above. if the original sprite goes down further than
				// the clip box, reduce it to the clip height
				if(iClipHeight < y_length)
					y_length = iClipHeight;
			}
			// save the top position now that the its moved
			y = iClipY;
		// the clip rect starts outside the sprite box
		} else {
			// the clip rect never goes inside the sprite box, so display nothing
			if(iClipY + iClipHeight < y) {
				y_length = 0;
			// we find out where the clipbox ends, then take away where the sprite starts to get the new height.
			} else {
				y_length = iClipY + iClipHeight - y;
			}
		}

		// compared the saved start positions to the new ones, to see how much they've changed
		iLeftDiff = x - x_org;
		iTopDiff = y - y_org;
		// compared the savedd end positions ( saved start + length ), to see how much they've changed
		iRightDiff = (x_org_length + x_org) - (x_length + x);
		iBottomDiff = (y_org_length + y_org) - (y_length + y);
	}

	// load the sprite as a model
	model_s *pModel = (model_s*)gEngfuncs.GetSpritePointer(hTempPic);

	// calculate how far down the texture our model is wanting. this fixes multiple sprites in the same file
	// clipped sprites, sprites which don't go up the edges, etc
	if(prc == NULL) {
		flTop = 0.0;
		flLeft = 0.0;
		flBottom = 1.0;
		flRight = 1.0;
	} else {
		flTop = (prc->top + iTopDiff) / (pModel->maxs.z - pModel->mins.z);
		flBottom = (prc->bottom - iBottomDiff) / (pModel->maxs.z - pModel->mins.z);
		flLeft = (prc->left + iLeftDiff) /  (pModel->maxs.x - pModel->mins.x);
		flRight = (prc->right - iRightDiff) /  (pModel->maxs.x - pModel->mins.x);
	}

	// do we want the image to be resized for the current resolution?
	// scaling size is piss-easy, however positions is another mater.  We have no idea which side to
	// take the extra pixels from or where to put them. we can either let the individual hud classes
	// make the scaled position adjustments for us, or we can do some fancy trickery to try to determine
	// where they are wanted to be placed and then adjust from there
	if(bScale) {
		x_length = XRES2(x_length);
		y_length = YRES2(y_length);
	}

	// try a box with the sprite in it
	// hopefully this will work in d3d and software as well as ogl
	gEngfuncs.pTriAPI->Begin( TRI_QUADS );

	gEngfuncs.pTriAPI->TexCoord2f(flLeft, flTop);
	gEngfuncs.pTriAPI->Vertex3f(x, y, 0);

	gEngfuncs.pTriAPI->TexCoord2f(flRight, flTop);
	gEngfuncs.pTriAPI->Vertex3f(x + x_length, y, 0);

	gEngfuncs.pTriAPI->TexCoord2f(flRight, flBottom);
	gEngfuncs.pTriAPI->Vertex3f(x + x_length, y + y_length, 0);

	gEngfuncs.pTriAPI->TexCoord2f(flLeft, flBottom);
	gEngfuncs.pTriAPI->Vertex3f(x, y + y_length, 0);

	gEngfuncs.pTriAPI->End();
}