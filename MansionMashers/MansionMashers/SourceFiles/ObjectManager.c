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

Sprite* addObject(void)
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

void resetObjectList(void)
{
	//Set up the memory to fit the desired amount of objects
	objectList = (Sprite *)calloc(OBJECTAMOUNT, OBJECTAMOUNT * sizeof(Sprite));
	//Make sure the malloc is not NULL
	if (objectList)
	{
		/*
		//Print the size allocated
		int i;
		for (i = 0; i < OBJECTAMOUNT; i++)
		{
			printf("%i\n", sizeof(objectList[i]));
		}
		*/
		printf("\nOBJECT LIST SET UP COMPLETE\n\n");
	}
	else
	{
		printf("\nOBJECT LIST SET UP FAILED\n\n");
	}
}

void drawObjectList(void)
{
	int i;
	for (i = 0; i < OBJECTAMOUNT; i++)
	{
		Sprite* objectNext = (objectList + i);
		//Make sure the sprite exists
		if (objectNext && objectNext->Created == 1)
			DrawSprite(objectNext);
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