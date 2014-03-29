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
#include "../HeaderFiles/FoxEngine.h"

// ---------------------------------------------------------------------------
// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals

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
AEGfxVertexList* createMesh(float width, float height, float offsetX, float offsetY)
{
	float halfWidth = width / 2;
	float halfHeight = height / 2;
	float Rotation = 0;
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
void* CreateSprite(char texture[], float width, float height, unsigned short ZIndex, int xFrames, int yFrames, float xPos, float yPos)
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
	CurrentSprite->SpriteMesh = createMesh(width, height, CurrentSprite->OffsetX, CurrentSprite->OffsetY);
	CurrentSprite->SpriteTexture = LoadTexture(texture);

	CurrentSprite->MeshOwner = TRUE;

	// Size of the sprite
	CurrentSprite->Width = width;
	CurrentSprite->Height = height;

	//Position of the sprite
	CurrentSprite->Position.x = xPos;
	CurrentSprite->Position.y = yPos;
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

	CurrentSprite->ScaleX = 1.0f;
	CurrentSprite->ScaleY = 1.0f;

	CurrentSprite->Created = TRUE;

	return CurrentSprite;
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
Sprite* CreateSpriteNoMesh(char texture[], float width, float height, unsigned short ZIndex, int xFrames, int yFrames, float xPos, float yPos)
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
	CurrentSprite->SpriteTexture = LoadTexture(texture);

	CurrentSprite->MeshOwner = FALSE;

	// Size of the sprite
	CurrentSprite->Width = width;
	CurrentSprite->Height = height;

	//Position of the sprite
	CurrentSprite->Position.x = xPos;
	CurrentSprite->Position.y = yPos;
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

	CurrentSprite->ScaleX = 1.0f;
	CurrentSprite->ScaleY = 1.0f;


	CurrentSprite->Created = TRUE;

	return CurrentSprite;
}

/*************************************************************************/
/*!
	\brief
	Updates the animation properties if needed

	\param CurrentSprite
	The sprite object to update

	\param offset
	The sprite offset to form
*/
/*************************************************************************/
void updateSpriteAnimation(Sprite *CurrentSprite, Vec2 *offset)
{
	//Calculate the offset difference between frames
	Vec2 offsetDiff;

	Vec2Set(&offsetDiff, 1.0f / CurrentSprite->NumWidthFrames, 1.0f / CurrentSprite->NumHeightFrames);
	Vec2Zero(offset);

	if(CurrentSprite->AnimationActive && !pause)
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
		offset->x = ((CurrentSprite->CurrentFrame) % CurrentSprite->NumWidthFrames) * offsetDiff.x;
		offset->y = ((CurrentSprite->CurrentFrame) / CurrentSprite->NumWidthFrames) * offsetDiff.y;
	}
	else
	{
		// Sets the correct offset for the current frame
		offset->x = ((CurrentSprite->CurrentFrame) % CurrentSprite->NumWidthFrames) * offsetDiff.x;
		offset->y = ((CurrentSprite->CurrentFrame) / CurrentSprite->NumWidthFrames) * offsetDiff.y;
	}
}

/*************************************************************************/
/*!
	\brief
	Draws the sprite

	\param CurrentSprite
	The sprite object to draw
*/
/*************************************************************************/
void DrawSprite(Sprite *CurrentSprite)
{
	Vec2 offset;      //For the sprite animation
	Matrix3 Mtx;	  //For the sprite position, rotation and scale
	Matrix3 FinalMtx; //The final matrix to set

	//Set draw mode to texture
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(CurrentSprite->Tint.x, CurrentSprite->Tint.y, CurrentSprite->Tint.z, 1.0f);

	//Set position for the sprite drawing
	Mtx = CreateTranslationMtx(CurrentSprite);

	//Integrate the load ratio matrix
	Matrix3Mult(&FinalMtx, &LoadRatioMtx, &Mtx);
	AEGfxSetTransform(FinalMtx.m);
	
	//Update the transparency of the sprite if visible or not
	if(CurrentSprite->Visible == FALSE)
		AEGfxSetTransparency(0.0f);
	else
		AEGfxSetTransparency(CurrentSprite->Alpha);

	//Update the current frame on the animation
	updateSpriteAnimation(CurrentSprite, &offset);

	//Draw it like you wanna draw it gurl
	/*if (GetLoadRatio() < 0.75f)
		AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
	else*/
	AEGfxSetTextureMode(AE_GFX_TM_PRECISE);

	//Draws the texture on the mesh
	AEGfxTextureSet(CurrentSprite->SpriteTexture, offset.x, offset.y);
	//Draws the mesh
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
	currentSprite->SpriteMesh = createMesh(currentSprite->Width, currentSprite->Height, currentSprite->OffsetX, currentSprite->OffsetY);
}

Matrix3 CreateTranslationMtx(struct Sprite *CurrentSprite)
{
	Matrix3 tempMtx;
	Matrix3 rotMtx; 
	Matrix3 transMtx; 
	Matrix3 scalMtx; 

	//Get the scale, rotation, and translation of the sprite
	Matrix3ScaleMatrix(&scalMtx, (float)(((2 * CurrentSprite->FlipX)-1)*-1) * CurrentSprite->ScaleX, (float)(((2 * CurrentSprite->FlipY)-1)*-1) * CurrentSprite->ScaleY);
	Matrix3RotRad(&rotMtx, CurrentSprite->Rotation);
	Matrix3Translate(&transMtx, CurrentSprite->Position.x, CurrentSprite->Position.y);

	Matrix3Mult(&tempMtx, &rotMtx, &scalMtx);
	Matrix3Mult(&tempMtx, &transMtx, &tempMtx);

	return tempMtx;
}
