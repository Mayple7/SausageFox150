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
\li					updateDamage
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
void InitializePlayer(struct Player *CurrentPlayer, enum Character Princess, int newID, float xPos, float yPos)
{
	int i;

	for(i = 0; i < COLLIDEAMOUNT; i++)
	{
		CurrentPlayer->CollisionData[i] = -1;
	}

	CurrentPlayer->Princess = Princess;

	/*////////////////////////////////
	//       PLAYER BASICS          //
	////////////////////////////////*/
	CurrentPlayer->Position.x = xPos;
	CurrentPlayer->Position.y = yPos;
	CurrentPlayer->FlipX = FALSE;
	CurrentPlayer->FlipY = FALSE;
	CurrentPlayer->FlipXPrev = FALSE;
	CurrentPlayer->FlipYPrev = FALSE;

	//Moving and bufftacular
	CurrentPlayer->CurrentPlayerStats.MoveSpeed = 600.0f;
	CurrentPlayer->CurrentPlayerStats.AttackSpeed = 12.0f;
	CurrentPlayer->CurrentPlayerStats.CurrentBuff = None;
	CurrentPlayer->CurrentPlayerStats.BuffTimer = 0;
	CurrentPlayer->BuffHeld = None;


	/*////////////////////////////////
	//      PLAYER COLLISION        //
	////////////////////////////////*/
	CreateCollisionBox(&CurrentPlayer->PlayerCollider, &CurrentPlayer->Position, PlayerType, PLAYER_WIDTH, PLAYER_HEIGHT, newID);
	CurrentPlayer->PlayerCollider.Offset.y = 20 * GetLoadRatio();
	CurrentPlayer->PlayerCollider.width = CurrentPlayer->PlayerCollider.width - 20 * GetLoadRatio();
	UpdateCollider(&CurrentPlayer->PlayerCollider, CurrentPlayer->PlayerCollider.width, CurrentPlayer->PlayerCollider.height);

	//Collider Debug
	CurrentPlayer->PlayerCollider.collisionDebug = TRUE;


	/*////////////////////////////////
	//       PLAYER SO RIGID        //
	////////////////////////////////*/
	InitializeRigidBody(&CurrentPlayer->PlayerRigidBody, FALSE, PLAYER_WIDTH, PLAYER_HEIGHT);
	CurrentPlayer->PlayerRigidBody.onGround = FALSE;
	CurrentPlayer->dropDown = FALSE;


	/*////////////////////////////////
	//    PLAYER WEAPON & STATS     //
	////////////////////////////////*/
	CurrentPlayer->PlayerWeapon = CreateWeapon("Fragile Stick", "TextureFiles/stick.png", Sword, Common, WeaponFriendly, 256, 256, newID++);

	if(LoadPlayer(CurrentPlayer) < 1)
	{
		LoadNewPlayer(CurrentPlayer, Princess);
		AE_ASSERT_MESG("SOMETHING BE BROKED");
	}

	CurrentPlayer->PlayerSpriteParts.Weapon = CurrentPlayer->PlayerWeapon->WeaponSprite;
	CurrentPlayer->PlayerSpriteParts.Weapon->ZIndex = 24;

	/*////////////////////////////////
	//       PLAYER SOUNDS          //
	////////////////////////////////*/
	CurrentPlayer->CurrentPlayerSounds.Swing1 = CreateSound("Sounds/ToolSwing1.mp3", SmallSnd);
	CurrentPlayer->CurrentPlayerSounds.Swing2 = CreateSound("Sounds/ToolSwing2.mp3", SmallSnd);


	/*////////////////////////////////
	//       PLAYER SPRITE          //
	////////////////////////////////*/
	CurrentPlayer->PlayerSprite = (Sprite *) CreateSprite("TextureFiles/Sausage.png", 100, 100, 100, 1, 1, 0, 0);
	CurrentPlayer->PlayerSprite->Visible = FALSE;
	CreatePlayerSprites(CurrentPlayer);
	CurrentPlayer->PlayerSpriteParts.Weapon->ZIndex = CurrentPlayer->PlayerSprite->ZIndex + 2;
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
	UpdateCollisionPosition(&CurrentPlayer->PlayerWeapon->WeaponAttack, &CurrentPlayer->PlayerWeapon->WeaponAttackPosition);

	if (FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT) && !CurrentPlayer->isAttacking)
	{
		//Pick a random swing sound to play
		printf("1 Playing: %i, 2 Playing: %i\n", FoxSoundCheckIsPlaying(CurrentPlayer->CurrentPlayerSounds.Swing1), FoxSoundCheckIsPlaying(CurrentPlayer->CurrentPlayerSounds.Swing2));
		if (rand() % 2)
		{
			if (!FoxSoundCheckIsPlaying(CurrentPlayer->CurrentPlayerSounds.Swing2))
				PlayAudio(CurrentPlayer->CurrentPlayerSounds.Swing1);
		}
		else
		{
			if (!FoxSoundCheckIsPlaying(CurrentPlayer->CurrentPlayerSounds.Swing1))
				PlayAudio(CurrentPlayer->CurrentPlayerSounds.Swing2);
		}

		//Set the attacking necessaries
		CurrentPlayer->isAttacking = TRUE;
		CurrentPlayer->PlayerSpriteParts.AttackRotation = 0;
		CurrentPlayer->PlayerSpriteParts.AttackRotationArm = 0;
		CurrentPlayer->PlayerSpriteParts.AttackRotationArmLower = 0;
		UpdateCollider(&CurrentPlayer->PlayerCollider,CurrentPlayer->PlayerCollider.width, CurrentPlayer->PlayerCollider.height);
	}

	// not key press for direction then slow down!
	if(!FoxInput_KeyDown('D') && !FoxInput_KeyDown('A'))
	{
		if (!(CurrentPlayer->Position.y > GROUNDLEVEL * GetLoadRatio()) && !CurrentPlayer->PlayerRigidBody.onGround)
		{
			if (CurrentPlayer->Speed - 48.0f * GetDeltaTime() >= 0.0f)
			{
				CurrentPlayer->Speed -= 48.0f * GetDeltaTime();
			}
			else
			{
				CurrentPlayer->Speed = 0.0f;
				CurrentPlayer->LegSinValue = 0;
			}
		}
		else
		{
			if (CurrentPlayer->Speed - 48.0f * GetDeltaTime() >= 0.0f)
			{
				CurrentPlayer->Speed -= 48.0f * GetDeltaTime();
			}
			else
			{
				CurrentPlayer->Speed = 0.0f;
				CurrentPlayer->LegSinValue = 0;
			}
		}
	}
	if(FoxInput_KeyTriggered('Q') && CurrentPlayer->CurrentPlayerStats.CurrentBuff == None)
	{
		if(CurrentPlayer->BuffHeld != None)
		{
			CurrentPlayer->CurrentPlayerStats.CurrentBuff = CurrentPlayer->BuffHeld;
			CurrentPlayer->BuffHeld = None;
			switch(CurrentPlayer->CurrentPlayerStats.CurrentBuff)
			{
			case AtkSpeed:
				CurrentPlayer->CurrentPlayerStats.AttackSpeed = 24.0f;
				break;
			case MovSpeed:
				CurrentPlayer->CurrentPlayerStats.MoveSpeed += 300.0f;
				break;
			case DmgBuff:
				CurrentPlayer->CurrentPlayerStats.Damage += 20;
				break;
			}
		}
	}
	if(FoxInput_KeyTriggered('1'))
	{
		CurrentPlayer->BuffHeld = AtkSpeed;
	}
	else if(FoxInput_KeyTriggered('2'))
	{
		CurrentPlayer->BuffHeld = MovSpeed;
	}
	else if(FoxInput_KeyTriggered('3'))
	{
		CurrentPlayer->BuffHeld = DmgBuff;
	}

	if(CurrentPlayer->CurrentPlayerStats.CurrentBuff != None)
	{
		++CurrentPlayer->CurrentPlayerStats.BuffTimer;
		if(CurrentPlayer->CurrentPlayerStats.BuffTimer > 5 * FRAMERATE)
		{
			switch(CurrentPlayer->CurrentPlayerStats.CurrentBuff)
			{
			case AtkSpeed:
				updateAttackSpeed(&CurrentPlayer->CurrentPlayerStats);
				break;
			case MovSpeed:
				updateMoveSpeed(&CurrentPlayer->CurrentPlayerStats);
				break;
			case DmgBuff:
				updateDamage(CurrentPlayer);
				break;
			}

			CurrentPlayer->CurrentPlayerStats.BuffTimer = 0;
			CurrentPlayer->CurrentPlayerStats.CurrentBuff = None;
		}
	}

	// Move left if A is pressed
	if(FoxInput_KeyDown('A'))
	{
		CurrentPlayer->FlipX = 0;
		CurrentPlayer->PlayerDirection = LEFT;
		CurrentPlayer->Speed = CurrentPlayer->CurrentPlayerStats.MoveSpeed * GetLoadRatio() * GetDeltaTime();
	}
	// Move right if D is pressed
	else if(FoxInput_KeyDown('D'))
	{
		CurrentPlayer->FlipX = 1;
		CurrentPlayer->PlayerDirection = RIGHT;
		CurrentPlayer->Speed = CurrentPlayer->CurrentPlayerStats.MoveSpeed * GetLoadRatio() * GetDeltaTime();
	}
	//Jump when space is pushed or drop down if S is pushed as well
	if(FoxInput_KeyTriggered(VK_SPACE))
	{
		Vec2 velocity;
		
		if(FoxInput_KeyDown('S') && CurrentPlayer->PlayerRigidBody.onGround)
		{
			CurrentPlayer->PlayerRigidBody.onGround = FALSE;
			CurrentPlayer->dropDown = TRUE;
		}

		
		Vec2Set(&velocity, 0.0f, 1080.0f * GetLoadRatio());
		if(CurrentPlayer->Position.y <= GROUNDLEVEL * GetLoadRatio() || CurrentPlayer->PlayerRigidBody.onGround)
		{
			if(CurrentPlayer->Position.y <= GROUNDLEVEL * GetLoadRatio())
				Vec2Set(&CurrentPlayer->Position, CurrentPlayer->Position.x, GROUNDLEVEL * GetLoadRatio() + 0.1f);
			CurrentPlayer->PlayerRigidBody.onGround = FALSE;
			ApplyVelocity(&CurrentPlayer->PlayerRigidBody, &velocity);
		}
	}
	//Throw an upward force on the fox when backspace is pushed
	if(FoxInput_KeyDown(VK_BACK))
	{
		Vec2 force;

		CurrentPlayer->PlayerRigidBody.Acceleration.x = 0;
		CurrentPlayer->PlayerRigidBody.Acceleration.y = 0;
		Vec2Set(&force, 0.0f, -FOX_GRAVITY_Y * GetLoadRatio());
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
	// Move the direction based on the speed
	MoveObject(&CurrentPlayer->Position, CurrentPlayer->PlayerDirection, CurrentPlayer->Speed);
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
	Vec2 velocityTime;

	if(CurrentPlayer->Position.y < GROUNDLEVEL * GetLoadRatio())
	{
		CurrentPlayer->Position.y = GROUNDLEVEL * GetLoadRatio();
	}
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
		CurrentPlayer->Position.y -= 300.0f * GetDeltaTime() * GetLoadRatio();
		if(CurrentPlayer->PlayerRigidBody.Velocity.y < 0)
		{
			CurrentPlayer->PlayerRigidBody.Velocity.y = -1800.0f * GetDeltaTime() * GetLoadRatio();
			CurrentPlayer->dropDown = FALSE;
		}
	}
	//Update velocity and acceleration
	UpdateVelocity(&CurrentPlayer->PlayerRigidBody);
	Vec2Scale(&velocityTime, &CurrentPlayer->PlayerRigidBody.Velocity, GetDeltaTime());
	
	Vec2Add(&CurrentPlayer->Position, &CurrentPlayer->Position, &velocityTime);

	//Updates the collision box
	UpdateCollisionPosition(&CurrentPlayer->PlayerCollider, &CurrentPlayer->Position);
	CurrentPlayer->PlayerRigidBody.onGround = FALSE;
}

/*************************************************************************/
/*!
	\brief
	Soon I'll disappear into the deepest space, I won't leave a trace
	
	\param CurrentPlayer
	A pointer to the current player
*/
/*************************************************************************/
void DestroyPlayer(Player *CurrentPlayer)
{
	//Free the debug box since it isn't getting released in the object manager
	CurrentPlayer->PlayerCollider.collisionDebug = FALSE;
	AEGfxMeshFree(CurrentPlayer->PlayerCollider.DebugMesh);
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
	CurrentPlayerStats->MoveSpeed = CurrentPlayerStats->Agility + 600.0f;
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
	CurrentPlayerStats->AttackSpeed = CurrentPlayerStats->Agility * 5.0f + 12.0f;
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
	Update the player's damage reduction based on their defense
	
	\param CurrentPlayerStats
	A pointer to the player's stats to update
*/
/*************************************************************************/
void updateDamage(Player *CurrentPlayer)
{
	//Placeholder damage reduction formula
	CurrentPlayer->CurrentPlayerStats.Damage = 10 + (CurrentPlayer->CurrentPlayerStats.Strength + CurrentPlayer->PlayerWeapon->BonusStrength) * 5;
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
	Enemy* eList = enemyList;
	int hit = 0;
	int hitPrev = 0;

	while(pList->objID != -1)
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
					AE_ASSERT_MESG("No collision and not colliding, should never be here.");
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
	while(fList->objID != -1)
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
					fList->objID = 0;
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
					AE_ASSERT_MESG("No collision and not colliding, should never be here.");
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
	while(wList->objID != -1)
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
					updateDamage(&CurrentPlayer);
				}
				// Found target, hit previous frame, on persistant
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 1)
				{
					//printf("FOUND PERSISTANT: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					PlayerCollideWeaponDrop(&CurrentPlayer, wList);
					updateDamage(&CurrentPlayer);
				}
				// Found target, did not hit previous frame, on start collision
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 0)
				{
					//printf("FOUND NEW COLLISION: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					CurrentPlayer.CollisionData[hitPrev] = wList->WeaponPickup.collisionID * 10 + 1;
					PlayerCollideWeaponDrop(&CurrentPlayer, wList);
					updateDamage(&CurrentPlayer);
				}
			}
			else
			{
				if(hitPrev < 0 || CurrentPlayer.CollisionData[hitPrev] % 10 == 0)
				{
					// NEVER COLLIDED OR DIDNT COLLIDE PREV FRAME
					AE_ASSERT_MESG("No collision and not colliding, should never be here.");
				}
				// Found target, collision ended
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 1)
				{
					//printf("END COLLISION: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					CurrentPlayer.CollisionData[hitPrev] = 0;
					if(wList->WeaponGlyphs->Glyph->Visible)
					{
						ChangeTextVisibility(wList->WeaponGlyphs);
						ChangeTextVisibility(wList->WeaponStatsGlyphs);
						wList->WeaponHoverBackground->Visible = FALSE;
					}
				}
			}
		}
		wList++;
	}
	while(eList->objID != -1)
	{
		// If the weapon is the enemy's
		if(eList->objID > 0 && eList->EnemyType != Dummy && eList->EnemyWeapon->WeaponFOF == EnemyWeapon)
		{
			hit = CollisionRectangles(&CurrentPlayer.PlayerCollider, &eList->EnemyWeapon->WeaponAttack);
			hitPrev = searchHitArray(CurrentPlayer.CollisionData, COLLIDEAMOUNT, eList->EnemyWeapon->WeaponAttack.collisionID);
			if(hit && eList->isAttacking)
			{
				// New target, on start collision
				if(hitPrev < 0)
				{
					CurrentPlayer.CollisionData[-hitPrev] = eList->EnemyWeapon->WeaponPickup.collisionID * 10 + 1;
					//printf("NOT FOUND: %i\n", -hitPrev);
					PlayerCollideEnemyWeapon(&CurrentPlayer, eList);
				}
				// Found target, hit previous frame, on persistant
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 1)
				{
					//printf("FOUND PERSISTANT: %i\n", CurrentPlayer.CollisionData[hitPrev]);
				}
				// Found target, did not hit previous frame, on start collision
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 0)
				{
					//printf("FOUND NEW COLLISION: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					CurrentPlayer.CollisionData[hitPrev] = eList->EnemyWeapon->WeaponAttack.collisionID * 10 + 1;
					PlayerCollideEnemyWeapon(&CurrentPlayer, eList);
				}
			}
			else
			{
				if(hitPrev < 0 || CurrentPlayer.CollisionData[hitPrev] % 10 == 0)
				{
					// NEVER COLLIDED OR DIDNT COLLIDE PREV FRAME
					AE_ASSERT_MESG("No collision and not colliding, should never be here.");
				}
				// Found target, collision ended
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 1)
				{
					//printf("END COLLISION: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					//CurrentPlayer.CollisionData[hitPrev] = 0;
				}
			}
			if(!(eList->isAttacking) && hitPrev > 0 && eList->EnemyWeapon->WeaponFOF == EnemyWeapon)
			{
				CurrentPlayer.CollisionData[hitPrev] = 0;
			}

		}
		eList++;
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
	float LegDistance = ((Object->CurrentPlayerStats.MoveSpeed * GetDeltaTime() * GetLoadRatio()) + (1.5f / ((Object->Speed * 0.15f + 0.1f)) ))-(Object->Speed);
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

	Object->LegSinValue += 10.0f * GetDeltaTime() * (Object->Speed * 0.1f); 

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
	Bdy->Position.y = Object->Position.y - ((float)sin(-Object->LegSinValue*2)*5/(LegDistance)) * GetLoadRatio();
	Skrt->Position = Bdy->Position;
	if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL * GetLoadRatio())
		Skrt->CurrentFrame = (int)floor(fabs(LegUpperDirection*4));
	else
		Skrt->CurrentFrame = 3;
	Tail->Position.y = Bdy->Position.y + (Bdy->Height/30);
	Tail->Rotation = (float)sin(Object->TailSinValue*1.25f)/4;

	if (Object->Speed * GetLoadRatio() > 90.0f * GetDeltaTime() * GetLoadRatio())
	{
		Tail->SpriteTexture = LoadTexture("TextureFiles/TailRun.png");
		Object->TailSinValue += 6.0f * GetDeltaTime();
		Object->PlayerSpriteParts.Tail->AnimationSpeed = (Object->Speed * GetLoadRatio())/2 + 3 * FRAMERATE / 60;
	}
	else
	{
		Tail->SpriteTexture = LoadTexture("TextureFiles/TailIdle.png");
		Object->TailSinValue = 0;
		Object->PlayerSpriteParts.Tail->AnimationSpeed = 4 * FRAMERATE / 60;
	}

	if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL * GetLoadRatio())
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
		LegUpperDirection = (float)sin(LegDistance/10) - 1.0f;
		LegUpperDirection2 = (float)sin(LegDistance/10) - 1.0f;//60.0f * GetDeltaTime();
		LegLowerDirection = LegUpperDirection + 0.5f;//30.0f * GetDeltaTime();
		LegLowerDirection2 = LegUpperDirection2 - 0.5f;//30.0f * GetDeltaTime();
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
		if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL * GetLoadRatio())
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*-8/(LegDistance) * GetLoadRatio();
		}
		LegUpr->Position.y = Object->Position.y + ((float)sin(Object->LegSinValue*2)*5/(LegDistance)) * GetLoadRatio();
		LegLwr->Position.x = (float)cos(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.x;
		LegLwr->Position.y = (float)sin(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.y;
		LegLwr->Rotation = LegLowerDirection;
		
		LegUpr2->Rotation = -LegUpperDirection2;
		LegUpr2->Position.x = Object->Position.x;
		if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL * GetLoadRatio())
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*8/(LegDistance) * GetLoadRatio();
		}
		LegUpr2->Position.y = Object->Position.y + ((float)sin(Object->LegSinValue*2)*5/(LegDistance)) * GetLoadRatio();
		LegLwr2->Position.x = (float)cos(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.x;
		LegLwr2->Position.y = (float)sin(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.y;
		LegLwr2->Rotation = -LegLowerDirection2;
		
		ArmUpr->Rotation = LegUpperDirection/1.5f + 1.5f;
		ArmLwr->Rotation = ArmUpr->Rotation - 1.25f + LegUpperDirection/2.0f;
		ArmUpr->Position.x = Bdy->Position.x;
		ArmUpr->Position.y = Bdy->Position.y + (Bdy->Width/5.25f);
		ArmLwr->Position.x = ArmUpr->Position.x - (float)cos(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);
		ArmLwr->Position.y = ArmUpr->Position.y - (float)sin(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);

		// Attacking!
		if (Object->isAttacking)
		{
			Object->PlayerSpriteParts.AttackRotation = RotateToAngle(Object->PlayerSpriteParts.AttackRotation, 0, 0.2f);
			Object->PlayerSpriteParts.AttackRotationArm = RotateToAngle(Object->PlayerSpriteParts.AttackRotationArm, (float)FOX_PI, Object->CurrentPlayerStats.AttackSpeed * GetDeltaTime());
			Object->PlayerSpriteParts.AttackRotationArmLower = RotateToAngle(Object->PlayerSpriteParts.AttackRotationArmLower, (float)FOX_PI/2, Object->CurrentPlayerStats.AttackSpeed * GetDeltaTime());
			ArmUpr2->Rotation = (float)FOX_PI * 1.5f + 30.0f * GetDeltaTime() - Object->PlayerSpriteParts.AttackRotationArm;
			ArmLwr2->Rotation = ArmUpr2->Rotation - (float)FOX_PI/2 + Object->PlayerSpriteParts.AttackRotationArmLower;
			//Weap->Rotation = ArmLwr2->Rotation + Object->PlayerSpriteParts.AttackRotation;
			if (Object->PlayerSpriteParts.AttackRotationArm == (float)FOX_PI)
				Object->isAttacking = FALSE;
		}
		else
		{
			ArmUpr2->Rotation = -LegUpperDirection/1.5f + 1.5f;
			ArmLwr2->Rotation = -(ArmUpr->Rotation - 1.75f + LegUpperDirection/2.0f);
			//Weap->Rotation = ArmLwr2->Rotation;
		}
		Weap->Rotation = ArmLwr2->Rotation;
		ArmUpr2->Position.x = Bdy->Position.x;
		ArmUpr2->Position.y = Bdy->Position.y + (Bdy->Width/5.25f);
		ArmLwr2->Position.x = ArmUpr2->Position.x - (float)cos(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);
		ArmLwr2->Position.y = ArmUpr2->Position.y - (float)sin(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);

		if ((Object->Speed * GetLoadRatio()) < 0.6f * GetDeltaTime() * GetLoadRatio())
		{
			if (!Object->isAttacking)
			{
				ArmLwr->Rotation = ArmUpr->Rotation - 0.2f;
				ArmLwr2->Rotation = ArmUpr2->Rotation - 0.5f;
				Weap->Rotation = ArmLwr2->Rotation;
			}
		}

		Weap->Position.x = ArmLwr2->Position.x - (float)cos(ArmLwr2->Rotation) * (ArmLwr2->Width/3.5f);
		Weap->Position.y = ArmLwr2->Position.y - (float)sin(ArmLwr2->Rotation) * (ArmLwr2->Width/3.5f);
		Weap->ZIndex = Object->PlayerSprite->ZIndex - 1;
	}
	else
	{
		Tail->Position.x = Bdy->Position.x-(Bdy->Width/20);
		
		LegUpr->Rotation = -LegUpperDirection;
		LegUpr->Position.x = Object->Position.x;
		if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL * GetLoadRatio())
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*-8/(LegDistance) * GetLoadRatio();
		}
		LegUpr->Position.y = Object->Position.y + ((float)sin(Object->LegSinValue*2)*5/(LegDistance)) * GetLoadRatio();
		LegLwr->Position.x = (float)cos(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.x;
		LegLwr->Position.y = (float)sin(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.y;
		LegLwr->Rotation = -LegLowerDirection;
		
		LegUpr2->Rotation = LegUpperDirection2;
		LegUpr2->Position.x = Object->Position.x;
		if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL * GetLoadRatio())
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*8/(LegDistance) * GetLoadRatio();
		}
		LegUpr2->Position.y = Object->Position.y + ((float)sin(Object->LegSinValue*2)*5/(LegDistance)) * GetLoadRatio();
		LegLwr2->Position.x = (float)cos(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.x;
		LegLwr2->Position.y = (float)sin(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.y;
		LegLwr2->Rotation = LegLowerDirection2;
		
		// Attacking!
		if (Object->isAttacking)
		{
			Object->PlayerSpriteParts.AttackRotation = RotateToAngle(Object->PlayerSpriteParts.AttackRotation, (float)FOX_PI/6, Object->CurrentPlayerStats.AttackSpeed * GetDeltaTime());
			Object->PlayerSpriteParts.AttackRotationArm = RotateToAngle(Object->PlayerSpriteParts.AttackRotationArm, (float)FOX_PI, Object->CurrentPlayerStats.AttackSpeed * GetDeltaTime());
			Object->PlayerSpriteParts.AttackRotationArmLower = RotateToAngle(Object->PlayerSpriteParts.AttackRotationArmLower, (float)FOX_PI/2, Object->CurrentPlayerStats.AttackSpeed * GetDeltaTime());
			ArmUpr->Rotation = (float)FOX_PI / 2 - 30.0f * GetDeltaTime() + Object->PlayerSpriteParts.AttackRotationArm;
			ArmLwr->Rotation = ArmUpr->Rotation + (float)FOX_PI/2 - Object->PlayerSpriteParts.AttackRotationArmLower;
			//Weap->Rotation = ArmLwr->Rotation - Object->PlayerSpriteParts.AttackRotation;
			if (Object->PlayerSpriteParts.AttackRotationArm == (float)FOX_PI)
				Object->isAttacking = FALSE;
		}
		else
		{
			ArmUpr->Rotation = -LegUpperDirection/1.5f - 1.5f;
			ArmLwr->Rotation = ArmUpr->Rotation + 1.25f - LegUpperDirection/2.0f;
		}
		Weap->Rotation = ArmLwr->Rotation;
		ArmUpr->Position.x = Bdy->Position.x;
		ArmUpr->Position.y = Bdy->Position.y + (Bdy->Width/5.25f);
		ArmLwr->Position.x = ArmUpr->Position.x + (float)cos(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);
		ArmLwr->Position.y = ArmUpr->Position.y + (float)sin(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);

		ArmUpr2->Rotation = LegUpperDirection/1.5f - 1.5f;
		ArmLwr2->Rotation = ArmUpr2->Rotation + 1.25f + LegUpperDirection/2.0f;
		ArmUpr2->Position.x = Bdy->Position.x;
		ArmUpr2->Position.y = Bdy->Position.y + (Bdy->Width/5.25f);
		ArmLwr2->Position.x = ArmUpr2->Position.x + (float)cos(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);
		ArmLwr2->Position.y = ArmUpr2->Position.y + (float)sin(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);

		if ((Object->Speed * GetLoadRatio()) < 0.6f * GetDeltaTime() * GetLoadRatio())
		{
			if (!Object->isAttacking)
			{
				ArmLwr->Rotation = ArmUpr->Rotation + 0.5f;
				ArmLwr2->Rotation = ArmUpr2->Rotation + 0.2f;
				Weap->Rotation = ArmLwr->Rotation;
			}
		}

		Weap->Position.x = ArmLwr->Position.x + (float)cos(ArmLwr->Rotation) * (ArmLwr->Width/3.5f);
		Weap->Position.y = ArmLwr->Position.y + (float)sin(ArmLwr->Rotation) * (ArmLwr->Width/3.5f);
		Weap->ZIndex = Object->PlayerSprite->ZIndex + 2;
	}

	Object->PlayerWeapon->WeaponAttackPosition.x = Weap->Position.x + (cosf(Weap->Rotation + FOX_PI / 2) * Object->PlayerWeapon->WeaponLength);
	Object->PlayerWeapon->WeaponAttackPosition.y = Weap->Position.y + (sinf(Weap->Rotation + FOX_PI / 2) * Object->PlayerWeapon->WeaponLength);

	//*************************************************************************************************
}

void CreatePlayerSprites(Player *Object)
{
	switch(Object->Princess)
	{
	case Mayple:
		Object->PlayerSpriteParts.ArmUpper2 = (Sprite *) CreateSprite("TextureFiles/ArmUpperMayple.png", 128.0f, 128.0f, Object->PlayerSprite->ZIndex - 2, 1, 1, 0, 0);
		Object->PlayerSpriteParts.Skirt = (Sprite *) CreateSprite("TextureFiles/SkirtMayple.png", 300.0f, 300.0f, Object->PlayerSprite->ZIndex + 1, 4, 1, 0, 0);
		Object->PlayerSpriteParts.Body = (Sprite *) CreateSprite("TextureFiles/BodyMayple.png", 300.0f, 300.0f, Object->PlayerSprite->ZIndex, 4, 1, 0, 0);
		Object->PlayerSpriteParts.ArmUpper = (Sprite *) CreateSprite("TextureFiles/ArmUpperMayple.png", 128.0f, 128.0f, Object->PlayerSprite->ZIndex + 2, 1, 1, 0, 0);
		break;
	case Ginko:
		Object->PlayerSpriteParts.ArmUpper2 = (Sprite *) CreateSprite("TextureFiles/ArmUpperGinko.png", 128.0f, 128.0f, Object->PlayerSprite->ZIndex - 2, 1, 1, 0, 0);
		Object->PlayerSpriteParts.Skirt = (Sprite *) CreateSprite("TextureFiles/SkirtGinko.png", 300.0f, 300.0f, Object->PlayerSprite->ZIndex + 1, 4, 1, 0, 0);
		Object->PlayerSpriteParts.Body = (Sprite *) CreateSprite("TextureFiles/BodyGinko.png", 300.0f, 300.0f, Object->PlayerSprite->ZIndex, 4, 1, 0, 0);
		Object->PlayerSpriteParts.ArmUpper = (Sprite *) CreateSprite("TextureFiles/ArmUpperGinko.png", 128.0f, 128.0f, Object->PlayerSprite->ZIndex + 2, 1, 1, 0, 0);
		break;
	case Holly:
		Object->PlayerSpriteParts.ArmUpper2 = (Sprite *) CreateSprite("TextureFiles/ArmUpperHolly.png", 128.0f, 128.0f, Object->PlayerSprite->ZIndex - 2, 1, 1, 0, 0);
		Object->PlayerSpriteParts.Skirt = (Sprite *) CreateSprite("TextureFiles/SkirtHolly.png", 300.0f, 300.0f, Object->PlayerSprite->ZIndex + 1, 4, 1, 0, 0);
		Object->PlayerSpriteParts.Body = (Sprite *) CreateSprite("TextureFiles/BodyHolly.png", 300.0f, 300.0f, Object->PlayerSprite->ZIndex, 4, 1, 0, 0);
		Object->PlayerSpriteParts.ArmUpper = (Sprite *) CreateSprite("TextureFiles/ArmUpperHolly.png", 128.0f, 128.0f, Object->PlayerSprite->ZIndex + 2, 1, 1, 0, 0);
		break;
	case Kaya:
		Object->PlayerSpriteParts.ArmUpper2 = (Sprite *) CreateSprite("TextureFiles/ArmUpperKaya.png", 128.0f, 128.0f, Object->PlayerSprite->ZIndex - 2, 1, 1, 0, 0);
		Object->PlayerSpriteParts.Skirt = (Sprite *) CreateSprite("TextureFiles/SkirtKaya.png", 300.0f, 300.0f, Object->PlayerSprite->ZIndex + 1, 4, 1, 0, 0);
		Object->PlayerSpriteParts.Body = (Sprite *) CreateSprite("TextureFiles/BodyKaya.png", 300.0f, 300.0f, Object->PlayerSprite->ZIndex, 4, 1, 0, 0);
		Object->PlayerSpriteParts.ArmUpper = (Sprite *) CreateSprite("TextureFiles/ArmUpperKaya.png", 128.0f, 128.0f, Object->PlayerSprite->ZIndex + 2, 1, 1, 0, 0);
		break;
	}

	Object->PlayerSpriteParts.ArmLower2 = (Sprite *) CreateSprite("TextureFiles/ArmLower.png", 128.0f, 128.0f, Object->PlayerSprite->ZIndex - 2, 1, 1, 0, 0);

	Object->PlayerSpriteParts.LegUpper = (Sprite *) CreateSprite("TextureFiles/LegUpper.png", 128.0f, 128.0f, Object->PlayerSprite->ZIndex, 1, 1, 0, 0);

	Object->PlayerSpriteParts.LegLower = (Sprite *) CreateSprite("TextureFiles/LegLower.png", 128.0f, 128.0f, Object->PlayerSprite->ZIndex, 1, 1, 0, 0);

	Object->PlayerSpriteParts.LegUpper2 = (Sprite *) CreateSprite("TextureFiles/LegUpper.png", 128.0f, 128.0f, Object->PlayerSprite->ZIndex, 1, 1, 0, 0);

	Object->PlayerSpriteParts.LegLower2 = (Sprite *) CreateSprite("TextureFiles/LegLower.png", 128.0f, 128.0f, Object->PlayerSprite->ZIndex, 1, 1, 0, 0);

	Object->PlayerSpriteParts.Skirt->AnimationActive = 0;

	Object->PlayerSpriteParts.Body->AnimationSpeed = 3;

	Object->PlayerSpriteParts.BlinkTimer = 0;

	Object->PlayerSpriteParts.Tail = (Sprite *) CreateSprite("TextureFiles/TailIdle.png", 300.0f, 300.0f, Object->PlayerSprite->ZIndex, 7, 2, 0, 0);

	Object->PlayerSpriteParts.Tail->AnimationSpeed = (Object->Speed * GetLoadRatio())/2 + 3;

	Object->TailSinValue = 0;

	Object->PlayerSpriteParts.ArmLower = (Sprite *) CreateSprite("TextureFiles/ArmLower.png", 128.0f, 128.0f, Object->PlayerSprite->ZIndex + 3, 1, 1, 0, 0);
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

/*************************************************************************/
/*!
	\brief
	Saves the player
	
	\param CurrentPlayer
	A pointer to the player to be saved
*/
/*************************************************************************/
void SavePlayer(Player *CurrentPlayer)
{
	FILE *fp;
	char* string = (char *)MallocMyAlloc(500, 1);

	sprintf(string, "Princess: %d\nBuffHeld: %d\nAgility: %d\nStrength: %d\nDefense: %d\nMoney: %d\nCurrentHealth: %d\nWeaponRarity: %d\nWeaponType: %d\nWeaponAgility: %d\nWeaponStrength: %d\nWeaponDefense: %d\n%s",
		CurrentPlayer->Princess, CurrentPlayer->BuffHeld, CurrentPlayer->CurrentPlayerStats.Agility, CurrentPlayer->CurrentPlayerStats.Strength, CurrentPlayer->CurrentPlayerStats.Defense, 
		CurrentPlayer->CurrentPlayerStats.Money, CurrentPlayer->CurrentPlayerStats.CurrentHealth, CurrentPlayer->PlayerWeapon->WeaponRarity, CurrentPlayer->PlayerWeapon->WeaponType,
		CurrentPlayer->PlayerWeapon->BonusAgility, CurrentPlayer->PlayerWeapon->BonusStrength, CurrentPlayer->PlayerWeapon->BonusDefense, CurrentPlayer->PlayerWeapon->WeaponName);
	
	fp = fopen("../GameData.cfg", "wt");
	if(fp)
	{
		int num = 0;
		num = fprintf(fp, "%s", string);
		fclose(fp);
	}

	FreeMyAlloc(string);
}

/*************************************************************************/
/*!
	\brief
	Loads the player
	
	\param CurrentPlayer
	A pointer to the player to be loaded
*/
/*************************************************************************/
int LoadPlayer(Player *CurrentPlayer)
{
	FILE *fp;

	fp = fopen("../GameData.cfg", "rt");
	if(fp)
	{
		int num = 0;
		num = fscanf(fp, "%*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %[^\n]",
			&CurrentPlayer->Princess, &CurrentPlayer->BuffHeld, &CurrentPlayer->CurrentPlayerStats.Agility, &CurrentPlayer->CurrentPlayerStats.Strength, &CurrentPlayer->CurrentPlayerStats.Defense, 
			&CurrentPlayer->CurrentPlayerStats.Money, &CurrentPlayer->CurrentPlayerStats.CurrentHealth, &CurrentPlayer->PlayerWeapon->WeaponRarity, &CurrentPlayer->PlayerWeapon->WeaponType,
			&CurrentPlayer->PlayerWeapon->BonusAgility, &CurrentPlayer->PlayerWeapon->BonusStrength, &CurrentPlayer->PlayerWeapon->BonusDefense, CurrentPlayer->PlayerWeapon->WeaponName);

		fclose(fp);
		if(num == 13)
		{
			int nameLen, statsLen;
			updateAttackSpeed(&CurrentPlayer->CurrentPlayerStats);
			updateMoveSpeed(&CurrentPlayer->CurrentPlayerStats);
			updateDamage(CurrentPlayer);
			updateDamageReduction(&CurrentPlayer->CurrentPlayerStats);
			updateMaxHealth(&CurrentPlayer->CurrentPlayerStats);
			
			ChangeTextString(CurrentPlayer->PlayerWeapon->WeaponGlyphs, CurrentPlayer->PlayerWeapon->WeaponName);
			CreateStatsString(CurrentPlayer->PlayerWeapon->WeaponStatsString, CurrentPlayer->PlayerWeapon->BonusStrength, CurrentPlayer->PlayerWeapon->BonusAgility, CurrentPlayer->PlayerWeapon->BonusDefense);
			ChangeTextString(CurrentPlayer->PlayerWeapon->WeaponStatsGlyphs, CurrentPlayer->PlayerWeapon->WeaponStatsString);
			switch(CurrentPlayer->PlayerWeapon->WeaponType)
			{
			case Sword:
				freeObject(CurrentPlayer->PlayerWeapon->WeaponSprite);
				CurrentPlayer->PlayerWeapon->WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Sword.png", 256, 256, 5, 1, 1, 0, 0);
				break;
			case Axe:
				freeObject(CurrentPlayer->PlayerWeapon->WeaponSprite);
				CurrentPlayer->PlayerWeapon->WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Axe.png", 256, 256, 5, 1, 1, 0, 0);
				break;
			case Hammer:
				freeObject(CurrentPlayer->PlayerWeapon->WeaponSprite);
				CurrentPlayer->PlayerWeapon->WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Hammer.png", 256, 256, 5, 1, 1, 0, 0);
				break;
			case Spear:
				freeObject(CurrentPlayer->PlayerWeapon->WeaponSprite);
				CurrentPlayer->PlayerWeapon->WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Spear.png", 256, 256, 5, 1, 1, 0, 0);
				break;
			default:
				freeObject(CurrentPlayer->PlayerWeapon->WeaponSprite);
				CurrentPlayer->PlayerWeapon->WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Sword.png", 256, 256, 5, 1, 1, 0, 0);
				break;
			}
			nameLen = strlen(CurrentPlayer->PlayerWeapon->WeaponName);
			statsLen = strlen(CurrentPlayer->PlayerWeapon->WeaponStatsString);
			if(nameLen >= statsLen)
			{
				CurrentPlayer->PlayerWeapon->WeaponHoverBackground = (Sprite *) CreateSprite("TextureFiles/WeaponHoverBackground.png", nameLen * 25.0f, 120, 10, 1, 1, CurrentPlayer->PlayerWeapon->WeaponPickup.Position.x / GetLoadRatio(), (CurrentPlayer->PlayerWeapon->WeaponPickup.Position.y + CurrentPlayer->PlayerWeapon->WeaponPickup.height * 1.5f) / GetLoadRatio());
			}
			else
			{
				CurrentPlayer->PlayerWeapon->WeaponHoverBackground = (Sprite *) CreateSprite("TextureFiles/WeaponHoverBackground.png", statsLen * 25.0f, 120, 10, 1, 1, CurrentPlayer->PlayerWeapon->WeaponPickup.Position.x / GetLoadRatio(), (CurrentPlayer->PlayerWeapon->WeaponPickup.Position.y + CurrentPlayer->PlayerWeapon->WeaponPickup.height * 1.5f) / GetLoadRatio());
			}
			CurrentPlayer->PlayerWeapon->WeaponHoverBackground->Visible = FALSE;

			return 1;
		}
		else
			return -1;
	}
	else
	{
		return 0;
	}
}

/*************************************************************************/
/*!
	\brief
	Loads a new player
	
	\param CurrentPlayer
	A pointer to the player to be loaded
*/
/*************************************************************************/
void LoadNewPlayer(Player *CurrentPlayer, enum Character Princess)
{
	int nameLen, statsLen;
	CurrentPlayer->Princess = Princess;
	CurrentPlayer->BuffHeld = None;

	CurrentPlayer->CurrentPlayerStats.Agility = 0;
	CurrentPlayer->CurrentPlayerStats.Strength = 0;
	CurrentPlayer->CurrentPlayerStats.Defense = 0;
	
	updateAttackSpeed(&CurrentPlayer->CurrentPlayerStats);
	updateMoveSpeed(&CurrentPlayer->CurrentPlayerStats);
	updateDamage(CurrentPlayer);
	updateDamageReduction(&CurrentPlayer->CurrentPlayerStats);
	updateMaxHealth(&CurrentPlayer->CurrentPlayerStats);

	CurrentPlayer->CurrentPlayerStats.Money = 0;
	CurrentPlayer->CurrentPlayerStats.CurrentHealth = CurrentPlayer->CurrentPlayerStats.MaxHealth;

	switch(Princess)
	{
	case Mayple:
		freeObject(CurrentPlayer->PlayerWeapon->WeaponSprite);
		CurrentPlayer->PlayerWeapon->WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Sword.png", 256, 256, 5, 1, 1, 0, 0);
		CurrentPlayer->PlayerWeapon->WeaponName = strcpy(CurrentPlayer->PlayerWeapon->WeaponName, "Mayples Toy Sword");
		CurrentPlayer->PlayerWeapon->WeaponType = Sword;
		break;
	case Ginko:
		freeObject(CurrentPlayer->PlayerWeapon->WeaponSprite);
		CurrentPlayer->PlayerWeapon->WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Axe.png", 256, 256, 5, 1, 1, 0, 0);
		CurrentPlayer->PlayerWeapon->WeaponName = strcpy(CurrentPlayer->PlayerWeapon->WeaponName, "Ginkos Toy Axe");
		CurrentPlayer->PlayerWeapon->WeaponType = Axe;
		break;
	case Kaya:
		freeObject(CurrentPlayer->PlayerWeapon->WeaponSprite);
		CurrentPlayer->PlayerWeapon->WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Hammer.png", 256, 256, 5, 1, 1, 0, 0);
		CurrentPlayer->PlayerWeapon->WeaponName = strcpy(CurrentPlayer->PlayerWeapon->WeaponName, "Kayas Toy Hammer");
		CurrentPlayer->PlayerWeapon->WeaponType = Hammer;
		break;
	case Holly:
		freeObject(CurrentPlayer->PlayerWeapon->WeaponSprite);
		CurrentPlayer->PlayerWeapon->WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Spear.png", 256, 256, 5, 1, 1, 0, 0);
		CurrentPlayer->PlayerWeapon->WeaponName = strcpy(CurrentPlayer->PlayerWeapon->WeaponName, "Hollys Toy Spear");
		CurrentPlayer->PlayerWeapon->WeaponType = Spear;
		break;
	default:
		freeObject(CurrentPlayer->PlayerWeapon->WeaponSprite);
		CurrentPlayer->PlayerWeapon->WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Stick.png", 256, 256, 5, 1, 1, 0, 0);
		CurrentPlayer->PlayerWeapon->WeaponName = strcpy(CurrentPlayer->PlayerWeapon->WeaponName, "Fragile Stick");
		break;
	}
	ChangeTextString(CurrentPlayer->PlayerWeapon->WeaponGlyphs, CurrentPlayer->PlayerWeapon->WeaponName);
	CreateStatsString(CurrentPlayer->PlayerWeapon->WeaponStatsString, CurrentPlayer->PlayerWeapon->BonusStrength, CurrentPlayer->PlayerWeapon->BonusAgility, CurrentPlayer->PlayerWeapon->BonusDefense);
	
	nameLen = strlen(CurrentPlayer->PlayerWeapon->WeaponName);
	statsLen = strlen(CurrentPlayer->PlayerWeapon->WeaponStatsString);
	if(nameLen >= statsLen)
	{
		CurrentPlayer->PlayerWeapon->WeaponHoverBackground = (Sprite *) CreateSprite("TextureFiles/WeaponHoverBackground.png", nameLen * 25.0f, 120, 10, 1, 1, CurrentPlayer->PlayerWeapon->WeaponPickup.Position.x / GetLoadRatio(), (CurrentPlayer->PlayerWeapon->WeaponPickup.Position.y + CurrentPlayer->PlayerWeapon->WeaponPickup.height * 1.5f) / GetLoadRatio());
	}
	else
	{
		CurrentPlayer->PlayerWeapon->WeaponHoverBackground = (Sprite *) CreateSprite("TextureFiles/WeaponHoverBackground.png", statsLen * 25.0f, 120, 10, 1, 1, CurrentPlayer->PlayerWeapon->WeaponPickup.Position.x / GetLoadRatio(), (CurrentPlayer->PlayerWeapon->WeaponPickup.Position.y + CurrentPlayer->PlayerWeapon->WeaponPickup.height * 1.5f) / GetLoadRatio());
	}
	CurrentPlayer->PlayerWeapon->WeaponHoverBackground->Visible = FALSE;
}
