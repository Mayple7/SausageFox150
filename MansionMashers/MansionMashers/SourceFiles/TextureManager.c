/*****************************************************************************/
/*!
\file				TextureManager.c
\author				Kaden Nugent (kaden.n)
\author				Dan Muller (d.muller)
\date				Jan 8, 2014

\brief				Manages loading and unloading textures

\par				Functions:
\li					LoadTexture
\li					UnloadTexture
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include <stdio.h>
#include "../HeaderFiles/FoxEngine.h"

// ---------------------------------------------------------------------------
// Main

/*************************************************************************/
/*!
	\brief
	Dynamically allocates the texture list
*/
/*************************************************************************/
void CreateTextureList(void)
{
	//Load my dick into the driver and then texturize me captain
	textureList = (Texture *) CallocMyAlloc(TEXTUREAMOUNT, sizeof(Texture));

	//Make sure the allocation is not NULL
	if (textureList)
	{
		printf("\nTEXTURE LIST SET UP COMPLETE\n");
	}
	else
	{
		printf("\nTEXTURE LIST SET UP FAILED\n");
	}
}

/*************************************************************************/
/*!
	\brief
	Removes all textures from texture list
*/
/*************************************************************************/
void DestroyTextureList(void)
{
	int i;
	for (i = 0; i < TEXTUREAMOUNT; i++)
	{
		//Find a sprite that is empty
		if (textureList[i].Created & CREATED)
		{
			textureList[i].Created = 0;
			AEGfxTextureUnload(textureList[i].TextureObject);
		}
	}

	FreeMyAlloc(textureList);
}

/*************************************************************************/
/*!
	\brief
	Adds a texture to the texture list
	
	\return
	Returns a pointer to the texture
*/
/*************************************************************************/
AEGfxTexture *LoadTexture(char *texture)
{
	int i;
	//Search to see if  that texture has already been created
	for (i = 0; i < TEXTUREAMOUNT; i++)
	{
		/* If an object is already created with that texture, 
		   share it like a good person should. */
		if (textureList[i].Created & CREATED)
		{
			if (!strcmp(textureList[i].TextureObject->mpName, texture))
			{
				return textureList[i].TextureObject;
			}
		}
		//No spot currently has the texture
		else
		{
			//Create a new texture
			textureList[i].Created = 1;
			textureList[i].TextureObject = AEGfxTextureLoad(texture);
			return textureList[i].TextureObject;
		}
	}

	//Everything borked if it gets here
	printf("We got a serious problem boss.");
	return NULL;
}
