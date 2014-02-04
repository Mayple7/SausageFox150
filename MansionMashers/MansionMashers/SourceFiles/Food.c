/*****************************************************************************/
/*!
\file				Food.c
\author				Dan Muller (d.muller)
\date				Jan 29, 2014

\brief				Functions to do with food are here

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
	Creates the food object
	
	\param CurrentPlatform
	A pointer to the platform to be initialized
*/
/*************************************************************************/
Food* CreateFood(char* textureName, int collisionGroup, float width, float height, int newID)
{
	Food *CurrentFood = AddFood();

	CurrentFood->FoodSprite = CreateSprite(textureName, width, height, 10, 1, 1);

	CreateCollisionBox(&CurrentFood->FoodCollider, &CurrentFood->Position, collisionGroup, width, height, newID);
	CurrentFood->objID = newID;
	CurrentFood->FoodCollider.collisionDebug = TRUE;

	return CurrentFood;
}

void UpdateFoodPosition(Food *CurrentFood, float x, float y)
{
	Vec2Set(&CurrentFood->Position, x, y);
	CurrentFood->FoodCollider.Position = CurrentFood->Position;
	CurrentFood->FoodSprite->Position = CurrentFood->Position;
}

