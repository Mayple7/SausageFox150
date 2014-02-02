/*****************************************************************************/
/*!
\file				Sprite.c
\author				Dan Muller (d.muller
\date				Jan 8, 2014

\brief				Has functions to create sprites and edit sprites

\par				Functions:
\li					createMesh
\li					CreateSprite
\li					DrawSprite
\li					UpdateMesh
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include "../AEEngine.h"
#include "../HeaderFiles/Sprite.h"
#include "../HeaderFiles/ObjectManager.h"
#include "../HeaderFiles/TextureManager.h"

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

/*************************************************************************/
/*!
	\brief
	Creates the mesh for the sprite
	
	\param width
	The width of the sprite

	\param height
	The height of the sprite
	
	\param offsetX
	The offset of the texture to place over the mesh in the X direction (1.0 is default)
	
	\param offsetY
	The offset of the texture to place over the mesh in the Y direction (1.0 is default)

	\param Rotation
	HUD objects to draw

	\return
	The lest of vertecies for the mesh creation
*/
/*************************************************************************/
AEGfxVertexList* createMesh(float width, float height, float offsetX, float offsetY, float Rotation)
{
	float halfWidth = width / 2;
	float halfHeight = height / 2;

	float len = (float)sqrt(halfWidth*halfWidth + halfHeight*halfHeight);

	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// 1 triangle at a time
	// X, Y, Color, texU, texV
	AEGfxTriAdd(
		len * (float)cos(atan2(halfHeight,halfWidth) + PI + Rotation), len * (float)sin(atan2(halfHeight,halfWidth) + PI + Rotation),		0x00FFFFFF, 0.0f, offsetY, 
		len * (float)cos(atan2(-halfHeight,halfWidth) + Rotation),		len * (float)sin(atan2(-halfHeight,halfWidth) + Rotation),			0x00FFFFFF, offsetX, offsetY,
		len * (float)cos(atan2(halfHeight,-halfWidth) + Rotation),		len * (float)sin(atan2(halfHeight,-halfWidth) + Rotation),			0x00FFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		len * (float)cos(atan2(-halfHeight,halfWidth) + Rotation),		len * (float)sin(atan2(-halfHeight,halfWidth) + Rotation),			0x00FFFFFF, offsetX, offsetY, 
		len * (float)cos(atan2(halfHeight,halfWidth) + Rotation),		len * (float)sin(atan2(halfHeight,halfWidth) + Rotation),			0x00FFFFFF, offsetX, 0.0f,
		len * (float)cos(atan2(halfHeight,-halfWidth) + Rotation),		len * (float)sin(atan2(halfHeight,-halfWidth) + Rotation),			0x00FFFFFF, 0.0f, 0.0f);

	//Returns the mesh
	return AEGfxMeshEnd();
}

/*************************************************************************/
/*!
	\brief
	Creates and returns the sprite and adds it to the object manager list

	\param texture
	Location of the texture for the sprite
	
	\param width
	The width of the sprite
	
	\param height
	The height of the sprite

	\param ZIndex
	The index for drawing the object (Highest number = drawn on top)

	\param xFrames
	The number of frames in the x direction

	\param yFrames
	The number of frames in the y direction

	\return
	A pointer to the sprite object
*/
/*************************************************************************/
Sprite* CreateSprite(char texture[], float width, float height, unsigned short ZIndex, int xFrames, int yFrames)
{	
	//Adds the sprite to the object manager list
	Sprite *CurrentSprite = AddObject();

	CurrentSprite->isHUD = FALSE;

	//Animation offset if needed
	CurrentSprite->OffsetX = 1.0f / xFrames;
	CurrentSprite->OffsetY = 1.0f / yFrames;

	//Rotation default to 0
	CurrentSprite->Rotation = 0.0f;
	CurrentSprite->RotationPrev = 0.0f;

	//Sprite Graphics Properties
	CurrentSprite->SpriteMesh = createMesh(width, height, CurrentSprite->OffsetX, CurrentSprite->OffsetY, CurrentSprite->Rotation);
	CurrentSprite->SpriteTexture = LoadTexture(texture);

	// Size of the sprite
	CurrentSprite->Width = width;
	CurrentSprite->Height = height;

	//Position of the sprite
	CurrentSprite->Position.x = 0.0f;
	CurrentSprite->Position.y = 0.0f;
	CurrentSprite->ZIndex     = ZIndex;

	//Animation Properties
	CurrentSprite->CurrentFrame = 0;
	CurrentSprite->TotalFrames = xFrames * yFrames;
	if(CurrentSprite->TotalFrames > 1)
		CurrentSprite->AnimationActive = TRUE;
	else
		CurrentSprite->AnimationActive = FALSE;
	CurrentSprite->AnimationSpeed = 12;
	CurrentSprite->AnimationTimer = 0;

	//Texture Properties
	//strcpy(CurrentSprite->TextureName, texture);
	CurrentSprite->NumHeightFrames = yFrames;
	CurrentSprite->NumWidthFrames = xFrames;

	CurrentSprite->Tint.x = 1.0f;
	CurrentSprite->Tint.y = 1.0f;
	CurrentSprite->Tint.z = 1.0f;

	CurrentSprite->Alpha = 1.0f;
	CurrentSprite->Visible = TRUE;
	CurrentSprite->FlipX = FALSE;
	CurrentSprite->FlipY = FALSE;
	CurrentSprite->FlipXPrev = FALSE;
	CurrentSprite->FlipYPrev = FALSE;

	CurrentSprite->Created = TRUE;

	return CurrentSprite;
}

/*************************************************************************/
/*!
	\brief
	Draws the sprite and updates the animation properties if needed

	\param CurrentSprite
	The sprite object to draw
*/
/*************************************************************************/
void DrawSprite(struct Sprite *CurrentSprite)
{
	//Calculate the offset difference between frames
	float offsetDiffX = 1.0f / CurrentSprite->NumWidthFrames;
	float offsetDiffY = 1.0f / CurrentSprite->NumHeightFrames;
	float offsetX = 0.0f;
	float offsetY = 0.0f;
	//Set draw mode to texture
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(CurrentSprite->Tint.x, CurrentSprite->Tint.y, CurrentSprite->Tint.z, 1.0f);
	// Set position for the sprite drawing
	AEGfxSetPosition(CurrentSprite->Position.x, CurrentSprite->Position.y);

	// Update the transparency of the sprite if visible or not
	if(CurrentSprite->Visible == FALSE)
	{
		AEGfxSetTransparency(0.0f);
	}
	else
	{
		AEGfxSetTransparency(CurrentSprite->Alpha);
	}
	// Update the current frame on the animation
	if(CurrentSprite->AnimationActive)
	{
		// Updates to the next frame of animation if needed
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
		// Sets the correct offset after updating the animation frame
		offsetX = ((CurrentSprite->CurrentFrame) % CurrentSprite->NumWidthFrames) * offsetDiffX;
		offsetY = ((CurrentSprite->CurrentFrame) / CurrentSprite->NumWidthFrames) * offsetDiffY;
	}
	else
	{
		// Sets the correct offset for the current frame
		offsetX = ((CurrentSprite->CurrentFrame) % CurrentSprite->NumWidthFrames) * offsetDiffX;
		offsetY = ((CurrentSprite->CurrentFrame) / CurrentSprite->NumWidthFrames) * offsetDiffY;
	}
	// Updates the rotation and mesh of the object to draw
	if (CurrentSprite->Rotation != CurrentSprite->RotationPrev || CurrentSprite->FlipX != CurrentSprite->FlipXPrev || CurrentSprite->FlipY != CurrentSprite->FlipYPrev)
	{
		CurrentSprite->SpriteMesh = createMesh(CurrentSprite->Width * (((2*CurrentSprite->FlipX)-1)*-1), CurrentSprite->Height * (((2*CurrentSprite->FlipY)-1)*-1), CurrentSprite->OffsetX, CurrentSprite->OffsetY, CurrentSprite->Rotation);
		CurrentSprite->RotationPrev = CurrentSprite->Rotation;
		CurrentSprite->FlipXPrev = CurrentSprite->FlipX;
		CurrentSprite->FlipYPrev = CurrentSprite->FlipY;
	}
	// Draws the texture on the mesh
	AEGfxTextureSet(CurrentSprite->SpriteTexture, offsetX, offsetY);
	// Draws the mesh
	AEGfxMeshDraw(CurrentSprite->SpriteMesh, AE_GFX_MDM_TRIANGLES);
}

/*************************************************************************/
/*!
	\brief
	Updates the mesh for a sprite
	
	\param currentSprite
	The sprite whose mesh will be updated
*/
/*************************************************************************/
void UpdateMesh(Sprite *currentSprite)
{
	currentSprite->SpriteMesh = createMesh(currentSprite->Width, currentSprite->Height, currentSprite->OffsetX, currentSprite->OffsetY, currentSprite->Rotation);
}

