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
void InitializePlatform(Platform* CurrentPlatform, int collisionGroup)
{
	CurrentPlatform->PlatformSprite = CreateSprite("BouncePad", "TextureFiles/BouncePad.png", 400, 100, 8, 1, 1, PlatformType);
	CurrentPlatform->PlatformSprite->Position.x = -1000;
	CurrentPlatform->PlatformSprite->Position.y = -200;
	InitializeRigidBody(&CurrentPlatform->PlatformRigidBody, TRUE, 400, 100);
}

