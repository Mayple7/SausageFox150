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

// ---------------------------------------------------------------------------
// Libraries

// ---------------------------------------------------------------------------
// globals

// ---------------------------------------------------------------------------
// Static function protoypes

// ---------------------------------------------------------------------------
// Main

////////  Collision Detection        [R&R]   ///////
void rectangleRectangleCollision(Sprite* objA, Sprite* objB)
{
	Vec2 posA = objA->Position;
	Vec2 posB = objB->Position;

	printf("%i    ::    %i\n", objA->Width, objB->Width);

	if (posA.x + objA->Width < posB.x && posA.x > posB.x + objB->Width
		&& posA.y + objA->Height < posB.y && posA.y > posB.y + objB->Height)
		printf("Detected interaction\n");
	else
		printf("No interaction\n");
}

////////  Collision Detection        [C&R]   ///////
void circleRectangleCollision(Sprite* objA, Sprite* objB)
{

}

////////  Collision Detection        [C&C]   ///////
void circleCircleCollision(Sprite* objA, Sprite* objB)
{

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
void detectCollision(void)
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
			searchForIntersection(objA);
		}
	}
}
