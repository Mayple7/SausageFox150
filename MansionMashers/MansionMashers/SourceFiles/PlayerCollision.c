/*****************************************************************************/
/*!
\file				PlayerCollision.c
\author				Dan Muller (d.muller
\date				Jan 28, 2014

\brief				Collision resolution functions for the player

\par				Functions:
\li					PlayerCollidePlatform
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/ObjectManager.h"
#include "../HeaderFiles/TextCreation.h"

/*************************************************************************/
/*!
	\brief
	Handles the collision between a player and platform
	
	\param CurrentPlayer
	Pointer to the player object

	\param CurrentPlatform
	Pointer to the platform object
*/
/*************************************************************************/
void PlayerCollidePlatform(Player *CurrentPlayer, Platform *CurrentPlatform)
{
	if(CurrentPlayer->PlayerRigidBody.Velocity.y <= 0)
	{
		if(CurrentPlayer->PlayerCollider.Position.y + CurrentPlayer->PlayerCollider.Offset.y - CurrentPlayer->PlayerCollider.height / 2.0f > CurrentPlatform->PlatformCollider.Position.y + CurrentPlatform->PlatformCollider.Offset.y)
		{
			if(CurrentPlayer->PlayerRigidBody.Velocity.y != 0 && CurrentPlatform->PlatformCollider.collisionGroup == PlatformType)
			{
				CurrentPlayer->Position.y = CurrentPlatform->PlatformCollider.Position.y + CurrentPlatform->PlatformCollider.Offset.y + CurrentPlatform->PlatformCollider.height / 2 - CurrentPlayer->PlayerCollider.Offset.y + CurrentPlayer->PlayerCollider.height / 2 - 0.01f;
				CurrentPlayer->PlayerRigidBody.onGround = TRUE;
			}
			else if(CurrentPlayer->PlayerRigidBody.Velocity.y != 0 && CurrentPlatform->PlatformCollider.collisionGroup == BounceType)
			{
				if(CurrentPlayer->PlayerRigidBody.Velocity.y > -(CurrentPlatform->PlatformRigidBody.Restitution + CurrentPlayer->PlayerRigidBody.Restitution))
				{
					CurrentPlayer->Position.y = CurrentPlatform->PlatformCollider.Position.y + CurrentPlatform->PlatformCollider.Offset.y + CurrentPlatform->PlatformCollider.height / 2 - CurrentPlayer->PlayerCollider.Offset.y + CurrentPlayer->PlayerCollider.height / 2 - 0.01f;
					CurrentPlayer->PlayerRigidBody.onGround = TRUE;
				}
				else
				{
					CurrentPlayer->Position.y = CurrentPlatform->PlatformCollider.Position.y + CurrentPlatform->PlatformCollider.Offset.y + CurrentPlatform->PlatformCollider.height / 2 - CurrentPlayer->PlayerCollider.Offset.y + CurrentPlayer->PlayerCollider.height / 2 + 0.5f;
					BounceObject(&CurrentPlayer->PlayerRigidBody, &CurrentPlatform->PlatformRigidBody);
					
				}
			}
			else
				CurrentPlayer->PlayerRigidBody.onGround = TRUE;
		}
	}
}


void PlayerCollideFood(Player *CurrentPlayer, Food *CurrentFood)
{
	printf("YUM YUM YUM YUM  DELICIOUSO\n");
	FreeFood(CurrentFood);
}

void PlayerCollideWeaponDrop(Player *CurrentPlayer, Weapon *wList)
{
	// Need to pop up weapon info //
	if(AEInputCheckTriggered('E'))
	{
		SwapWeapons(CurrentPlayer->PlayerWeapon, wList);
		CurrentPlayer->PlayerWeapon->WeaponSprite->ZIndex = (unsigned short)22;
		CurrentPlayer->PlayerWeapon->WeaponFOF = PlayerWeapon;
		CurrentPlayer->PlayerSpriteParts.Weapon = CurrentPlayer->PlayerWeapon->WeaponSprite;
		
		wList->WeaponSprite->ZIndex = (unsigned short)5;
		wList->WeaponFOF = DroppedWeapon;
		wList->WeaponSprite->Rotation = (float)FOX_PI / 4;
		wList->WeaponSprite->Position = wList->WeaponPickup.Position;
	}
}
