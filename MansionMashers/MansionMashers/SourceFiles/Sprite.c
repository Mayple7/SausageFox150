/*
File:				Sprite.c
Author:				Dan Muller (d.muller)
Creation Date:		Jan 8, 2014

Purpose:			Sprite functions are here

Functions:			CreateSprite - Creates a sprite
 
Copyright (C) 2014 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited. 
*/ 



// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/Sprite.h"
#include "../HeaderFiles/ObjectManager.h"

// ---------------------------------------------------------------------------

// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals

int AnimationActive = 0;

// ---------------------------------------------------------------------------
// Static function protoypes
AEGfxVertexList* createMesh(float width, float height);


// ---------------------------------------------------------------------------
// main

AEGfxVertexList* createMesh(float width, float height, float offsetX, float offsetY, float Rotation)
{
	float halfWidth = width / 2;
	float halfHeight = height / 2;

	double len = sqrt(halfWidth*halfWidth + halfHeight*halfHeight);

	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// 1 triangle at a time
	// X, Y, Color, texU, texV
	AEGfxTriAdd(
		len * cos(atan2(halfHeight,halfWidth) + PI + Rotation), len * sin(atan2(halfHeight,halfWidth) + PI + Rotation),		0x00FFFFFF, 0.0f, offsetY, 
		len * cos(atan2(-halfHeight,halfWidth) + Rotation),		len * sin(atan2(-halfHeight,halfWidth) + Rotation),			0x00FFFFFF, offsetX, offsetY,
		len * cos(atan2(halfHeight,-halfWidth) + Rotation),		len * sin(atan2(halfHeight,-halfWidth) + Rotation),			0x00FFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		len * cos(atan2(-halfHeight,halfWidth) + Rotation),		len * sin(atan2(-halfHeight,halfWidth) + Rotation),			0x00FFFFFF, offsetX, offsetY, 
		len * cos(atan2(halfHeight,halfWidth) + Rotation),		len * sin(atan2(halfHeight,halfWidth) + Rotation),			0x00FFFFFF, offsetX, 0.0f,
		len * cos(atan2(halfHeight,-halfWidth) + Rotation),		len * sin(atan2(halfHeight,-halfWidth) + Rotation),			0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1

	return AEGfxMeshEnd();
}

//////  SPRITE NAME STRING    TEXTURE STRING    WIDTH    HEIGHT    AMNT FRAMES X    DRAW Z INDEX     AMNT FRAMES Y
Sprite* CreateSprite(char SpriteName[], char* texture, float width, float height, unsigned short ZIndex, int xFrames, int yFrames)
{	
	Sprite *CurrentSprite = AddObject();

	CurrentSprite->OffsetX = 1.0f / xFrames;
	CurrentSprite->OffsetY = 1.0f / yFrames;

	CurrentSprite->Rotation = 0.0f;
	CurrentSprite->RotationPrev = 0.0f;

	//Sprite Graphics Properties
	CurrentSprite->SpriteMesh = createMesh(width, height, CurrentSprite->OffsetX, CurrentSprite->OffsetY, CurrentSprite->Rotation);
	CurrentSprite->SpriteTexture = AEGfxTextureLoad(texture);

	// Size of the sprite
	CurrentSprite->Width = width;
	CurrentSprite->Height = height;

	//Position of the sprite
	CurrentSprite->Position.x = 0.0f;
	CurrentSprite->Position.y = 0.0f;
	CurrentSprite->ZIndex     = ZIndex;

	//Animation Properties
	CurrentSprite->AnimationActive = 0;
	CurrentSprite->CurrentFrame = 0;
	CurrentSprite->TotalFrames = xFrames * yFrames;
	CurrentSprite->AnimationSpeed = 12;
	CurrentSprite->AnimationTimer = 0;

	//Texture Properties
	strcpy(CurrentSprite->TextureName, texture);
	CurrentSprite->NumHeightFrames = yFrames;
	CurrentSprite->NumWidthFrames = xFrames;

	CurrentSprite->Alpha = 1.0f;
	CurrentSprite->Visible = 1;
	CurrentSprite->FlipX = 0;
	CurrentSprite->FlipY = 0;
	CurrentSprite->FlipXPrev = 0;
	CurrentSprite->FlipYPrev = 0;

	//Collision
	CurrentSprite->CanCollide     = 1;
	CurrentSprite->Ghost          = 1;
	CurrentSprite->SensorType     = RectangleCollider;
	CurrentSprite->CollideSize.x  = CurrentSprite->Width;
	CurrentSprite->CollideSize.y  = CurrentSprite->Height;

	//The sprite has now been created
	CurrentSprite->SpriteType = PartType;
	strcpy(CurrentSprite->SpriteName, SpriteName);
	CurrentSprite->Created = 1;

	return CurrentSprite;
}

void DrawSprite(struct Sprite *CurrentSprite)
{
	float offsetDiffX = 1.0f / CurrentSprite->NumWidthFrames;
	float offsetDiffY = 1.0f / CurrentSprite->NumHeightFrames;
	float offsetX = 0.0f;
	float offsetY = 0.0f;
	// Drawing Selector
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Set poisition for object 2
	AEGfxSetPosition(CurrentSprite->Position.x, CurrentSprite->Position.y);

	// Drawing the mesh (list of triangles)
	AEGfxSetTransparency(CurrentSprite->Alpha);
	//printf("%d\n", CurrentSprite->AnimationActive);
	if(CurrentSprite->AnimationActive)
	{
		CurrentSprite->AnimationTimer++;
		if(CurrentSprite->CurrentFrame >= CurrentSprite->TotalFrames - 1 && CurrentSprite->AnimationTimer >= CurrentSprite->AnimationSpeed)
		{
			CurrentSprite->CurrentFrame = 0;
			CurrentSprite->AnimationTimer = 0;
		}
		else if(CurrentSprite->AnimationTimer >= CurrentSprite->AnimationSpeed)
		{
			CurrentSprite->CurrentFrame++;
			CurrentSprite->AnimationTimer = 0;
		}
		offsetX = ((CurrentSprite->CurrentFrame) % CurrentSprite->NumWidthFrames) * offsetDiffX;
		offsetY = ((CurrentSprite->CurrentFrame) / CurrentSprite->NumWidthFrames) * offsetDiffY;
	}
	else
	{
		offsetX = ((CurrentSprite->CurrentFrame) % CurrentSprite->NumWidthFrames) * offsetDiffX;
		offsetY = ((CurrentSprite->CurrentFrame) % CurrentSprite->NumHeightFrames) * offsetDiffY;
	}
	if (CurrentSprite->Rotation != CurrentSprite->RotationPrev || CurrentSprite->FlipX != CurrentSprite->FlipXPrev || CurrentSprite->FlipY != CurrentSprite->FlipYPrev)
	{
		CurrentSprite->SpriteMesh = createMesh(CurrentSprite->Width * (((2*CurrentSprite->FlipX)-1)*-1), CurrentSprite->Height * (((2*CurrentSprite->FlipY)-1)*-1), CurrentSprite->OffsetX, CurrentSprite->OffsetY, CurrentSprite->Rotation);
		CurrentSprite->RotationPrev = CurrentSprite->Rotation;
		CurrentSprite->FlipXPrev = CurrentSprite->FlipX;
		CurrentSprite->FlipYPrev = CurrentSprite->FlipY;
	}
	AEGfxTextureSet(CurrentSprite->SpriteTexture, offsetX, offsetY);
	AEGfxMeshDraw(CurrentSprite->SpriteMesh, AE_GFX_MDM_TRIANGLES);
}


