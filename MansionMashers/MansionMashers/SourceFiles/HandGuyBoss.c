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
	LoadTexture("TextureFiles/TempHandGuy.png");
	LoadTexture("TextureFiles/TempHandGuyShout.png");
	LoadTexture("TextureFiles/QuickJab.png");
	LoadTexture("TextureFiles/DebugCircle.png");
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
	CurrentBoss->BodySprite = (Sprite *) CreateSprite("TextureFiles/TempHandGuy.png", 150, 300, 10, 1, 1, 700, -200);
	CurrentBoss->BodySprite->FlipX = TRUE;
	CurrentBoss->JabSprite = (Sprite *) CreateSprite("TextureFiles/QuickJab.png", 200, 200, 11, 4, 4, 580, -120);
	CurrentBoss->JabSprite->FlipX = TRUE;
	CurrentBoss->JabSprite->AnimationSpeed = 3;
	CurrentBoss->JabSprite->Visible = FALSE;
	CurrentBoss->playerHit = 0;
	CurrentBoss->MaxHealth = 1000;
	CurrentBoss->CurrentHealth = 1000;
	CurrentBoss->CurrentState = Cooldown;
	CurrentBoss->InnerState = Start;
	CurrentBoss->PositionState = B;

	// Armguy colliders
	CurrentBoss->ShoutRadius = 600.0f;
	CreateCollisionBox(&CurrentBoss->BossCollider, &CurrentBoss->Position, EnemyType, 150, 320, (*objID)++);
	CreateCollisionBox(&CurrentBoss->JabAttack, &CurrentBoss->Position, WeaponEnemy, 200, 200, (*objID)++); 

	// Sets the initial position of all colliders
	CurrentBoss->JabAttack.Position.x = CurrentBoss->Position.x - CurrentBoss->JabAttack.width / 2;
	CurrentBoss->JabAttack.Position.y = CurrentBoss->Position.y;

	// Physics stuff
	InitializeRigidBody(&CurrentBoss->HandGuyRigidBody, FALSE, 150, 300);
	CurrentBoss->HandGuyRigidBody.Mass = 7;
	CurrentBoss->dropDown = FALSE;

	CurrentBoss->playerHit = -1; // No need for a collision list
	CurrentBoss->cooldownTimer = 0;
	CurrentBoss->ShoutDamage = 30;
	CurrentBoss->QuestionDamage = 10;
	CurrentBoss->JabDamage = 15;

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
	Vec2 velocityTime;
	Projectile *CurrentProjectile;
	int projectileID = 100;
	static int numProjectiles;
	float projectileAngle;
	int movementPicker;

	switch(CurrentBoss->CurrentState)
	{
	case Jab:
		switch(CurrentBoss->InnerState)
		{
		case Start:
			//printf("JAB TIME START\n");
			CurrentBoss->dropDown = FALSE;
			CurrentBoss->cooldownTimer += GetDeltaTime();

			// Move into damage range
			if(CurrentBoss->Position.x > CurrentPlayer.Position.x + CurrentBoss->JabAttack.width)
				CurrentBoss->Position.x -= 1000 * GetDeltaTime();
			else if(CurrentBoss->Position.x < CurrentPlayer.Position.x - CurrentBoss->JabAttack.width)
				CurrentBoss->Position.x += 1000 * GetDeltaTime();
			// Don't attack on the very first frame
			else if(CurrentBoss->cooldownTimer > GetDeltaTime() * 2 && CollisionRectangles(&CurrentPlayer.PlayerCollider, &CurrentBoss->JabAttack))
			{
				CurrentBoss->InnerState = Attack;
				CurrentBoss->cooldownTimer = 0.0f;
				CurrentBoss->InnerState = Attack;
			}

			// Jump up to the platform if needed
			if((CurrentPlayer.PlayerRigidBody.Velocity.y > 0 || CurrentPlayer.Position.y > 0) && CurrentBoss->Position.y <= GROUNDLEVEL + CurrentBoss->BodySprite->Height / 4)
			{
				// Set y velocity for jumping
				Vec2 velocity;
				CurrentBoss->Position.y += 3;
				Vec2Set(&velocity, 0.0f, 1200.0f);
				ApplyVelocity(&CurrentBoss->HandGuyRigidBody, &velocity);
			}
			// Drop down if needed
			else if(CurrentPlayer.Position.y <= GROUNDLEVEL && CurrentBoss->Position.y > 0)
			{
				CurrentBoss->dropDown = TRUE;
				CurrentBoss->HandGuyRigidBody.onGround = FALSE;
			}
			break;
		case Attack:
			//printf("JAB TIME ATTACK\n");
			CurrentBoss->JabSprite->Visible = TRUE;
			CurrentBoss->cooldownTimer += GetDeltaTime();

			// Deal 1 damage if in range
			if(CollisionRectangles(&CurrentPlayer.PlayerCollider, &CurrentBoss->JabAttack))
			{
				PlayerDamageResult(1);
			}
			
			// Once all damage is dealt move to a new location
			if(CurrentBoss->cooldownTimer > GetDeltaTime() * CurrentBoss->JabDamage)
			{
				CurrentBoss->InnerState = End;
				CurrentBoss->cooldownTimer = 0.0f;
			}
			break;
		case End:
			//printf("JAB TIME END\n");
			CurrentBoss->JabSprite->Visible = FALSE;
			CurrentBoss->cooldownTimer += GetDeltaTime();

			if(CurrentBoss->cooldownTimer > 1.0f)
			{
				CurrentBoss->CurrentState = Move;
				CurrentBoss->InnerState = Start;
				CurrentBoss->cooldownTimer = 0.0f;
			}
			break;
		}
		break;
	case Shout:
		switch(CurrentBoss->InnerState)
		{
		case Start:
			//printf("SHOUT TIME START\n");
			CurrentBoss->BodySprite->SpriteTexture = LoadTexture("TextureFiles/TempHandGuyShout.png");
			CurrentBoss->cooldownTimer += GetDeltaTime();
			if(CurrentBoss->cooldownTimer > 2.0f)
				CurrentBoss->InnerState = Attack;
			break;
		case Attack:
			//printf("SHOUT TIME ATTACK\n");
			if(RectCircleCollision(&CurrentBoss->Position, CurrentBoss->ShoutRadius, &CurrentPlayer.PlayerCollider))
			{
				// If attack hits, daze player and switch to the jab
				PlayerDamageResult(CurrentBoss->ShoutDamage);
				CurrentPlayer.CurrentPlayerStats.MoveSpeed /= 3.0f;
				CurrentPlayer.CurrentPlayerStats.HasteTimer = 2;
				CurrentBoss->CurrentState = Jab;
				CurrentBoss->InnerState = Start;
				CurrentBoss->cooldownTimer = 0.0f;
				CurrentBoss->BodySprite->SpriteTexture = LoadTexture("TextureFiles/TempHandGuy.png");
			}
			else
			{
				CurrentBoss->InnerState = End;
				CurrentBoss->cooldownTimer = 0.0f;
			}
			break;
			// Move if the shout doesn't hit
		case End:
			//printf("SHOUT TIME END\n");
			CurrentBoss->BodySprite->SpriteTexture = LoadTexture("TextureFiles/TempHandGuy.png");

			CurrentBoss->cooldownTimer += GetDeltaTime();
			if(CurrentBoss->cooldownTimer > 2.0f)
			{
				CurrentBoss->cooldownTimer = 0.0f;
				CurrentBoss->InnerState = Attack;
				CurrentBoss->CurrentState = Move;
				CurrentBoss->InnerState = Start;
			}
			break;
		}
		break;
	case Question:
		switch(CurrentBoss->InnerState)
		{
		case Start:
			//printf("QUESTION TIME START\n");
			numProjectiles = 0;
			projectileAngle = 0;
			CurrentBoss->InnerState = Attack;
			break;
		case Attack:
			//printf("QUESTION TIME START\n");
			CurrentBoss->cooldownTimer += GetDeltaTime();
			// Throws out ? every 0.75s
			if(CurrentBoss->cooldownTimer >= 0.75f)
			{
				// Creates the projectile and aims to in the direction of the player in a cone
				if(CurrentPlayer.Position.x > CurrentBoss->Position.x)
				{
					projectileAngle = 0;
					CurrentProjectile = CreateProjectile("TextureFiles/QuestionProjectile.png", 120, 120, CurrentBoss->Position.x, CurrentBoss->Position.y, Arrow, WeaponEnemy, projectileID++, CurrentBoss->QuestionDamage, 500, projectileAngle + (numProjectiles * 12 * FOX_PI / 180.0f));
					FreeSprite(CurrentProjectile->ProjectileSprite);
					CurrentProjectile->ProjectileSprite = (Sprite *)CreateSprite("TextureFiles/QuestionProjectile.png", 120, 120, 36, 3, 3, CurrentBoss->Position.x, CurrentBoss->Position.y);
					CurrentProjectile->ProjectileSprite->AnimationSpeed = 4;
				}
				// Creates the projectile and aims to in the direction of the player in a cone
				else
				{
					projectileAngle = FOX_PI;
					CurrentProjectile = CreateProjectile("TextureFiles/QuestionProjectile.png", 120, 120, CurrentBoss->Position.x, CurrentBoss->Position.y, Arrow, WeaponEnemy, projectileID++, CurrentBoss->QuestionDamage, 500, projectileAngle - (float)(numProjectiles * 12 * FOX_PI / 180.0f));
					FreeSprite(CurrentProjectile->ProjectileSprite);
					CurrentProjectile->ProjectileSprite = (Sprite *)CreateSprite("TextureFiles/QuestionProjectile.png", 120, 120, 36, 3, 3, CurrentBoss->Position.x, CurrentBoss->Position.y);
					CurrentProjectile->ProjectileSprite->AnimationSpeed = 4;
				}

				CurrentBoss->cooldownTimer = 0.0f;
				++numProjectiles;
			}

			// Change to end once enough projectiles are created
			if(numProjectiles >= 5)
			{
				CurrentBoss->InnerState = End;
				CurrentBoss->cooldownTimer = 0.0f;
			}
			break;
		case End:
			//printf("QUESTION TIME START\n");
			CurrentBoss->cooldownTimer += GetDeltaTime();

			if(CurrentBoss->cooldownTimer >= 2)
			{
				CurrentBoss->CurrentState = Move;
				CurrentBoss->InnerState = Start;
			}

			break;
		}
		break;
	case Move:
		switch(CurrentBoss->InnerState)
		{
		case Start:
			//printf("MOVE TIME START\n");
			movementPicker = rand() % 100;
			switch(CurrentBoss->PositionState)
			{
			case A:
				// Choose a new state based on the random number
				if(movementPicker > 80)
					CurrentBoss->PositionState = C;		// 20% chance
				else if(movementPicker > 40)
					CurrentBoss->PositionState = B;		// 40% chance
				else
					CurrentBoss->PositionState = D;		// 40% chance;
				break;
			case B:
				// Choose a new state based on the random number
				if(movementPicker > 80)
					CurrentBoss->PositionState = C;		// 20% chance
				else if(movementPicker > 40)
					CurrentBoss->PositionState = A;		// 40% chance
				else
					CurrentBoss->PositionState = D;		// 40% chance
				break;
			case C:
				// Choose a new state based on the random number
				if(movementPicker > 90)
					CurrentBoss->PositionState = A;		// 10% chance
				else if(movementPicker > 80)
					CurrentBoss->PositionState = B;		// 10% chance
				else
					CurrentBoss->PositionState = D;		// 80% chance
				break;
			case D:
				// Choose a new state based on the random number
				if(movementPicker > 90)
					CurrentBoss->PositionState = C;		// 10% chance
				else if(movementPicker > 45)
					CurrentBoss->PositionState = B;		// 45% chance
				else
					CurrentBoss->PositionState = A;		// 45% chance
				break;
			}

			// Do the movement next
			CurrentBoss->InnerState = Attack;
			break;
		case Attack:
			//printf("MOVE TIME START\n");
			switch(CurrentBoss->PositionState)
			{
			// Left side
			case A:
				CurrentBoss->dropDown = TRUE;

				if(CurrentBoss->Position.x > -775.0f)
					CurrentBoss->Position.x -= 750 * GetDeltaTime();
				else
					CurrentBoss->InnerState = End;
				break;
			// Right side
			case B:
				CurrentBoss->dropDown = TRUE;

				if(CurrentBoss->Position.x < 775.0f)
					CurrentBoss->Position.x += 750 * GetDeltaTime();
				else
					CurrentBoss->InnerState = End;
				break;
			// Under platform
			case C:
				CurrentBoss->dropDown = TRUE;

				// Move to the middle
				if(CurrentBoss->Position.x < -20.0f)
					CurrentBoss->Position.x += 750 * GetDeltaTime();
				else if(CurrentBoss->Position.x > 20.0f)
					CurrentBoss->Position.x -= 750 * GetDeltaTime();
				else
					CurrentBoss->InnerState = End;
				break;
			// On platform
			case D:
				CurrentBoss->dropDown = FALSE;

				// Move to the middle
				if(CurrentBoss->Position.x < -20.0f)
					CurrentBoss->Position.x += 750 * GetDeltaTime();
				else if(CurrentBoss->Position.x > 20.0f)
					CurrentBoss->Position.x -= 750 * GetDeltaTime();
				else if(CurrentBoss->Position.y > 0 && CurrentBoss->HandGuyRigidBody.onGround)
					CurrentBoss->InnerState = End;

				// Jump up to the platform if needed
				if(CurrentBoss->Position.y <= GROUNDLEVEL + CurrentBoss->BodySprite->Height / 4 && (CurrentBoss->Position.x < 200 && CurrentBoss->Position.x > -200))
				{
					// Set y velocity for jumping
					Vec2 velocity;
					CurrentBoss->Position.y += 3;
					Vec2Set(&velocity, 0.0f, 1200.0f);
					ApplyVelocity(&CurrentBoss->HandGuyRigidBody, &velocity);
				}
				
				break;
			}
			CurrentBoss->cooldownTimer = 0.0f;
			break;
		case End:
			//printf("MOVE TIME START\n");
			CurrentBoss->InnerState = Start;
			CurrentBoss->CurrentState = Cooldown;
			CurrentBoss->cooldownTimer = 0.0f;
			break;
		}
		break;
	case Cooldown:
		//printf("CD TIME\n");
		CurrentBoss->cooldownTimer += GetDeltaTime();
		CurrentBoss->dropDown = FALSE;
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

	// Set acceleration to zero
	ZeroAcceleration(&CurrentBoss->HandGuyRigidBody);

	//Brings the player back to the surface if something bad happens
	if(CurrentBoss->Position.y - CurrentBoss->BodySprite->Height / 4 < GROUNDLEVEL)
	{
		CurrentBoss->Position.y = GROUNDLEVEL + CurrentBoss->BodySprite->Height / 4;
	}
	//Stop vertical velocity and acceleration when the player lands on the floor
	if(CurrentBoss->Position.y - CurrentBoss->BodySprite->Height / 4 <= GROUNDLEVEL || CurrentBoss->HandGuyRigidBody.onGround)
	{
		Vec2Zero(&CurrentBoss->HandGuyRigidBody.Acceleration);
		Vec2Zero(&CurrentBoss->HandGuyRigidBody.Velocity);
		ZeroGravity(&CurrentBoss->HandGuyRigidBody);
	}
	//Set gravity if not on floor or on a platform
	else
	{
		SetGravity(&CurrentBoss->HandGuyRigidBody, 0.0f, FOX_GRAVITY_Y);
	}

	//Update velocity and acceleration
	UpdateVelocity(&CurrentBoss->HandGuyRigidBody);
	Vec2Scale(&velocityTime, &CurrentBoss->HandGuyRigidBody.Velocity, GetDeltaTime());
	
	Vec2Add(&CurrentBoss->Position, &CurrentBoss->Position, &velocityTime);

	//Updates the collision box
	UpdateCollisionPosition(&CurrentBoss->BossCollider, &CurrentBoss->Position);
	CurrentBoss->HandGuyRigidBody.onGround = FALSE;

	CurrentBoss->BodySprite->Position = CurrentBoss->Position;
	CurrentBoss->JabAttack.Position.y = CurrentBoss->Position.y;
	CurrentBoss->JabSprite->Position.y = CurrentBoss->Position.y;

	if(CurrentPlayer.Position.x > CurrentBoss->Position.x)
	{
		// Flip to face player
		CurrentBoss->BodySprite->FlipX = FALSE;
		CurrentBoss->JabSprite->Position.x = CurrentBoss->Position.x + CurrentBoss->JabAttack.width / 2;
		CurrentBoss->JabSprite->FlipX = FALSE;
		CurrentBoss->JabAttack.Position.x = CurrentBoss->Position.x + CurrentBoss->JabAttack.width / 2;
		
	}
	else
	{
		// Flip to face player
		CurrentBoss->BodySprite->FlipX = TRUE;
		CurrentBoss->JabSprite->Position.x = CurrentBoss->Position.x - CurrentBoss->JabAttack.width / 2;
		CurrentBoss->JabSprite->FlipX = TRUE;
		CurrentBoss->JabAttack.Position.x = CurrentBoss->Position.x - CurrentBoss->JabAttack.width / 2;
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
void DetectHandGuyBossCollision(HandGuyBoss *CurrentBoss)
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
				HandGuyPlatformCollision(CurrentBoss, pList);
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
					HandGuyBossCollideWeapon(CurrentBoss);
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
void HandGuyBossCollideWeapon(HandGuyBoss *CurrentBoss)
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

void HandGuyPlatformCollision(HandGuyBoss* CurrentBoss, Platform* CurrentPlatform)
{
	if(CurrentBoss->dropDown)
		return;
	if(CurrentBoss->HandGuyRigidBody.Velocity.y <= 0)
	{
		if(CurrentBoss->BossCollider.Position.y + CurrentBoss->BossCollider.Offset.y - CurrentBoss->BossCollider.height / 2.0f > CurrentPlatform->PlatformCollider.Position.y + CurrentPlatform->PlatformCollider.Offset.y)
		{
			if(CurrentBoss->HandGuyRigidBody.Velocity.y != 0 && CurrentPlatform->PlatformCollider.collisionGroup == PlatformType)
			{
				CurrentBoss->Position.y = CurrentPlatform->PlatformCollider.Position.y + CurrentPlatform->PlatformCollider.Offset.y + CurrentPlatform->PlatformCollider.height / 2 - CurrentBoss->BossCollider.Offset.y + CurrentBoss->BossCollider.height / 2 - 0.01f;
				CurrentBoss->HandGuyRigidBody.onGround = TRUE;
			}
			else if(CurrentBoss->HandGuyRigidBody.Velocity.y != 0 && CurrentPlatform->PlatformCollider.collisionGroup == BounceType)
			{
				if(CurrentBoss->HandGuyRigidBody.Velocity.y > -(CurrentPlatform->PlatformRigidBody.Restitution + CurrentBoss->HandGuyRigidBody.Restitution))
				{
					CurrentBoss->Position.y = CurrentPlatform->PlatformCollider.Position.y + CurrentPlatform->PlatformCollider.Offset.y + CurrentPlatform->PlatformCollider.height / 2 - CurrentBoss->BossCollider.Offset.y + CurrentBoss->BossCollider.height / 2 - 0.01f;
					CurrentBoss->HandGuyRigidBody.onGround = TRUE;
				}
				else
				{
					CurrentBoss->Position.y = CurrentPlatform->PlatformCollider.Position.y + CurrentPlatform->PlatformCollider.Offset.y + CurrentPlatform->PlatformCollider.height / 2 - CurrentBoss->BossCollider.Offset.y + CurrentBoss->BossCollider.height / 2 + 0.5f;
					BounceObject(&CurrentBoss->HandGuyRigidBody, &CurrentPlatform->PlatformRigidBody);
					
				}
			}
			else
				CurrentBoss->HandGuyRigidBody.onGround = TRUE;
		}
	}
}
