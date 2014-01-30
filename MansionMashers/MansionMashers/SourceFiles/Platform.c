/*****************************************************************************/
/*!
\file				Platform.c
\author				Dan Muller (d.muller)
\date				Jan 27, 2014

\brief				Functions to do with a platform are here

\par				Functions:
\li					CreatePlatform
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include "../AEEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxObjects.h"

/*************************************************************************/
/*!
	\brief
	Create the platform
	
	\param CurrentPlatform
	A pointer to the platform to be initialized
*/
/*************************************************************************/
Platform* CreatePlatform(char* textureName, int collisionGroup, float width, float height, int newID)
{
	Platform *CurrentPlatform = AddPlatform();

	CurrentPlatform->PlatformSprite = CreateSprite(textureName, width, height, 4, 1, 1, PlatformType);

	CreateCollisionBox(&CurrentPlatform->PlatformCollider, &CurrentPlatform->Position, collisionGroup, width, height, newID);
	CurrentPlatform->objID = newID;
	InitializeRigidBody(&CurrentPlatform->PlatformRigidBody, TRUE, 400, 100);

	return CurrentPlatform;
}

