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
	Vec2 glyphPos;
	
	// Need to pop up weapon info //
	if(!wList->WeaponGlyphs->Glyph->Visible)
	{
		ChangeTextVisibility(wList->WeaponGlyphs);
		wList->WeaponHoverBackground->Visible = TRUE;
	}
	if(!wList->WeaponStatsGlyphs->Glyph->Visible)
	{
		ChangeTextVisibility(wList->WeaponStatsGlyphs);
	}
	//Switch weapons
	if(AEInputCheckTriggered('E'))
	{
		int Shopping = FALSE;

		//Check if the weapon is in a shop
		if (wList->CurrentShop)
		{
			if (wList->CurrentShop->Coin > CurrentPlayer->CurrentPlayerStats.Money)
			{
				//Not enough money
				Vec3 textColor;
				TextGlyphs *FirstLetter;
	
				// Create Floating Error Text
				Vec3Set(&textColor, 0.0f, 0.0f, 0.0f);
				FirstLetter = CreateText("Not Enough Coin!", (CurrentPlayer->Position.x - 4) / GetLoadRatio(), (CurrentPlayer->Position.y - 4 + CurrentPlayer->PlayerSpriteParts.Body->Height / 2) / GetLoadRatio(), 100, textColor, Center);
				AddFloatingText(FirstLetter);
				ChangeTextVisibility(FirstLetter);

				Vec3Set(&textColor, 1.0f, 0.1f, 0.1f);
				FirstLetter = CreateText("Not Enough Coin!", CurrentPlayer->Position.x / GetLoadRatio(), (CurrentPlayer->Position.y + CurrentPlayer->PlayerSpriteParts.Body->Height / 2) / GetLoadRatio(), 100, textColor, Center);
				AddFloatingText(FirstLetter);
				ChangeTextVisibility(FirstLetter);
				return;
			}
			
			CurrentPlayer->CurrentPlayerStats.Money -= wList->CurrentShop->Coin;
			Shopping = TRUE;
		}

		SwapWeapons(CurrentPlayer->PlayerWeapon, wList);

		//Set up the new player weapon sprite
		CurrentPlayer->PlayerWeapon->WeaponSprite->ZIndex = (unsigned short)50;
		CurrentPlayer->PlayerWeapon->WeaponFOF = PlayerWeapon;
		CurrentPlayer->PlayerSpriteParts.Weapon = CurrentPlayer->PlayerWeapon->WeaponSprite;
		if(CurrentPlayer->PlayerWeapon->WeaponGlyphs->Glyph->Visible)
		{
			ChangeTextVisibility(CurrentPlayer->PlayerWeapon->WeaponGlyphs);
		}
		if(CurrentPlayer->PlayerWeapon->WeaponStatsGlyphs->Glyph->Visible)
		{
			ChangeTextVisibility(CurrentPlayer->PlayerWeapon->WeaponStatsGlyphs);
		}
		CurrentPlayer->PlayerWeapon->WeaponHoverBackground->Visible = FALSE;
		
		//Put the new dropped weapon down
		wList->Position.x = CurrentPlayer->PlayerWeapon->Position.x;
		wList->Position.y = CurrentPlayer->PlayerWeapon->Position.y;
		wList->WeaponSprite->ZIndex = (unsigned short)50;
		wList->WeaponFOF = DroppedWeapon;
			
		wList->WeaponSprite->Rotation = (float)FOX_PI / 4;
		wList->WeaponSprite->Position = wList->WeaponPickup.Position;
		//If it is a shop we like it straight, oh ja
		if (Shopping)
		{
			//Put the weapon on the ground
			wList->WeaponSprite->Position.x = wList->WeaponPickup.Position.x;
			wList->WeaponSprite->Position.y = wList->WeaponPickup.Position.y - wList->CurrentShop->ShopSprite->Height / 1.8f;
			wList->WeaponPickup.Position = wList->WeaponSprite->Position;
			//Nothing is for sale, no need to keep the text
			FreeText(wList->CurrentShop->ItemTextCoin);
			FreeText(wList->CurrentShop->ItemTextName);
			//No longer in the shop
			wList->CurrentShop = NULL;
		}
		else
			wList->WeaponSprite->Position = wList->WeaponPickup.Position;

		wList->WeaponHoverBackground->Position.x = wList->WeaponPickup.Position.x;
		wList->WeaponHoverBackground->Position.y = wList->WeaponPickup.Position.y + wList->WeaponPickup.height * 1.5f;
		Vec2Set(&glyphPos, wList->WeaponPickup.Position.x, (wList->WeaponPickup.Position.y + wList->WeaponPickup.height * 1.5f + wList->WeaponGlyphs->Glyph->Height / 2));
		ChangeTextPosition(wList->WeaponGlyphs, glyphPos, Center);
		Vec2Set(&glyphPos, wList->WeaponPickup.Position.x, (wList->WeaponPickup.Position.y + wList->WeaponPickup.height * 1.5f - wList->WeaponGlyphs->Glyph->Height / 2));
		ChangeTextPosition(wList->WeaponStatsGlyphs, glyphPos, Center);
		//wList->WeaponHoverBackground->Position = CurrentPlayer->PlayerWeapon->WeaponHoverBackground->Position;
	}
}
// height +/- (fontsize/2)
void PlayerCollideEnemyWeapon(Player* CurrentPlayer, Enemy *CurrentEnemy)
{
	int damageDealt;
	char num[10];
	Vec3 textColor;
	TextGlyphs *FirstLetter;
	Vec3Set(&textColor, 1.0f, 1.0f, 1.0f);
	
	// Calculate damage
	if(Cheats)
		damageDealt = 0;
	else
		damageDealt = CurrentEnemy->CurrentEnemyStats.Damage;
	
	CurrentPlayer->CurrentPlayerStats.CurrentHealth -= damageDealt;
	sprintf(num, "%d", damageDealt);
	// Create Floating Combat Text
	FirstLetter = CreateText(num, CurrentPlayer->Position.x / GetLoadRatio(), (CurrentPlayer->Position.y + CurrentPlayer->PlayerSpriteParts.Body->Height / 2) / GetLoadRatio(), 100, textColor, Center);
	AddFloatingText(FirstLetter);
	ChangeTextVisibility(FirstLetter);
}

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
void PlayerCollideWall(Player *CurrentPlayer, Wall *CurrentWall)
{
	// Grab the edges of each collider box
	float PlayerLeft = CurrentPlayer->PlayerCollider.Position.x + CurrentPlayer->PlayerCollider.Offset.x - CurrentPlayer->PlayerCollider.width / 2;
	float PlayerRight = CurrentPlayer->PlayerCollider.Position.x + CurrentPlayer->PlayerCollider.Offset.x + CurrentPlayer->PlayerCollider.width / 2;
	float PlayerTop = CurrentPlayer->PlayerCollider.Position.y + CurrentPlayer->PlayerCollider.Offset.y + CurrentPlayer->PlayerCollider.height / 2;
	float PlayerBottom = CurrentPlayer->PlayerCollider.Position.y + CurrentPlayer->PlayerCollider.Offset.y - CurrentPlayer->PlayerCollider.height / 2;
	
	float WallLeft = CurrentWall->WallCollider.Position.x + CurrentWall->WallCollider.Offset.x - CurrentWall->WallCollider.width / 2;
	float WallRight = CurrentWall->WallCollider.Position.x + CurrentWall->WallCollider.Offset.x + CurrentWall->WallCollider.width / 2;
	float WallTop = CurrentWall->WallCollider.Position.y + CurrentWall->WallCollider.Offset.y + CurrentWall->WallCollider.height / 2;
	float WallBottom = CurrentWall->WallCollider.Position.y + CurrentWall->WallCollider.Offset.y - CurrentWall->WallCollider.height / 2;

	//Logic for being on top of the wall
	if(CurrentPlayer->PlayerRigidBody.Velocity.y <= 0 && PlayerBottom > WallTop - CurrentWall->WallCollider.height / 8)
	{
		CurrentPlayer->Position.y = WallTop - CurrentPlayer->PlayerCollider.Offset.y + CurrentPlayer->PlayerCollider.height / 2 - 0.01f;
		CurrentPlayer->PlayerRigidBody.Velocity.y = 0;
		CurrentPlayer->PlayerRigidBody.onGround = TRUE;
	}
	//Logic for being on the left side of the wall
	else if(CurrentPlayer->Speed > 0 && CurrentPlayer->PlayerDirection == RIGHT && PlayerRight < WallLeft + CurrentWall->WallCollider.width / 8)
	{
		CurrentPlayer->Speed = 0.0f;
		CurrentPlayer->Position.x = WallLeft - CurrentPlayer->PlayerCollider.Offset.x - CurrentPlayer->PlayerCollider.width / 2 + 0.01f;
	}
	//Logic for being on the right side of the wall
	else if(CurrentPlayer->Speed > 0 && CurrentPlayer->PlayerDirection == LEFT && PlayerLeft > WallRight - CurrentWall->WallCollider.width / 8)
	{
		CurrentPlayer->Speed = 0.0f;
		CurrentPlayer->Position.x = WallRight - CurrentPlayer->PlayerCollider.Offset.x + CurrentPlayer->PlayerCollider.width / 2 - 0.01f;
	}
	//Logic for being under a wall
	else if(CurrentPlayer->PlayerRigidBody.Velocity.y > 0 && PlayerTop < WallBottom + CurrentWall->WallCollider.height / 8)
	{
		CurrentPlayer->Position.y = WallBottom - CurrentPlayer->PlayerCollider.Offset.y - CurrentPlayer->PlayerCollider.height / 2 + 0.01f;
		CurrentPlayer->PlayerRigidBody.Velocity.y = 0;
		CurrentPlayer->PlayerRigidBody.onGround = TRUE;
	}
}
