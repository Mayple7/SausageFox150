/*****************************************************************************/
/*!
\file				TextureManager.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_TEX_MANAGER
#define FOX_TEX_MANAGER

#include "../AEEngine.h"

#define TEXTUREAMOUNT 200

typedef struct Texture
{
	//Created (BOOL 1 or 0 only)
	int Created;
	//Texture
	AEGfxTexture *TextureObject;
}Texture;

//Texture allocated sexture
Texture *textureList;

//Functionz
void CreateTextureList(void);
void DestroyTextureList(void);
AEGfxTexture *LoadTexture(char *texture);

#endif