/*****************************************************************************/
/*!
\file				Wall.c
\author				Dan Muller (d.muller)
\date				Feb 23, 2014

\brief				Functions to do with a wall are here

\par				Functions:
\li					CreateWall
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include "../AEEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxObjects.h"

/*************************************************************************/
/*!
	\brief
	Create the Wall
	
	\param CurrentWall
	A pointer to the Wall to be initialized
*/
/*************************************************************************/
Wall* CreateWall(char* textureName, float width, float height, float xPos, float yPos)
{
	Wall *CurrentWall = AddWall();
	Vec2 ColliderPos;
	ColliderPos.x = xPos;
	ColliderPos.y = yPos;
	CurrentWall->enemyNotCollidable = FALSE;
	Vec2Set(&CurrentWall->Position, xPos, yPos);
	CurrentWall->WallSprite = (Sprite *) CreateSprite(textureName, width, height, 4, 1, 1, xPos, yPos);

	CreateCollisionBox(&CurrentWall->WallCollider, &ColliderPos, WallType, width, height, GetObjectID());
	CurrentWall->objID = GetObjectID();
	InitializeRigidBody(&CurrentWall->WallRigidBody, TRUE, width, height);

	return CurrentWall;
}

void UpdateWallPosition(Wall* CurrentWall, float x, float y)
{
	Vec2Set(&CurrentWall->Position, x, y);
	CurrentWall->WallCollider.Position = CurrentWall->Position;
	CurrentWall->WallSprite->Position = CurrentWall->Position;
}

