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

void detectCollision(void)
{
	//Sort through the objects to find which to detect collision
	/*int i;
	for (i = 0; i < objectAmount; i++)
	{
		Sprite* objectNext = (objectList + i);
		//Make sure the sprite exists
		if (objectNext && objectNext->Created == 1)
		{
			//Free the mesh and texture data
			AEGfxMeshFree(objectNext->SpriteMesh);
			AEGfxTextureUnload(objectNext->SpriteTexture);
			printf("Slot %i is now empty\n", i);
		}
	}*/
}
