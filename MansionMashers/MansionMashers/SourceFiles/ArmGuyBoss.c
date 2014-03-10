/*****************************************************************************/
/*!
\file				Boss.c
\author				Dan Muller (d.muller)
\date				Mar 4, 2014

\brief				Boss components and code

\par				Functions:
\li					LoadBoss
\li					CreateBoss
\li					UpdateBoss
\li					DetectBossCollision
\li					RotateToAngle
  
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

// ---------------------------------------------------------------------------
// globals
static int LogicTimer = 0;
static int playerHit = FALSE;

static enum ArmGuyState { Cooldown, Jab, Smash, Spin };
static enum InnerState {Start, Attack, End, SpinR, SpinL };

/*************************************************************************/
/*!
	\brief
	Handles the collision between an enemy and the attacking weapon
	
	\param CurrentEnemy
	Pointer to the enemy object
*/
/*************************************************************************/
void LoadArmGuyBoss(void)
{
	LoadTexture("TextureFiles/TempArmGuy.png");
	LoadTexture("TextureFiles/TempArmGuyArm.png");
}

/*************************************************************************/
/*!
	\brief
	Handles the collision between an enemy and the attacking weapon
	
	\param CurrentEnemy
	Pointer to the enemy object
*/
/*************************************************************************/
ArmGuyBoss* CreateArmGuyBoss(float xPos, float yPos, int *objID)
{
	ArmGuyBoss *CurrentBoss = (ArmGuyBoss *) CallocMyAlloc(1, sizeof(ArmGuyBoss));

	//Initialize boss struct
	Vec2Set(&CurrentBoss->Position, 700, -200);
	CurrentBoss->BodySprite = (Sprite *) CreateSprite("TextureFiles/TempArmGuy.png", 304, 583, 10, 1, 1, 700, -200);
	CurrentBoss->BodySprite->FlipX = TRUE;
	CurrentBoss->ArmSprite = (Sprite *) CreateSprite("TextureFiles/TempArmGuyArm.png", 237, 110, 11, 1, 1, 580, -120);
	CurrentBoss->ArmSprite->FlipX = TRUE;
	CurrentBoss->playerHit = 0;
	CurrentBoss->MaxHealth = 1000;
	CurrentBoss->CurrentHealth = 1000;
	CurrentBoss->CurrentState = Cooldown;
	CurrentBoss->InnerState = Start;

	// Armguy colliders
	CreateCollisionBox(&CurrentBoss->BossCollider, &CurrentBoss->Position, EnemyType, 150, 530, (*objID)++);
	CreateCollisionBox(&CurrentBoss->SpinAttack, &CurrentBoss->Position, WeaponEnemy, 500, 200, (*objID)++); 
	CreateCollisionBox(&CurrentBoss->JabAttack, &CurrentBoss->Position, WeaponEnemy, 200, 100, (*objID)++); 
	CreateCollisionBox(&CurrentBoss->SmashAttack, &CurrentBoss->Position, WeaponEnemy, 800, 200, (*objID)++);

	// Sets the initial position of all colliders
	CurrentBoss->SpinAttack.Position.y -= CurrentBoss->SpinAttack.height / 2;
	CurrentBoss->JabAttack.Position.x = CurrentBoss->Position.x - 200 * GetLoadRatio();
	CurrentBoss->JabAttack.Position.y = -40 * GetLoadRatio();
	CurrentBoss->SmashAttack.Position.x = 640 * GetLoadRatio();
	CurrentBoss->SmashAttack.Position.y = -40 * GetLoadRatio();

	CurrentBoss->playerHit = -1; // No need for a collision list
	CurrentBoss->cooldownTimer = 0;
	CurrentBoss->SpinDamage = 10;
	CurrentBoss->JabDamage = 10;
	CurrentBoss->SmashDamage = 20;

	return CurrentBoss;
}

/*************************************************************************/
/*!
	\brief
	Handles the collision between an enemy and the attacking weapon
	
	\param CurrentEnemy
	Pointer to the enemy object
*/
/*************************************************************************/
void UpdateArmGuyBoss(ArmGuyBoss *CurrentBoss)
{
	switch(CurrentBoss->CurrentState)
	{
	case Jab:
		switch(CurrentBoss->InnerState)
		{
		case Start:
			//printf("JAB TIME START\n");
			// Boss is on the right
			if(CurrentBoss->Position.x > 0)
			{
				// Pull arm back
				CurrentBoss->JabAttack.Position.x += 250 * GetLoadRatio() * GetDeltaTime();
				
				// Its back enough, switch state
				if(CurrentBoss->JabAttack.Position.x >= CurrentBoss->Position.x + 100 * GetLoadRatio())
				{
					CurrentBoss->InnerState = Attack;
					playerHit = FALSE;
				}
			}
			// Boss is on the left
			else
			{
				// Pull arm back
				CurrentBoss->JabAttack.Position.x -= 250 * GetLoadRatio() * GetDeltaTime();

				// Its back enough, switch state
				if(CurrentBoss->JabAttack.Position.x <= CurrentBoss->Position.x - 100 * GetLoadRatio())
				{
					CurrentBoss->InnerState = Attack;
					playerHit = FALSE;
				}
			}
			break;
		case Attack:
			//printf("JAB TIME ATTACK\n");
			// Boss is on the right
			if(CurrentBoss->Position.x > 0)
			{
				// Punch hard
				CurrentBoss->JabAttack.Position.x -= 2400 * GetLoadRatio() * GetDeltaTime();

				// Check for collision (special to boss)
				if(!playerHit && CollisionRectangles(&CurrentPlayer.PlayerCollider, &CurrentBoss->JabAttack))
				{
					// Damage player's HP here
					playerHit = TRUE;
					PlayerDamageResult(CurrentBoss->JabDamage);
				}
				// Player gets shot back when hit
				else if(playerHit)
				{
					CurrentPlayer.Position.x -= 3200 * GetLoadRatio() * GetDeltaTime();
				}

				// Max range hit, switch state
				if(CurrentBoss->JabAttack.Position.x <= CurrentBoss->Position.x - 400 * GetLoadRatio())
					CurrentBoss->InnerState = End;
			}
			// Boss is on the left
			else
			{
				// Punch hard
				CurrentBoss->JabAttack.Position.x += 2400 * GetLoadRatio() * GetDeltaTime();

				// Check for collision (special to boss)
				if(!playerHit && CollisionRectangles(&CurrentPlayer.PlayerCollider, &CurrentBoss->JabAttack))
				{
					// Damage player's HP here
					playerHit = TRUE;
					PlayerDamageResult(CurrentBoss->JabDamage);
				}
				// Player gets shot back when hit
				else if(playerHit)
				{
					CurrentPlayer.Position.x += 3200 * GetLoadRatio() * GetDeltaTime();
				}

				// Max range hit, switch state
				if(CurrentBoss->JabAttack.Position.x >= CurrentBoss->Position.x + 400 * GetLoadRatio())
					CurrentBoss->InnerState = End;
			}
			break;
		case End:
			//printf("JAB TIME END\n");
			// Reset the player hit fake bool
			
			// Boss is on the right
			if(CurrentBoss->Position.x > 0)
			{
				// Pull arm back into position
				CurrentBoss->JabAttack.Position.x += 400 * GetLoadRatio() * GetDeltaTime();

				// Player is still reeling back a bit from the blow
				if(playerHit)
				{
					CurrentPlayer.Position.x -= 1500 * GetLoadRatio() * GetDeltaTime();
				}

				// Back into position, onto the cooldown
				if(CurrentBoss->JabAttack.Position.x >= CurrentBoss->Position.x - 200 * GetLoadRatio())
				{
					CurrentBoss->CurrentState = Cooldown;
					CurrentBoss->cooldownTimer = 0;
					playerHit = FALSE;
				}
			}
			// Boss is on the left
			else
			{
				// Pull arm back into position
				CurrentBoss->JabAttack.Position.x -= 400 * GetLoadRatio() * GetDeltaTime();

				// Player is still reeling back a bit from the blow
				if(playerHit)
				{
					CurrentPlayer.Position.x += 1500 * GetLoadRatio() * GetDeltaTime();
				}

				// Back into position, onto the cooldown
				if(CurrentBoss->JabAttack.Position.x <= CurrentBoss->Position.x + 200 * GetLoadRatio())
				{
					CurrentBoss->CurrentState = Cooldown;
					CurrentBoss->cooldownTimer = 0;
					playerHit = FALSE;
				}
			}
			break;
		}
		break;
	case Smash:
		switch(CurrentBoss->InnerState)
		{
		case Start:
			//printf("SMASH TIME START\n");

			// Raise the arm and perhaps the roof!
			CurrentBoss->SmashAttack.Position.y += 500 * GetLoadRatio() * GetDeltaTime();
			if(CurrentBoss->SmashAttack.Position.y > 550 * GetLoadRatio())
			{
				CurrentBoss->InnerState = Attack;
				playerHit = FALSE;
			}

			break;
		case Attack:
			//printf("SMASH TIME ATTACK\n");
			
			// Bring down the arm and perhaps the house!
			CurrentBoss->SmashAttack.Position.y -= 3500 * GetLoadRatio() * GetDeltaTime();

			// Check for collision (special to boss)
			if(!playerHit && CollisionRectangles(&CurrentPlayer.PlayerCollider, &CurrentBoss->SmashAttack))
			{
				// Halve the player's movespeed for 2 seconds and deal damage
				playerHit = TRUE;
				CurrentPlayer.CurrentPlayerStats.MoveSpeed /= 2.0f;
				CurrentPlayer.CurrentPlayerStats.HasteTimer = 2;
				PlayerDamageResult(CurrentBoss->SmashDamage);
			}
			// Player gets taken down when hit
			else if(playerHit)
			{
				CurrentPlayer.Position.y -= 3500 * GetLoadRatio() * GetDeltaTime();
			}

			if(CurrentBoss->SmashAttack.Position.y < GROUNDLEVEL * GetLoadRatio())
			{
				CurrentBoss->InnerState = End;
				
			}
			break;
		case End:
			//printf("SMASH TIME END\n");

			// Put things back in order
			CurrentBoss->SmashAttack.Position.y += 500 * GetLoadRatio() * GetDeltaTime();

			if(CurrentBoss->SmashAttack.Position.y > -40 * GetLoadRatio())
			{
				CurrentBoss->CurrentState = Cooldown;
				CurrentBoss->cooldownTimer = 0;
				playerHit = FALSE;
			}
			break;
		}
		break;
	case Spin:
		printf("SPIN TIME START\n");
		switch(CurrentBoss->InnerState)
		{
		case Start:
			CurrentBoss->cooldownTimer += GetDeltaTime();
			// Boss is on the right and start up timer is done
			if(CurrentBoss->cooldownTimer > 0.5f && CurrentBoss->Position.x > 0)
			{
				CurrentBoss->InnerState = SpinL;
				playerHit = FALSE;
			}
			// Boss is on the left and start up timer is done
			else if(CurrentBoss->cooldownTimer > 0.5f)
			{
				CurrentBoss->InnerState = SpinR;
				playerHit = FALSE;
			}
			break;
		case SpinR:
			// Spin to the right
			CurrentBoss->Position.x += 1800 * GetLoadRatio() * GetDeltaTime();
			CurrentBoss->BodySprite->Position.x += 1800 * GetLoadRatio() * GetDeltaTime();
			CurrentBoss->SpinAttack.Position.x += 1800 * GetLoadRatio() * GetDeltaTime();
			CurrentBoss->BossCollider.Position.x += 1800 * GetLoadRatio() * GetDeltaTime();

			// Check for collision (special to boss)
			if(!playerHit && CollisionRectangles(&CurrentPlayer.PlayerCollider, &CurrentBoss->SpinAttack))
			{
				// Player gets shot up
				playerHit = TRUE;
				Vec2Set(&CurrentPlayer.PlayerRigidBody.Velocity, 0.0f, 1500.0f);
				CurrentPlayer.Position.y += 2.0f;
				PlayerDamageResult(CurrentBoss->SpinDamage);
			}

			if(CurrentBoss->Position.x >= 700 * GetLoadRatio())
			{
				CurrentBoss->BodySprite->Position.x = 700 * GetLoadRatio();
				CurrentBoss->Position.x = 700 * GetLoadRatio();
				CurrentBoss->SpinAttack.Position.x = 700 * GetLoadRatio();
				CurrentBoss->BossCollider.Position.x = 700 * GetLoadRatio();
				CurrentBoss->InnerState = End;
				CurrentBoss->cooldownTimer = 0;
			}
			break;
		case SpinL:
			// Spin to the left
			CurrentBoss->Position.x -= 1800 * GetLoadRatio() * GetDeltaTime();
			CurrentBoss->BodySprite->Position.x -= 1800 * GetLoadRatio() * GetDeltaTime();
			CurrentBoss->SpinAttack.Position.x -= 1800 * GetLoadRatio() * GetDeltaTime();
			CurrentBoss->BossCollider.Position.x -= 1800 * GetLoadRatio() * GetDeltaTime();

			// Check for collision (special to boss)
			if(!playerHit && CollisionRectangles(&CurrentPlayer.PlayerCollider, &CurrentBoss->SpinAttack))
			{
				// Player gets shot up
				playerHit = TRUE;
				Vec2Set(&CurrentPlayer.PlayerRigidBody.Velocity, 0.0f, 1500.0f);
				CurrentPlayer.Position.y += 2.0f;
				PlayerDamageResult(CurrentBoss->SpinDamage);
			}

			if(CurrentBoss->Position.x <= -700 * GetLoadRatio())
			{
				CurrentBoss->BodySprite->Position.x = -700 * GetLoadRatio();
				CurrentBoss->Position.x = -700 * GetLoadRatio();
				CurrentBoss->SpinAttack.Position.x = -700 * GetLoadRatio();
				CurrentBoss->BossCollider.Position.x = -700 * GetLoadRatio();
				CurrentBoss->InnerState = End;
				CurrentBoss->cooldownTimer = 0;
			}
			break;
		case End:
			CurrentBoss->cooldownTimer += GetDeltaTime();
			if(CurrentBoss->cooldownTimer > 0.5f && CurrentBoss->Position.x > 0)
			{
				CurrentBoss->BodySprite->FlipX = TRUE;
				CurrentBoss->ArmSprite->Position.x = 580 * GetLoadRatio();
				CurrentBoss->ArmSprite->FlipX = TRUE;
				CurrentBoss->JabAttack.Position.x = CurrentBoss->Position.x - 200 * GetLoadRatio();
				CurrentBoss->SmashAttack.Position.x = 640 * GetLoadRatio();
				CurrentBoss->CurrentState = Cooldown;
				CurrentBoss->cooldownTimer = 0;
				playerHit = FALSE;
			}
			else if(CurrentBoss->cooldownTimer > 0.5f)
			{
				CurrentBoss->BodySprite->FlipX = FALSE;
				CurrentBoss->ArmSprite->Position.x = -580 * GetLoadRatio();
				CurrentBoss->ArmSprite->FlipX = FALSE;
				CurrentBoss->JabAttack.Position.x = CurrentBoss->Position.x + 200 * GetLoadRatio();
				CurrentBoss->SmashAttack.Position.x = -640 * GetLoadRatio();
				CurrentBoss->CurrentState = Cooldown;
				CurrentBoss->cooldownTimer = 0;
				playerHit = FALSE;
			}
			break;
		}
		break;
	case Cooldown:
		//printf("CD TIME\n");
		CurrentBoss->cooldownTimer += GetDeltaTime();

		// Cooldown is up, choose a move, Boss on right
		if(CurrentBoss->cooldownTimer > 2 && CurrentBoss->BodySprite->Position.x > 0)
		{
			// Player is far, spin toward him
			if(CurrentPlayer.Position.x < 0)
			{
				CurrentBoss->CurrentState = Spin;
				CurrentBoss->InnerState = Start;
				CurrentBoss->cooldownTimer = 0;
			}
			// Player is up high, smash or jab
			else if(CurrentPlayer.Position.y > -150 * GetLoadRatio())
			{
				if(rand() % 2)
				{
					CurrentBoss->CurrentState = Smash;
					CurrentBoss->InnerState = Start;
					CurrentBoss->cooldownTimer = 0;
				}
				else
				{
					CurrentBoss->CurrentState = Jab;
					CurrentBoss->InnerState = Start;
					CurrentBoss->cooldownTimer = 0;
				}
			}
			// Player is low, smash or spin
			else
			{
				if(rand() % 2)
				{
					CurrentBoss->CurrentState = Smash;
					CurrentBoss->InnerState = Start;
					CurrentBoss->cooldownTimer = 0;
				}
				else
				{
					CurrentBoss->CurrentState = Spin;
					CurrentBoss->InnerState = Start;
					CurrentBoss->cooldownTimer = 0;
				}
			}
		}
		// Cooldown is up, choose a move, Boss on left
		else if(CurrentBoss->cooldownTimer > 2)
		{
			// Player is far, spin toward him
			if(CurrentPlayer.Position.x > 0)
			{
				CurrentBoss->CurrentState = Spin;
				CurrentBoss->InnerState = Start;
				CurrentBoss->cooldownTimer = 0;
			}
			// Player is up high, smash or jab
			else if(CurrentPlayer.Position.y > -150 * GetLoadRatio())
			{
				if(rand() % 2)
				{
					CurrentBoss->CurrentState = Smash;
					CurrentBoss->InnerState = Start;
					CurrentBoss->cooldownTimer = 0;
				}
				else
				{
					CurrentBoss->CurrentState = Jab;
					CurrentBoss->InnerState = Start;
					CurrentBoss->cooldownTimer = 0;
				}
			}
			// Player is low, smash or spin
			else
			{
				if(rand() % 2)
				{
					CurrentBoss->CurrentState = Smash;
					CurrentBoss->InnerState = Start;
					CurrentBoss->cooldownTimer = 0;
				}
				else
				{
					CurrentBoss->CurrentState = Spin;
					CurrentBoss->InnerState = Start;
					CurrentBoss->cooldownTimer = 0;
				}
			}
		}
		break;
	}

	//Check if boss is dead
	//Give ability to end the level
}

/*************************************************************************/
/*!
	\brief
	Handles the collision between an enemy and the attacking weapon
	
	\param CurrentEnemy
	Pointer to the enemy object
*/
/*************************************************************************/
void DetectBossCollision(ArmGuyBoss *CurrentBoss)
{
	Weapon* wList = weaponList;

	int hit = 0;
	int hitPrev = 0;

	while(wList->objID != -1)
	{
		if(wList->objID > 0 && wList->WeaponFOF == PlayerWeapon)
		{
			hit = CollisionRectangles(&CurrentBoss->BossCollider, &wList->WeaponAttack);
			hitPrev = CurrentBoss->playerHit;
			if(hit && CurrentPlayer.isAttacking)
			{
				// On start collision
				if(hitPrev <= 0)
				{
					CurrentBoss->playerHit = wList->WeaponAttack.collisionID * 10 + 1;
					//printf("NOT FOUND: %i\n", -hitPrev);
					BossCollideWeapon(CurrentBoss);
				}
				// Found target, hit previous frame, on persistant
				else if(hitPrev % 10 == 1)
				{
					//printf("FOUND PERSISTANT: %i\n", CurrentEnemy.CollisionData[hitPrev]);
				}
			}
			else if(hitPrev > 0 && !CurrentPlayer.isAttacking)
			{
				CurrentBoss->playerHit = 0;
			}
			else
			{
				if(hitPrev <= 0)
				{
					// NEVER COLLIDED OR DIDNT COLLIDE PREV FRAME
					//AE_ASSERT_MESG("No collision and not colliding, should never be here.");
				}
				// Found target, collision ended
				else if(hitPrev % 10 == 1)
				{
					//printf("END COLLISION: %i\n", CurrentEnemy.CollisionData[hitPrev]);
					//CurrentEnemy->CollisionData[hitPrev] = 0;
				}
			}
		}
		wList++;
	}
}

/*************************************************************************/
/*!
	\brief
	Handles the collision between an enemy and the attacking weapon
	
	\param CurrentEnemy
	Pointer to the enemy object
*/
/*************************************************************************/
void BossCollideWeapon(ArmGuyBoss *CurrentBoss)
{
	int damageDealt;
	char num[10];
	Vec3 textColor;
	TextGlyphs *FirstLetter;
	Vec3Set(&textColor, 1.0f, 0.0f, 0.0f);

	// Calculate damage including enemy's damage reduction
	if(Cheats)
		damageDealt = 9999;
	else
		damageDealt = CurrentPlayer.CurrentPlayerStats.Damage;
	
	CurrentBoss->CurrentHealth -= damageDealt;
	sprintf(num, "%d", damageDealt);
	// Create Floating Combat Text
	FirstLetter = CreateText(num, CurrentBoss->Position.x, (CurrentBoss->Position.y + CurrentBoss->BodySprite->Height / 2), 100, textColor, Center);
	AddFloatingText(FirstLetter);
	ChangeTextVisibility(FirstLetter);
}

/*************************************************************************/
/*!
	\brief
	Deals player damage resolution
	
	\param damage
	The damage to do to the player
*/
/*************************************************************************/
void PlayerDamageResult(int damage)
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
		damageDealt = damage;
	
	CurrentPlayer.CurrentPlayerStats.CurrentHealth -= damageDealt;
	sprintf(num, "%d", damageDealt);

	// Create Floating Combat Text
	FirstLetter = CreateText(num, CurrentPlayer.Position.x / GetLoadRatio(), (CurrentPlayer.Position.y + CurrentPlayer.PlayerSpriteParts.Body->Height / 2) / GetLoadRatio(), 100, textColor, Center);
	AddFloatingText(FirstLetter);
	ChangeTextVisibility(FirstLetter);
}
