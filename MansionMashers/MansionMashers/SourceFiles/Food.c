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
Food* CreateFood(int buffType, float width, float height, float xPos, float yPos, int objID)
{
	Food *CurrentFood = AddFood();

	Vec2Set(&CurrentFood->Position, xPos, yPos);
	
	CurrentFood->FoodType = buffType;

	switch(buffType)
	{
	case Agility:
		CurrentFood->FoodSprite = (Sprite *) CreateSprite("TextureFiles/Taco.png", width, height, 10, 1, 1, xPos, yPos);
		break;
	case Strength:
		CurrentFood->FoodSprite = (Sprite *) CreateSprite("TextureFiles/Ham.png", width, height, 10, 1, 1, xPos, yPos);
		break;
	case Defense:
		CurrentFood->FoodSprite = (Sprite *) CreateSprite("TextureFiles/Pizza.png", width, height, 10, 1, 1, xPos, yPos);
		break;
	case Haste:
		CurrentFood->FoodSprite = (Sprite *) CreateSprite("TextureFiles/Cake.png", width, height, 10, 1, 1, xPos, yPos);
		break;
	}

	CreateCollisionBox(&CurrentFood->FoodCollider, &CurrentFood->Position, FoodType, width, height, objID);
	CurrentFood->objID = objID;

	return CurrentFood;
}

