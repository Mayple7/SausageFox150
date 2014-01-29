/*****************************************************************************/
/*!
\file				Platform.c
\author				Dan Muller (d.muller)
\date				Jan 27, 2014

\brief				Functions to do with a platform are here

\par				Functions:
\li					InitializePlatform
  
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
	Initializes the platform
	
	\param CurrentPlatform
	A pointer to the platform to be initialized
*/
/*************************************************************************/
Platform* CreatePlatform(char* textureName, int collisionGroup, int newID, float width, float height)
{
	Platform *CurrentPlatform = AddPlatform();

	CurrentPlatform->PlatformSprite = CreateSprite("BouncePad", textureName, width, height, 8, 1, 1, PlatformType);


	CreateCollisionBox(&CurrentPlatform->PlatformCollider, &CurrentPlatform->Position, width, height);
	CurrentPlatform->objID = newID;
	InitializeRigidBody(&CurrentPlatform->PlatformRigidBody, TRUE, 400, 100);

	return CurrentPlatform;
}

