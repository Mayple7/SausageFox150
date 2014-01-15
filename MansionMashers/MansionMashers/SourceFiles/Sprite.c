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

AEGfxVertexList* CreateSpriteTexture(float width, float height)
{
	float halfWidth = width / 2;
	float halfHeight = height / 2;

	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// 1 triangle at a time
	// X, Y, Color, texU, texV
	AEGfxTriAdd(
		-halfWidth, -halfHeight, 0x00FFFFFF, 0.0f, 1.0f, 
		halfWidth,  -halfHeight, 0x00FFFFFF, 1.0f, 1.0f,
		-halfWidth,  halfHeight, 0x00FFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		halfWidth, -halfHeight, 0x00FFFFFF, 1.0f, 1.0f, 
		halfWidth,  halfHeight, 0x00FFFFFF, 1.0f, 0.0f,
		-halfWidth,  halfHeight, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1

	return AEGfxMeshEnd();
}

AEGfxVertexList* createMesh(float width, float height, float offsetX, float offsetY)
{
	float halfWidth = width / 2;
	float halfHeight = height / 2;

	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// 1 triangle at a time
	// X, Y, Color, texU, texV
	AEGfxTriAdd(
		-halfWidth, -halfHeight, 0x00FFFFFF, 0.0f, offsetY, 
		halfWidth,  -halfHeight, 0x00FFFFFF, offsetX, offsetY,
		-halfWidth,  halfHeight, 0x00FFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		halfWidth, -halfHeight, 0x00FFFFFF, offsetX, offsetY, 
		halfWidth,  halfHeight, 0x00FFFFFF, offsetX, 0.0f,
		-halfWidth,  halfHeight, 0x00FFFFFF, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1

	return AEGfxMeshEnd();
}

Sprite* CreateSprite(float width, float height, int xFrames, int yFrames, char* texture)
{	
	Sprite *CurrentSprite = addObject();

	CurrentSprite->OffsetX = 1.0f / xFrames;
	CurrentSprite->OffsetY = 1.0f / yFrames;

	//Sprite Graphics Properties
	CurrentSprite->SpriteMesh = createMesh(width, height, CurrentSprite->OffsetX, CurrentSprite->OffsetY);
	CurrentSprite->SpriteTexture = AEGfxTextureLoad(texture);

	// Size of the sprite
	CurrentSprite->Width = width;
	CurrentSprite->Height = height;

	//Position of the sprite
	CurrentSprite->Position.x = 0.0f;
	CurrentSprite->Position.y = 0.0f;

	//Animation Properties
	CurrentSprite->AnimationActive = 0;
	CurrentSprite->CurrentFrame = 0;
	CurrentSprite->TotalFrames = xFrames * yFrames;
	CurrentSprite->AnimationSpeed = 1;
	CurrentSprite->AnimationTimer = 0;

	//Texture Properties
	strcpy(CurrentSprite->TextureName, texture);
	CurrentSprite->NumHeightFrames = yFrames;
	CurrentSprite->NumWidthFrames = xFrames;

	CurrentSprite->Alpha = 1.0f;
	CurrentSprite->Visible = 1;
	CurrentSprite->FlipX = 0;
	CurrentSprite->FlipY = 0;

	//Collision
	CurrentSprite->CanCollide = 1;
	CurrentSprite->Ghost      = 1;
	CurrentSprite->IsHit      = 0;
	CurrentSprite->SensorType = RectangleCollider;

	//The sprite has now been created
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

	AEGfxTextureSet(CurrentSprite->SpriteTexture, offsetX, offsetY);
	AEGfxMeshDraw(CurrentSprite->SpriteMesh, AE_GFX_MDM_TRIANGLES);
}


