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

static enum ArmGuyState { Cooldown, Jab, Smash, Spin };

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
	CurrentBoss->BodySprite = (Sprite *) CreateSprite("TextureFiles/TempArmGuy.png", 277, 530, 10, 1, 1, 700, -200);
	CurrentBoss->BodySprite->FlipX = TRUE;
	CurrentBoss->ArmSprite = (Sprite *) CreateSprite("TextureFiles/TempArmGuyArm.png", 237, 110, 11, 1, 1, 580, -120);
	CurrentBoss->ArmSprite->FlipX = TRUE;
	CurrentBoss->playerHit = 0;
	CurrentBoss->MaxHealth = 1000;
	CurrentBoss->CurrentHealth = 1000;
	CurrentBoss->CurrentState = Cooldown;

	// Armguy colliders
	CreateCollisionBox(&CurrentBoss->BossCollider, &CurrentBoss->Position, EnemyType, 135, 530, *objID++);
	CreateCollisionBox(&CurrentBoss->SpinAttack, &CurrentBoss->Position, WeaponEnemy, 300, 200, *objID++); 
	CreateCollisionBox(&CurrentBoss->ArmAttack, &CurrentBoss->Position, WeaponEnemy, 500, 200, *objID++); 

	CurrentBoss->playerHit = -1; // No need for a collision list
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
		break;
	case Smash:
		break;
	case Spin:
		break;
	case Cooldown:
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
	FirstLetter = CreateText(num, CurrentBoss->Position.x / GetLoadRatio(), (CurrentBoss->Position.y + CurrentBoss->BodySprite->Height / 2) / GetLoadRatio(), 100, textColor, Center);
	AddFloatingText(FirstLetter);
	ChangeTextVisibility(FirstLetter);
}
