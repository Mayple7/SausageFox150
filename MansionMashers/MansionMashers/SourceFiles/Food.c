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
Food *CreateFood(int buffType, float width, float height, float xPos, float yPos)
{
	Food *CurrentFood = AddFood();

	Vec2Set(&CurrentFood->Position, xPos, yPos);
	
	CurrentFood->FoodType = buffType;

	switch(buffType)
	{
	case Agility:
		CurrentFood->FoodSprite = (Sprite *) CreateSprite("TextureFiles/AgilityBuff.png", width, height, 10, 1, 1, xPos, yPos);
		break;
	case Strength:
		CurrentFood->FoodSprite = (Sprite *) CreateSprite("TextureFiles/StrengthBuff.png", width, height, 10, 1, 1, xPos, yPos);
		break;
	case Defense:
		CurrentFood->FoodSprite = (Sprite *) CreateSprite("TextureFiles/DefenseBuff.png", width, height, 10, 1, 1, xPos, yPos);
		break;
	case Heal:
		CurrentFood->FoodSprite = (Sprite *) CreateSprite("TextureFiles/HealthBuff.png", width, height, 10, 1, 1, xPos, yPos);
		break;
	}

	CreateCollisionBox(&CurrentFood->FoodCollider, &CurrentFood->Position, FoodType, width, height, GetObjectID());
	CurrentFood->objID = GetObjectID();

	CurrentFood->FoodParticle = CreateFoxParticleSystem("TextureFiles/ParticlePlatform.png", xPos, yPos, 9, -1, 3, .15f, 270, 0, .5f, 0, 110, 100, 50.0f, 1.0f, 0.7f);


	return CurrentFood;
}
