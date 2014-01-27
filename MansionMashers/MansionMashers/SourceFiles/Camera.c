/*****************************************************************************/
/*!
\file				Camera.c
\author				Juli Gregg (j.gregg)
\date				Jan 11, 2014

\brief				Camera movement functions

\par				Functions:
\li					SetCamera
\li					ResetCamera
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/Camera.h"
#include "../HeaderFiles/Sprite.h"


/*************************************************************************/
/*!
	\brief
	Sets the camera position
	
	\param playerX
	Where the player is located
	
	\param offset
	The distance from the edge of the camera the player needs to be
	before changing the camera position
	
	\param hudmain
	HUD objects to draw
*/
/*************************************************************************/
void SetCamera(Vec2 *playerX, int offset, struct HUDLayer* hudmain)
{
	//Variables for camera position
	float camX, camY;	
	int i;

	//Get camera position
	AEGfxGetCamPosition(&camX, &camY);

	//Draws the hud items in the correct position
	for (i = 0; hudmain->HudItem[i]; i++)
	{
		if (i % 2 == 0)
		{
			hudmain->HudItem[i]->Position.x = camX - 480 + ((i/2)*320);
			hudmain->HudItem[i]->Position.y = camY + 275;
			DrawSprite(hudmain->HudItem[i]);
		}
		else
		{
			hudmain->HudItem[i]->Position.x = camX - 480 - 70 + (((i-1)/2)*320);
			hudmain->HudItem[i]->Position.y = camY + 275 - 23;
			DrawSprite(hudmain->HudItem[i]);
		}
	}

	//Moves camera Right
	if(playerX->x > (camX + offset))
		AEGfxSetCamPosition(playerX->x - offset, camY);
	
	//Moves camera Left
	else if(playerX->x < (camX - offset))
		AEGfxSetCamPosition(playerX->x + offset, camY);


}

/*************************************************************************/
/*!
	\brief
	Resets the camera position to (0,0)
*/
/*************************************************************************/
void ResetCamera(void)
{
	//Sets camera position to (0,0)
	AEGfxSetCamPosition(0.0, 0.0);
}