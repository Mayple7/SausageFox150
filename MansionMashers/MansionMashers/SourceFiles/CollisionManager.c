/*
File:				ObjectManager.c
Author:				Kaden Nugent (kaden.n)
Creation Date:		Jan 8, 2014

Purpose:			Manages Collision

Functions:			
 
Copyright (C) 2014 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited. 
*/ 



// ---------------------------------------------------------------------------
// includes

#include "../HeaderFiles/ObjectManager.h"
#include "../HeaderFiles/CollisionManager.h"
#include "../HeaderFiles/Player.h"

// ---------------------------------------------------------------------------
// Libraries

// ---------------------------------------------------------------------------
// globals

// ---------------------------------------------------------------------------
// Static function protoypes

// ---------------------------------------------------------------------------
// Main

////////         Collision Detected          ///////
void collisionDetected(Sprite* objA, Sprite* objB)
{
	/*if (objB->SpriteType == PlayerType)
	{
		collisionDetected(objB, objA);
		return;
	}
	else */
	
	if (objA->SpriteType == PlayerType)
	{
		HandleCollision(objB);
	}
}

////////  Collision Detection        [R&R]   ///////
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
	//if (leftAx < rightBx && rightAx > leftBx && topAy < bottomBy && bottomAy > topBy)
		//collisionDetected(objA, objB);

	// Within X boundaries
	if((leftAx < rightBx && leftAx > leftBx) || (rightAx < rightBx && rightAx > leftBx))
	{
		if(objA->SpriteType == PlayerType)
			printf("--%s--\n", objB->SpriteName);
		//Within Y boundaries
		if((topAy > bottomBy && topAy < topBy) || (bottomAy > bottomBy && bottomAy < topBy))
		{
			if(objA->SpriteType == PlayerType)
				printf("%s\n", objB->SpriteName);
			collisionDetected(objA, objB);
		}

	}
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

////////    SEARCH FOR THE COLLIDEE    [B]   ///////
void searchForIntersection(Sprite* objA)
{
	//Sort through the objects to find which to detect collision
	int j;
	for (j = 0; j < OBJECTAMOUNT; j++)
	{
		Sprite* objB = (objectList + j);
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

////////    SEARCH FOR THE COLLIDER    [A]    ///////
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

/*
void DetectCollision(void)
{
	//Sort through the objects to find which to detect collision
	int i;
	for (i = 0; i < OBJECTAMOUNT; i++)
	{
		Sprite* objA = (objectList + i);
		//Make sure the sprite exists
		if (objA && objA->Created == 1 && objA->CanCollide == 1)
		{
			//Search the workspace for any colliding objects
			//printf("%i    ::    %i\n", objA->Width, objA->Width);
			searchForIntersection(objA);
		}
	}
}*/
