/*****************************************************************************/
/*!
\file				HandGuyBoss.c
\author				Dan Muller (d.muller)
\date				Mar 18, 2014

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
#include "../HeaderFiles/HandGuyBoss.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxObjects.h"

// ---------------------------------------------------------------------------
// globals
static int LogicTimer = 0;
static int playerHit = FALSE;

static enum HandGuyState { Cooldown, Shout, Question, Jab, Move };
static enum InnerState { Start, Attack, End };
static enum PositionState { A, B, C, D };

/*************************************************************************/
/*!
	\brief
	Handles the collision between an enemy and the attacking weapon
	
	\param CurrentEnemy
	Pointer to the enemy object
*/
/*************************************************************************/
void LoadHandGuyBoss(void)
{
	LoadTexture("TextureFiles/TempArmGuy.png");
	LoadTexture("TextureFiles/TempArmGuyArm.png");
	LoadTexture("TextureFiles/TempBossSpin.png");
}

/*************************************************************************/
/*!
	\brief
	Handles the collision between an enemy and the attacking weapon
	
	\param CurrentEnemy
	Pointer to the enemy object
*/
/*************************************************************************/
HandGuyBoss* CreateHandGuyBoss(float xPos, float yPos, int *objID)
{
	HandGuyBoss *CurrentBoss = (HandGuyBoss *) CallocMyAlloc(1, sizeof(HandGuyBoss));

	//Initialize boss struct
	Vec2Set(&CurrentBoss->Position, 700, -200);
	CurrentBoss->BodySprite = (Sprite *) CreateSprite("TextureFiles/TempArmGuy.png", 319, 612, 10, 1, 1, 700, -200);
	CurrentBoss->BodySprite->FlipX = TRUE;
	CurrentBoss->JabSprite = (Sprite *) CreateSprite("TextureFiles/TempArmGuyArm.png", 237, 110, 11, 1, 1, 580, -120);
	CurrentBoss->JabSprite->FlipX = TRUE;
	CurrentBoss->playerHit = 0;
	CurrentBoss->MaxHealth = 1000;
	CurrentBoss->CurrentHealth = 1000;
	CurrentBoss->CurrentState = Cooldown;
	CurrentBoss->InnerState = Start;
	CurrentBoss->PositionState = B;

	// Armguy colliders
	CurrentBoss->ShoutRadius = 400.0f;
	CreateCollisionBox(&CurrentBoss->BossCollider, &CurrentBoss->Position, EnemyType, 150, 530, (*objID)++);
	CreateCollisionBox(&CurrentBoss->JabAttack, &CurrentBoss->Position, WeaponEnemy, 200, 100, (*objID)++); 

	// Sets the initial position of all colliders
	CurrentBoss->JabAttack.Position.x = CurrentBoss->Position.x - 200;
	CurrentBoss->JabAttack.Position.y = -40;

	CurrentBoss->playerHit = -1; // No need for a collision list
	CurrentBoss->cooldownTimer = 0;
	CurrentBoss->ShoutDamage = 30;
	CurrentBoss->QuestionDamage = 10;
	CurrentBoss->JabDamage = 10;

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
void UpdateHandGuyBoss(HandGuyBoss *CurrentBoss)
{
	switch(CurrentBoss->CurrentState)
	{
	case Jab:
		switch(CurrentBoss->InnerState)
		{
		case Start:
			//printf("JAB TIME START\n");
			
			break;
		case Attack:
			//printf("JAB TIME ATTACK\n");
			
			break;
		case End:
			//printf("JAB TIME END\n");
			
			break;
		}
		break;
	case Shout:
		switch(CurrentBoss->InnerState)
		{
		case Start:
			//printf("SMASH TIME START\n");

			break;
		case Attack:
			//printf("SMASH TIME ATTACK\n");
			
			break;
		case End:
			//printf("SMASH TIME END\n");

			break;
		}
		break;
	case Question:
		switch(CurrentBoss->InnerState)
		{
		case Start:
			//printf("SPIN TIME START\n");
			break;
		case Attack:
			//printf("SPIN TIME START\n");
			break;
		case End:
			//printf("SPIN TIME START\n");
			break;
		}
		break;
	case Move:
		switch(CurrentBoss->InnerState)
		{
		case Start:
			//printf("SPIN TIME START\n");
			break;
		case Attack:
			//printf("SPIN TIME START\n");
			break;
		case End:
			//printf("SPIN TIME START\n");
			break;
		}
		break;
	case Cooldown:
		//printf("CD TIME\n");
		CurrentBoss->cooldownTimer += GetDeltaTime();

		// Cooldown is up, choose a move
		if(CurrentBoss->cooldownTimer > 2)
		{
			// Choose move based on position state
			switch(CurrentBoss->PositionState)
			{
			// Boss is on left
			case A:
				// 33% chance to shout then move
				if(rand() % 100 > 66)
				{
					CurrentBoss->CurrentState = Shout;
					CurrentBoss->InnerState = Start;
					CurrentBoss->cooldownTimer = 0;
				}
				// 66% chance to throw questions
				else
				{
					CurrentBoss->CurrentState = Question;
					CurrentBoss->InnerState = Start;
					CurrentBoss->cooldownTimer = 0;
				}
				break;
			case B:
				// 33% chance to shout then move
				if(rand() % 100 > 66)
				{
					CurrentBoss->CurrentState = Shout;
					CurrentBoss->InnerState = Start;
					CurrentBoss->cooldownTimer = 0;
				}
				// 66% chance to throw questions
				else
				{
					CurrentBoss->CurrentState = Question;
					CurrentBoss->InnerState = Start;
					CurrentBoss->cooldownTimer = 0;
				}
				break;
			case C:
			case D:
				CurrentBoss->CurrentState = Shout;
				CurrentBoss->InnerState = Start;
				CurrentBoss->cooldownTimer = 0;
				break;
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
void DetectBossCollision(HandGuyBoss *CurrentBoss)
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
void BossCollideWeapon(HandGuyBoss *CurrentBoss)
{
	int damageDealt;
	char num[10];
	Vec3 textColor;
	TextGlyphs *FirstLetter;
	Vec3Set(&textColor, 1.0f, 1.0f, 1.0f);

	// Calculate damage including enemy's damage reduction
	if(Cheats)
		damageDealt = 9999;
	else
		damageDealt = CurrentPlayer.CurrentPlayerStats.Damage;
	
	CurrentBoss->CurrentHealth -= damageDealt;
	sprintf(num, "-%d", damageDealt);
	// Create Floating Combat Text
	FirstLetter = CreateText(num, (CurrentBoss->Position.x + rand() % 81 - 40), (CurrentBoss->Position.y + CurrentBoss->BodySprite->Height / 2), 80, textColor, Center, Border);
	AddFloatingText(FirstLetter);
	ChangeTextVisibility(FirstLetter);
	ChangeTextZIndex(FirstLetter, 201);
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
	Vec3Set(&textColor, 1.0f, 0.0f, 0.0f);
	
	// Calculate damage
	if(Cheats)
		damageDealt = 0;
	else
		damageDealt = damage;
	
	CurrentPlayer.CurrentPlayerStats.CurrentHealth -= damageDealt;
	sprintf(num, "-%d", damageDealt);

	// Create Floating Combat Text
	FirstLetter = CreateText(num, (CurrentPlayer.Position.x + rand() % 81 - 40), (CurrentPlayer.Position.y + CurrentPlayer.PlayerSpriteParts.Body->Height / 2), 80, textColor, Center, Border);
	AddFloatingText(FirstLetter);
	ChangeTextVisibility(FirstLetter);
	ChangeTextZIndex(FirstLetter, 201);
}
