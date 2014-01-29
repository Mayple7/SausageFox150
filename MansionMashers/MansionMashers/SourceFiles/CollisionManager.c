/*****************************************************************************/
/*!
\file				CollisionManager.c
\author				Kaden Nugent (kaden.n)
\date				Jan 8, 2014

\brief				Manages collision detection and resolution

\par				Functions:
\li					collisionDetected
\li					rectangleRectangleCollision
\li					searchForIntersection
\li					DetectCollision
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes

#include "../HeaderFiles/ObjectManager.h"
#include "../HeaderFiles/CollisionManager.h"
#include "../HeaderFiles/Player.h"
#include "../HeaderFiles/CollisionBox.h"

// ---------------------------------------------------------------------------
// Main

/*************************************************************************/
/*!
	\brief
	Sends a collision event to the correct collision handler
	  
	\param objA
	The object being collided
	  
	\param objB
	The colliding object
*/
/*************************************************************************/
void collisionDetected(Sprite* objA, Sprite* objB)
{	
	//Send event to the player's collision handler
	if (objA->CollisionGroup == PlayerType)
	{
		HandleCollision(objB);
	}
}

/*************************************************************************/
/*!
	\brief
	Determines if there is a collision between 2 rectangle colliders
	  
	\param objA
	The object being collided
	  
	\param objB
	The colliding object
*/
/*************************************************************************/
void rectangleRectangleCollision(Sprite* objA, Sprite* objB)
{
	//Get the positions and offset for quick access
	Vec2 posA = objA->Position;
	Vec2 posB = objB->Position;
	Vec2 offA = objA->CollideOffset;
	Vec2 offB = objB->CollideOffset;

	//Collidable 1
	float leftAx   = posA.x + offA.x - (objA->CollideSize.x / 2);
	float rightAx  = leftAx + objA->CollideSize.x;
	float topAy    = posA.y + offA.y + (objA->CollideSize.y / 2);
	float bottomAy = topAy - objA->CollideSize.y;

	//Collidable 2
	float leftBx   = posB.x + offB.x - (objB->CollideSize.x / 2);
	float rightBx  = leftBx + objB->CollideSize.x;
	float topBy    = posB.y + offB.y + (objB->CollideSize.y / 2);
	float bottomBy = topBy - objB->CollideSize.y;

	//Check if all requires for collision are true
	if (leftAx < rightBx && rightAx > leftBx && topAy > bottomBy && bottomAy < topBy)
		collisionDetected(objA, objB);
}

////////  Collision Detection        [C&R]   ///////
void circleRectangleCollision(Sprite* objA, Sprite* objB)
{
}

////////  Collision Detection        [C&C]   ///////
void circleCircleCollision(Sprite* objA, Sprite* objB)
{
	/*Vec2 posA = objA->Position;
	Vec2 posB = objB->Position;
	float radiusA = ((objA->Width + objA->Height) / 2) / 2;
	float radiusB = ((objB->Width + objB->Height) / 2) / 2;

	printf("%f\n", sqrt((posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y)));
	printf("%f\n", (radiusA + radiusB));

	if (sqrt((posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y)) < (radiusA + radiusB))
		printf("Detected interaction\n");
	else
		printf("No interaction\n");*/
}

/*************************************************************************/
/*!
	\brief
	Sorts through all collidable to search for a collision
	  
	\param objA
	The object being collided
*/
/*************************************************************************/
void searchForIntersection(Sprite* objA)
{
	//Sort through the objects to find which to detect collision
	int j;
	for (j = 0; j < OBJECTAMOUNT; j++)
	{
		Sprite* objB = (drawList + j);
		//Make sure the sprite exists
		if (objB && objB->Created == 1 && objB != objA && objB->CanCollide == 1)
		{
			//Run detection based on type
			if (objB->SensorType == RectangleCollider && objA->SensorType == RectangleCollider)
				rectangleRectangleCollision(objA, objB);
			else if (objB->SensorType == CircleCollider && objA->SensorType == RectangleCollider)
				circleRectangleCollision(objA, objB);
			else if (objB->SensorType == RectangleCollider && objA->SensorType == CircleCollider)
				circleRectangleCollision(objB, objA);
			else if (objB->SensorType == CircleCollider && objA->SensorType == CircleCollider)
				circleCircleCollision(objA, objB);
		}
	}
}

/*************************************************************************/
/*!
	\brief
	Searches through all objects for a collidable object to detect collision
*/
/*************************************************************************/
void DetectCollision(void)
{
	int i;
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (collidables[i].Created == 1)
		{
			//Check for collision (We will assume everything in the collidables list CanCollide)
			searchForIntersection(&collidables[i]);
		}
	}
}

/*************************************************************************/
/*!
	\brief
	Determines if there is a collision between 2 rectangle colliders
	  
	\param objA
	The object being collided
	  
	\param objB
	The colliding object
*/
/*************************************************************************/
int CollisionRectangles(CollisionBox* objA, CollisionBox* objB)
{
	//Get the positions and offset for quick access
	Vec2 posA = objA->Position;
	Vec2 posB = objB->Position;
	Vec2 offA = objA->Offset;
	Vec2 offB = objB->Offset;

	//Collidable 1
	float leftAx   = posA.x + offA.x - (objA->width / 2);
	float rightAx  = leftAx + objA->width;
	float topAy    = posA.y + offA.y + (objA->height / 2);
	float bottomAy = topAy - objA->height;

	//Collidable 2
	float leftBx   = posB.x + offB.x - (objB->width / 2);
	float rightBx  = leftBx + objB->width;
	float topBy    = posB.y + offB.y + (objB->height / 2);
	float bottomBy = topBy - objB->height;

	//Check if all requires for collision are true
	if (leftAx < rightBx && rightAx > leftBx && topAy > bottomBy && bottomAy < topBy)
		return TRUE;
	else
		return FALSE;
}