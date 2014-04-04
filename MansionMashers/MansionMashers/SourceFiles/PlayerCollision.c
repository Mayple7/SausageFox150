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
	if(CurrentPlayer->dropdownTimer > 0.0f)
		return;
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
	if(!CurrentPlayer->BuffHeld[0] && !CurrentPlayer->BuffHeld[1] && !CurrentPlayer->BuffHeld[2] && !CurrentPlayer->BuffHeld[3])
	{
		CurrentPlayer->BuffSelected = CurrentFood->FoodType;
	}

	switch(CurrentFood->FoodType)
	{
	case Agility:
		CurrentPlayer->BuffHeld[0] = TRUE;
		break;
	case Strength:
		CurrentPlayer->BuffHeld[1] = TRUE;
		break;
	case Defense:
		CurrentPlayer->BuffHeld[2] = TRUE;
		break;
	case Heal:
		CurrentPlayer->BuffHeld[3] = TRUE;
		break;
	}

	printf("YUM YUM YUM YUM  DELICIOUSO\n");
	FreeFood(CurrentFood);
}

void PlayerCollideWeaponDrop(Player *CurrentPlayer, Weapon *wList)
{
	Vec2 glyphPos;
	
	// Need to pop up weapon info //
	if(!wList->WeaponGlyphs->Glyph->Visible)
	{
		TextAllVisible(wList->WeaponGlyphs);
		wList->WeaponHoverBackground->Visible = TRUE;
		TextAllVisible(CurrentPlayer->ComparisonGlyphs);
		CreateComparisonText(CurrentPlayer, wList);
		ComparisonTextUpdate(CurrentPlayer->ComparisonGlyphs);
		ChangeTextZIndex(CurrentPlayer->ComparisonGlyphs, 451);
		ChangeTextZIndex(wList->WeaponGlyphs, 451);
		ChangeTextZIndex(wList->WeaponStatsGlyphs, 451);
	}
	if(!wList->WeaponStatsGlyphs->Glyph->Visible)
	{
		TextAllVisible(wList->WeaponStatsGlyphs);
	}

	Vec2Set(&glyphPos, wList->WeaponPickup.Position.x, wList->WeaponPickup.Position.y + wList->WeaponPickup.height * 1.5f - wList->WeaponGlyphs->Glyph->Height);
	ChangeTextPosition(CurrentPlayer->ComparisonGlyphs, glyphPos, Center);
	Vec2Set(&glyphPos, wList->WeaponPickup.Position.x, wList->WeaponPickup.Position.y + wList->WeaponPickup.height * 1.5f);
	ChangeTextPosition(wList->WeaponStatsGlyphs, glyphPos, Center);
	Vec2Set(&glyphPos, wList->WeaponPickup.Position.x, wList->WeaponPickup.Position.y + wList->WeaponPickup.height * 1.5f + wList->WeaponGlyphs->Glyph->Height);
	ChangeTextPosition(wList->WeaponGlyphs, glyphPos, Center);

	//Switch weapons
	if(FoxInput_KeyTriggered('E'))
	{
		int Shopping = FALSE;
		float healthRatio = 0;

		//Check if the weapon is in a shop
		if (wList->CurrentShop)
		{
			//Negitive coin, what?
			if (wList->CurrentShop->Coin < 0)
			{
				if (!Cheats)
				{
					PlayAudio(CurrentPlayer->CurrentPlayerSounds.MooseNotEnoughCoins);
					return;
				}

				//Go positive again...
				wList->CurrentShop->Coin *= -1;
			}

			//Not enough money, too expensive
			if (wList->CurrentShop->Coin > CurrentPlayer->CurrentPlayerStats.Money && !Cheats)
			{
				//Not enough money
				Vec3 textColor;
				TextGlyphs *FirstLetter;

				// Create Floating Error Text
				Vec3Set(&textColor, 1.0f, 0.1f, 0.1f);
				FirstLetter = CreateText("Not Enough Coin!", CurrentPlayer->Position.x, (CurrentPlayer->Position.y + CurrentPlayer->PlayerSpriteParts.Body->Height / 2), 100, textColor, Center, Border);
				AddFloatingText(FirstLetter);
				ChangeTextVisibility(FirstLetter);
				ChangeTextZIndex(FirstLetter, 202);

				//YOU WILL NEVER BUY THIS AGAIN, HAHAHAHAHAHA (Until you leave, get more money and then come back)
				wList->CurrentShop->Coin *= -1;

				if (!CurrentPlayer->CurrentPlayerSounds.MoosePlay)
				{
					PlayAudio(CurrentPlayer->CurrentPlayerSounds.MooseNotEnoughCoins);
					CurrentPlayer->CurrentPlayerSounds.MoosePlay = TRUE;
				}

				return;
			}
			
			//If you aren't cheating, we will take your money
			if (!Cheats)
				CurrentPlayer->CurrentPlayerStats.Money -= wList->CurrentShop->Coin;

			if (!CurrentPlayer->CurrentPlayerSounds.MoosePlay)
			{
				if (rand() % 2)
					PlayAudio(CurrentPlayer->CurrentPlayerSounds.MoosePurchase[0]);
				else
					PlayAudio(CurrentPlayer->CurrentPlayerSounds.MoosePurchase[1]);

				CurrentPlayer->CurrentPlayerSounds.MoosePlay = TRUE;
			}
			Shopping = TRUE;
		}

		//FreeParticleSystem(wList->WeaponParticle);
		SwapWeapons(CurrentPlayer->PlayerWeapon, wList);

		//Set up the new player weapon sprite
		CurrentPlayer->PlayerWeapon->WeaponSprite->ZIndex = (unsigned short)50;
		CurrentPlayer->PlayerWeapon->WeaponFOF = PlayerWeapon;
		CurrentPlayer->PlayerSpriteParts.Weapon = CurrentPlayer->PlayerWeapon->WeaponSprite;
		if(CurrentPlayer->PlayerWeapon->WeaponGlyphs->Glyph->Visible)
		{
			TextAllNotVisible(CurrentPlayer->PlayerWeapon->WeaponGlyphs);
		}
		if(CurrentPlayer->PlayerWeapon->WeaponStatsGlyphs->Glyph->Visible)
		{
			TextAllNotVisible(CurrentPlayer->PlayerWeapon->WeaponStatsGlyphs);
			TextAllNotVisible(CurrentPlayer->ComparisonGlyphs);
		}
		CurrentPlayer->PlayerWeapon->WeaponHoverBackground->Visible = FALSE;
		
		//Put the new dropped weapon down
		wList->Position.x = CurrentPlayer->PlayerWeapon->Position.x;
		wList->Position.y = CurrentPlayer->PlayerWeapon->Position.y;
		wList->WeaponSprite->ZIndex = (unsigned short)50;
		wList->WeaponFOF = DroppedWeapon;
		
		wList->WeaponSprite->Rotation = FOX_PI / 2 + (float)(rand() % 5 - 2) / 10;;
		wList->WeaponSprite->Position = wList->WeaponPickup.Position;

		//If it is a shop we like it straight, oh ja
		if (Shopping)
		{
			//Nothing is for sale, no need to keep the text
			FreeText(wList->CurrentShop->ItemTextCoin);
			FreeText(wList->CurrentShop->ItemTextName);
			FreeSprite(wList->CurrentShop->ShopTextSprite);
			//No longer in the shop
			wList->CurrentShop = NULL;
		}

		//Put the weapon on the ground
		wList->WeaponSprite->Position.x = wList->WeaponPickup.Position.x;
		wList->WeaponSprite->Position.y = CurrentPlayer->PlayerCollider.Position.y;
		wList->WeaponPickup.Position = wList->WeaponSprite->Position;
		wList->WeaponFalling = TRUE;

		wList->WeaponHoverBackground->Position.x = wList->WeaponPickup.Position.x;
		wList->WeaponHoverBackground->Position.y = wList->WeaponPickup.Position.y + wList->WeaponPickup.height * 1.5f;
		wList->WeaponHoverBackground->Height = CurrentPlayer->PlayerWeapon->WeaponHoverBackground->Height;

		// Update the text positions
		Vec2Set(&glyphPos, wList->WeaponPickup.Position.x, (wList->WeaponPickup.Position.y + wList->WeaponPickup.height * 1.5f + wList->WeaponGlyphs->Glyph->Height));
		ChangeTextPosition(wList->WeaponGlyphs, glyphPos, Center);
		Vec2Set(&glyphPos, wList->WeaponPickup.Position.x, (wList->WeaponPickup.Position.y + wList->WeaponPickup.height * 1.5f));
		ChangeTextPosition(wList->WeaponStatsGlyphs, glyphPos, Center);

		updateAttackSpeed(&CurrentPlayer->CurrentPlayerStats);
		updateMoveSpeed(&CurrentPlayer->CurrentPlayerStats);
		updateDamage(CurrentPlayer);
		updateDamageReduction(&CurrentPlayer->CurrentPlayerStats);
		healthRatio = (float)(CurrentPlayer->CurrentPlayerStats.CurrentHealth) / CurrentPlayer->CurrentPlayerStats.MaxHealth;
		updateMaxHealth(&CurrentPlayer->CurrentPlayerStats);
		CurrentPlayer->CurrentPlayerStats.CurrentHealth = (float)ceil(CurrentPlayer->CurrentPlayerStats.MaxHealth * healthRatio);
		// Make sure the player keeps their buff after the weapon swap
		if(CurrentPlayer->CurrentPlayerStats.StrengthTimer > 0)
		{
			CurrentPlayer->CurrentPlayerStats.Damage = (int)(CurrentPlayer->CurrentPlayerStats.Damage * 2.0f);;
		}
		if(CurrentPlayer->CurrentPlayerStats.AgilityTimer > 0)
		{
			CurrentPlayer->CurrentPlayerStats.AttackSpeed *= 2.0f;
		}
		if(CurrentPlayer->CurrentPlayerStats.DefenseTimer > 0)
		{
			CurrentPlayer->CurrentPlayerStats.DamageReduction *= 1.5f;
		}
	}
}

/*************************************************************************/
/*!
	\brief
	Handles the collision with a enemy weapon
*/
/*************************************************************************/
void PlayerCollideEnemyWeapon(Player* CurrentPlayer, Enemy *CurrentEnemy)
{
	// height +/- (fontsize/2)
	int damageDealt;
	char num[10];
	Vec3 textColor;
	TextGlyphs *FirstLetter;
	Vec3Set(&textColor, 1.0f, 0.0f, 0.0f);
	
	// Calculate damage
	if(Cheats)
		damageDealt = 0;
	else
	{
		damageDealt = (int)(CurrentEnemy->CurrentEnemyStats.Damage * (1 - CurrentPlayer->CurrentPlayerStats.DamageReduction));
		if(damageDealt == 0)
			damageDealt = 1;
	}

	CurrentPlayer->CurrentPlayerStats.CurrentHealth -= damageDealt;
	PlayAudio(rand() % 2 ? CurrentPlayer->CurrentPlayerSounds.GetHit1 : CurrentPlayer->CurrentPlayerSounds.GetHit2);
	sprintf(num, "-%d", damageDealt);
	// Create Floating Combat Text
	FirstLetter = CreateText(num, (CurrentPlayer->Position.x + rand() % 81 - 40), (CurrentPlayer->Position.y + CurrentPlayer->PlayerSpriteParts.Body->Height / 2), 80, textColor, Center, Border);
	AddFloatingText(FirstLetter);
	ChangeTextVisibility(FirstLetter);
	ChangeTextZIndex(FirstLetter, 201);
}

/*************************************************************************/
/*!
	\brief
	Handles the collision with a deadly projectile
*/
/*************************************************************************/
void PlayerCollideEnemyProjectile(Player* CurrentPlayer, Projectile *CurrentProjectile)
{
	int damageDealt;
	char num[10];
	Vec3 textColor;
	TextGlyphs *FirstLetter;
	Vec3Set(&textColor, 1.0f, 0.0f, 0.0f);
	
	// Calculate damage
	if(Cheats)
		damageDealt = 0;
	else
	{
		damageDealt = (int)(CurrentProjectile->Damage * (1 - CurrentPlayer->CurrentPlayerStats.DamageReduction));
		if(damageDealt == 0)
			damageDealt = 1;
	}
	
	CurrentPlayer->CurrentPlayerStats.CurrentHealth -= damageDealt;
	PlayAudio(rand() % 2 ? CurrentPlayer->CurrentPlayerSounds.GetHit1 : CurrentPlayer->CurrentPlayerSounds.GetHit2);
	sprintf(num, "-%d", damageDealt);
	// Create Floating Combat Text
	FirstLetter = CreateText(num, (CurrentPlayer->Position.x + rand() % 81 - 40), (CurrentPlayer->Position.y + CurrentPlayer->PlayerSpriteParts.Body->Height / 2), 80, textColor, Center, Border);
	AddFloatingText(FirstLetter);
	ChangeTextVisibility(FirstLetter);
	ChangeTextZIndex(FirstLetter, 201);
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
