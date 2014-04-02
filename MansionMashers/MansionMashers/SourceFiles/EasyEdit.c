/*****************************************************************************/
/*!
\file				EasyEdit.c
\author				Juli Gregg
\date				Mar 11, 2014

\brief				

\par				Functions:
\li					
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

#include "../AEEngine.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxObjects.h"

/*************************************************************************/
/*!
	\brief
	For moving sprites around while editing levels

	\param obj
	Object to move around

	\param mult
	How many space to move object at a time
*/
/*************************************************************************/
void EasyEditSprite(Sprite *obj, int mult)
{
	//Use arrow keys for input
	if(FoxInput_KeyTriggered(VK_UP))
		obj->Position.y += 1 * mult;
	if(FoxInput_KeyTriggered(VK_DOWN))
		obj->Position.y -= 1 * mult;
	if(FoxInput_KeyTriggered(VK_LEFT))
		obj->Position.x -= 1 * mult;
	if(FoxInput_KeyTriggered(VK_RIGHT))
		obj->Position.x += 1 * mult;

	//Print coordinates to console
	if(FoxInput_KeyTriggered('P'))
	{
		printf("OBJ X : %f\n", obj->Position.x);
		printf("OBJ Y : %f\n", obj->Position.y);
	}

}

/*************************************************************************/
/*!
	\brief
	For moving platforms around while editing levels

	\param obj
	Object to move around

	\param mult
	How many space to move object at a time
*/
/*************************************************************************/
void EasyEditPlatform(Platform *obj, int mult)
{
	//Use arrow keys for input
	if(FoxInput_KeyTriggered(VK_UP))
		obj->PlatformCollider.Position.y += 1 * mult;
	if(FoxInput_KeyTriggered(VK_DOWN))
		obj->PlatformCollider.Position.y -= 1 * mult;
	if(FoxInput_KeyTriggered(VK_LEFT))
		obj->PlatformCollider.Position.x -= 1 * mult;
	if(FoxInput_KeyTriggered(VK_RIGHT))
		obj->PlatformCollider.Position.x += 1 * mult;

	//Print coordinates to console
	if(FoxInput_KeyTriggered('P'))
	{
		printf("OBJ X : %f\n", obj->PlatformCollider.Position.x);
		printf("OBJ Y : %f\n", obj->PlatformCollider.Position.y);
	}
}

/*************************************************************************/
/*!
	\brief
	For moving walls around while editing levels

	\param obj
	Object to move around

	\param mult
	How many space to move object at a time
*/
/*************************************************************************/
void EasyEditWall(Wall *obj, int mult)
{
	//Use arrow keys for input
	if(FoxInput_KeyTriggered(VK_UP))
		obj->WallCollider.Position.y += 1 * mult;
	if(FoxInput_KeyTriggered(VK_DOWN))
		obj->WallCollider.Position.y -= 1 * mult;
	if(FoxInput_KeyTriggered(VK_LEFT))
		obj->WallCollider.Position.x -= 1 * mult;
	if(FoxInput_KeyTriggered(VK_RIGHT))
		obj->WallCollider.Position.x += 1 * mult;

	//Print coordinates to console
	if(FoxInput_KeyTriggered('P'))
	{
		printf("OBJ X : %f\n", obj->WallCollider.Position.x);
		printf("OBJ Y : %f\n", obj->WallCollider.Position.y);
	}
}
