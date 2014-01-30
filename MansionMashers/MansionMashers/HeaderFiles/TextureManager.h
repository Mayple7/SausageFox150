#ifndef FOX_TEX_MANAGER
#define FOX_TEX_MANAGER

#include "../AEEngine.h"

#define TEXTUREAMOUNT 20

typedef struct Texture
{
	//Created (BOOL 1 or 0 only)
	int Created;
	//Name
	char TextureName[24];
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