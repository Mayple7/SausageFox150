/*****************************************************************************/
/*!
\file				ObjectManager.c
\author				Kaden Nugent (kaden.n)
\author				Dan Muller (d.muller)
\date				Jan 8, 2014

\brief				Manages the objects and allocating and freeing the memory

\par				Functions:
\li					AddObject
\li					AddCollidable
\li					resetObjectList
\li					drawObjectList
\li					freeObject
\li					freeObjectList
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include "../HeaderFiles/ObjectManager.h"
#include "../HeaderFiles/FoxEngine.h"

// ---------------------------------------------------------------------------
// Main

/*************************************************************************/
/*!
	\brief
	Adds an object to the list
	
	\return
	Returns a sprite pointer of the object added
*/
/*************************************************************************/
Sprite* AddObject(void)
{
	int i;
	for (i = 0; i < OBJECTAMOUNT; i++)
	{
		//Find a sprite that is empty
		if (drawList[i].Created != 1)
		{
			printf("Slot %i is now filled\n", i);
			return &drawList[i];
		}
	}
	return NULL;
}

/*************************************************************************/
/*!
	\brief
	Adds a collidable sprite to the collidable list
	
	\param newCollidable
	The sprite of a collidable to add to the list
*/
/*************************************************************************/
void AddCollidable(Sprite *newCollidable)
{
	int i;
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Find a sprite that is empty
		if (collidables[i].Created != 1)
		{
			printf("Slot %i is now collidable\n", i);
			collidables = newCollidable;
			return;
		}
	}
}

/*************************************************************************/
/*!
	\brief
	Adds a platform to the platform list
	
	\param newPlatform
	The platform to add to the list
*/
/*************************************************************************/
Platform* AddPlatform(void)
{
	int i;
	for (i = 0; i < COLLIDEAMOUNT; i++)
	{
		if(platformList[i].objID == NULL)
		{
			printf("Platform at %i Created\n", i);
			return &platformList[i];
		}

	}

}

/*************************************************************************/
/*!
	\brief
	Resets all the objects in the list and callocs a new array of sprites
*/
/*************************************************************************/
void resetObjectList(void)
{
	//Set up the memory to fit the desired amount of objects
	drawList  = (Sprite *) calloc(OBJECTAMOUNT, OBJECTAMOUNT * sizeof(Sprite));
	//Make sure the calloc is not NULL
	if (drawList)
	{
		printf("\nOBJECT LIST SET UP COMPLETE\n\n");

		//Set up collisions
		platformList = (Platform *) calloc(COLLIDEAMOUNT, COLLIDEAMOUNT * sizeof(Platform));
		collideList = (Sprite *) calloc(COLLIDEAMOUNT, COLLIDEAMOUNT * sizeof(Sprite));
		collidables = (Sprite *) calloc(COLLIDEAMOUNT, COLLIDEAMOUNT * sizeof(Sprite));

		//Make sure the malloc is not NULL
		if (collideList && collidables && platformList)
		{
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

/*************************************************************************/
/*!
	\brief
	Draws all the objects in the object list
*/
/*************************************************************************/
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
			Sprite* objectNext = (drawList + i);
			//Make sure the sprite exists
			if (objectNext && objectNext->Created == 1 && objectNext->ZIndex >= currentZ && objectNext->CollisionGroup != HudType)
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

/*************************************************************************/
/*!
	\brief
	Frees the sprite being passed
	
	\param objectNext
	The pointer to a sprite whose memory is to be deallocated
*/
/*************************************************************************/
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

/*************************************************************************/
/*!
	\brief
	Cycles through the object list freeing all objects in the list
*/
/*************************************************************************/
void freeObjectList(void)
{
	// Freeing the objects and textures
	int i;
	for (i = 0; i < OBJECTAMOUNT; i++)
	{
		//Make sure the sprite exists
		if (drawList[i].Created == 1)
		{
			//Free the mesh and texture data
			AEGfxMeshFree(drawList[i].SpriteMesh);
			AEGfxTextureUnload(drawList[i].SpriteTexture);
			printf("Slot %i is now empty\n", i);
		}
	}
	//Free the object list data allocation
	free(drawList);
	free(platformList);
	//free(collideList);
	//free(collidables);
}


