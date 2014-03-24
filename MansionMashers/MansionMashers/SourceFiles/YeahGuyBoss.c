/*****************************************************************************/
/*!
\file				YeahGuyBoss.c
\author				Dan Muller (d.muller)
\date				Mar 22, 2014

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
#include "../HeaderFiles/YeahGuyBoss.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxObjects.h"

// ---------------------------------------------------------------------------
// globals
static int LogicTimer = 0;
static int playerHit = FALSE;

static enum YeahGuyState { Cooldown, AOE, ProjectYeah, Pound, Vault };
static enum InnerState {Start, Attack, End };

/*************************************************************************/
/*!
	\brief
	Handles the collision between an enemy and the attacking weapon
	
	\param CurrentEnemy
	Pointer to the enemy object
*/
/*************************************************************************/
void LoadYeahGuyBoss(void)
{
	LoadTexture("TextureFiles/YeahProjectile.png");
	LoadTexture("TextureFiles/YeahProjectileBack.png");
	LoadTexture("TextureFiles/TempYeahGuy.png");
}

/*************************************************************************/
/*!
	\brief
	Handles the collision between an enemy and the attacking weapon
	
	\param CurrentEnemy
	Pointer to the enemy object
*/
/*************************************************************************/
YeahGuyBoss* CreateYeahGuyBoss(float xPos, float yPos, int *objID)
{
	YeahGuyBoss *CurrentBoss = (YeahGuyBoss *) CallocMyAlloc(1, sizeof(YeahGuyBoss));

	//Initialize boss struct
	Vec2Set(&CurrentBoss->Position, 700, -200);
	CurrentBoss->BodySprite = (Sprite *) CreateSprite("TextureFiles/TempYeahGuy.png", 225, 300, 10, 1, 1, 700, -200);
	CurrentBoss->BodySprite->FlipX = TRUE;

	CurrentBoss->playerHit = 0;
	CurrentBoss->MaxHealth = 1000;
	CurrentBoss->CurrentHealth = 1000;
	CurrentBoss->CurrentState = ProjectYeah;
	CurrentBoss->InnerState = Start;

	// Armguy colliders
	CreateCollisionBox(&CurrentBoss->BossCollider, &CurrentBoss->Position, EnemyType, 150, 320, (*objID)++);

	// Physics stuff
	InitializeRigidBody(&CurrentBoss->YeahGuyRigidBody, FALSE, 150, 300);
	CurrentBoss->YeahGuyRigidBody.Mass = 7;

	CurrentBoss->playerHit = -1; // No need for a collision list
	CurrentBoss->cooldownTimer = 0;
	CurrentBoss->numHeads = 3;
	
	CurrentBoss->YeahAOEDamage = 30;
	CurrentBoss->YeahAOERadius = 80;

	CurrentBoss->YeahPoundDamage = 30;
	CurrentBoss->YeahProjectileDamage = 20;

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
void UpdateYeahGuyBoss(YeahGuyBoss *CurrentBoss)
{
	Vec2 velocityTime;
	Projectile *CurrentProjectile;
	int projectileID = 100;
	static int numProjectiles = 0;
	float projectileAngle;
	//int movementPicker;

	switch(CurrentBoss->CurrentState)
	{
	case AOE:
		switch(CurrentBoss->InnerState)
		{
		case Start:
			//printf("AOE TIME START\n");
			CurrentBoss->BodySprite->SpriteTexture = LoadTexture("TextureFiles/TempHandGuyShout.png");
			CurrentBoss->cooldownTimer += GetDeltaTime();
			if(CurrentBoss->cooldownTimer > 2.0f)
				CurrentBoss->InnerState = Attack;
			break;
		case Attack:
			//printf("AOE TIME ATTACK\n");
			if(RectCircleCollision(&CurrentBoss->Position, CurrentBoss->YeahAOERadius, &CurrentPlayer.PlayerCollider))
			{
				// If attack hits, daze player and switch to the jab
				PlayerDamageResult(CurrentBoss->YeahAOEDamage);
				CurrentPlayer.CurrentPlayerStats.MoveSpeed /= 3.0f;
				CurrentPlayer.CurrentPlayerStats.HasteTimer = 2;
			}
			CurrentBoss->InnerState = End;
			CurrentBoss->cooldownTimer = 0.0f;
			break;
		case End:
			//printf("AOE TIME END\n");
			CurrentBoss->BodySprite->SpriteTexture = LoadTexture("TextureFiles/TempYeahGuy.png");

			CurrentBoss->cooldownTimer += GetDeltaTime();
			if(CurrentBoss->cooldownTimer > 2.0f)
			{
				CurrentBoss->cooldownTimer = 0.0f;
				CurrentBoss->CurrentState = Cooldown;
				CurrentBoss->InnerState = Start;
			}
			break;
		}
		break;
	case ProjectYeah:
		switch(CurrentBoss->InnerState)
		{
		case Start:
			//printf("QUESTION TIME START\n");
			// Change boss sprite to attacking
			projectileAngle = atan2(CurrentPlayer.Position.y - CurrentBoss->Position.y, CurrentPlayer.Position.x - CurrentBoss->Position.x);
			CurrentProjectile = CreateProjectile("TextureFiles/YeahProjectile.png", 210, 90, CurrentBoss->Position.x, CurrentBoss->Position.y + CurrentBoss->BodySprite->Height / 2, Arrow, WeaponEnemy, projectileID++, CurrentBoss->YeahProjectileDamage / CurrentBoss->numHeads, 800, projectileAngle);
			if(CurrentBoss->Position.x > CurrentPlayer.Position.x)
			{
				CurrentProjectile->ProjectileSprite->SpriteTexture = LoadTexture("TextureFiles/YeahProjectileBack.png");
			}
			++numProjectiles;
			CurrentBoss->InnerState = Attack;
			break;
		case Attack:
			//printf("QUESTION TIME START\n");
			CurrentBoss->cooldownTimer += GetDeltaTime();
			// Waits 2 seconds
			if(CurrentBoss->cooldownTimer >= 1.0f)
			{
				CurrentBoss->cooldownTimer = 0.0f;
				CurrentBoss->InnerState = End;
			}
			break;
		case End:
			//printf("QUESTION TIME START\n");
			CurrentBoss->cooldownTimer += GetDeltaTime();
			
			// Shoots out as many projectiles as he has heads
			if(numProjectiles >= CurrentBoss->numHeads)
			{
				CurrentBoss->CurrentState = Cooldown;
				numProjectiles = 0;
			}

			CurrentBoss->InnerState = Start;

			break;
		}
		break;
	case Pound:
		switch(CurrentBoss->InnerState)
		{
		case Start:
			break;
		case Attack:
			break;
		case End:
			break;
		}
		break;
	case Vault:
		switch(CurrentBoss->InnerState)
		{
		case Start:
			break;
		case Attack:
			break;
		case End:
			break;
		}
		break;
	case Cooldown:
		switch(CurrentBoss->InnerState)
		{
		case Start:
			CurrentBoss->cooldownTimer += GetDeltaTime();
			if(CurrentBoss->cooldownTimer > 2.0f)
			{
				CurrentBoss->CurrentState = ProjectYeah;
				CurrentBoss->cooldownTimer = 0.0f;
			}

			break;
		case Attack:
			break;
		case End:
			break;
		}
		break;
	}

	// Set acceleration to zero
	ZeroAcceleration(&CurrentBoss->YeahGuyRigidBody);

	//Brings the player back to the surface if something bad happens
	if(CurrentBoss->Position.y - CurrentBoss->BodySprite->Height / 4 < GROUNDLEVEL)
	{
		CurrentBoss->Position.y = GROUNDLEVEL + CurrentBoss->BodySprite->Height / 4;
	}
	//Stop vertical velocity and acceleration when the player lands on the floor
	if(CurrentBoss->Position.y - CurrentBoss->BodySprite->Height / 4 <= GROUNDLEVEL || CurrentBoss->YeahGuyRigidBody.onGround)
	{
		Vec2Zero(&CurrentBoss->YeahGuyRigidBody.Acceleration);
		Vec2Zero(&CurrentBoss->YeahGuyRigidBody.Velocity);
		ZeroGravity(&CurrentBoss->YeahGuyRigidBody);
	}
	//Set gravity if not on floor or on a platform
	else
	{
		SetGravity(&CurrentBoss->YeahGuyRigidBody, 0.0f, FOX_GRAVITY_Y);
	}

	//Update velocity and acceleration
	UpdateVelocity(&CurrentBoss->YeahGuyRigidBody);
	Vec2Scale(&velocityTime, &CurrentBoss->YeahGuyRigidBody.Velocity, GetDeltaTime());
	
	Vec2Add(&CurrentBoss->Position, &CurrentBoss->Position, &velocityTime);

	//Updates the collision box
	UpdateCollisionPosition(&CurrentBoss->BossCollider, &CurrentBoss->Position);
	CurrentBoss->YeahGuyRigidBody.onGround = FALSE;

	CurrentBoss->BodySprite->Position = CurrentBoss->Position;

	if(CurrentPlayer.Position.x > CurrentBoss->Position.x)
	{
		// Flip to face player
		CurrentBoss->BodySprite->FlipX = FALSE;
	}
	else
	{
		// Flip to face player
		CurrentBoss->BodySprite->FlipX = TRUE;
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
void DetectYeahGuyBossCollision(YeahGuyBoss *CurrentBoss)
{
	Weapon* wList = weaponList;
	Platform* pList = platformList;

	int hit = 0;
	int hitPrev = 0;

	//Cycle through the platform list
	while(pList->objID != -1)
	{
		//If platform exists
		if(pList->objID > 0)
		{
			//Checks if there is collision this frame
			hit = CollisionRectangles(&CurrentBoss->BossCollider, &pList->PlatformCollider);
			
			if(hit)
			{
				YeahGuyPlatformCollision(CurrentBoss, pList);
			}
		}
		pList++;
	}
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
					YeahGuyBossCollideWeapon(CurrentBoss);
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
void YeahGuyBossCollideWeapon(YeahGuyBoss *CurrentBoss)
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
	Handles the collision between an enemy and the attacking weapon
	
	\param CurrentEnemy
	Pointer to the enemy object
*/
/*************************************************************************/
static void PlayerDamageResult(int damage)
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

/*************************************************************************/
/*!
	\brief
	Handles the collision between an enemy and the attacking weapon
	
	\param CurrentEnemy
	Pointer to the enemy object
*/
/*************************************************************************/
void YeahGuyPlatformCollision(YeahGuyBoss* CurrentBoss, Platform* CurrentPlatform)
{
	if(CurrentBoss->dropDown)
		return;
	if(CurrentBoss->YeahGuyRigidBody.Velocity.y <= 0)
	{
		if(CurrentBoss->BossCollider.Position.y + CurrentBoss->BossCollider.Offset.y - CurrentBoss->BossCollider.height / 2.0f > CurrentPlatform->PlatformCollider.Position.y + CurrentPlatform->PlatformCollider.Offset.y)
		{
			if(CurrentBoss->YeahGuyRigidBody.Velocity.y != 0 && CurrentPlatform->PlatformCollider.collisionGroup == PlatformType)
			{
				CurrentBoss->Position.y = CurrentPlatform->PlatformCollider.Position.y + CurrentPlatform->PlatformCollider.Offset.y + CurrentPlatform->PlatformCollider.height / 2 - CurrentBoss->BossCollider.Offset.y + CurrentBoss->BossCollider.height / 2 - 0.01f;
				CurrentBoss->YeahGuyRigidBody.onGround = TRUE;
			}
			else if(CurrentBoss->YeahGuyRigidBody.Velocity.y != 0 && CurrentPlatform->PlatformCollider.collisionGroup == BounceType)
			{
				if(CurrentBoss->YeahGuyRigidBody.Velocity.y > -(CurrentPlatform->PlatformRigidBody.Restitution + CurrentBoss->YeahGuyRigidBody.Restitution))
				{
					CurrentBoss->Position.y = CurrentPlatform->PlatformCollider.Position.y + CurrentPlatform->PlatformCollider.Offset.y + CurrentPlatform->PlatformCollider.height / 2 - CurrentBoss->BossCollider.Offset.y + CurrentBoss->BossCollider.height / 2 - 0.01f;
					CurrentBoss->YeahGuyRigidBody.onGround = TRUE;
				}
				else
				{
					CurrentBoss->Position.y = CurrentPlatform->PlatformCollider.Position.y + CurrentPlatform->PlatformCollider.Offset.y + CurrentPlatform->PlatformCollider.height / 2 - CurrentBoss->BossCollider.Offset.y + CurrentBoss->BossCollider.height / 2 + 0.5f;
					BounceObject(&CurrentBoss->YeahGuyRigidBody, &CurrentPlatform->PlatformRigidBody);
					
				}
			}
			else
				CurrentBoss->YeahGuyRigidBody.onGround = TRUE;
		}
	}
}
