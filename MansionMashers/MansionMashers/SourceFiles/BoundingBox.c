/*****************************************************************************/
/*!
\file				BoundingBox.h
\author				Juli Gregg
\date				Mar 11, 2014

\brief				

\par				Functions:
					- CreateBoundingBoxes	
					- BoundingBoxUpdate
					- CreateBlockerBoxes
					- UpdateBlockerBoxes
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

#include "../AEEngine.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"

//Bounding Boxes
static Sprite *BoundTop;
static Sprite *BoundBottom;
static Sprite *BoundLeft;
static Sprite *BoundRight;

static Wall *BBWallLeft;
static Wall *BBWallRight;

/*************************************************************************/
/*!
	\brief
	Creates sprite for the bounding boxes
*/
/*************************************************************************/
void CreateBoundingBoxes(void)
{
	Vec3 BoundingTint;

	Vec3Set(&BoundingTint, 0.0f, 0.0f, 0.0f);

	BoundTop = (Sprite *) CreateSprite("TextureFiles/BoundingBox.png", 1920, 1080, 5000, 1, 1, 0, 1080);
	BoundBottom = (Sprite *) CreateSprite("TextureFiles/BoundingBox.png", 1920, 1080, 5000, 1, 1, 0, -1080);
	BoundLeft = (Sprite *) CreateSprite("TextureFiles/BoundingBox.png", 1920, 2080, 5000, 1, 1, -1920, 0);
	BoundRight = (Sprite *) CreateSprite("TextureFiles/BoundingBox.png", 1920, 2080, 5000, 1, 1, 1920, 0);

	BoundTop->Tint = BoundingTint;
	BoundBottom->Tint = BoundingTint;
	BoundLeft->Tint = BoundingTint;
	BoundRight->Tint = BoundingTint;
}

/*************************************************************************/
/*!
	\brief
	Updates the position of the bounding boxes
*/
/*************************************************************************/
void BoundingBoxUpdate(void)
{
	float camX, camY;	

	//Get camera position
	AEGfxGetCamPosition(&camX, &camY);

	BoundTop->Position.x = camX / GetLoadRatio();
	BoundBottom->Position.x = camX / GetLoadRatio();
	BoundRight->Position.x = camX / GetLoadRatio() + 1920;
	BoundLeft->Position.x = camX / GetLoadRatio() - 1920;
}

/*************************************************************************/
/*!
	\brief
	Creates sprite for the blocker boxes
*/
/*************************************************************************/
void CreateBlockerBoxes(void)
{
	// Bounding Box Walls
	BBWallLeft = CreateWall("TextureFiles/BlankPlatform.png", 200.0f, 1080.0f, 0, 0);
	BBWallLeft->WallSprite->Visible = FALSE;
	BBWallLeft->enemyNotCollidable = TRUE;
	BBWallRight = CreateWall("TextureFiles/BlankPlatform.png", 200.0f, 1080.0f, 0, 0);
	BBWallRight->WallSprite->Visible = FALSE;
	BBWallRight->enemyNotCollidable = TRUE;
}

/*************************************************************************/
/*!
	\brief
	Updates the blocker boxes

	\param panelsize
	Size of the panel in the level
*/
/*************************************************************************/
void UpdateBlockerBoxes(float panelsize)
{
	//Update position (including blocked state)
	BBWallLeft->Position.y = -1080.0f + 1080.0f * GetCameraLockState();
	BBWallLeft->Position.x = GetCameraXPosition() - (panelsize / 2);
	UpdateCollisionPosition(&BBWallLeft->WallCollider, &BBWallLeft->Position);
	BBWallRight->Position.y = -1080.0f + 1080.0f * GetCameraLockState();
	BBWallRight->Position.x = GetCameraXPosition() + (panelsize / 2);
	UpdateCollisionPosition(&BBWallRight->WallCollider, &BBWallRight->Position);
}
