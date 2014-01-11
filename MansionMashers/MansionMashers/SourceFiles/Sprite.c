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

// ---------------------------------------------------------------------------

// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals

int AnimationActive = 0;

// ---------------------------------------------------------------------------
// Static function protoypes

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

int UpdateFrame(int totalFrames, int currentFrame, int frameUpdate, float *offsetX, float *offsetY)
{
	if(currentFrame >= totalFrames - 1 && frameUpdate)
		currentFrame = 0;
	else if(frameUpdate)
		currentFrame++;

	switch(currentFrame)
	{
	case 0:
		*offsetX = 0.0f;
		*offsetY = 0.0f;
		break;
	case 1:
		*offsetX = 0.25f;
		*offsetY = 0.0f;
		break;
	case 2:
		*offsetX = 0.5f;
		*offsetY = 0.0f;
		break;
	case 3:
		*offsetX = 0.75f;
		*offsetY = 0.0f;
		break;
	case 4:
		*offsetX = 0.0f;
		*offsetY = 0.5f;
		break;
	case 5:
		*offsetX = 0.25f;
		*offsetY = 0.5f;
		break;
	case 6:
		*offsetX = 0.5f;
		*offsetY = 0.5f;
		break;
	case 7:
		*offsetX = 0.75f;
		*offsetY = 0.5f;
		break;
	default:
		*offsetX = *offsetX;
		*offsetY = *offsetY;
		break;
	}

	return currentFrame;
}

struct Sprite CreateSprite(float width, float height, char* texture)
{
	struct Sprite CurrentSprite;
	
	//Sprite Graphics Properties
	AEGfxVertexList* SpriteMesh;
	AEGfxTexture *SpriteTexture;

	// Size of the sprite
	float Width;
	float Height;

	//Position of the sprite
	float XPosition;
	float YPosition;

	//Animation Properties
	int AnimationActive;
	int CurrentFrame;
	int TotalFrames;
	float AnimationSpeed;
	int AnimationTimer;

	//Texture Properties
	char TextureName[256];
	int NumHeightFrames;
	int NumWidthFrames;

	int Visible;
	int FlipX;
	int FlipY;
}

