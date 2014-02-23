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
	if(CurrentPlatform->PlatformCollider.collisionGroup != WallType)
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
	else if(CurrentPlatform->PlatformCollider.collisionGroup == WallType)
	{
		float PlayerLeft = CurrentPlayer->PlayerCollider.Position.x + CurrentPlayer->PlayerCollider.Offset.x - CurrentPlayer->PlayerCollider.width / 2;
		float PlayerRight = CurrentPlayer->PlayerCollider.Position.x + CurrentPlayer->PlayerCollider.Offset.x + CurrentPlayer->PlayerCollider.width / 2;
		float PlayerTop = CurrentPlayer->PlayerCollider.Position.y + CurrentPlayer->PlayerCollider.Offset.y + CurrentPlayer->PlayerCollider.height / 2;
		float PlayerBottom = CurrentPlayer->PlayerCollider.Position.y + CurrentPlayer->PlayerCollider.Offset.y - CurrentPlayer->PlayerCollider.height / 2;
		float PlatLeft = CurrentPlatform->PlatformCollider.Position.x + CurrentPlatform->PlatformCollider.Offset.x - CurrentPlatform->PlatformCollider.width / 2;
		float PlatRight = CurrentPlatform->PlatformCollider.Position.x + CurrentPlatform->PlatformCollider.Offset.x + CurrentPlatform->PlatformCollider.width / 2;
		float PlatTop = CurrentPlatform->PlatformCollider.Position.y + CurrentPlatform->PlatformCollider.Offset.y + CurrentPlatform->PlatformCollider.height / 2;
		float PlatBottom = CurrentPlatform->PlatformCollider.Position.y + CurrentPlatform->PlatformCollider.Offset.y - CurrentPlatform->PlatformCollider.height / 2;

		if((PlayerRight > PlatLeft || PlayerLeft < PlatRight) && PlayerTop > PlatBottom && CurrentPlayer->PlayerRigidBody.Velocity.y > 0)
		{
			CurrentPlayer->Position.y -= 2;
			ZeroVelocity(&CurrentPlayer->PlayerRigidBody);
		}
		if((PlayerRight > PlatLeft || PlayerLeft < PlatRight) && PlayerBottom < PlatTop && CurrentPlayer->PlayerRigidBody.Velocity.y <= 0)
		{
			ZeroVelocity(&CurrentPlayer->PlayerRigidBody);
			CurrentPlayer->PlayerRigidBody.onGround = TRUE;
		}
  		if(PlayerRight > PlatLeft && PlayerLeft < PlatLeft && CurrentPlayer->PlayerDirection == RIGHT)
		{
			CurrentPlayer->Speed = 0;
			CurrentPlayer->Position.x = PlatLeft - CurrentPlayer->PlayerCollider.width / 2 - .01f;
		}
		if(PlayerLeft <= PlatRight && PlayerRight >= PlatRight && CurrentPlayer->PlayerDirection == LEFT)
		{
			CurrentPlayer->Speed = 0;
			CurrentPlayer->Position.x = PlatRight + CurrentPlayer->PlayerCollider.width / 2 + .01f;
		}

		//if(CurrentPlayer.PlayerCollider.Position.x - CurrentPlayer.PlayerCollider.width / 2 < -7 * Level1Panel1->Width / 1)
			//CurrentPlayer.Position.x = (-7 * Level1Panel1->Width / 16) + (CurrentPlayer.PlayerCollider.width / 2) + 1;
		/*if(PlayerRight > PlatLeft && PlayerTop > PlatBottom && PlayerRight < PlatRight)
		{
			CurrentPlayer->Position.x = PlatLeft - (CurrentPlayer->PlayerCollider.width / 2) - 1;
		}
		else if(PlayerRight > PlatRight && PlayerTop > PlatBottom)
		{
			CurrentPlayer->Position.x = PlatRight + (CurrentPlayer->PlayerCollider.width / 2) - 1;
		}
		else if(PlayerRight > PlatLeft && PlayerRight < PlatRight)
		{
			if(CurrentPlayer->PlayerRigidBody.Velocity.y > 0 && PlayerTop > PlatBottom - (10.0f * GetLoadRatio()))
				ZeroVelocity(&CurrentPlayer->PlayerRigidBody);
		}*/
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
	if(AEInputCheckTriggered('E'))
	{
		SwapWeapons(CurrentPlayer->PlayerWeapon, wList);
		CurrentPlayer->PlayerWeapon->WeaponSprite->ZIndex = (unsigned short)22;
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
		
		
		wList->Position.x = CurrentPlayer->PlayerWeapon->Position.x;
		wList->Position.y = CurrentPlayer->PlayerWeapon->Position.y;
		wList->WeaponSprite->ZIndex = (unsigned short)5;
		wList->WeaponFOF = DroppedWeapon;
		wList->WeaponSprite->Rotation = (float)FOX_PI / 4;
		wList->WeaponSprite->Position = wList->WeaponPickup.Position;
		wList->WeaponHoverBackground->Position.x = wList->WeaponPickup.Position.x;
		wList->WeaponHoverBackground->Position.y = wList->WeaponPickup.Position.y + wList->WeaponPickup.height * 1.5f;
		Vec2Set(&glyphPos, wList->WeaponPickup.Position.x, (wList->WeaponPickup.Position.y + wList->WeaponPickup.height * 1.5f + wList->WeaponGlyphs->Glyph->Height / 2));
		ChangeTextPosition(wList->WeaponGlyphs, glyphPos, Center);
		Vec2Set(&glyphPos, wList->WeaponPickup.Position.x, (wList->WeaponPickup.Position.y + wList->WeaponPickup.height * 1.5f - wList->WeaponGlyphs->Glyph->Height / 2));
		ChangeTextPosition(wList->WeaponStatsGlyphs, glyphPos, Center);
		wList->WeaponHoverBackground->Position = CurrentPlayer->PlayerWeapon->WeaponHoverBackground->Position;
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
	damageDealt = CurrentEnemy->CurrentEnemyStats.Damage;
	
	CurrentPlayer->CurrentPlayerStats.CurrentHealth -= damageDealt;
	sprintf(num, "%d", damageDealt);
	// Create Floating Combat Text
	FirstLetter = CreateText(num, CurrentPlayer->Position.x / GetLoadRatio(), (CurrentPlayer->Position.y + CurrentPlayer->PlayerSpriteParts.Body->Height / 2) / GetLoadRatio(), 100, textColor, Center);
	AddFloatingText(FirstLetter);
	ChangeTextVisibility(FirstLetter);
}