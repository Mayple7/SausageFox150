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
void CreateCollisionBox(CollisionBox *newBox, Vec2 *newPosition, int collisionGroup, float width, float height)
{
	newBox->canCollide = TRUE;
	newBox->Position = *newPosition;
	newBox->collisionGroup = collisionGroup;
	newBox->width = width;
	newBox->height = height;
	newBox->Offset.x = 0;
	newBox->Offset.y = 0;
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

