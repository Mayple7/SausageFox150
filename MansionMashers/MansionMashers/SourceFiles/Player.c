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

#define PANELSIZE 1920.0f

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
	CurrentPlayer->Position.x = xPos;
	CurrentPlayer->Position.y = yPos;
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
	CurrentPlayer->PlayerCollider.Offset.y = 20;
	CurrentPlayer->PlayerCollider.width = CurrentPlayer->PlayerCollider.width - 20;
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
	CurrentPlayer->CurrentPlayerSounds.Swing1 = CreateSound("Sounds/SwordSwing1.wav", SmallSnd);
	CurrentPlayer->CurrentPlayerSounds.Swing2 = CreateSound("Sounds/SwordSwing2.wav", SmallSnd);
	CurrentPlayer->CurrentPlayerSounds.GetHit1 = CreateSound("Sounds/SwordHit1.wav", SmallSnd);
	CurrentPlayer->CurrentPlayerSounds.GetHit2 = CreateSound("Sounds/SwordHit2.wav", SmallSnd);


	/*////////////////////////////////
	//       PLAYER SPRITE          //
	////////////////////////////////*/
	CurrentPlayer->Zindex = 100;
	CreatePlayerSprites(CurrentPlayer);
	CurrentPlayer->PlayerSpriteParts.Weapon->ZIndex = CurrentPlayer->Zindex + 2;
	Animation(CurrentPlayer);

	InitializePlayerRank(CurrentPlayer);
	if (GetCurrentState() != GS_CharacterSelect && GetCurrentState() != GS_MapLevel)
		InitializePlayerHurt(CurrentPlayer);
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
	int mouseX = FoxInput_GetMousePositionX();
	float camX = GetCameraXPosition();

	UpdateCollisionPosition(&CurrentPlayer->PlayerWeapon->WeaponAttack, &CurrentPlayer->PlayerWeapon->WeaponAttackPosition);

	if ((FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT) || FoxInput_KeyTriggered('N')) && !CurrentPlayer->isAttacking)
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
		CurrentPlayer->AttackType  = 0;
		CurrentPlayer->PlayerSpriteParts.AttackRotation = 0;
		CurrentPlayer->PlayerSpriteParts.AttackRotationArm = 0;
		CurrentPlayer->PlayerSpriteParts.AttackRotationArmLower = 0;
		UpdateCollider(&CurrentPlayer->PlayerCollider,CurrentPlayer->PlayerCollider.width, CurrentPlayer->PlayerCollider.height);
	}
	else if ((FoxInput_MouseTriggered(MOUSE_BUTTON_RIGHT) || FoxInput_KeyTriggered('M')) && !CurrentPlayer->isAttacking)
	{
		//Pick a random shoot sound to play
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
		CurrentPlayer->AttackType  = 1;
		CurrentPlayer->PlayerSpriteParts.AttackRotation = 0;
		CurrentPlayer->PlayerSpriteParts.AttackRotationArm = 0;
		CurrentPlayer->PlayerSpriteParts.AttackRotationArmLower = 0;
		UpdateCollider(&CurrentPlayer->PlayerCollider,CurrentPlayer->PlayerCollider.width, CurrentPlayer->PlayerCollider.height);

		//Wind of the weapons
		{
			Projectile *theWindOfAFox;
			float projectileSpeed = 1400;
			if (!CurrentPlayer->PlayerSpriteParts.Weapon->FlipX)
				projectileSpeed *= -1;

			//NASTY NASTY HACKKKK
			theWindOfAFox = CreateProjectile("TextureFiles/Wind.png", 
										     CurrentPlayer->PlayerSpriteParts.Weapon->Width / 2, CurrentPlayer->PlayerSpriteParts.Weapon->Height / 2, 
											 CurrentPlayer->PlayerSpriteParts.Body->Position.x, CurrentPlayer->PlayerSpriteParts.Body->Position.y + 30, 
										     Arrow, WeaponFriendly, 80000 + (int)CurrentPlayer->LegSinValue, (int)(CurrentPlayer->CurrentPlayerStats.Damage / 2), projectileSpeed, 0);

			theWindOfAFox->ProjectileFOF = PlayerWeapon;

			if (!theWindOfAFox->ProjectileSprite->FlipX)
			{
				theWindOfAFox->ProjectileAttack.Offset.x = theWindOfAFox->ProjectileAttack.width / 3;
				theWindOfAFox->Position.x += 50;
			}
			else
			{
				theWindOfAFox->ProjectileAttack.Offset.x = -theWindOfAFox->ProjectileAttack.width / 3;
				theWindOfAFox->Position.x -= 50;
			}
			UpdateCollider(&theWindOfAFox->ProjectileAttack, theWindOfAFox->ProjectileAttack.width / 4, theWindOfAFox->ProjectileAttack.height / 2);
		}
	}

	if ((FoxInput_MouseDown(MOUSE_BUTTON_RIGHT) || FoxInput_KeyDown('M')) && !CurrentPlayer->isAttacking)
	{
		CurrentPlayer->isBlocking = TRUE;
	}
	else if(FoxInput_MouseUp(MOUSE_BUTTON_RIGHT) || FoxInput_KeyUp('M') || CurrentPlayer->isAttacking)
	{
		CurrentPlayer->isBlocking = FALSE;
	}

	if (LookAtMouse)
	{
		if (camX + (mouseX - PANELSIZE / 2) < CurrentPlayer->Position.x - 20)
			CurrentPlayer->FlipX = FALSE;
		else if (camX + (mouseX - PANELSIZE / 2) > CurrentPlayer->Position.x + 20)
			CurrentPlayer->FlipX = TRUE;
	}

	// Move left if A is pressed
	if(FoxInput_KeyDown('A'))
	{
		CurrentPlayer->PlayerDirection = LEFT;
		CurrentPlayer->Speed = CurrentPlayer->CurrentPlayerStats.MoveSpeed * GetDeltaTime();
		if (!LookAtMouse)
			CurrentPlayer->FlipX = FALSE;
	}
	// Move right if D is pressed
	else if(FoxInput_KeyDown('D'))
	{
		CurrentPlayer->PlayerDirection = RIGHT;
		CurrentPlayer->Speed = CurrentPlayer->CurrentPlayerStats.MoveSpeed * GetDeltaTime();
		if (!LookAtMouse)
			CurrentPlayer->FlipX = TRUE;
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

	// Drop down when S is pushed
	if(FoxInput_KeyTriggered('S') && CurrentPlayer->PlayerRigidBody.onGround)
	{
		CurrentPlayer->PlayerRigidBody.onGround = FALSE;
		CurrentPlayer->dropDown = TRUE;
		CurrentPlayer->dropdownTimer = 0.25f;
	}

	//Jump when space is pushed or drop down if S is pushed as well
	if(FoxInput_KeyTriggered(VK_SPACE))
	{
		Vec2 velocity;
		
		Vec2Set(&velocity, 0.0f, 1080.0f);
		if(CurrentPlayer->Position.y <= GROUNDLEVEL || CurrentPlayer->PlayerRigidBody.onGround)
		{
			//if(CurrentPlayer->Position.y <= GROUNDLEVEL)
			Vec2Set(&CurrentPlayer->Position, CurrentPlayer->Position.x, CurrentPlayer->Position.y + 300.0f * GetDeltaTime());
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
		Vec2Set(&force, 0.0f, -FOX_GRAVITY_Y);
		if(CurrentPlayer->Position.y > GROUNDLEVEL)
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

	//FOR TESTING ONLY
	if(FoxInput_KeyTriggered('L')) //YOU CAN REALLY MAKE STUFF EXPLODE WITH THIS
	{
		if (animationTest > 0.5f)
			animationTest -= 0.05f;
	}
	if(FoxInput_KeyTriggered('K')) //YOU CAN REALLY MAKE STUFF EXPLODE WITH THIS
	{
		if (animationTest < 1.5f)
			animationTest += 0.05f;
	}
#else
	CurrentPlayer->PlayerRigidBody.Acceleration.x = 0;
	CurrentPlayer->PlayerRigidBody.Acceleration.y = 0;
#endif

	// Update Speed and such if blocking
	if(CurrentPlayer->isBlocking)
	{
		CurrentPlayer->Speed /= 2;
	}

	Animation(CurrentPlayer);
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

	//Update the player XP and Rank
	UpdatePlayerRank(CurrentPlayer);

	//Update the player hurt overlay
	if (GetCurrentState() != GS_CharacterSelect && GetCurrentState() != GS_MapLevel)
		UpdatePlayerHurt(CurrentPlayer);

	//Brings the player back to the surface if something bad happens
	if(CurrentPlayer->Position.y < GROUNDLEVEL)
	{
		CurrentPlayer->Position.y = GROUNDLEVEL;
	}
	//Stop vertical velocity and acceleration when the player lands on the floor
	if(CurrentPlayer->Position.y <= GROUNDLEVEL || CurrentPlayer->PlayerRigidBody.onGround)
	{
		Vec2Zero(&CurrentPlayer->PlayerRigidBody.Acceleration);
		Vec2Zero(&CurrentPlayer->PlayerRigidBody.Velocity);
		ZeroGravity(&CurrentPlayer->PlayerRigidBody);
	}
	//Set gravity if not on floor or on a platform
	else
	{
		SetGravity(&CurrentPlayer->PlayerRigidBody, 0.0f, FOX_GRAVITY_Y);
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
	// Max health formula
	CurrentPlayerStats->MaxHealth = 100 + CurrentPlayerStats->Defense * 10;
	if(CurrentPlayer.PlayerWeapon)
		CurrentPlayerStats->MaxHealth += CurrentPlayer.PlayerWeapon->BonusDefense * 10;
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
	// Move speed formula
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
	// Attack speed formula
	CurrentPlayerStats->AttackSpeed = CurrentPlayerStats->Agility * 2.0f + 8.0f;
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
	// Damage reduction formula
	CurrentPlayerStats->DamageReduction = 1.0f - (1.0f / CurrentPlayerStats->Defense) / 2.0f;
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
	CurrentPlayer->CurrentPlayerStats.Damage = 10 + (CurrentPlayer->CurrentPlayerStats.Strength + CurrentPlayer->PlayerWeapon->BonusStrength) * 2;
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
	Platform *pList = platformList;
	Food *fList = foodList;
	Weapon *wList = weaponList;
	Enemy *eList = enemyList;
	Projectile *bList = projectileList; //bList as in Ballista, I didn't even think of that before hand, what a brilliant coincident
	Wall *walls = wallList;
	EnemySpawner *spawner = spawnerList;

	//Fake booleans for hit and hit previous frame
	int hit = 0;
	int hitPrev = 0;

	// Used to only display one weapon
	float closestDropLength = 100000;
	Weapon *displayWeapon = NULL;

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
				float currentDist = Vec2SquareDistance(&CurrentPlayer.Position, &wList->WeaponPickup.Position);

				TextAllNotVisible(wList->WeaponGlyphs);
				TextAllNotVisible(wList->WeaponStatsGlyphs);
				wList->WeaponHoverBackground->Visible = FALSE;

				if(currentDist < closestDropLength)
				{
					closestDropLength = currentDist;
					displayWeapon = wList;
				}

				// New target, on start collision
				if(hitPrev < 0)
				{
					CurrentPlayer.CollisionData[-hitPrev] = wList->WeaponPickup.collisionID * 10 + 1;
					//printf("NOT FOUND: %i\n", -hitPrev);

					//PlayerCollideWeaponDrop(&CurrentPlayer, wList);
					updateDamage(&CurrentPlayer);
				}
				// Found target, hit previous frame, on persistant
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 1)
				{
					//printf("FOUND PERSISTANT: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					//PlayerCollideWeaponDrop(&CurrentPlayer, wList);
					updateDamage(&CurrentPlayer);
				}
				// Found target, did not hit previous frame, on start collision
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 0)
				{
					//printf("FOUND NEW COLLISION: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					CurrentPlayer.CollisionData[hitPrev] = wList->WeaponPickup.collisionID * 10 + 1;
					//PlayerCollideWeaponDrop(&CurrentPlayer, wList);
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
						TextAllNotVisible(wList->WeaponGlyphs);
						TextAllNotVisible(wList->WeaponStatsGlyphs);
						wList->WeaponHoverBackground->Visible = FALSE;
					}
				}
			}
		}
		wList++;
	}
	// Display the weapon info and handle that collision
	if(displayWeapon)
	{
		PlayerCollideWeaponDrop(&CurrentPlayer, displayWeapon);
	}

	//Projectiles ('b' as in Ballista)
	while(bList->objID != -1)
	{
		// If the weapon is the enemy's
		if(bList->objID > 0 && bList->ProjectileFOF == EnemyWeapon)
		{
			hit = CollisionRectangles(&CurrentPlayer.PlayerCollider, &bList->ProjectileAttack);
			hitPrev = searchHitArray(CurrentPlayer.CollisionData, COLLIDEAMOUNT, bList->ProjectileAttack.collisionID);
			if(hit)
			{
				// New target, on start collision
				if(hitPrev < 0)
				{
					//Damage the player
					PlayerCollideEnemyProjectile(&CurrentPlayer, bList);
					PoofProjectile(bList);
				}
				// Found target, hit previous frame, on persistant
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 1)
				{
				}
				// Found target, did not hit previous frame, on start collision
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 0)
				{
					PlayerCollideEnemyProjectile(&CurrentPlayer, bList);
					PoofProjectile(bList);
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
				}
			}
			if(hitPrev > 0 && bList->ProjectileFOF == EnemyWeapon)
			{
				CurrentPlayer.CollisionData[hitPrev] = 0;
			}

		}
		bList++;
	}
	//Enemy list for their pointy sticks
	while(eList->objID != -1)
	{
		// If the weapon is the enemy's
		if(eList->objID > 0 && eList->EnemyType != Dummy && eList->EnemyWeapon->WeaponFOF == EnemyWeapon)
		{
			hit = CollisionRectangles(&CurrentPlayer.PlayerCollider, &eList->EnemyWeapon->WeaponAttack);
			hitPrev = searchHitArray(CurrentPlayer.CollisionData, COLLIDEAMOUNT, eList->EnemyWeapon->WeaponAttack.collisionID);
			if(hit && eList->isAttacking && (eList->EnemySpriteParts.AttackRotationArmLower > FOX_PI / 3 || eList->EnemySpriteParts.AttackRotationArmLower < -FOX_PI / 3))
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

	Object->PlayerSpriteParts.Tail->AnimationSpeed = (Object->Speed)/2 + 3;

	Object->TailSinValue = 0;

	Object->PlayerSpriteParts.ArmLower = (Sprite *) CreateSprite("TextureFiles/ArmLower.png", 128.0f, 128.0f, Object->Zindex + 3, 1, 1, 0, 0);

	Object->PlayerSpriteParts.LegUpper->FlipX = TRUE;
	Object->PlayerSpriteParts.LegLower->FlipX = TRUE;
	Object->PlayerSpriteParts.LegUpper2->FlipX = TRUE;
	Object->PlayerSpriteParts.LegLower2->FlipX = TRUE;
	Object->PlayerSpriteParts.Body->FlipX = TRUE;
	Object->PlayerSpriteParts.Skirt->FlipX = TRUE;
	Object->PlayerSpriteParts.Tail->FlipX = TRUE;
	Object->PlayerSpriteParts.ArmUpper->FlipX = TRUE;
	Object->PlayerSpriteParts.ArmLower->FlipX = TRUE;
	Object->PlayerSpriteParts.ArmUpper2->FlipX = TRUE;
	Object->PlayerSpriteParts.ArmLower2->FlipX = TRUE;
	Object->PlayerSpriteParts.Weapon->FlipX = TRUE;
	Object->FlipX = TRUE;
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
	sprintf(string, "Level: %d\nLevelBitFlags: %d\nRank: %d\nXP: %d\nArmUnlock: %d\nHandUnlock: %d\nArmClear: %d\nHandClear: %d\nPrincess: %d\nBuffHeld: %d\nAgility: %d\nStrength: %d\nDefense: %d\nMoney: %d\nCurrentHealth: %d\nWeaponRarity: %d\nWeaponType: %d\nWeaponAgility: %d\nWeaponStrength: %d\nWeaponDefense: %d\n%s",
		CurrentPlayer->CurrentLevel, CurrentPlayer->levelClearBitFlags, CurrentPlayer->CurrentPlayerStats.Rank, CurrentPlayer->CurrentPlayerStats.Experience, CurrentPlayer->armUnlock, CurrentPlayer->handUnlock, CurrentPlayer->armClear, CurrentPlayer->handClear, CurrentPlayer->Princess, BuffValue, CurrentPlayer->CurrentPlayerStats.Agility, CurrentPlayer->CurrentPlayerStats.Strength, CurrentPlayer->CurrentPlayerStats.Defense, 
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
		num = fscanf(fp, "%*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %[^\n]",
			&CurrentPlayer->CurrentLevel, &CurrentPlayer->levelClearBitFlags, &CurrentPlayer->CurrentPlayerStats.Rank, &CurrentPlayer->CurrentPlayerStats.Experience, &CurrentPlayer->armUnlock, &CurrentPlayer->handUnlock, &CurrentPlayer->armClear, &CurrentPlayer->handClear, &CurrentPlayer->Princess, &BuffValue, &CurrentPlayer->CurrentPlayerStats.Agility, &CurrentPlayer->CurrentPlayerStats.Strength, &CurrentPlayer->CurrentPlayerStats.Defense, 
			&CurrentPlayer->CurrentPlayerStats.Money, &CurrentPlayer->CurrentPlayerStats.CurrentHealth, &CurrentPlayer->PlayerWeapon->WeaponRarity, &CurrentPlayer->PlayerWeapon->WeaponType,
			&CurrentPlayer->PlayerWeapon->BonusAgility, &CurrentPlayer->PlayerWeapon->BonusStrength, &CurrentPlayer->PlayerWeapon->BonusDefense, CurrentPlayer->PlayerWeapon->WeaponName);
		fclose(fp);

		//If all the data was read successfully
		if(num == 21)
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
				CurrentPlayer->PlayerWeapon->WeaponHoverBackground = (Sprite *) CreateSprite("TextureFiles/WeaponHoverBackground.png", nameLen * 25.0f, 120, 10, 1, 1, CurrentPlayer->PlayerWeapon->WeaponPickup.Position.x, (CurrentPlayer->PlayerWeapon->WeaponPickup.Position.y + CurrentPlayer->PlayerWeapon->WeaponPickup.height * 1.5f));
			}
			else
			{
				CurrentPlayer->PlayerWeapon->WeaponHoverBackground = (Sprite *) CreateSprite("TextureFiles/WeaponHoverBackground.png", statsLen * 25.0f, 120, 10, 1, 1, CurrentPlayer->PlayerWeapon->WeaponPickup.Position.x, (CurrentPlayer->PlayerWeapon->WeaponPickup.Position.y + CurrentPlayer->PlayerWeapon->WeaponPickup.height * 1.5f));
			}
			CurrentPlayer->PlayerWeapon->WeaponHoverBackground->Visible = FALSE;

			if (!strcmp(CurrentPlayer->PlayerWeapon->WeaponName,"Sausage Sausage of sausage"))
				CurrentPlayer->PlayerWeapon->WeaponSprite->SpriteTexture = LoadTexture("TextureFiles/BattleAxe.png");

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

	CurrentPlayer->CurrentPlayerStats.Agility = 1;
	CurrentPlayer->CurrentPlayerStats.Strength = 1;
	CurrentPlayer->CurrentPlayerStats.Defense = 1;

	CurrentPlayer->levelClearBitFlags = 0;
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
		CurrentPlayer->PlayerWeapon->WeaponHoverBackground = (Sprite *) CreateSprite("TextureFiles/WeaponHoverBackground.png", nameLen * 25.0f, 120, 10, 1, 1, CurrentPlayer->PlayerWeapon->WeaponPickup.Position.x, (CurrentPlayer->PlayerWeapon->WeaponPickup.Position.y + CurrentPlayer->PlayerWeapon->WeaponPickup.height * 1.5f));
	}
	else
	{
		CurrentPlayer->PlayerWeapon->WeaponHoverBackground = (Sprite *) CreateSprite("TextureFiles/WeaponHoverBackground.png", statsLen * 25.0f, 120, 10, 1, 1, CurrentPlayer->PlayerWeapon->WeaponPickup.Position.x, (CurrentPlayer->PlayerWeapon->WeaponPickup.Position.y + CurrentPlayer->PlayerWeapon->WeaponPickup.height * 1.5f));
	}
	CurrentPlayer->PlayerWeapon->WeaponHoverBackground->Visible = FALSE;

	updateAttackSpeed(&CurrentPlayer->CurrentPlayerStats);
	updateMoveSpeed(&CurrentPlayer->CurrentPlayerStats);
	updateDamage(CurrentPlayer);
	updateDamageReduction(&CurrentPlayer->CurrentPlayerStats);
	updateMaxHealth(&CurrentPlayer->CurrentPlayerStats);

	CurrentPlayer->CurrentPlayerStats.Rank = 1;
	CurrentPlayer->CurrentPlayerStats.Experience = 0;

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
