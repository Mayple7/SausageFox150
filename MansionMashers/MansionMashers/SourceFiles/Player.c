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
void InitializePlayer(struct Player *CurrentPlayer, enum Character Princess, float xPos, float yPos)
{
	int i, startingBuff;

	for(i = 0; i < COLLIDEAMOUNT; i++)
	{
		CurrentPlayer->CollisionData[i] = -1;
	}

	CurrentPlayer->Princess = Princess;
	CurrentPlayer->CurrentLevel = GS_Tutorial;
	CurrentPlayer->PlayerActive = TRUE;

	/*////////////////////////////////
	//       PLAYER BASICS          //
	////////////////////////////////*/
	CurrentPlayer->Position.x = xPos * GetLoadRatio();
	CurrentPlayer->Position.y = yPos * GetLoadRatio();
	CurrentPlayer->FlipX = FALSE;
	CurrentPlayer->FlipY = FALSE;
	CurrentPlayer->FlipXPrev = FALSE;
	CurrentPlayer->FlipYPrev = FALSE;

	//Moving and bufftacular
	CurrentPlayer->CurrentPlayerStats.MoveSpeed = 600.0f;
	CurrentPlayer->CurrentPlayerStats.AttackSpeed = 12.0f;

	CurrentPlayer->CurrentPlayerStats.AgilityTimer = 0;
	CurrentPlayer->CurrentPlayerStats.StrengthTimer = 0;
	CurrentPlayer->CurrentPlayerStats.DefenseTimer = 0;
	CurrentPlayer->CurrentPlayerStats.HasteTimer = 0;

	CurrentPlayer->BuffHeld[0] = FALSE;
	CurrentPlayer->BuffHeld[1] = FALSE;
	CurrentPlayer->BuffHeld[2] = FALSE;
	CurrentPlayer->BuffHeld[3] = FALSE;

	CurrentPlayer->BuffSelected = 0;

	/*////////////////////////////////
	//      PLAYER COLLISION        //
	////////////////////////////////*/
	CreateCollisionBox(&CurrentPlayer->PlayerCollider, &CurrentPlayer->Position, PlayerType, PLAYER_WIDTH, PLAYER_HEIGHT, 1);
	CurrentPlayer->PlayerCollider.Offset.y = 20 * GetLoadRatio();
	CurrentPlayer->PlayerCollider.width = CurrentPlayer->PlayerCollider.width - 20 * GetLoadRatio();
	UpdateCollider(&CurrentPlayer->PlayerCollider, CurrentPlayer->PlayerCollider.width, CurrentPlayer->PlayerCollider.height);

	//Collider Debug
	CurrentPlayer->PlayerCollider.collisionDebug = FALSE;


	/*////////////////////////////////
	//       PLAYER SO RIGID        //
	////////////////////////////////*/
	InitializeRigidBody(&CurrentPlayer->PlayerRigidBody, FALSE, PLAYER_WIDTH, PLAYER_HEIGHT);
	CurrentPlayer->PlayerRigidBody.onGround = FALSE;
	CurrentPlayer->dropDown = FALSE;


	/*////////////////////////////////
	//    PLAYER WEAPON & STATS     //
	////////////////////////////////*/
	CurrentPlayer->PlayerWeapon = CreateWeapon("Fragile Stick", "TextureFiles/stick.png", Sword, Common, WeaponFriendly, 256, 256, 2);

	if(LoadPlayer(CurrentPlayer) < 1)
	{
		LoadNewPlayer(CurrentPlayer, Princess);
	}

	startingBuff = 3;

	while(!CurrentPlayer->BuffHeld[CurrentPlayer->BuffSelected] && startingBuff != CurrentPlayer->BuffSelected)
	{
		CurrentPlayer->BuffSelected++;
		if(CurrentPlayer->BuffSelected > 3)
		{
			CurrentPlayer->BuffSelected = 0;
		}
	}

	CurrentPlayer->PlayerSpriteParts.Weapon = CurrentPlayer->PlayerWeapon->WeaponSprite;
	
	/*////////////////////////////////
	//       PLAYER HEALTH          //
	////////////////////////////////*/
	
	updateMaxHealth(&CurrentPlayer->CurrentPlayerStats);

	CurrentPlayer->CurrentPlayerStats.CurrentHealth = CurrentPlayer->CurrentPlayerStats.MaxHealth;


	/*////////////////////////////////
	//       PLAYER SOUNDS          //
	////////////////////////////////*/
	CurrentPlayer->CurrentPlayerSounds.Swing1 = CreateSound("Sounds/ToolSwing1.mp3", SmallSnd);
	CurrentPlayer->CurrentPlayerSounds.Swing2 = CreateSound("Sounds/ToolSwing2.mp3", SmallSnd);


	/*////////////////////////////////
	//       PLAYER SPRITE          //
	////////////////////////////////*/
	CurrentPlayer->Zindex = 100;
	CreatePlayerSprites(CurrentPlayer);
	CurrentPlayer->PlayerSpriteParts.Weapon->ZIndex = CurrentPlayer->Zindex + 2;
	Animation(CurrentPlayer);
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
	else
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

	//Cycle through the buffs
	if(FoxInput_KeyTriggered('Q'))
	{
		//The starting buff so we don't infini-loop
		int startingBuff = CurrentPlayer->BuffSelected++;

		//Loop back around
		if(CurrentPlayer->BuffSelected > 3)
			CurrentPlayer->BuffSelected = 0;

		//Go to the next acquired buff, or stop looping
		while(!CurrentPlayer->BuffHeld[CurrentPlayer->BuffSelected] && startingBuff != CurrentPlayer->BuffSelected)
		{
			CurrentPlayer->BuffSelected++;
			if(CurrentPlayer->BuffSelected > 3)
			{
				CurrentPlayer->BuffSelected = 0;
			}
		}
	}

	//Use the buff
	if(FoxInput_KeyTriggered('F'))
	{
		if(CurrentPlayer->BuffHeld[CurrentPlayer->BuffSelected])
		{
			int startingBuff;
			// Currently going with a 20% increase
			// Sets the timer for 10 seconds
			switch(CurrentPlayer->BuffSelected)
			{
			// Buffs the player's attack speed
			case Agility:
				CurrentPlayer->CurrentPlayerStats.AttackSpeed *= 1.2f;
				CurrentPlayer->CurrentPlayerStats.AgilityTimer = 10;
				break;
			// Buffs the player's damage
			case Strength:
				CurrentPlayer->CurrentPlayerStats.Damage = (int)(CurrentPlayer->CurrentPlayerStats.Damage * 1.2f);
				CurrentPlayer->CurrentPlayerStats.StrengthTimer = 10;
				break;
			// Buffs the player's damage reduction
			case Defense:
				CurrentPlayer->CurrentPlayerStats.DamageReduction *= 1.2f;
				CurrentPlayer->CurrentPlayerStats.DefenseTimer = 10;
				break;
			// Buffs the player's move speed
			case Haste:
				CurrentPlayer->CurrentPlayerStats.MoveSpeed *= 1.2f;
				CurrentPlayer->CurrentPlayerStats.HasteTimer = 10;
				break;
			}
			CurrentPlayer->BuffHeld[CurrentPlayer->BuffSelected] = FALSE;
			//Cycles automatically to the next available buff
			//The starting buff so we don't infini-loop
			startingBuff = CurrentPlayer->BuffSelected++;
			//Loop back around
			if(CurrentPlayer->BuffSelected > 3)
				CurrentPlayer->BuffSelected = 0;

			//Go to the next acquired buff, or stop looping
			while(!CurrentPlayer->BuffHeld[CurrentPlayer->BuffSelected] && startingBuff != CurrentPlayer->BuffSelected)
			{
				CurrentPlayer->BuffSelected++;
				if(CurrentPlayer->BuffSelected > 3)
				{
					CurrentPlayer->BuffSelected = 0;
				}
			}
		}
	}

	//Jump when space is pushed or drop down if S is pushed as well
	if(FoxInput_KeyTriggered(VK_SPACE))
	{
		Vec2 velocity;
		
		if(FoxInput_KeyDown('S') && CurrentPlayer->PlayerRigidBody.onGround)
		{
			CurrentPlayer->PlayerRigidBody.onGround = FALSE;
			CurrentPlayer->dropDown = TRUE;
			CurrentPlayer->dropdownTimer = 0.25f;
		}

		
		Vec2Set(&velocity, 0.0f, 1080.0f * GetLoadRatio());
		if(CurrentPlayer->Position.y <= GROUNDLEVEL * GetLoadRatio() || CurrentPlayer->PlayerRigidBody.onGround)
		{
			//if(CurrentPlayer->Position.y <= GROUNDLEVEL * GetLoadRatio())
			Vec2Set(&CurrentPlayer->Position, CurrentPlayer->Position.x, CurrentPlayer->Position.y + 300.0f * GetDeltaTime() * GetLoadRatio());
			CurrentPlayer->PlayerRigidBody.onGround = FALSE;
			ApplyVelocity(&CurrentPlayer->PlayerRigidBody, &velocity);
		}
	}
#if defined _DEBUG
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
#else
	CurrentPlayer->PlayerRigidBody.Acceleration.x = 0;
	CurrentPlayer->PlayerRigidBody.Acceleration.y = 0;
#endif
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

	//Update the buff timers
	UpdateBuffTimers(CurrentPlayer);

	//Brings the player back to the surface if something bad happens
	if(CurrentPlayer->Position.y < GROUNDLEVEL * GetLoadRatio())
	{
		CurrentPlayer->Position.y = GROUNDLEVEL * GetLoadRatio();
	}
	//Stop vertical velocity and acceleration when the player lands on the floor
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
		CurrentPlayer->dropdownTimer -= GetDeltaTime();
		//Once gravity takes control stop the drop down stuffs
		if(CurrentPlayer->dropdownTimer <= 0.0f)
		{
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
	CurrentPlayer->PlayerActive = FALSE;
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
	CurrentPlayerStats->MaxHealth = 50 + CurrentPlayerStats->Strength * 20;
	if(CurrentPlayer.PlayerWeapon)
		CurrentPlayerStats->MaxHealth += CurrentPlayer.PlayerWeapon->BonusStrength * 20;
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
	CurrentPlayerStats->MoveSpeed = CurrentPlayerStats->Agility * 15.0f + 600.0f;
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
	//Make local pointers to all the lists
	Platform* pList = platformList;
	Food* fList = foodList;
	Weapon* wList = weaponList;
	Enemy* eList = enemyList;
	Wall* walls = wallList;
	EnemySpawner* spawner = spawnerList;

	//Fake booleans for hit and hit previous frame
	int hit = 0;
	int hitPrev = 0;

	//Cycle through the platform list
	while(pList->objID != -1)
	{
		//If platform exists
		if(pList->objID > 0)
		{
			//Checks if there is collision this frame
			hit = CollisionRectangles(&CurrentPlayer.PlayerCollider, &pList->PlatformCollider);
			//Searches the hit array on the player if it collided last frame
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
	//Same thing with fud
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
	//Now with weapons
	while(wList->objID != -1)
	{
		if(wList->objID > 0 && wList->WeaponFOF == DroppedWeapon)
		{
			//Make the weapon fall here since we are already looking through the list, might as well
			WeaponOnTheRun(wList);

			//Continue with your collision venture
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
	//Enemy list for their pointy sticks
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
	//Don't wall me in plz
	while(walls->objID != -1)
	{
		if(walls->objID > 0)
		{
			hit = CollisionRectangles(&CurrentPlayer.PlayerCollider, &walls->WallCollider);
			hitPrev = searchHitArray(CurrentPlayer.CollisionData, COLLIDEAMOUNT, walls->WallCollider.collisionID);
			if(hit)
			{
				// New target, on start collision
				if(hitPrev < 0)
				{
					CurrentPlayer.CollisionData[-hitPrev] = walls->WallCollider.collisionID * 10 + 1;
					//printf("NOT FOUND: %i\n", -hitPrev);
					PlayerCollideWall(&CurrentPlayer, walls);
				}
				// Found target, hit previous frame, on persistant
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 1)
				{
					//printf("FOUND PERSISTANT: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					PlayerCollideWall(&CurrentPlayer, walls);
				}
				// Found target, did not hit previous frame, on start collision
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 0)
				{
					//printf("FOUND NEW COLLISION: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					CurrentPlayer.CollisionData[hitPrev] = walls->WallCollider.collisionID * 10 + 1;
					PlayerCollideWall(&CurrentPlayer, walls);
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
		walls++;
	}
	//Enemy Spawners
	while(spawner->objID != -1)
	{
		//If platform exists
		if(spawner->objID > 0)
		{
			//Checks if there is collision this frame
			hit = CollisionRectangles(&CurrentPlayer.PlayerCollider, &spawner->SpawnerCollider);
			//Searches the hit array on the player if it collided last frame
			hitPrev = searchHitArray(CurrentPlayer.CollisionData, COLLIDEAMOUNT, spawner->SpawnerCollider.collisionID);
			
			if(hit)
			{
				// New target, on start collision
				if(hitPrev < 0)
				{
					CurrentPlayer.CollisionData[-hitPrev] = spawner->SpawnerCollider.collisionID * 10 + 1;
					//printf("NOT FOUND: %i\n", -hitPrev);
					SpawnEnemies(spawner);
					
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
					CurrentPlayer.CollisionData[hitPrev] = spawner->SpawnerCollider.collisionID * 10 + 1;
					
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
		spawner++;
	}
}

/*************************************************************************/
/*!
	\brief
	Animates the players legs.

	\param Object
	The player to animate
*/
/*************************************************************************/
void Animation(Player *Object)
{
	float sinOfLegValue = (float)sin(Object->LegSinValue);
	float sinOfTwoLegValue = (float)sin(Object->LegSinValue*2);
	float LegDistance = ((Object->CurrentPlayerStats.MoveSpeed * GetDeltaTime() * GetLoadRatio()) + (2.3f * GetLoadRatio() / (((Object->Speed) * 0.15f + 0.15f * GetLoadRatio())) ))-(Object->Speed);
	float LegUpperDirection = sinOfLegValue/(LegDistance);
	float LegLowerDirection;
	float LegUpperDirection2 = sinOfLegValue/(LegDistance);
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
		if(Object->Princess == Mayple)
			Object->PlayerSpriteParts.Tail->AnimationSpeed = 2 * FRAMERATE / 60;
		else
			Object->PlayerSpriteParts.Tail->AnimationSpeed = 4 * FRAMERATE / 60;
	}

	if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL * GetLoadRatio())
	{
		if (LegUpperDirection < 0)
			LegLowerDirection = (sinOfLegValue/1.25f + sinOfLegValue * -0.1f)/(LegDistance);
		else
			LegLowerDirection = (LegUpperDirection + sinOfLegValue + sinOfLegValue * 0.4f)/(LegDistance);

		if (LegUpperDirection2 > 0)
			LegLowerDirection2 = (sinOfLegValue/1.25f + sinOfLegValue * -0.1f)/(LegDistance);
		else
			LegLowerDirection2 = (LegUpperDirection2 + sinOfLegValue + sinOfLegValue * 0.4f)/(LegDistance);
	}
	else
	{
		float sinLegOverTen = (float)sin(LegDistance/10);
		LegUpperDirection = sinLegOverTen - 1.0f;
		LegUpperDirection2 = sinLegOverTen - 1.0f;//60.0f * GetDeltaTime();
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
			LegUpr2->Position.x += sinOfLegValue*-8/(LegDistance) * GetLoadRatio();
		}
		LegUpr->Position.y = Object->Position.y + (sinOfTwoLegValue*5/(LegDistance)) * GetLoadRatio();
		LegLwr->Position.x = (float)cos(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.x;
		LegLwr->Position.y = (float)sin(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.y;
		LegLwr->Rotation = LegLowerDirection;
		
		LegUpr2->Rotation = -LegUpperDirection2;
		LegUpr2->Position.x = Object->Position.x;
		if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL * GetLoadRatio())
		{
			LegUpr2->Position.x += sinOfLegValue*8/(LegDistance) * GetLoadRatio();
		}
		LegUpr2->Position.y = Object->Position.y + (sinOfTwoLegValue*5/(LegDistance)) * GetLoadRatio();
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
		Weap->ZIndex = Object->Zindex - 1;
	}
	else
	{
		Tail->Position.x = Bdy->Position.x-(Bdy->Width/20);
		
		LegUpr->Rotation = -LegUpperDirection;
		LegUpr->Position.x = Object->Position.x;
		if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL * GetLoadRatio())
		{
			LegUpr2->Position.x += sinOfLegValue*-8/(LegDistance) * GetLoadRatio();
		}
		LegUpr->Position.y = Object->Position.y + (sinOfTwoLegValue*5/(LegDistance)) * GetLoadRatio();
		LegLwr->Position.x = (float)cos(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.x;
		LegLwr->Position.y = (float)sin(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.y;
		LegLwr->Rotation = -LegLowerDirection;
		
		LegUpr2->Rotation = LegUpperDirection2;
		LegUpr2->Position.x = Object->Position.x;
		if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL * GetLoadRatio())
		{
			LegUpr2->Position.x += sinOfLegValue*8/(LegDistance) * GetLoadRatio();
		}
		LegUpr2->Position.y = Object->Position.y + (sinOfTwoLegValue*5/(LegDistance)) * GetLoadRatio();
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
		Weap->ZIndex = Object->Zindex + 2;
	}

	Object->PlayerWeapon->WeaponAttackPosition.x = Weap->Position.x + (cosf(Weap->Rotation + FOX_PI / 2) * Object->PlayerWeapon->WeaponLength);
	Object->PlayerWeapon->WeaponAttackPosition.y = Weap->Position.y + (sinf(Weap->Rotation + FOX_PI / 2) * Object->PlayerWeapon->WeaponLength);

	//*************************************************************************************************
}

/*************************************************************************/
/*!
	\brief
	Creates the player's sprites

	\param Object
	The player to create
*/
/*************************************************************************/
void CreatePlayerSprites(Player *Object)
{
	//Different sprites for different princesses
	switch(Object->Princess)
	{
	case Mayple:
		Object->PlayerSpriteParts.ArmUpper2 = (Sprite *) CreateSprite("TextureFiles/ArmUpperMayple.png", 128.0f, 128.0f, Object->Zindex - 2, 1, 1, 0, 0);
		Object->PlayerSpriteParts.Skirt = (Sprite *) CreateSprite("TextureFiles/SkirtMayple.png", 300.0f, 300.0f, Object->Zindex + 1, 4, 1, 0, 0);
		Object->PlayerSpriteParts.Body = (Sprite *) CreateSprite("TextureFiles/BodyMayple.png", 300.0f, 300.0f, Object->Zindex, 4, 1, 0, 0);
		Object->PlayerSpriteParts.ArmUpper = (Sprite *) CreateSprite("TextureFiles/ArmUpperMayple.png", 128.0f, 128.0f, Object->Zindex + 2, 1, 1, 0, 0);
		break;
	case Ginko:
		Object->PlayerSpriteParts.ArmUpper2 = (Sprite *) CreateSprite("TextureFiles/ArmUpperGinko.png", 128.0f, 128.0f, Object->Zindex - 2, 1, 1, 0, 0);
		Object->PlayerSpriteParts.Skirt = (Sprite *) CreateSprite("TextureFiles/SkirtGinko.png", 300.0f, 300.0f, Object->Zindex + 1, 4, 1, 0, 0);
		Object->PlayerSpriteParts.Body = (Sprite *) CreateSprite("TextureFiles/BodyGinko.png", 300.0f, 300.0f, Object->Zindex, 4, 1, 0, 0);
		Object->PlayerSpriteParts.ArmUpper = (Sprite *) CreateSprite("TextureFiles/ArmUpperGinko.png", 128.0f, 128.0f, Object->Zindex + 2, 1, 1, 0, 0);
		break;
	case Holly:
		Object->PlayerSpriteParts.ArmUpper2 = (Sprite *) CreateSprite("TextureFiles/ArmUpperHolly.png", 128.0f, 128.0f, Object->Zindex - 2, 1, 1, 0, 0);
		Object->PlayerSpriteParts.Skirt = (Sprite *) CreateSprite("TextureFiles/SkirtHolly.png", 300.0f, 300.0f, Object->Zindex + 1, 4, 1, 0, 0);
		Object->PlayerSpriteParts.Body = (Sprite *) CreateSprite("TextureFiles/BodyHolly.png", 300.0f, 300.0f, Object->Zindex, 4, 1, 0, 0);
		Object->PlayerSpriteParts.ArmUpper = (Sprite *) CreateSprite("TextureFiles/ArmUpperHolly.png", 128.0f, 128.0f, Object->Zindex + 2, 1, 1, 0, 0);
		break;
	case Kaya:
		Object->PlayerSpriteParts.ArmUpper2 = (Sprite *) CreateSprite("TextureFiles/ArmUpperKaya.png", 128.0f, 128.0f, Object->Zindex - 2, 1, 1, 0, 0);
		Object->PlayerSpriteParts.Skirt = (Sprite *) CreateSprite("TextureFiles/SkirtKaya.png", 300.0f, 300.0f, Object->Zindex + 1, 4, 1, 0, 0);
		Object->PlayerSpriteParts.Body = (Sprite *) CreateSprite("TextureFiles/BodyKaya.png", 300.0f, 300.0f, Object->Zindex, 4, 1, 0, 0);
		Object->PlayerSpriteParts.ArmUpper = (Sprite *) CreateSprite("TextureFiles/ArmUpperKaya.png", 128.0f, 128.0f, Object->Zindex + 2, 1, 1, 0, 0);
		break;
	}

	Object->PlayerSpriteParts.ArmLower2 = (Sprite *) CreateSprite("TextureFiles/ArmLower.png", 128.0f, 128.0f, Object->Zindex - 2, 1, 1, 0, 0);

	Object->PlayerSpriteParts.LegUpper = (Sprite *) CreateSprite("TextureFiles/LegUpper.png", 128.0f, 128.0f, Object->Zindex, 1, 1, 0, 0);

	Object->PlayerSpriteParts.LegLower = (Sprite *) CreateSprite("TextureFiles/LegLower.png", 128.0f, 128.0f, Object->Zindex, 1, 1, 0, 0);

	Object->PlayerSpriteParts.LegUpper2 = (Sprite *) CreateSprite("TextureFiles/LegUpper.png", 128.0f, 128.0f, Object->Zindex, 1, 1, 0, 0);

	Object->PlayerSpriteParts.LegLower2 = (Sprite *) CreateSprite("TextureFiles/LegLower.png", 128.0f, 128.0f, Object->Zindex, 1, 1, 0, 0);

	Object->PlayerSpriteParts.Skirt->AnimationActive = 0;

	Object->PlayerSpriteParts.Body->AnimationSpeed = 3;

	Object->PlayerSpriteParts.BlinkTimer = 0;
	
	Object->PlayerSpriteParts.Tail = (Sprite *) CreateSprite("TextureFiles/TailRun.png", 300.0f, 300.0f, Object->Zindex, 7, 2, 0, 0);
	Object->PlayerSpriteParts.Tail->SpriteTexture = LoadTexture("TextureFiles/TailIdle.png");

	Object->PlayerSpriteParts.Tail->AnimationSpeed = (Object->Speed * GetLoadRatio())/2 + 3;

	Object->TailSinValue = 0;

	Object->PlayerSpriteParts.ArmLower = (Sprite *) CreateSprite("TextureFiles/ArmLower.png", 128.0f, 128.0f, Object->Zindex + 3, 1, 1, 0, 0);
}

/*************************************************************************/
/*!
	\brief
	Rotates something to a certain angle

	\param angle
	Current angle

	\param angleTo
	Angle to rotate to

	\param speed
	Speed of the rotation

	\return
	The new resulting angle
*/
/*************************************************************************/
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
	Saves the player from peril
	
	\param CurrentPlayer
	A pointer to the player to be saved
*/
/*************************************************************************/
void SavePlayer(Player *CurrentPlayer)
{
	FILE *fp;
	// Malloc me a long string
	char* string = (char *)MallocMyAlloc(500, 1);

	int BuffValue = 0;
	if(CurrentPlayer->BuffHeld[0])
		BuffValue = BuffValue | 0x1;
	if(CurrentPlayer->BuffHeld[1])
		BuffValue = BuffValue | 0x2;
	if(CurrentPlayer->BuffHeld[2])
		BuffValue = BuffValue | 0x4;
	if(CurrentPlayer->BuffHeld[3])
		BuffValue = BuffValue | 0x8;

	// Ugly code that puts all needed info into one string
	sprintf(string, "Level: %d\nArmUnlock: %d\nHandUnlock: %d\nArmClear: %d\nHandClear: %d\nPrincess: %d\nBuffHeld: %d\nAgility: %d\nStrength: %d\nDefense: %d\nMoney: %d\nCurrentHealth: %d\nWeaponRarity: %d\nWeaponType: %d\nWeaponAgility: %d\nWeaponStrength: %d\nWeaponDefense: %d\n%s",
		CurrentPlayer->CurrentLevel, CurrentPlayer->armUnlock, CurrentPlayer->handUnlock, CurrentPlayer->armClear, CurrentPlayer->handClear, CurrentPlayer->Princess, BuffValue, CurrentPlayer->CurrentPlayerStats.Agility, CurrentPlayer->CurrentPlayerStats.Strength, CurrentPlayer->CurrentPlayerStats.Defense, 
		CurrentPlayer->CurrentPlayerStats.Money, CurrentPlayer->CurrentPlayerStats.CurrentHealth, CurrentPlayer->PlayerWeapon->WeaponRarity, CurrentPlayer->PlayerWeapon->WeaponType,
		CurrentPlayer->PlayerWeapon->BonusAgility, CurrentPlayer->PlayerWeapon->BonusStrength, CurrentPlayer->PlayerWeapon->BonusDefense, CurrentPlayer->PlayerWeapon->WeaponName);
	
	//Opens the file for writing
	fp = fopen(GameData, "wt");
	if(fp)
	{
		//Writes the ugly string to the file
		int num = 0;
		num = fprintf(fp, "%s", string);
		fclose(fp);
	}

	// Memory angel save me!
	FreeMyAlloc(string);
}

/*************************************************************************/
/*!
	\brief
	Loads the player
	
	\param CurrentPlayer
	A pointer to the player to be loaded

	\return
	An int to indicate success or other errors
*/
/*************************************************************************/
int LoadPlayer(Player *CurrentPlayer)
{
	FILE *fp;
	int BuffValue;
	// Opens a file
	fp = fopen(GameData, "rt");
	if(fp)
	{
		//Ugly code which should read the file if its in the correct format
		int num = 0;
		num = fscanf(fp, "%*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %[^\n]",
			&CurrentPlayer->CurrentLevel, &CurrentPlayer->armUnlock, &CurrentPlayer->handUnlock, &CurrentPlayer->armClear, &CurrentPlayer->handClear, &CurrentPlayer->Princess, &BuffValue, &CurrentPlayer->CurrentPlayerStats.Agility, &CurrentPlayer->CurrentPlayerStats.Strength, &CurrentPlayer->CurrentPlayerStats.Defense, 
			&CurrentPlayer->CurrentPlayerStats.Money, &CurrentPlayer->CurrentPlayerStats.CurrentHealth, &CurrentPlayer->PlayerWeapon->WeaponRarity, &CurrentPlayer->PlayerWeapon->WeaponType,
			&CurrentPlayer->PlayerWeapon->BonusAgility, &CurrentPlayer->PlayerWeapon->BonusStrength, &CurrentPlayer->PlayerWeapon->BonusDefense, CurrentPlayer->PlayerWeapon->WeaponName);
		fclose(fp);

		//If all the data was read successfully
		if(num == 18)
		{
			//Update all the other required player data
			int nameLen, statsLen;
			updateAttackSpeed(&CurrentPlayer->CurrentPlayerStats);
			updateMoveSpeed(&CurrentPlayer->CurrentPlayerStats);
			updateDamage(CurrentPlayer);
			updateDamageReduction(&CurrentPlayer->CurrentPlayerStats);
			updateMaxHealth(&CurrentPlayer->CurrentPlayerStats);
			
			if(BuffValue & 0x1)
				CurrentPlayer->BuffHeld[0] = TRUE;
			if(BuffValue & 0x2)
				CurrentPlayer->BuffHeld[1] = TRUE;
			if(BuffValue & 0x4)
				CurrentPlayer->BuffHeld[2] = TRUE;
			if(BuffValue & 0x8)
				CurrentPlayer->BuffHeld[3] = TRUE;

			//Update those text strings
			ChangeTextString(CurrentPlayer->PlayerWeapon->WeaponGlyphs, CurrentPlayer->PlayerWeapon->WeaponName);
			CreateStatsString(CurrentPlayer->PlayerWeapon->WeaponStatsString, CurrentPlayer->PlayerWeapon->BonusStrength, CurrentPlayer->PlayerWeapon->BonusAgility, CurrentPlayer->PlayerWeapon->BonusDefense);
			ChangeTextString(CurrentPlayer->PlayerWeapon->WeaponStatsGlyphs, CurrentPlayer->PlayerWeapon->WeaponStatsString);
			//Updates the weapon sprite for the correct weapon
			switch(CurrentPlayer->PlayerWeapon->WeaponType)
			{
			case Sword:
				FreeSprite(CurrentPlayer->PlayerWeapon->WeaponSprite);
				CurrentPlayer->PlayerWeapon->WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Sword.png", 256, 256, 5, 1, 1, 0, 0);
				break;
			case Axe:
				FreeSprite(CurrentPlayer->PlayerWeapon->WeaponSprite);
				CurrentPlayer->PlayerWeapon->WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Axe.png", 256, 256, 5, 1, 1, 0, 0);
				break;
			case Hammer:
				FreeSprite(CurrentPlayer->PlayerWeapon->WeaponSprite);
				CurrentPlayer->PlayerWeapon->WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Hammer.png", 256, 256, 5, 1, 1, 0, 0);
				break;
			case Spear:
				FreeSprite(CurrentPlayer->PlayerWeapon->WeaponSprite);
				CurrentPlayer->PlayerWeapon->WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Spear.png", 256, 256, 5, 1, 1, 0, 0);
				break;
			default:
				FreeSprite(CurrentPlayer->PlayerWeapon->WeaponSprite);
				CurrentPlayer->PlayerWeapon->WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Sword.png", 256, 256, 5, 1, 1, 0, 0);
				break;
			}
			//Updates weapon hover text
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

			//Success!
			return 1;
		}
		else
			return -1;	// You done goofed
	}
	// New player ^_^
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

	\param Princess
	Enum of the princess to create
*/
/*************************************************************************/
void LoadNewPlayer(Player *CurrentPlayer, enum Character Princess)
{
	//Sets all required player stats and data
	int nameLen, statsLen;
	CurrentPlayer->Princess = Princess;
	CurrentPlayer->BuffHeld[0] = FALSE;
	CurrentPlayer->BuffHeld[1] = FALSE;
	CurrentPlayer->BuffHeld[2] = FALSE;
	CurrentPlayer->BuffHeld[3] = FALSE;

	CurrentPlayer->CurrentPlayerStats.Agility = 0;
	CurrentPlayer->CurrentPlayerStats.Strength = 0;
	CurrentPlayer->CurrentPlayerStats.Defense = 0;

	CurrentPlayer->armUnlock = FALSE;
	CurrentPlayer->handUnlock = FALSE;
	CurrentPlayer->armClear = FALSE;
	CurrentPlayer->handClear = FALSE;

	//Princess specific data to set
	switch(Princess)
	{
	case Mayple:
		FreeSprite(CurrentPlayer->PlayerWeapon->WeaponSprite);
		CurrentPlayer->PlayerWeapon->WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Sword.png", 256, 256, 5, 1, 1, 0, 0);
		CurrentPlayer->PlayerWeapon->WeaponName = strcpy(CurrentPlayer->PlayerWeapon->WeaponName, "Mayples Toy Sword");
		CurrentPlayer->PlayerWeapon->WeaponType = Sword;
		break;
	case Ginko:
		FreeSprite(CurrentPlayer->PlayerWeapon->WeaponSprite);
		CurrentPlayer->PlayerWeapon->WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Axe.png", 256, 256, 5, 1, 1, 0, 0);
		CurrentPlayer->PlayerWeapon->WeaponName = strcpy(CurrentPlayer->PlayerWeapon->WeaponName, "Ginkos Toy Axe");
		CurrentPlayer->PlayerWeapon->WeaponType = Axe;
		break;
	case Kaya:
		FreeSprite(CurrentPlayer->PlayerWeapon->WeaponSprite);
		CurrentPlayer->PlayerWeapon->WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Hammer.png", 256, 256, 5, 1, 1, 0, 0);
		CurrentPlayer->PlayerWeapon->WeaponName = strcpy(CurrentPlayer->PlayerWeapon->WeaponName, "Kayas Toy Hammer");
		CurrentPlayer->PlayerWeapon->WeaponType = Hammer;
		break;
	case Holly:
		FreeSprite(CurrentPlayer->PlayerWeapon->WeaponSprite);
		CurrentPlayer->PlayerWeapon->WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Spear.png", 256, 256, 5, 1, 1, 0, 0);
		CurrentPlayer->PlayerWeapon->WeaponName = strcpy(CurrentPlayer->PlayerWeapon->WeaponName, "Hollys Toy Spear");
		CurrentPlayer->PlayerWeapon->WeaponType = Spear;
		break;
	default:
		FreeSprite(CurrentPlayer->PlayerWeapon->WeaponSprite);
		CurrentPlayer->PlayerWeapon->WeaponSprite = (Sprite *) CreateSprite("TextureFiles/Stick.png", 256, 256, 5, 1, 1, 0, 0);
		CurrentPlayer->PlayerWeapon->WeaponName = strcpy(CurrentPlayer->PlayerWeapon->WeaponName, "Fragile Stick");
		break;
	}

	//Update those strings
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

	updateAttackSpeed(&CurrentPlayer->CurrentPlayerStats);
	updateMoveSpeed(&CurrentPlayer->CurrentPlayerStats);
	updateDamage(CurrentPlayer);
	updateDamageReduction(&CurrentPlayer->CurrentPlayerStats);
	updateMaxHealth(&CurrentPlayer->CurrentPlayerStats);

	CurrentPlayer->CurrentPlayerStats.Money = 15;
	CurrentPlayer->CurrentPlayerStats.CurrentHealth = CurrentPlayer->CurrentPlayerStats.MaxHealth;
	CurrentPlayer->CurrentLevel = GS_Tutorial;
}

void UpdateBuffTimers(Player* CurrentPlayer)
{
	//Update buff timers
	if(CurrentPlayer->CurrentPlayerStats.AgilityTimer > 0)
	{
		CurrentPlayer->CurrentPlayerStats.AgilityTimer -= GetDeltaTime();
	}
	else
	{
		updateAttackSpeed(&CurrentPlayer->CurrentPlayerStats);
	}
	//Update buff timers
	if(CurrentPlayer->CurrentPlayerStats.StrengthTimer > 0)
	{
		CurrentPlayer->CurrentPlayerStats.StrengthTimer -= GetDeltaTime();
	}
	else
	{
		updateDamage(CurrentPlayer);
	}
	//Update buff timers
	if(CurrentPlayer->CurrentPlayerStats.DefenseTimer > 0)
	{
		CurrentPlayer->CurrentPlayerStats.DefenseTimer -= GetDeltaTime();
	}
	else
	{
		updateDamageReduction(&CurrentPlayer->CurrentPlayerStats);
	}
	//Update buff timers
	if(CurrentPlayer->CurrentPlayerStats.HasteTimer > 0)
	{
		CurrentPlayer->CurrentPlayerStats.HasteTimer -= GetDeltaTime();
	}
	else
	{
		updateMoveSpeed(&CurrentPlayer->CurrentPlayerStats);
	}
}
