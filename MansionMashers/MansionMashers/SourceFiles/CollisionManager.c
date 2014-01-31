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
