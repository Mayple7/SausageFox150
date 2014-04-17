/*****************************************************************************/
/*!
\file				CollisionBox.c
\author				Dan Muller (d.muller
\date				Jan 28, 2014

\brief				Has the creation function for a collision box

\par				Functions:
\li					CreateCollisionBox
\li					UpdateCollisionPosition
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include "../HeaderFiles/FoxEngine.h"
#include "../AEEngine.h"

/*************************************************************************/
/*!
	\brief
	Creates the collision box
	
	\param newBox
	The collision box object

	\param newPosition
	The position of the collision box

	\param collisionGroup
	The collision group of the collision group

	\param width
	The width of the collision box

	\param height
	The height of the collision box
*/
/*************************************************************************/
void CreateCollisionBox(CollisionBox *newBox, Vec2 *newPosition, int collisionGroup, float width, float height, int objectID)
{
	newBox->canCollide = TRUE;
	newBox->Position = *newPosition;
	newBox->collisionGroup = collisionGroup;
	newBox->collisionID = objectID;
	newBox->width = width;
	newBox->height = height;
	newBox->Offset.x = 0;
	newBox->Offset.y = 0;
	newBox->DebugMesh = createMesh(width, height, 1.0f, 1.0f);
	newBox->DebugTexture = LoadTexture("TextureFiles/DebugBox.png");
}

/*************************************************************************/
/*!
	\brief
	Updates the position of the collision box
	
	\param Collider
	The collision box object

	\param newPosition
	The position of the collision box
*/
/*************************************************************************/
void UpdateCollisionPosition(CollisionBox *Collider, Vec2 *newPosition)
{
	Collider->Position = *newPosition;
}

/*************************************************************************/
/*!
	\brief
	Updates the size of the collision box
	
	\param Collider
	The collision box object

	\param width
	The width of the collision box

	\param height
	The height of the collision box
*/
/*************************************************************************/
void UpdateCollider(CollisionBox *Collider, float width, float height)
{
	if(Collider->collisionID)
	{
		//Update collision size
		Collider->width = width;
		Collider->height = height;

		//Update collision debug box
		AEGfxMeshFree(Collider->DebugMesh);
		Collider->DebugMesh = createMesh(width, height, 1.0f, 1.0f);
	}
}

/*************************************************************************/
/*!
	\brief
	Draws the collision debug box
	
	\param Collider
	The collision box object
*/
/*************************************************************************/
void displayCollisionDebug(CollisionBox *Collider)
{
	if(Collider->collisionID > 0 && Collider->DebugTexture)
	{
		Matrix3 Mtx;	  //For the sprite position, rotation and scale
		Matrix3 FinalMtx; //The final matrix to set

		Matrix3 TempMtx;  //A placeholder
		Matrix3 TransMtx; //The traslation matrix

		//Sprite Graphics Properties
		//AEGfxSetPosition(Collider->Position.x + Collider->Offset.x, Collider->Position.y + Collider->Offset.y);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxTextureSet(Collider->DebugTexture, 1.0f, 1.0f);
		AEGfxSetTransparency(1.0f);

		//GET COLLIDER TRANSLATION RATIO
		Matrix3Translate(&TransMtx, Collider->Position.x + Collider->Offset.x, Collider->Position.y + Collider->Offset.y);

		//Just an empty matrix (No scale or rotation)
		TempMtx.m[0][0] = 1;
		TempMtx.m[0][1] = 0;
		TempMtx.m[0][2] = 0;
		TempMtx.m[1][0] = 0;
		TempMtx.m[1][1] = 1;
		TempMtx.m[1][2] = 0;
		TempMtx.m[2][0] = 0;
		TempMtx.m[2][1] = 0;
		TempMtx.m[2][2] = 1;

		Matrix3Mult(&Mtx, &TransMtx, &TempMtx);

		//Integrate the load ratio matrix
		Matrix3Mult(&FinalMtx, &LoadRatioMtx, &Mtx);
		AEGfxSetTransform(FinalMtx.m);
		AEGfxMeshDraw(Collider->DebugMesh, AE_GFX_MDM_TRIANGLES);
	}
}
