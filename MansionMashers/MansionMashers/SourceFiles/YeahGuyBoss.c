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

static enum InnerState {Start, Attack, End };
static enum PositionState { A, B, C, D, E };
static enum HeadColor {RedHead, GreenHead, BlueHead };
static enum YeahGuyState { Cooldown, AOE, ProjectYeah, Pound, Vault };

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
	LoadTexture("TextureFiles/TempYeahGuyShout.png");
	LoadTexture("TextureFiles/YeahGuyBody.png");
	LoadTexture("TextureFiles/YeahGuyRedHead.png");
	LoadTexture("TextureFiles/YeahGuyBlueHead.png");
	LoadTexture("TextureFiles/YeahGuyGreenHead.png");
	LoadTexture("TextureFiles/YeahGuyLegLower.png");
	LoadTexture("TextureFiles/YeahGuyLegUpper.png");
}

/*************************************************************************/
/*!
	\brief
	Handles the collision between an enemy and the attacking weapon
	
	\param CurrentEnemy
	Pointer to the enemy object
*/
/*************************************************************************/
YeahGuyBoss* CreateYeahGuyBoss(float xPos, float yPos)
{
	YeahGuyBoss *CurrentBoss = (YeahGuyBoss *) CallocMyAlloc(1, sizeof(YeahGuyBoss));

	//Initialize boss struct
	Vec2Set(&CurrentBoss->Position, 580, 0);
	CurrentBoss->BodySprite = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 225, 300, 10, 1, 1, 580, 0);
	CurrentBoss->BodySprite->Visible = FALSE;
	CurrentBoss->BodySprite->FlipX = TRUE;

	CurrentBoss->playerHit = 0;
	CurrentBoss->MaxHealth = 1000;
	CurrentBoss->CurrentRedHealth = 1000;
	CurrentBoss->CurrentGreenHealth = 1000;
	CurrentBoss->CurrentBlueHealth = 1000;

	CurrentBoss->redHead = TRUE;
	CurrentBoss->greenHead = TRUE;
	CurrentBoss->blueHead = TRUE;

	// Sprites
	CreateYeahGuySprites(CurrentBoss);
	CurrentBoss->LegSinValue  = 0;
	CurrentBoss->TailSinValue = 0;
	CurrentBoss->Speed		  = 0;

	// Default starting states
	CurrentBoss->CurrentState = Cooldown;
	CurrentBoss->InnerState = Start;
	CurrentBoss->PositionState = E;

	// Armguy colliders
	CreateCollisionBox(&CurrentBoss->BossCollider, &CurrentBoss->Position, EnemyType, 150, 320, GetObjectID());

	// Physics stuff
	InitializeRigidBody(&CurrentBoss->YeahGuyRigidBody, FALSE, 150, 300);
	CurrentBoss->YeahGuyRigidBody.Mass = 5;

	CurrentBoss->playerHit = -1; // No need for a collision list
	CurrentBoss->cooldownTimer = 0;
	CurrentBoss->numHeads = 3;
	
	CurrentBoss->YeahAOEDamage = 30;
	CurrentBoss->YeahAOERadius = 600;

	CurrentBoss->YeahPoundDamage = 30;

	CurrentBoss->YeahProjectileDamage = 20;

	CurrentBoss->HeadAttacking = RedHead;

	//Sounds
	CurrentBoss->YeahGuyHit[0] = CreateSound("Sounds/YeahGuyHit1.mp3", SmallSnd);
	CurrentBoss->YeahGuyHit[1] = CreateSound("Sounds/YeahGuyHit2.mp3", SmallSnd);
	CurrentBoss->YeahGuyHit[2] = CreateSound("Sounds/YeahGuyPhrase1.mp3", SmallSnd);
	CurrentBoss->YeahGuyHit[3] = CreateSound("Sounds/YeahGuyPhrase2.mp3", SmallSnd);
	CurrentBoss->YeahGuyYell = CreateSound("Sounds/YeahGuyYell.mp3", SmallSnd);
	CurrentBoss->YeahGuyDie = CreateSound("Sounds/YeahGuyDie.mp3", SmallSnd);
	CurrentBoss->YeahGuySoundsPlay = FALSE;

	CurrentBoss->SmashSFX = CreateSound("Sounds/YGSmash.wav", SmallSnd);
	
	// ParticleSystems
	CurrentBoss->DamageParticle		= CreateFoxParticleSystem("TextureFiles/Particle.png",			CurrentBoss->Position.x, CurrentBoss->Position.y,	CurrentBoss->BodySprite->ZIndex + 1,	0,	5,		0.0f,	270,	90,		1.0f,	-5.0f,	100,	250,	50,		2.0f,	0.5f);
	CurrentBoss->PoundParticle[0]	= CreateFoxParticleSystem("TextureFiles/YeahGuyParticle.png",	CurrentBoss->Position.x, GROUNDLEVEL - 75.0f,		CurrentBoss->BodySprite->ZIndex + 1,	0,	5,		0.0f,	175,	10,		1.0f,	-5.0f,	25,		5,		750,	2.0f,	0.5f);
	CurrentBoss->PoundParticle[1]	= CreateFoxParticleSystem("TextureFiles/YeahGuyParticle.png",	CurrentBoss->Position.x, GROUNDLEVEL - 75.0f,		CurrentBoss->BodySprite->ZIndex + 1,	0,	5,		0.0f,	5,		10,		1.0f,	-5.0f,	25,		5,		750,	2.0f,	0.5f);
	CurrentBoss->AOEParticle		= CreateFoxParticleSystem("TextureFiles/YeahGuyParticle2.png",	CurrentBoss->Position.x, CurrentBoss->Position.y,	CurrentBoss->BodySprite->ZIndex - 5,	0,	100,	0.15f,	0,		360,	1.0f,	-5.0f,	25,		24,		2050,	0.3f,	1.0f);
	CurrentBoss->AOEParticle->RandomVelocity = FALSE;


	return CurrentBoss;
}

/*************************************************************************/
/*!
	\brief
	Creates the boss's sprites

	\param Object
	The boss to create
*/
/*************************************************************************/
void CreateYeahGuySprites(YeahGuyBoss *Object)
{
	Object->YeahGuySpriteParts.Body = (Sprite *) CreateSprite("TextureFiles/YeahGuyBody.png", 450.0f, 450.0f, Object->BodySprite->ZIndex, 1, 1, 0, 0);
	Object->YeahGuySpriteParts.HeadRed = (Sprite *) CreateSprite("TextureFiles/YeahGuyRedHead.png", 450.0f, 450.0f, Object->BodySprite->ZIndex + 1, 2, 1, 0, 0);
	Object->YeahGuySpriteParts.HeadRed->AnimationActive = FALSE;
	Object->YeahGuySpriteParts.HeadBlue = (Sprite *) CreateSprite("TextureFiles/YeahGuyBlueHead.png", 450.0f, 450.0f, Object->BodySprite->ZIndex + 2, 2, 1, 0, 0);
	Object->YeahGuySpriteParts.HeadBlue->AnimationActive = FALSE;
	Object->YeahGuySpriteParts.HeadGreen = (Sprite *) CreateSprite("TextureFiles/YeahGuyGreenHead.png", 450.0f, 450.0f, Object->BodySprite->ZIndex + 3, 2, 1, 0, 0);
	Object->YeahGuySpriteParts.HeadGreen->AnimationActive = FALSE;

	Object->YeahGuySpriteParts.LegUpper = (Sprite *) CreateSprite("TextureFiles/YeahGuyLegUpper.png", 192.0f, 192.0f, Object->BodySprite->ZIndex, 1, 1, 0, 0);

	Object->YeahGuySpriteParts.LegLower = (Sprite *) CreateSpriteNoMesh("TextureFiles/YeahGuyLegLower.png", 192.0f, 192.0f, Object->BodySprite->ZIndex, 1, 1, 0, 0);
	Object->YeahGuySpriteParts.LegLower->SpriteMesh = Object->YeahGuySpriteParts.LegUpper->SpriteMesh;

	Object->YeahGuySpriteParts.LegUpper2 = (Sprite *) CreateSpriteNoMesh("TextureFiles/YeahGuyLegUpper.png", 192.0f, 192.0f, Object->BodySprite->ZIndex, 1, 1, 0, 0);
	Object->YeahGuySpriteParts.LegUpper2->SpriteMesh = Object->YeahGuySpriteParts.LegUpper->SpriteMesh;

	Object->YeahGuySpriteParts.LegLower2 = (Sprite *) CreateSpriteNoMesh("TextureFiles/YeahGuyLegLower.png", 192.0f, 192.0f, Object->BodySprite->ZIndex, 1, 1, 0, 0);
	Object->YeahGuySpriteParts.LegLower2->SpriteMesh = Object->YeahGuySpriteParts.LegUpper->SpriteMesh;

	Object->YeahGuySpriteParts.Tail = (Sprite *) CreateSpriteNoMesh("TextureFiles/TailDog.png", 450.0f, 450.0f, Object->BodySprite->ZIndex, 1, 1, 0, 0);
	Object->YeahGuySpriteParts.Tail->SpriteMesh = Object->YeahGuySpriteParts.Body->SpriteMesh;

	Object->YeahGuySpriteParts.Tail->AnimationSpeed = (Object->Speed)/2 + 3;

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
	static int movementPicker = 0;

	CurrentBoss->numHeads = CurrentBoss->redHead + CurrentBoss->greenHead + CurrentBoss->blueHead;

	switch(CurrentBoss->numHeads)
	{
	case 1:
		CurrentBoss->YeahAOEDamage = 30;
		CurrentBoss->YeahAOERadius = 400;

		CurrentBoss->YeahPoundDamage = 30;

		CurrentBoss->YeahProjectileDamage = 45;
		break;
	case 2:
		CurrentBoss->YeahAOEDamage = 20;
		CurrentBoss->YeahAOERadius = 500;

		CurrentBoss->YeahPoundDamage = 20;

		CurrentBoss->YeahProjectileDamage = 30;
		break;
	case 3:
		CurrentBoss->YeahAOEDamage = 10;
		CurrentBoss->YeahAOERadius = 600;

		CurrentBoss->YeahPoundDamage = 10;

		CurrentBoss->YeahProjectileDamage = 15;
		break;
	}


	switch(CurrentBoss->CurrentState)
	{
	case AOE:
		switch(CurrentBoss->InnerState)
		{
		case Start:
			if(!CurrentBoss->YeahGuySoundsPlay && !CurrentBoss->YeahGuyYell->hasPlayed)
			{
				CurrentBoss->YeahGuyYell->hasPlayed = TRUE;
				PlayAudio(CurrentBoss->YeahGuyYell);
			}
			//printf("AOE TIME START\n");
			CurrentBoss->cooldownTimer += GetDeltaTime();

			if(CurrentBoss->cooldownTimer > 2.0f)
				CurrentBoss->InnerState = Attack;
			break;
		case Attack:
			CurrentBoss->YeahGuyYell->hasPlayed = FALSE;
			//printf("AOE TIME ATTACK\n");
			CurrentBoss->AOEParticle->amountTotal = 100;
			CurrentBoss->AOEParticle->emitVelocity = 1333 + CurrentBoss->numHeads * 333.33333f;

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
			CurrentBoss->cooldownTimer = 0.0f;
			CurrentBoss->CurrentState = Cooldown;
			CurrentBoss->InnerState = Start;
			break;
		}
		break;
	case ProjectYeah:
		switch(CurrentBoss->InnerState)
		{
		case Start:
			//printf("QUESTION TIME START\n");
			// Change boss sprite to attacking
			projectileAngle = (float)atan2(CurrentPlayer.Position.y - CurrentBoss->Position.y, CurrentPlayer.Position.x - CurrentBoss->Position.x);
			CurrentProjectile = CreateProjectile("TextureFiles/YeahProjectile.png", 210, 90, CurrentBoss->Position.x, CurrentBoss->Position.y + CurrentBoss->BodySprite->Height / 2, Arrow, WeaponEnemy, CurrentBoss->YeahProjectileDamage, 800, projectileAngle);

			PlayAudio(CurrentBoss->YeahGuyHit[(int)((rand() / (float)RAND_MAX) * 3.5)]);

			// Select the correct texture depending on which direction attacking
			if(CurrentBoss->Position.x > CurrentPlayer.Position.x)
			{
				CurrentProjectile->ProjectileSprite->SpriteTexture = LoadTexture("TextureFiles/YeahProjectileBack.png");
			}

			switch(numProjectiles)
			{
				case 0:
					if (!CurrentBoss->greenHead)
					{
						CurrentBoss->HeadAttacking = BlueHead;
						break;
					}
					if (!CurrentBoss->blueHead && !CurrentBoss->greenHead)
					{
						CurrentBoss->HeadAttacking = RedHead;
						break;
					}
					CurrentBoss->HeadAttacking = GreenHead;
					break;
				case 1:
					if (!CurrentBoss->greenHead)
					{
						CurrentBoss->HeadAttacking = RedHead;
						break;
					}
					if (!CurrentBoss->blueHead)
					{
						CurrentBoss->HeadAttacking = RedHead;
						break;
					}
					CurrentBoss->HeadAttacking = BlueHead;
					break;
				case 2:
					CurrentBoss->HeadAttacking = RedHead;
					break;

			}


			++numProjectiles;
			CurrentBoss->InnerState = Attack;
			break;
		case Attack:
			//printf("QUESTION TIME START\n");
			CurrentBoss->cooldownTimer += GetDeltaTime();
			// Waits 1 second
			if(CurrentBoss->cooldownTimer >= 1.0f)
			{
				CurrentBoss->cooldownTimer = 0.0f;
				CurrentBoss->InnerState = End;
			}
			break;
		case End:
			//printf("QUESTION TIME START\n");
			
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
			// Choose where to ground pound
			movementPicker = rand() % 2;
			if(movementPicker)
				CurrentBoss->PositionState = D;
			else
				CurrentBoss->PositionState = B;
			CurrentBoss->InnerState = Attack;
			break;
		case Attack:
			if(CurrentBoss->PositionState == B)
			{
				// Set velocity based on position
				if(CurrentBoss->Position.x < -300 && CurrentBoss->YeahGuyRigidBody.Velocity.x >= 0)
				{
					CurrentBoss->YeahGuyRigidBody.Velocity.x = 1000;
				}
				else if(CurrentBoss->Position.x > -250 && CurrentBoss->YeahGuyRigidBody.Velocity.x <= 0)
				{
					CurrentBoss->YeahGuyRigidBody.Velocity.x = -1000;
				}
				// X position reached, time to pound down
				else
				{
					CurrentBoss->Position.x = -275;
					CurrentBoss->InnerState = End;
				}

			}
			else if(CurrentBoss->PositionState == D)
			{
				// Set velocity based on position
				if(CurrentBoss->Position.x < 300 && CurrentBoss->YeahGuyRigidBody.Velocity.x >= 0)
				{
					CurrentBoss->YeahGuyRigidBody.Velocity.x = 1000;
				}
				else if(CurrentBoss->Position.x > 250 && CurrentBoss->YeahGuyRigidBody.Velocity.x <= 0)
				{
					CurrentBoss->YeahGuyRigidBody.Velocity.x = -1000;
				}
				// X position reached, time to pound down
				else
				{
					CurrentBoss->Position.x = 275;
					CurrentBoss->InnerState = End;
				}
			}

			// Jump if on a platform
			if(CurrentBoss->YeahGuyRigidBody.onGround)
			{
				// Set y velocity for jumping
				Vec2 velocity;
				CurrentBoss->Position.y += 3;
				Vec2Set(&velocity, 0.0f, 1500.0f);
				ApplyVelocity(&CurrentBoss->YeahGuyRigidBody, &velocity);
				CurrentBoss->YeahGuyRigidBody.onGround = FALSE;
			}

			break;
		case End:
			// Stop horizontal velocity and ground pound down.
			CurrentBoss->YeahGuyRigidBody.Velocity.x = 0;
			CurrentBoss->YeahGuyRigidBody.Gravity.y = CurrentBoss->YeahGuyRigidBody.Gravity.y * 2;
			
			// When the boss hits the floor
			if(CurrentBoss->Position.y - CurrentBoss->BodySprite->Height / 4 <= GROUNDLEVEL)
			{
				// Set the boss to be on the ground
				CurrentBoss->Position.y = GROUNDLEVEL + CurrentBoss->BodySprite->Height / 4;
				CurrentBoss->BossCollider.Position.y = CurrentBoss->Position.y;

				PlayAudio(CurrentBoss->SmashSFX);
				CurrentBoss->PoundParticle[0]->amountTotal = 25;
				CurrentBoss->PoundParticle[1]->amountTotal = 25;

				// Check if the ground pound hit the player
				if(CollisionRectangles(&CurrentBoss->BossCollider, &CurrentPlayer.PlayerCollider) || CurrentPlayer.Position.y <= GROUNDLEVEL)
				{
					PlayerDamageResult(CurrentBoss->YeahPoundDamage);
				}

				// Go to cooldown state to pick the next move
				CurrentBoss->CurrentState = Cooldown;
				CurrentBoss->InnerState = Start;
				CurrentBoss->cooldownTimer = 0.0f;
			}

			break;
		}
		break;
	case Vault:
		switch(CurrentBoss->InnerState)
		{
		case Start:
			// Choose where to ground pound
			movementPicker = rand() % 3;

			CurrentBoss->InnerState = Attack;
			break;
		case Attack:
			if(CurrentBoss->PositionState == B)
			{
				// Vault to A
				if(movementPicker == 1)
				{
					// Set velocity based on position
					if(CurrentBoss->Position.x < -630 && CurrentBoss->YeahGuyRigidBody.Velocity.x >= 0)
					{
						CurrentBoss->YeahGuyRigidBody.Velocity.x = 250;
					}
					else if(CurrentBoss->Position.x > -530 && CurrentBoss->YeahGuyRigidBody.Velocity.x <= 0)
					{
						CurrentBoss->YeahGuyRigidBody.Velocity.x = -250;
					}
					// X position reached, time to pound down
					else if(CurrentBoss->YeahGuyRigidBody.onGround)
					{
						CurrentBoss->YeahGuyRigidBody.Velocity.x = 0;
						CurrentBoss->PositionState = A;
						CurrentBoss->InnerState = End;
					}

					// Jump if on the ground
					if(CurrentBoss->Position.y - CurrentBoss->BodySprite->Height / 4 <= GROUNDLEVEL)
					{
						// Set y velocity for jumping
						Vec2 velocity;
						CurrentBoss->Position.y += 3;
						Vec2Set(&velocity, 0.0f, 1500.0f);
						ApplyVelocity(&CurrentBoss->YeahGuyRigidBody, &velocity);
						CurrentBoss->YeahGuyRigidBody.onGround = FALSE;
					}
				}
				// Vault to C
				else if(movementPicker == 2)
				{
					// Set velocity based on position
					if(CurrentBoss->Position.x < -50 && CurrentBoss->YeahGuyRigidBody.Velocity.x >= 0)
					{
						CurrentBoss->YeahGuyRigidBody.Velocity.x = 250;
					}
					else if(CurrentBoss->Position.x > 50 && CurrentBoss->YeahGuyRigidBody.Velocity.x <= 0)
					{
						CurrentBoss->YeahGuyRigidBody.Velocity.x = -250;
					}
					// X position reached, time to pound down
					else if(CurrentBoss->YeahGuyRigidBody.onGround)
					{
						CurrentBoss->YeahGuyRigidBody.Velocity.x = 0;
						CurrentBoss->PositionState = C;
						CurrentBoss->InnerState = End;
					}

					// Jump if on the ground
					if(CurrentBoss->Position.y - CurrentBoss->BodySprite->Height / 4 <= GROUNDLEVEL)
					{
						// Set y velocity for jumping
						Vec2 velocity;
						CurrentBoss->Position.y += 3;
						Vec2Set(&velocity, 0.0f, 1800.0f);
						ApplyVelocity(&CurrentBoss->YeahGuyRigidBody, &velocity);
						CurrentBoss->YeahGuyRigidBody.onGround = FALSE;
					}
				}
				// Vault to E
				else
				{
					// Set velocity based on position
					if(CurrentBoss->Position.x < 530 && CurrentBoss->YeahGuyRigidBody.Velocity.x >= 0)
					{
						CurrentBoss->YeahGuyRigidBody.Velocity.x = 670;
					}
					else if(CurrentBoss->Position.x > 630 && CurrentBoss->YeahGuyRigidBody.Velocity.x <= 0)
					{
						CurrentBoss->YeahGuyRigidBody.Velocity.x = -670;
					}
					// X position reached, time to pound down
					else if(CurrentBoss->YeahGuyRigidBody.onGround)
					{
						CurrentBoss->YeahGuyRigidBody.Velocity.x = 0;
						CurrentBoss->PositionState = E;
						CurrentBoss->InnerState = End;
					}

					// Jump if on the ground
					if(CurrentBoss->Position.y - CurrentBoss->BodySprite->Height / 4 <= GROUNDLEVEL)
					{
						// Set y velocity for jumping
						Vec2 velocity;
						CurrentBoss->Position.y += 3;
						Vec2Set(&velocity, 0.0f, 1550.0f);
						ApplyVelocity(&CurrentBoss->YeahGuyRigidBody, &velocity);
						CurrentBoss->YeahGuyRigidBody.onGround = FALSE;
					}
				}
			}
			else if(CurrentBoss->PositionState == D)
			{
				// Vault to A
				if(movementPicker == 1)
				{
					// Set velocity based on position
					if(CurrentBoss->Position.x < -630 && CurrentBoss->YeahGuyRigidBody.Velocity.x >= 0)
					{
						CurrentBoss->YeahGuyRigidBody.Velocity.x = 670;
					}
					else if(CurrentBoss->Position.x > -530 && CurrentBoss->YeahGuyRigidBody.Velocity.x <= 0)
					{
						CurrentBoss->YeahGuyRigidBody.Velocity.x = -670;
					}
					// X position reached, time to pound down
					else if(CurrentBoss->YeahGuyRigidBody.onGround)
					{
						CurrentBoss->YeahGuyRigidBody.Velocity.x = 0;
						CurrentBoss->PositionState = A;
						CurrentBoss->InnerState = End;
					}

					// Jump if on the ground
					if(CurrentBoss->Position.y - CurrentBoss->BodySprite->Height / 4 <= GROUNDLEVEL)
					{
						// Set y velocity for jumping
						Vec2 velocity;
						CurrentBoss->Position.y += 3;
						Vec2Set(&velocity, 0.0f, 1550.0f);
						ApplyVelocity(&CurrentBoss->YeahGuyRigidBody, &velocity);
						CurrentBoss->YeahGuyRigidBody.onGround = FALSE;
					}
				}
				// Vault to C
				else if(movementPicker == 2)
				{
					// Set velocity based on position
					if(CurrentBoss->Position.x < -50 && CurrentBoss->YeahGuyRigidBody.Velocity.x >= 0)
					{
						CurrentBoss->YeahGuyRigidBody.Velocity.x = 250;
					}
					else if(CurrentBoss->Position.x > 50 && CurrentBoss->YeahGuyRigidBody.Velocity.x <= 0)
					{
						CurrentBoss->YeahGuyRigidBody.Velocity.x = -250;
					}
					// X position reached, time to pound down
					else if(CurrentBoss->YeahGuyRigidBody.onGround)
					{
						CurrentBoss->YeahGuyRigidBody.Velocity.x = 0;
						CurrentBoss->PositionState = C;
						CurrentBoss->InnerState = End;
					}

					// Jump if on the ground
					if(CurrentBoss->Position.y - CurrentBoss->BodySprite->Height / 4 <= GROUNDLEVEL)
					{
						// Set y velocity for jumping
						Vec2 velocity;
						CurrentBoss->Position.y += 3;
						Vec2Set(&velocity, 0.0f, 1800.0f);
						ApplyVelocity(&CurrentBoss->YeahGuyRigidBody, &velocity);
						CurrentBoss->YeahGuyRigidBody.onGround = FALSE;
					}
				}
				// Vault to E
				else
				{
					// Set velocity based on position
					if(CurrentBoss->Position.x < 530 && CurrentBoss->YeahGuyRigidBody.Velocity.x >= 0)
					{
						CurrentBoss->YeahGuyRigidBody.Velocity.x = 250;
					}
					else if(CurrentBoss->Position.x > 630 && CurrentBoss->YeahGuyRigidBody.Velocity.x <= 0)
					{
						CurrentBoss->YeahGuyRigidBody.Velocity.x = -250;
					}
					// X position reached, time to pound down
					else if(CurrentBoss->YeahGuyRigidBody.onGround)
					{
						CurrentBoss->YeahGuyRigidBody.Velocity.x = 0;
						CurrentBoss->PositionState = E;
						CurrentBoss->InnerState = End;
					}

					// Jump if on the ground
					if(CurrentBoss->Position.y - CurrentBoss->BodySprite->Height / 4 <= GROUNDLEVEL)
					{
						// Set y velocity for jumping
						Vec2 velocity;
						CurrentBoss->Position.y += 3;
						Vec2Set(&velocity, 0.0f, 1500.0f);
						ApplyVelocity(&CurrentBoss->YeahGuyRigidBody, &velocity);
						CurrentBoss->YeahGuyRigidBody.onGround = FALSE;
					}
				}
			}
			break;
		case End:
			CurrentBoss->CurrentState = Cooldown;
			CurrentBoss->InnerState = Start;
			CurrentBoss->cooldownTimer = 0.0f;
			break;
		}
		break;
	case Cooldown:
		switch(CurrentBoss->InnerState)
		{
		case Start:
			CurrentBoss->cooldownTimer += GetDeltaTime();
			if(CurrentBoss->cooldownTimer > 3.0f / CurrentBoss->numHeads)
			{
				CurrentBoss->InnerState = Attack;
				CurrentBoss->cooldownTimer = 0.0f;
			}

			break;
		case Attack:
			// Choses the next move to use
			movementPicker = rand() % 3;

			switch(CurrentBoss->PositionState)
			{
			case A:
				// 33$ chance to go to a new location
				if(movementPicker == 1)
				{
					CurrentBoss->CurrentState = AOE;
					CurrentBoss->AOEParticle->amountTotal = -1;
					CurrentBoss->AOEParticle->emitVelocity = 250;
				}
				else if(movementPicker == 2)
					CurrentBoss->CurrentState = ProjectYeah;
				else
					CurrentBoss->CurrentState = Pound;
				break;
			case B:
				// 33$ chance to go to a new location
				if(movementPicker == 1)
				{
					CurrentBoss->CurrentState = AOE;
					CurrentBoss->AOEParticle->amountTotal = -1;
					CurrentBoss->AOEParticle->emitVelocity = 250;
				}
				else if(movementPicker == 2)
					CurrentBoss->CurrentState = ProjectYeah;
				else
					CurrentBoss->CurrentState = Vault;
				break;
			case C:
				// 33$ chance to go to a new location
				if(movementPicker == 1)
				{
					CurrentBoss->CurrentState = AOE;
					CurrentBoss->AOEParticle->amountTotal = -1;
					CurrentBoss->AOEParticle->emitVelocity = 250;
				}
				else if(movementPicker == 2)
					CurrentBoss->CurrentState = ProjectYeah;
				else
					CurrentBoss->CurrentState = Pound;
				break;
			case D:
				// 33$ chance to go to a new location
				if(movementPicker == 1)
				{
					CurrentBoss->CurrentState = AOE;
					CurrentBoss->AOEParticle->amountTotal = -1;
					CurrentBoss->AOEParticle->emitVelocity = 250;
				}
				else if(movementPicker == 2)
					CurrentBoss->CurrentState = ProjectYeah;
				else
					CurrentBoss->CurrentState = Vault;
				break;
			case E:
				// 33$ chance to go to a new location
				if(movementPicker == 1)
				{
					CurrentBoss->CurrentState = AOE;
					CurrentBoss->AOEParticle->amountTotal = -1;
					CurrentBoss->AOEParticle->emitVelocity = 250;
				}
				else if(movementPicker == 2)
					CurrentBoss->CurrentState = ProjectYeah;
				else
					CurrentBoss->CurrentState = Pound;
				break;
			}
			CurrentBoss->cooldownTimer = 0.0f;
			CurrentBoss->InnerState = Start;
			break;
		case End:
			break;
		}
		break;
	}

	// Particle Placement Update
	CurrentBoss->PoundParticle[0]->Position.x = CurrentBoss->Position.x - 25;
	CurrentBoss->PoundParticle[1]->Position.x = CurrentBoss->Position.x + 25;
	
	CurrentBoss->AOEParticle->Position.x = CurrentBoss->Position.x;
	CurrentBoss->AOEParticle->Position.y = CurrentBoss->Position.y + 100.0f;

	CurrentBoss->DamageParticle->Position = CurrentBoss->Position;

	// Speed for the animation
	CurrentBoss->Speed = (float)fabs(CurrentBoss->YeahGuyRigidBody.Velocity.x);

	// Sthap!
	if (CurrentBoss->Speed < 0.001f)
		CurrentBoss->LegSinValue = 0;

	// Do the animation!
	YeahGuyAnimation(CurrentBoss);

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
		//Vec2Zero(&CurrentBoss->YeahGuyRigidBody.Velocity);
		CurrentBoss->YeahGuyRigidBody.Velocity.y = 0;
		ZeroGravity(&CurrentBoss->YeahGuyRigidBody);
	}
	//Set gravity if not on floor or on a platform
	else
	{
		if(CurrentBoss->CurrentState != Pound || CurrentBoss->InnerState != End)
		{
			SetGravity(&CurrentBoss->YeahGuyRigidBody, 0.0f, FOX_GRAVITY_Y);
		}
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

	// Booleans for if head is alive
	if(CurrentBoss->CurrentRedHealth <= 0 && CurrentBoss->redHead)
	{
		CurrentBoss->CurrentRedHealth = 0;
		CurrentBoss->redHead = FALSE;
		PoofHead(CurrentBoss->YeahGuySpriteParts.HeadRed);
	}
	if(CurrentBoss->CurrentGreenHealth <= 0 && CurrentBoss->greenHead)
	{
		CurrentBoss->CurrentGreenHealth = 0;
		CurrentBoss->greenHead = FALSE;
		PoofHead(CurrentBoss->YeahGuySpriteParts.HeadGreen);
	}
	if(CurrentBoss->CurrentBlueHealth <= 0 && CurrentBoss->blueHead)
	{
		CurrentBoss->CurrentBlueHealth = 0;
		CurrentBoss->blueHead = FALSE;
		PoofHead(CurrentBoss->YeahGuySpriteParts.HeadBlue);
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
void DetectYeahGuyBossCollision(YeahGuyBoss *CurrentBoss, int CurrentBuff)
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
			if(hit && CurrentPlayer.isAttacking && CurrentPlayer.AttackType == 0)
			{
				// On start collision
				if(hitPrev <= 0)
				{
					CurrentBoss->playerHit = wList->WeaponAttack.collisionID * 10 + 1;
					//printf("NOT FOUND: %i\n", -hitPrev);
					YeahGuyBossCollideWeapon(CurrentBoss, CurrentBuff);
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
void YeahGuyBossCollideWeapon(YeahGuyBoss *CurrentBoss, int CurrentBuff)
{
	int damageDealt;
	char num[10];

	//bool for sounds
	int SoundIsPlaying = FALSE;
	int i;
	int randNum = ((int)((rand() / (float)RAND_MAX) * 60)) % 4;

	Vec3 textColor;
	TextGlyphs *FirstLetter;
	Vec3Set(&textColor, 1.0f, 1.0f, 1.0f);

	// Calculate damage including enemy's damage reduction
	if(Cheats)
		damageDealt = 9999;
	else
		damageDealt = CurrentPlayer.CurrentPlayerStats.Damage;
	
	// Particles
	CurrentBoss->DamageParticle->amountTotal += 5;

	// Deal damage to the specific head
	if(CurrentBoss->redHead && CurrentBuff == 1)
		CurrentBoss->CurrentRedHealth -= damageDealt * 2;
	else if(CurrentBoss->greenHead && CurrentBuff == 2)
		CurrentBoss->CurrentGreenHealth -= damageDealt * 2;
	else if(CurrentBoss->blueHead && CurrentBuff == 3)
		CurrentBoss->CurrentBlueHealth -= damageDealt * 2;
	else
		damageDealt = 0;

	sprintf(num, "-%d", damageDealt * 2);
	// Create Floating Combat Text
	FirstLetter = CreateText(num, (CurrentBoss->Position.x + rand() % 81 - 40), (CurrentBoss->Position.y + CurrentBoss->BodySprite->Height / 2), 80, textColor, Center, Border);
	AddFloatingText(FirstLetter);
	ChangeTextVisibility(FirstLetter);
	ChangeTextZIndex(FirstLetter, 201);

	//Voice Hit Reponse
	for(i = 0; i < 4; i++)
	{
		if(FoxSoundCheckIsPlaying(CurrentBoss->YeahGuyHit[i]))
			SoundIsPlaying = TRUE;
	}

	if(FoxSoundCheckIsPlaying(CurrentBoss->YeahGuyYell) || FoxSoundCheckIsPlaying(CurrentBoss->YeahGuyDie))
		SoundIsPlaying = TRUE;

	if(!SoundIsPlaying)
		PlayAudio(CurrentBoss->YeahGuyHit[randNum]);
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

	//bool for sounds
	int SoundIsPlaying = FALSE;
	int i;
	int randNum = ((int)((rand() / (float)RAND_MAX) * 60)) % 4;

	Vec3 textColor;
	TextGlyphs *FirstLetter;
	Vec3Set(&textColor, 1.0f, 0.0f, 0.0f);
	
	// Calculate damage
	if(Cheats)
		damageDealt = 0;
	else
	{
		damageDealt = (int)(damage * (1.0f - CurrentPlayer.CurrentPlayerStats.DamageReduction));
		if(damageDealt == 0)
			damageDealt = 1;
	}
	
	CurrentPlayer.CurrentPlayerStats.CurrentHealth -= damageDealt;
	sprintf(num, "-%d", damageDealt);

	//Voice Hit Reponse
	for(i = 0; i < 5; i++)
	{
		if(FoxSoundCheckIsPlaying(CurrentPlayer.CurrentPlayerSounds.VoiceHit[i]))
			SoundIsPlaying = TRUE;
	}

	if(!SoundIsPlaying)
		PlayAudio(CurrentPlayer.CurrentPlayerSounds.VoiceHit[randNum]);

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

/*************************************************************************/
/*!
	\brief
	Frees all the boss sprites etc
	
	\param CurrentBoss
	Pointer to the current boss
*/
/*************************************************************************/
void FreeYeahGuyBoss(YeahGuyBoss* CurrentBoss)
{
	// Once Luke makes the art we can do something with this ^_^
	// FREES ALL THE THINGS!!!!
	PoofSelf(CurrentBoss->BodySprite);

	FreeSprite(CurrentBoss->BodySprite);
	FreeSprite(CurrentBoss->YeahGuySpriteParts.Body);
	FreeSprite(CurrentBoss->YeahGuySpriteParts.HeadBlue);
	FreeSprite(CurrentBoss->YeahGuySpriteParts.HeadGreen);
	FreeSprite(CurrentBoss->YeahGuySpriteParts.HeadRed);
	FreeSprite(CurrentBoss->YeahGuySpriteParts.LegLower);
	FreeSprite(CurrentBoss->YeahGuySpriteParts.LegLower2);
	FreeSprite(CurrentBoss->YeahGuySpriteParts.LegUpper);
	FreeSprite(CurrentBoss->YeahGuySpriteParts.LegUpper2);
	FreeSprite(CurrentBoss->YeahGuySpriteParts.Tail);
}

/*************************************************************************/
/*!
	\brief
	Removes the Head in a fashionable way, really fashionable... wow
*/
/*************************************************************************/
void PoofHead(Sprite *Head)
{
	//Poof the head away
	ParticleSystem *Poof = CreateFoxParticleSystem("TextureFiles/Particle.png", Head->Position.x, Head->Position.y + 150.0f, Head->ZIndex + 5, 20, 20, 0.0f, 0, 360, 1.0f, -5.0f, 25, 24, 20, 2.0f, 1.0f);
	Poof->emitDisplacementX = 50;
	Poof->emitDisplacementY = 50;
	Poof->emitScale = 1.5f;
	Poof->emitLife = 1.0f;
	Poof->emitThenDestroy = TRUE;
}

/*************************************************************************/
/*!
	\brief
	Removes the Boss in a fashionable way
*/
/*************************************************************************/
static void PoofSelf(Sprite *Boss)
{
	//Poof the self away
	ParticleSystem *Poof = CreateFoxParticleSystem("TextureFiles/Particle.png", Boss->Position.x, Boss->Position.y, Boss->ZIndex + 6, 40, 40, 0.0f, 0, 360, 1.0f, -5.0f, 25, 24, 50, 2.0f, 1.0f);
	Poof->emitDisplacementX = 200;
	Poof->emitDisplacementY = 200;
	Poof->emitScale = 1.5f;
	Poof->emitLife = 2.0f;
	Poof->emitThenDestroy = TRUE;
}
