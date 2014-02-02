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
void CreateCollisionBox(CollisionBox *newBox, Vec2 *newPosition, int collisionGroup, float width, float height, int newID)
{
	newBox->canCollide = TRUE;
	newBox->Position = *newPosition;
	newBox->collisionGroup = collisionGroup;
	newBox->collisionID = newID;
	newBox->width = width;
	newBox->height = height;
	newBox->Offset.x = 0;
	newBox->Offset.y = 0;
	newBox->DebugMesh = createMesh(width, height, 1.0f, 1.0f, 0.0f);
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

	//Check if debug should be displayed
	if (Collider->collisionDebug)
		displayCollisionDebug(Collider);
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
	//Sprite Graphics Properties
	AEGfxSetPosition(Collider->Position.x + Collider->Offset.x, Collider->Position.y + Collider->Offset.y);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(Collider->DebugTexture, 1.0f, 1.0f);
	AEGfxMeshDraw(Collider->DebugMesh, AE_GFX_MDM_TRIANGLES);

	//NEED TO FREE THIS
	//AEGfxMeshFree(DebugMesh);
}