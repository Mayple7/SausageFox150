/*****************************************************************************/
/*!
\file				Player.c
\author				Dan Muller (d.muller)
\date				Jan 8, 2014

\brief				Player movement and logic functions

\par				Functions:
\li					InitializePlayer
\li					InputPlayer
\li					HandleCollision
\li					UpdatePlayerPosition
\li					updateMaxHealth
\li					updateMoveSpeed
\li					updateAttackSpeed
\li					updateDamageReduction
\li					DetectPlayerCollision
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include "../AEEngine.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxEngine.h"

// ---------------------------------------------------------------------------
// globals
Player CurrentPlayer;

/*************************************************************************/
/*!
	\brief
	Initializes the player
	
	\param CurrentPlayer
	A pointer to the player to be initialized
*/
/*************************************************************************/
void InitializePlayer(struct Player *CurrentPlayer, int newID, float xPos, float yPos)
{
	int i;

	for(i = 0; i < COLLIDEAMOUNT; i++)
	{
		CurrentPlayer->CollisionData[i] = -1;
	}

	//Creates the sprite for the player
	CreatePlayerSprites(CurrentPlayer);

	//Default position of the player
	CurrentPlayer->Position.x = xPos * GetLoadRatio();
	CurrentPlayer->Position.y = yPos * GetLoadRatio();
	CurrentPlayer->FlipX = FALSE;
	CurrentPlayer->FlipY = FALSE;
	CurrentPlayer->FlipXPrev = FALSE;
	CurrentPlayer->FlipYPrev = FALSE;

	//Collision properties
	CreateCollisionBox(&CurrentPlayer->PlayerCollider, &CurrentPlayer->Position, PlayerType, PLAYER_WIDTH, PLAYER_HEIGHT, newID);
	CurrentPlayer->PlayerCollider.Offset.y = 20 * GetLoadRatio();
	CurrentPlayer->PlayerCollider.width = CurrentPlayer->PlayerCollider.width - 20 * GetLoadRatio();
	UpdateCollider(&CurrentPlayer->PlayerCollider, CurrentPlayer->PlayerCollider.width, CurrentPlayer->PlayerCollider.height);

	//Collider Debug
	CurrentPlayer->PlayerCollider.collisionDebug = TRUE;

	//Initialize rigidbody
	InitializeRigidBody(&CurrentPlayer->PlayerRigidBody, FALSE, PLAYER_WIDTH, PLAYER_HEIGHT);
	CurrentPlayer->PlayerRigidBody.onGround = FALSE;
	CurrentPlayer->dropDown = FALSE;

	CurrentPlayer->PlayerWeapon = CreateWeapon("Fragile Stick", "TextureFiles/stick.png", Sword, Common, WeaponFriendly, 256, 256, newID++);

	CurrentPlayer->PlayerSpriteParts.Weapon = CurrentPlayer->PlayerWeapon->WeaponSprite;
}

/*************************************************************************/
/*!
	\brief
	Updates the player based on the input
	
	\param CurrentPlayer
	A pointer to the player to update
*/
/*************************************************************************/
void InputPlayer(struct Player *CurrentPlayer)
{
	Animation(CurrentPlayer);

	if (AEInputCheckTriggered('F'))
	{
		CurrentPlayer->isAttacking = TRUE;
		if (CurrentPlayer->FlipX)
		{
			CurrentPlayer->PlayerSpriteParts.ArmUpper->Rotation = (float)FOX_PI / 2;
			CurrentPlayer->PlayerSpriteParts.ArmLower->Rotation = CurrentPlayer->PlayerSpriteParts.ArmUpper->Rotation - 1;
		}
		else
		{
			CurrentPlayer->PlayerSpriteParts.ArmUpper2->Rotation = (float)FOX_PI * 1.5f;
			CurrentPlayer->PlayerSpriteParts.ArmLower2->Rotation = CurrentPlayer->PlayerSpriteParts.ArmUpper2->Rotation + 1;
		}
	}
/*	if (AEInputCheckTriggered('1'))
		CurrentPlayer->PlayerSpriteParts.Weapon->SpriteTexture = LoadTexture("TextureFiles/Sword.png");
	if (AEInputCheckTriggered('2'))
		CurrentPlayer->PlayerSpriteParts.Weapon->SpriteTexture = LoadTexture("TextureFiles/Axe.png");
	if (AEInputCheckTriggered('3'))
		CurrentPlayer->PlayerSpriteParts.Weapon->SpriteTexture = LoadTexture("TextureFiles/Hammer.png");
	if (AEInputCheckTriggered('4'))
		CurrentPlayer->PlayerSpriteParts.Weapon->SpriteTexture = LoadTexture("TextureFiles/Bow.png");
	if (AEInputCheckTriggered('5'))
		CurrentPlayer->PlayerSpriteParts.Weapon->SpriteTexture = LoadTexture("TextureFiles/Spear.png");
	if (AEInputCheckTriggered('6'))
		CurrentPlayer->PlayerSpriteParts.Weapon->SpriteTexture = LoadTexture("TextureFiles/FlameThrower.png");
	if (AEInputCheckTriggered('7'))
		CurrentPlayer->PlayerSpriteParts.Weapon->SpriteTexture = LoadTexture("TextureFiles/BattleAxe.png");
	if (AEInputCheckTriggered('8'))
		CurrentPlayer->PlayerSpriteParts.Weapon->SpriteTexture = LoadTexture("TextureFiles/Sausage.png");
	if (AEInputCheckTriggered('P'))
		CurrentPlayer->PlayerSpriteParts.Weapon->SpriteTexture = LoadTexture("Textures/GinkoSmall.png");
		*/



	// Move the direction based on the speed
	MoveObject(&CurrentPlayer->Position, CurrentPlayer->PlayerDirection, CurrentPlayer->Speed);

	// not key press for direction then slow down!
	if(!AEInputCheckCurr('D') && !AEInputCheckCurr('A'))
	{
		if (!(CurrentPlayer->Position.y > GROUNDLEVEL * GetLoadRatio()) && !CurrentPlayer->PlayerRigidBody.onGround)
		{
			if (CurrentPlayer->Speed - 0.8f >= 0.0f)
			{
				CurrentPlayer->Speed -= 0.8f;
			}
			else
			{
				CurrentPlayer->Speed = 0.0f;
				CurrentPlayer->LegSinValue = 0;
			}
		}
		else
		{
			if (CurrentPlayer->Speed - 0.8f >= 0.0f)
			{
				CurrentPlayer->Speed -= 0.8f;
			}
			else
			{
				CurrentPlayer->Speed = 0.0f;
				CurrentPlayer->LegSinValue = 0;
			}
		}
	}
	// Move left if A is pressed
	if(AEInputCheckCurr('A'))
	{
		CurrentPlayer->FlipX = 0;
		CurrentPlayer->PlayerDirection = LEFT;
		CurrentPlayer->Speed = 8.0f * GetLoadRatio();
	}
	// Move right if D is pressed
	else if(AEInputCheckCurr('D'))
	{
		CurrentPlayer->FlipX = 1;
		CurrentPlayer->PlayerDirection = RIGHT;
		CurrentPlayer->Speed = 8.0f * GetLoadRatio();
	}
	//Jump when space is pushed or drop down if S is pushed as well
	if(AEInputCheckTriggered(VK_SPACE))
	{
		Vec2 velocity;
		
		if(AEInputCheckCurr('S') && CurrentPlayer->PlayerRigidBody.onGround)
		{
			CurrentPlayer->PlayerRigidBody.onGround = FALSE;
			CurrentPlayer->dropDown = TRUE;
		}

		
		Vec2Set(&velocity, 0.0f, 12.0f * GetLoadRatio());
		if(CurrentPlayer->Position.y < GROUNDLEVEL * GetLoadRatio() || CurrentPlayer->PlayerRigidBody.onGround)
		{
			if(CurrentPlayer->Position.y < GROUNDLEVEL * GetLoadRatio())
				Vec2Set(&CurrentPlayer->Position, CurrentPlayer->Position.x, GROUNDLEVEL * GetLoadRatio() + 0.1f);
			CurrentPlayer->PlayerRigidBody.onGround = FALSE;
			ApplyVelocity(&CurrentPlayer->PlayerRigidBody, &velocity);
		}
	}
	//Throw an upward force on the fox when backspace is pushed
	if(AEInputCheckCurr(VK_BACK))
	{
		Vec2 force;

		CurrentPlayer->PlayerRigidBody.Acceleration.x = 0;
		CurrentPlayer->PlayerRigidBody.Acceleration.y = 0;
		Vec2Set(&force, 0.0f, 15.0f * GetLoadRatio());
		if(CurrentPlayer->Position.y > GROUNDLEVEL * GetLoadRatio())
		{
			ApplyForce(&CurrentPlayer->PlayerRigidBody, &force);
		}
	}
	//No acceleration when backspace is not being pushed
	else
	{
		CurrentPlayer->PlayerRigidBody.Acceleration.x = 0;
		CurrentPlayer->PlayerRigidBody.Acceleration.y = 0;
	}
}

/*************************************************************************/
/*!
	\brief
	Updates the player's position
	
	\param CurrentPlayer
	A pointer to the player to update
*/
/*************************************************************************/
void UpdatePlayerPosition(Player *CurrentPlayer)
{
	//Stop velocity and acceleration when the player lands on the floor
	if(CurrentPlayer->Position.y <= GROUNDLEVEL * GetLoadRatio() || CurrentPlayer->PlayerRigidBody.onGround)
	{
		Vec2Zero(&CurrentPlayer->PlayerRigidBody.Acceleration);
		Vec2Zero(&CurrentPlayer->PlayerRigidBody.Velocity);
		ZeroGravity(&CurrentPlayer->PlayerRigidBody);
	}
	//Set gravity if not on floor or on a platform
	else
	{
		SetGravity(&CurrentPlayer->PlayerRigidBody, 0.0f, FOX_GRAVITY_Y * GetLoadRatio());
	}
	//Player position updated when dropping down from a platform
	if(CurrentPlayer->dropDown)
	{
		CurrentPlayer->Position.y -= 5.0f;
		if(CurrentPlayer->PlayerRigidBody.Velocity.y < 0)
		{
			CurrentPlayer->PlayerRigidBody.Velocity.y = -5.0f * GetLoadRatio();
			CurrentPlayer->dropDown = FALSE;
		}
	}

	//Update velocity and acceleration
	UpdateVelocity(&CurrentPlayer->PlayerRigidBody);
	Vec2Add(&CurrentPlayer->Position, &CurrentPlayer->Position, &CurrentPlayer->PlayerRigidBody.Velocity);

	//Updates the collision box
	UpdateCollisionPosition(&CurrentPlayer->PlayerCollider, &CurrentPlayer->Position);
	CurrentPlayer->PlayerRigidBody.onGround = FALSE;
}

/*************************************************************************/
/*!
	\brief
	Update the player's max HP based on their strength
	
	\param CurrentPlayerStats
	A pointer to the player's stats to update
*/
/*************************************************************************/
void updateMaxHealth(PlayerStats *CurrentPlayerStats)
{
	//Placeholder max health formula
	CurrentPlayerStats->MaxHealth = 5 + CurrentPlayerStats->Strength * 2;
}

/*************************************************************************/
/*!
	\brief
	Update the player's movespeed based on their agility
	
	\param CurrentPlayerStats
	A pointer to the player's stats to update
*/
/*************************************************************************/
void updateMoveSpeed(PlayerStats *CurrentPlayerStats)
{
	//Placeholder move speed formula
	CurrentPlayerStats->MoveSpeed = CurrentPlayerStats->Agility + 3.0f;
}

/*************************************************************************/
/*!
	\brief
	Update the player's attack speed based on their agility
	
	\param CurrentPlayerStats
	A pointer to the player's stats to update
*/
/*************************************************************************/
void updateAttackSpeed(PlayerStats *CurrentPlayerStats)
{
	//Placeholder attack speed formula
	CurrentPlayerStats->AttackSpeed = CurrentPlayerStats->Agility * 5.0f + 10.0f;
}

/*************************************************************************/
/*!
	\brief
	Update the player's damage reduction based on their defense
	
	\param CurrentPlayerStats
	A pointer to the player's stats to update
*/
/*************************************************************************/
void updateDamageReduction(PlayerStats *CurrentPlayerStats)
{
	//Placeholder damage reduction formula
	CurrentPlayerStats->DamageReduction = CurrentPlayerStats->Defense * 2.0f / 100.0f;
}

/*************************************************************************/
/*!
	\brief
	Detects collisions with the player and calls the correct resolution functions
*/
/*************************************************************************/
void DetectPlayerCollision(void)
{
	Platform* pList = platformList;
	Food* fList = foodList;
	Weapon* wList = weaponList;
	int hit = 0;
	int hitPrev = 0;

	while(pList->objID != 0)
	{
		if(pList->objID > 0)
		{
			hit = CollisionRectangles(&CurrentPlayer.PlayerCollider, &pList->PlatformCollider);
			hitPrev = searchHitArray(CurrentPlayer.CollisionData, COLLIDEAMOUNT, pList->PlatformCollider.collisionID);
			if(hit)
			{
				// New target, on start collision
				if(hitPrev < 0)
				{
					CurrentPlayer.CollisionData[-hitPrev] = pList->PlatformCollider.collisionID * 10 + 1;
					//printf("NOT FOUND: %i\n", -hitPrev);
					PlayerCollidePlatform(&CurrentPlayer, pList);
				}
				// Found target, hit previous frame, on persistant
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 1)
				{
					//printf("FOUND PERSISTANT: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					PlayerCollidePlatform(&CurrentPlayer, pList);
				}
				// Found target, did not hit previous frame, on start collision
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 0)
				{
					//printf("FOUND NEW COLLISION: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					CurrentPlayer.CollisionData[hitPrev] = pList->PlatformCollider.collisionID * 10 + 1;
					PlayerCollidePlatform(&CurrentPlayer, pList);
				}
			}
			else
			{
				if(hitPrev < 0 || CurrentPlayer.CollisionData[hitPrev] % 10 == 0)
				{
					// NEVER COLLIDED OR DIDNT COLLIDE PREV FRAME
				}
				// Found target, collision ended
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 1)
				{
					//printf("END COLLISION: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					CurrentPlayer.CollisionData[hitPrev] = 0;
				}
			}
		}
		pList++;
	}
	while(fList->objID != 0)
	{
		if(fList->objID > 0)
		{
			hit = CollisionRectangles(&CurrentPlayer.PlayerCollider, &fList->FoodCollider);
			hitPrev = searchHitArray(CurrentPlayer.CollisionData, COLLIDEAMOUNT, fList->FoodCollider.collisionID);

			if(hit)
			{
				// New target, on start collision
				if(hitPrev < 0)
				{
					CurrentPlayer.CollisionData[-hitPrev] = fList->FoodCollider.collisionID * 10 + 1;
					//printf("NOT FOUND: %i\n", -hitPrev);
					PlayerCollideFood(&CurrentPlayer, fList);
					fList->objID = 0;
				}
				// Found target, hit previous frame, on persistant
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 1)
				{
					//printf("FOUND PERSISTANT: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					PlayerCollideFood(&CurrentPlayer, fList);
					fList->objID = -1;
				}
				// Found target, did not hit previous frame, on start collision
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 0)
				{
					//printf("FOUND NEW COLLISION: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					CurrentPlayer.CollisionData[hitPrev] = fList->FoodCollider.collisionID * 10 + 1;
					PlayerCollideFood(&CurrentPlayer, fList);
					fList->objID = 0;
				}
			}
			else
			{
				if(hitPrev < 0 || CurrentPlayer.CollisionData[hitPrev] % 10 == 0)
				{
					// NEVER COLLIDED OR DIDNT COLLIDE PREV FRAME
				}
				// Found target, collision ended
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 1)
				{
					//printf("END COLLISION: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					CurrentPlayer.CollisionData[hitPrev] = 0;
				}
			}
		}
		fList++;
	}
	while(wList->objID != 0)
	{
		if(wList->objID > 0 && wList->WeaponFOF == DroppedWeapon)
		{
			hit = CollisionRectangles(&CurrentPlayer.PlayerCollider, &wList->WeaponPickup);
			hitPrev = searchHitArray(CurrentPlayer.CollisionData, COLLIDEAMOUNT, wList->WeaponPickup.collisionID);
			if(hit)
			{
				// New target, on start collision
				if(hitPrev < 0)
				{
					CurrentPlayer.CollisionData[-hitPrev] = wList->WeaponPickup.collisionID * 10 + 1;
					//printf("NOT FOUND: %i\n", -hitPrev);
					PlayerCollideWeaponDrop(&CurrentPlayer, wList);
				}
				// Found target, hit previous frame, on persistant
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 1)
				{
					//printf("FOUND PERSISTANT: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					PlayerCollideWeaponDrop(&CurrentPlayer, wList);
				}
				// Found target, did not hit previous frame, on start collision
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 0)
				{
					//printf("FOUND NEW COLLISION: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					CurrentPlayer.CollisionData[hitPrev] = wList->WeaponPickup.collisionID * 10 + 1;
					PlayerCollideWeaponDrop(&CurrentPlayer, wList);
				}
			}
			else
			{
				if(hitPrev < 0 || CurrentPlayer.CollisionData[hitPrev] % 10 == 0)
				{
					// NEVER COLLIDED OR DIDNT COLLIDE PREV FRAME
				}
				// Found target, collision ended
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 1)
				{
					//printf("END COLLISION: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					CurrentPlayer.CollisionData[hitPrev] = 0;
				}
			}
		}
		wList++;
	}

	// Check projectile collisions
	//	-> Handle collision if true
	// other collisions!
}

/*************************************************************************/
/*!
	\brief
	Animates the players legs.
*/
/*************************************************************************/
void Animation(Player *Object)
{
	float LegDistance = 9.5f-Object->Speed;
	float LegUpperDirection = (float)sin(Object->LegSinValue)/(LegDistance);
	float LegLowerDirection;
	float LegUpperDirection2 = (float)sin(Object->LegSinValue)/(LegDistance);
	float LegLowerDirection2;
	
	Sprite *LegUpr = Object->PlayerSpriteParts.LegUpper;
	Sprite *LegUpr2 = Object->PlayerSpriteParts.LegUpper2;
	Sprite *LegLwr = Object->PlayerSpriteParts.LegLower;
	Sprite *LegLwr2 = Object->PlayerSpriteParts.LegLower2;
	Sprite *ArmUpr = Object->PlayerSpriteParts.ArmUpper;
	Sprite *ArmUpr2 = Object->PlayerSpriteParts.ArmUpper2;
	Sprite *ArmLwr = Object->PlayerSpriteParts.ArmLower;
	Sprite *ArmLwr2 = Object->PlayerSpriteParts.ArmLower2;
	Sprite *Skrt = Object->PlayerSpriteParts.Skirt;
	Sprite *Bdy = Object->PlayerSpriteParts.Body;
	Sprite *Weap = Object->PlayerSpriteParts.Weapon;
	Sprite *Tail = Object->PlayerSpriteParts.Tail;

	Object->LegSinValue += 0.1f; 

	Object->PlayerSpriteParts.BlinkTimer += 1;

	if (Object->PlayerSpriteParts.BlinkTimer <= 150)
	{
		Object->PlayerSpriteParts.Body->AnimationActive = 0;
	}
	else
	{
		Object->PlayerSpriteParts.Body->AnimationActive = 1;
		if (Object->PlayerSpriteParts.Body->CurrentFrame == 3)
		{
			Object->PlayerSpriteParts.BlinkTimer = 0;
			Object->PlayerSpriteParts.Body->CurrentFrame = 0;
		}
	}

	Bdy->Position.x = Object->Position.x;
	Bdy->Position.y = Object->Position.y - (float)sin(-Object->LegSinValue*2)*5/(LegDistance);
	Skrt->Position = Bdy->Position;
	if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
		Skrt->CurrentFrame = (int)floor(fabs(LegUpperDirection*4));
	else
		Skrt->CurrentFrame = 3;
	Tail->Position.y = Bdy->Position.y+10;
	Tail->Rotation = (float)sin(Object->TailSinValue*1.25f)/4;

	if (Object->Speed > 1.5f)
	{
		Tail->SpriteTexture = LoadTexture("TextureFiles/TailRun.png");
		Object->TailSinValue += 0.1f;
		Object->PlayerSpriteParts.Tail->AnimationSpeed = Object->Speed/2 + 3;
	}
	else
	{
		Tail->SpriteTexture = LoadTexture("TextureFiles/TailIdle.png");
		Object->TailSinValue = 0;
		Object->PlayerSpriteParts.Tail->AnimationSpeed = 5;
	}

	if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
	{
    if (LegUpperDirection < 0)
        LegLowerDirection = ((float)sin(Object->LegSinValue)/1.25f + (float)sin(Object->LegSinValue) * -0.1f)/(LegDistance);
    else
        LegLowerDirection = (LegUpperDirection + (float)sin(Object->LegSinValue) + (float)sin(Object->LegSinValue) * 0.4f)/(LegDistance);

    if (LegUpperDirection2 > 0)
        LegLowerDirection2 = ((float)sin(Object->LegSinValue)/1.25f + (float)sin(Object->LegSinValue) * -0.1f)/(LegDistance);
    else
        LegLowerDirection2 = (LegUpperDirection2 + (float)sin(Object->LegSinValue) + (float)sin(Object->LegSinValue) * 0.4f)/(LegDistance);
	}
	else
	{
		LegUpperDirection = (float)sin(LegDistance/10)-1.0f;
		LegUpperDirection2 = (float)sin(LegDistance/10)-1.0f;
		LegLowerDirection = LegUpperDirection + 0.5f;
		LegLowerDirection2 = LegUpperDirection2 - 0.5f;
	}
	LegUpr->FlipX = Object->FlipX;
	LegLwr->FlipX = Object->FlipX;
	LegUpr2->FlipX = Object->FlipX;
	LegLwr2->FlipX = Object->FlipX;
	Bdy->FlipX = Object->FlipX;
	Skrt->FlipX = Object->FlipX;
	Tail->FlipX = Object->FlipX;
	ArmUpr->FlipX = Object->FlipX;
	ArmLwr->FlipX = Object->FlipX;
	ArmUpr2->FlipX = Object->FlipX;
	ArmLwr2->FlipX = Object->FlipX;
	Weap->FlipX = Object->FlipX;
	

	if (Object->FlipX == FALSE)
	{
		Tail->Position.x = Bdy->Position.x+(Bdy->Width/20);
		
		LegUpr->Rotation = LegUpperDirection;
		LegUpr->Position.x = Object->Position.x;
		if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*-8/(LegDistance);
		}
		LegUpr->Position.y = Object->Position.y + (float)sin(Object->LegSinValue*2)*5/(LegDistance);
		LegLwr->Position.x = (float)cos(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.x;
		LegLwr->Position.y = (float)sin(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.y;
		LegLwr->Rotation = LegLowerDirection;
		
		
		LegUpr2->Rotation = -LegUpperDirection2;
		LegUpr2->Position.x = Object->Position.x;
		if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*8/(LegDistance);
		}
		LegUpr2->Position.y = Object->Position.y + (float)sin(Object->LegSinValue*2)*5/(LegDistance);
		LegLwr2->Position.x = (float)cos(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.x;
		LegLwr2->Position.y = (float)sin(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.y;
		LegLwr2->Rotation = -LegLowerDirection2;
		
		
		ArmUpr->Rotation = LegUpperDirection/1.5f + 1.5f;
		ArmLwr->Rotation = ArmUpr->Rotation - 1.25f + LegUpperDirection/2.0f;
		ArmUpr->Position.x = Bdy->Position.x;
		ArmUpr->Position.y = Bdy->Position.y + (Bdy->Width/6);
		ArmLwr->Position.x = ArmUpr->Position.x - (float)cos(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);
		ArmLwr->Position.y = ArmUpr->Position.y - (float)sin(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);

		// Attacking!
		if (Object->isAttacking)
		{
			ArmUpr2->Rotation = RotateToAngle(ArmUpr2->Rotation, (float)FOX_PI-2.0f, 0.25f);
			ArmLwr2->Rotation = ArmUpr2->Rotation;
			if (ArmUpr2->Rotation == (float)FOX_PI-2.0f)
				Object->isAttacking = FALSE;
		}
		else
		{
			ArmUpr2->Rotation = -LegUpperDirection/1.5f + 1.5f;
			ArmLwr2->Rotation = -(ArmUpr->Rotation - 1.75f + LegUpperDirection/2.0f);
		}
		
		ArmUpr2->Position.x = Bdy->Position.x;
		ArmUpr2->Position.y = Bdy->Position.y + (Bdy->Width/6);
		ArmLwr2->Position.x = ArmUpr2->Position.x - (float)cos(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);
		ArmLwr2->Position.y = ArmUpr2->Position.y - (float)sin(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);

		if (Object->Speed < 0.01)
		{
			ArmLwr->Rotation = ArmUpr->Rotation - 0.5f;
			ArmLwr2->Rotation = ArmUpr2->Rotation - 0.5f;
		}

		Weap->Position.x = ArmLwr2->Position.x - (float)cos(ArmLwr2->Rotation) * (ArmLwr2->Width/3.5f);
		Weap->Position.y = ArmLwr2->Position.y - (float)sin(ArmLwr2->Rotation) * (ArmLwr2->Width/3.5f);
		Weap->Rotation = ArmLwr2->Rotation;
		Weap->ZIndex = 21;

	}
	else
	{
		Tail->Position.x = Bdy->Position.x-(Bdy->Width/20);
		
		LegUpr->Rotation = -LegUpperDirection;
		LegUpr->Position.x = Object->Position.x;
		if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*-8/(LegDistance);
		}
		LegUpr->Position.y = Object->Position.y + (float)sin(Object->LegSinValue*2)*5/(LegDistance);
		LegLwr->Position.x = (float)cos(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.x;
		LegLwr->Position.y = (float)sin(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.y;
		LegLwr->Rotation = -LegLowerDirection;
		

		LegUpr2->Rotation = LegUpperDirection2;
		LegUpr2->Position.x = Object->Position.x;
		if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*8/(LegDistance);
		}
		LegUpr2->Position.y = Object->Position.y + (float)sin(Object->LegSinValue*2)*5/(LegDistance);
		LegLwr2->Position.x = (float)cos(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.x;
		LegLwr2->Position.y = (float)sin(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.y;
		LegLwr2->Rotation = LegLowerDirection2;
		
		// Attacking!
		if (Object->isAttacking)
		{
			ArmUpr->Rotation = RotateToAngle(ArmUpr->Rotation, (float)FOX_PI+2.0f, 0.25f);
			ArmLwr->Rotation = ArmUpr->Rotation;
			if (ArmUpr->Rotation == (float)FOX_PI+2.0f)
				Object->isAttacking = FALSE;
		}
		else
		{
			ArmUpr->Rotation = -LegUpperDirection/1.5f - 1.5f;
			ArmLwr->Rotation = ArmUpr->Rotation + 1.25f - LegUpperDirection/2.0f;
		}
		ArmUpr->Position.x = Bdy->Position.x;
		ArmUpr->Position.y = Bdy->Position.y + (Bdy->Width/6);
		ArmLwr->Position.x = ArmUpr->Position.x + (float)cos(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);
		ArmLwr->Position.y = ArmUpr->Position.y + (float)sin(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);

		ArmUpr2->Rotation = LegUpperDirection/1.5f - 1.5f;
		ArmLwr2->Rotation = -(ArmUpr->Rotation + 1.75f - LegUpperDirection/2.0f);
		ArmUpr2->Position.x = Bdy->Position.x;
		ArmUpr2->Position.y = Bdy->Position.y + (Bdy->Width/6);
		ArmLwr2->Position.x = ArmUpr2->Position.x + (float)cos(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);
		ArmLwr2->Position.y = ArmUpr2->Position.y + (float)sin(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);

		if (Object->Speed < 0.01)
		{
			ArmLwr->Rotation = ArmUpr->Rotation + 0.5f;
			ArmLwr2->Rotation = ArmUpr2->Rotation + 0.5f;
		}

		Weap->Position.x = ArmLwr->Position.x + (float)cos(ArmLwr->Rotation) * (ArmLwr->Width/3.5f);
		Weap->Position.y = ArmLwr->Position.y + (float)sin(ArmLwr->Rotation) * (ArmLwr->Width/3.5f);
		Weap->Rotation = ArmLwr->Rotation;
		Weap->ZIndex = 22;


	}

	//*************************************************************************************************
}

void CreatePlayerSprites(Player *Object)
{
	Object->PlayerSpriteParts.ArmUpper2 = CreateSprite("TextureFiles/ArmUpper.png", 128.0f, 128.0f, 20, 1, 1, 0, 0);

	Object->PlayerSpriteParts.ArmLower2 = CreateSprite("TextureFiles/ArmLower.png", 128.0f, 128.0f, 20, 1, 1, 0, 0);

	Object->PlayerSpriteParts.LegUpper = CreateSprite("TextureFiles/LegUpper.png", 128.0f, 128.0f, 22, 1, 1, 0, 0);

	Object->PlayerSpriteParts.LegLower = CreateSprite("TextureFiles/LegLower.png", 128.0f, 128.0f, 22, 1, 1, 0, 0);

	Object->PlayerSpriteParts.LegUpper2 = CreateSprite("TextureFiles/LegUpper.png", 128.0f, 128.0f, 22, 1, 1, 0, 0);

	Object->PlayerSpriteParts.LegLower2 = CreateSprite("TextureFiles/LegLower.png", 128.0f, 128.0f, 22, 1, 1, 0, 0);

	Object->PlayerSpriteParts.Skirt = CreateSprite("TextureFiles/Skirt.png", 300.0f, 300.0f, 22, 4, 1, 0, 0);

	Object->PlayerSpriteParts.Skirt->AnimationActive = 0;

	Object->PlayerSpriteParts.Body = CreateSprite("TextureFiles/Body.png", 300.0f, 300.0f, 22, 4, 1, 0, 0);

	Object->PlayerSpriteParts.Body->AnimationSpeed = 3;

	Object->PlayerSpriteParts.BlinkTimer = 0;

	Object->PlayerSpriteParts.Tail = CreateSprite("TextureFiles/TailIdle.png", 300.0f, 300.0f, 22, 7, 2, 0, 0);

	Object->PlayerSpriteParts.Tail->AnimationSpeed = Object->Speed/2 + 3;

	Object->TailSinValue = 0;

	Object->PlayerSpriteParts.ArmUpper = CreateSprite("TextureFiles/ArmUpper.png", 128.0f, 128.0f, 22, 1, 1, 0, 0);

	Object->PlayerSpriteParts.ArmLower = CreateSprite("TextureFiles/ArmLower.png", 128.0f, 128.0f, 22, 1, 1, 0, 0);
}


float RotateToAngle(float angle, float angleTo, float speed)
{
	int diff;
	
	diff = (((( (int)(angleTo/(float)FOX_PI * 180) - (int)(angle/(float)FOX_PI * 180) ) % 360) + 540) % 360) - 180;
	
	if (diff > 0)
	{
		if ((diff*(float)FOX_PI / 180) < speed)
			return angleTo;
	}
	else
	{
		if (-(diff*(float)FOX_PI / 180) < speed)
			return angleTo;
	}
	if (abs(diff) > 0)
		diff = diff / abs(diff);
	else
		return angleTo;
	return angle + diff * speed;	
}

