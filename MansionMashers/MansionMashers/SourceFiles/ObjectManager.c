/*
File:				ObjectManager.c
Author:				Kaden Nugent (kaden.n)
Creation Date:		Jan 8, 2014

Purpose:			Manages Objects

Functions:			
 
Copyright (C) 2014 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited. 
*/ 



// ---------------------------------------------------------------------------
// includes
#include "../HeaderFiles/ObjectManager.h"
#include "../HeaderFiles/FoxEngine.h"

// ---------------------------------------------------------------------------
// Libraries

// ---------------------------------------------------------------------------
// globals

// ---------------------------------------------------------------------------
// Static function protoypes

// ---------------------------------------------------------------------------
// Main

Sprite* AddObject(void)
{
	int i;
	for (i = 0; i < OBJECTAMOUNT; i++)
	{
		Sprite* objectNext = (objectList + i);
		//Find a sprite that is empty
		if (objectNext && objectNext->Created != 1)
		{
			printf("Slot %i is now filled\n", i);
			return objectNext;
		}
	}
	return NULL;
}

void AddCollidable(Sprite *newCollidable)
{
	int i;
	for (i = 0; i < OBJECTAMOUNT; i++)
	{
		//Sprite* collidableNext = (collidables + i);
		//Find a sprite that is empty
		if ((collidables + i) && (collidables + i)->Created != 1)
		{
			printf("Slot %i is now collidable\n", i);
			collidables = newCollidable;
			return;
		}
	}
}

void resetObjectList(void)
{
	//Set up the memory to fit the desired amount of objects
	objectList  = (Sprite *)calloc(OBJECTAMOUNT, OBJECTAMOUNT * sizeof(Sprite));
	//Make sure the calloc is not NULL
	if (objectList)
	{
		printf("\nOBJECT LIST SET UP COMPLETE\n\n");

		//Set up collisions
		collideList = (Sprite *)calloc(OBJECTAMOUNT, OBJECTAMOUNT * sizeof(Sprite*));
		collidables = (Sprite *)calloc(OBJECTAMOUNT, OBJECTAMOUNT * sizeof(Sprite*));

		//Make sure the malloc is not NULL
		if (collideList && collidables)
		{
			/*int i;
			for (i = 0; i < OBJECTAMOUNT; i++)
			{
				collideList = NULL;
				collidables = NULL;
			}*/

			printf("COLLIDE LIST SET UP COMPLETE\n\n");
		}
		else
		{
			printf("COLLIDE LIST SET UP FAILED\n\n");
		}
	}
	else
	{
		printf("\nOBJECT LIST SET UP FAILED\n\n");
	}
}

void drawObjectList(void)
{
	int currentZ = 0;
	int nextZ    = 0;
	//Draw the sprites so the highest Z is in front
	while (nextZ != -1)
	{
		int i;
		nextZ = -1;
		for (i = 0; i < OBJECTAMOUNT; i++)
		{
			Sprite* objectNext = (objectList + i);
			//Make sure the sprite exists
			if (objectNext && objectNext->Created == 1 && objectNext->ZIndex >= currentZ && objectNext->SpriteType != HudType)
			{
				if (objectNext->ZIndex == currentZ)
				{
					DrawSprite(objectNext);
				}
				else
				{
					if (nextZ == -1 || nextZ > objectNext->ZIndex)
						nextZ = objectNext->ZIndex;
				}
			}
		}
		currentZ = nextZ;
	}
}

void freeObject(Sprite* objectNext)
{
	//Make sure the sprite exists
	if (objectNext && objectNext->Created == 1)
	{
		//Free the mesh and texture data
		objectNext->Created = 0;
		AEGfxMeshFree(objectNext->SpriteMesh);
		AEGfxTextureUnload(objectNext->SpriteTexture);
	}
}

void freeObjectList(void)
{
	// Freeing the objects and textures
	int i;
	for (i = 0; i < OBJECTAMOUNT; i++)
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
	}
	//Free the object list data allocation
	free(objectList);
}