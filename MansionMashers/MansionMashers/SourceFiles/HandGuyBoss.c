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
#include "../HeaderFiles/Movement.h"

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

	LoadTexture("TextureFiles/QuickJab.png");
	LoadTexture("TextureFiles/HandGuyBody.png");
	LoadTexture("TextureFiles/HandGuyLegLower.png");
	LoadTexture("TextureFiles/HandGuyLegUpper.png");
	LoadTexture("TextureFiles/HandGuyArmUpper.png");
	LoadTexture("TextureFiles/HandGuyArmLowerIn.png");
	LoadTexture("TextureFiles/HandGuyArmLowerOut.png");
	LoadTexture("TextureFiles/YeahGuyParticle2.png");
	LoadTexture("TextureFiles/Particle.png");
}

/*************************************************************************/
/*!
	\brief
	Handles the collision between an enemy and the attacking weapon
	
	\param CurrentEnemy
	Pointer to the enemy object
*/
/*************************************************************************/
HandGuyBoss* CreateHandGuyBoss(float xPos, float yPos)
{
	HandGuyBoss *CurrentBoss = (HandGuyBoss *) CallocMyAlloc(1, sizeof(HandGuyBoss));

	//Initialize boss struct
	Vec2Set(&CurrentBoss->Position, 700, -200);
	CurrentBoss->BodySprite = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 150, 300, 10, 1, 1, 700, -200);
	CurrentBoss->BodySprite->FlipX = TRUE;
	CurrentBoss->BodySprite->Visible = FALSE;
	CurrentBoss->JabSprite = (Sprite *) CreateSprite("TextureFiles/QuickJab.png", 100, 100, 11, 4, 4, 580, -120);
	CurrentBoss->JabSprite->FlipX = TRUE;
	CurrentBoss->JabSprite->AnimationSpeed = 3;
	CurrentBoss->JabSprite->Visible = FALSE;

	CurrentBoss->playerHit = 0;
	if(CurrentPlayer.armClear)
	{
		CurrentBoss->MaxHealth = 2000;
		CurrentBoss->CurrentHealth = 2000;

		CurrentBoss->ShoutDamage = 40;
		CurrentBoss->QuestionDamage = 20;
		CurrentBoss->JabDamage = 30;
	}
	else
	{
		CurrentBoss->MaxHealth = 1000;
		CurrentBoss->CurrentHealth = 1000;

		CurrentBoss->ShoutDamage = 30;
		CurrentBoss->QuestionDamage = 10;
		CurrentBoss->JabDamage = 20;
	}

	CurrentBoss->CurrentState = Cooldown;
	CurrentBoss->InnerState = Start;
	CurrentBoss->PositionState = B;
	CurrentBoss->Speed = 0;

	// Animation Bool
	CurrentBoss->QuestionAttackAnimation = FALSE;

	// Armguy colliders
	CurrentBoss->ShoutRadius = 600.0f;
	CreateCollisionBox(&CurrentBoss->BossCollider, &CurrentBoss->Position, EnemyType, 100, 300, GetObjectID());
	CurrentBoss->BossCollider.Offset.y = CurrentBoss->BossCollider.height / 4;
	CreateCollisionBox(&CurrentBoss->JabAttack, &CurrentBoss->Position, WeaponEnemy, 200, 200, GetObjectID()); 

	// Sets the initial position of all colliders
	CurrentBoss->JabAttack.Position.x = CurrentBoss->Position.x - CurrentBoss->JabAttack.width / 2;
	CurrentBoss->JabAttack.Position.y = CurrentBoss->Position.y;

	// Physics stuff
	InitializeRigidBody(&CurrentBoss->HandGuyRigidBody, FALSE, 150, 300);
	CurrentBoss->HandGuyRigidBody.Mass = 7;
	CurrentBoss->dropDown = FALSE;

	CurrentBoss->playerHit = -1; // No need for a collision list
	CurrentBoss->cooldownTimer = 0;

	// Player Sprites
	CreateHandGuySprites(CurrentBoss);

	//Sounds
	CurrentBoss->HandGuyHit[0] = CreateSound("Sounds/HandGuyHit1.mp3", SmallSnd);
	CurrentBoss->HandGuyHit[1] = CreateSound("Sounds/HandGuyHit2.mp3", SmallSnd);
	CurrentBoss->HandGuyHit[2] = CreateSound("Sounds/HandGuyHit3.mp3", SmallSnd);
	CurrentBoss->HandGuyHit[3] = CreateSound("Sounds/HandGuyHit4.mp3", SmallSnd);
	CurrentBoss->HandGuyHit[4] = CreateSound("Sounds/HandGuyHit5.mp3", SmallSnd);
	CurrentBoss->HandGuyHit[5] = CreateSound("Sounds/HandGuyHit6.mp3", SmallSnd);

	CurrentBoss->HandGuyPhrase = CreateSound("Sounds/HandGuyPhrase.mp3", SmallSnd);
	CurrentBoss->PunchSFX = CreateSound("Sounds/HGPunch.mp3", SmallSnd);
	CurrentBoss->SwooshSFX = CreateSound("Sounds/HGSwoosh.mp3", SmallSnd);

	CurrentBoss->HandGuyYell = CreateSound("Sounds/HandGuyYell.mp3", SmallSnd);
	CurrentBoss->HandGuySoundsPlay = FALSE;

	// Particles
	CurrentBoss->DamageParticle		= CreateFoxParticleSystem("TextureFiles/Particle.png",			CurrentBoss->Position.x, CurrentBoss->Position.y,	CurrentBoss->BodySprite->ZIndex + 1,	0,	5,		0.0f,	270,	90,		1.0f,	-5.0f,	100,	250,	50,		2.0f,	0.5f);
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
void CreateHandGuySprites(HandGuyBoss *Object)
{
	Object->HandGuySpriteParts.Body = (Sprite *) CreateSprite("TextureFiles/HandGuyBody.png", 450.0f, 450.0f, Object->BodySprite->ZIndex, 1, 1, 0, 0);

	Object->HandGuySpriteParts.ArmUpper = (Sprite *) CreateSprite("TextureFiles/HandGuyArmUpper.png", 192.0f, 192.0f, Object->BodySprite->ZIndex + 2, 1, 1, 0, 0);

	Object->HandGuySpriteParts.ArmUpper2 = (Sprite *) CreateSpriteNoMesh("TextureFiles/HandGuyArmUpper.png", 192.0f, 192.0f, Object->BodySprite->ZIndex - 2, 1, 1, 0, 0);
	Object->HandGuySpriteParts.ArmUpper2->SpriteMesh = Object->HandGuySpriteParts.ArmUpper->SpriteMesh;

	Object->HandGuySpriteParts.ArmLower2 = (Sprite *) CreateSpriteNoMesh("TextureFiles/HandGuyArmLowerIn.png", 192.0f, 192.0f, Object->BodySprite->ZIndex - 2, 1, 1, 0, 0);
	Object->HandGuySpriteParts.ArmLower2->SpriteMesh = Object->HandGuySpriteParts.ArmUpper->SpriteMesh;

	Object->HandGuySpriteParts.LegUpper = (Sprite *) CreateSpriteNoMesh("TextureFiles/HandGuyLegUpper.png", 192.0f, 192.0f, Object->BodySprite->ZIndex, 1, 1, 0, 0);
	Object->HandGuySpriteParts.LegUpper->SpriteMesh = Object->HandGuySpriteParts.ArmUpper->SpriteMesh;

	Object->HandGuySpriteParts.LegLower = (Sprite *) CreateSpriteNoMesh("TextureFiles/HandGuyLegLower.png", 192.0f, 192.0f, Object->BodySprite->ZIndex, 1, 1, 0, 0);
	Object->HandGuySpriteParts.LegLower->SpriteMesh = Object->HandGuySpriteParts.ArmUpper->SpriteMesh;

	Object->HandGuySpriteParts.LegUpper2 = (Sprite *) CreateSpriteNoMesh("TextureFiles/HandGuyLegUpper.png", 192.0f, 192.0f, Object->BodySprite->ZIndex, 1, 1, 0, 0);
	Object->HandGuySpriteParts.LegUpper2->SpriteMesh = Object->HandGuySpriteParts.ArmUpper->SpriteMesh;

	Object->HandGuySpriteParts.LegLower2 = (Sprite *) CreateSpriteNoMesh("TextureFiles/HandGuyLegLower.png", 192.0f, 192.0f, Object->BodySprite->ZIndex, 1, 1, 0, 0);
	Object->HandGuySpriteParts.LegLower2->SpriteMesh = Object->HandGuySpriteParts.ArmUpper->SpriteMesh;

	Object->HandGuySpriteParts.Tail = (Sprite *) CreateSpriteNoMesh("TextureFiles/TailDog.png", 450.0f, 450.0f, Object->BodySprite->ZIndex, 1, 1, 0, 0);
	Object->HandGuySpriteParts.Tail->SpriteMesh = Object->HandGuySpriteParts.Body->SpriteMesh;

	Object->HandGuySpriteParts.Tail->AnimationSpeed = (Object->Speed)/2 + 3;

	Object->HandGuySpriteParts.ArmLower = (Sprite *) CreateSpriteNoMesh("TextureFiles/HandGuyArmLowerOut.png", 192.0f, 192.0f, Object->BodySprite->ZIndex + 2, 1, 1, 0, 0);
	Object->HandGuySpriteParts.ArmLower->SpriteMesh = Object->HandGuySpriteParts.ArmUpper->SpriteMesh;

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
	CurrentBoss->Speed = 0;

	switch(CurrentBoss->CurrentState)
	{
	case Jab:
		switch(CurrentBoss->InnerState)
		{
		case Start:
			//printf("JAB TIME START\n");
			CurrentBoss->dropDown = FALSE;
			CurrentBoss->cooldownTimer += GetDeltaTime();
			CurrentBoss->Speed = 1000.0f;
			// Move into damage range
			if(CurrentBoss->Position.x > CurrentPlayer.Position.x + CurrentBoss->JabAttack.width)
				CurrentBoss->Position.x -= CurrentBoss->Speed * GetDeltaTime();
			else if(CurrentBoss->Position.x < CurrentPlayer.Position.x - CurrentBoss->JabAttack.width)
				CurrentBoss->Position.x += CurrentBoss->Speed * GetDeltaTime();
			// Don't attack on the very first frame
			else if(CurrentBoss->cooldownTimer > GetDeltaTime() * 2 && CollisionRectangles(&CurrentPlayer.PlayerCollider, &CurrentBoss->JabAttack))
			{
				CurrentBoss->InnerState = Attack;
				CurrentBoss->cooldownTimer = 0.0f;
				CurrentBoss->InnerState = Attack;
			}

			// Jump up to the platform if needed
			if((CurrentPlayer.PlayerRigidBody.Velocity.y >= 0 && CurrentPlayer.Position.y > GROUNDLEVEL) && CurrentBoss->Position.y <= GROUNDLEVEL)
			{
				// Set y velocity for jumping
				Vec2 velocity;
				CurrentBoss->Position.y += 3;
				Vec2Set(&velocity, 0.0f, 1200.0f);
				ApplyVelocity(&CurrentBoss->HandGuyRigidBody, &velocity);
			}
			// Drop down if needed
			else
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
			if(!CurrentBoss->HandGuySoundsPlay && !CurrentBoss->HandGuyYell->hasPlayed)
			{
				CurrentBoss->HandGuyYell->hasPlayed = TRUE;
				PlayAudio(CurrentBoss->HandGuyYell);
			}

			CurrentBoss->AOEParticle->amountTotal = 100;
			CurrentBoss->AOEParticle->emitVelocity = 250;

			//printf("SHOUT TIME START\n");
			//CurrentBoss->BodySprite->SpriteTexture = LoadTexture("TextureFiles/TempHandGuyShout.png");
			CurrentBoss->cooldownTimer += GetDeltaTime();
			if(CurrentBoss->cooldownTimer > 2.0f)
				CurrentBoss->InnerState = Attack;
			break;
		case Attack:
			//printf("SHOUT TIME ATTACK\n");
			CurrentBoss->HandGuyYell->hasPlayed = FALSE;

			CurrentBoss->AOEParticle->amountTotal = 100;
			CurrentBoss->AOEParticle->emitVelocity = 2333;

			if(RectCircleCollision(&CurrentBoss->Position, CurrentBoss->ShoutRadius, &CurrentPlayer.PlayerCollider))
			{
				// If attack hits, daze player and switch to the jab
				PlayAudio(CurrentBoss->PunchSFX);
				PlayerDamageResult(CurrentBoss->ShoutDamage);
				CurrentPlayer.CurrentPlayerStats.MoveSpeed /= 3.0f;
				CurrentPlayer.CurrentPlayerStats.HasteTimer = 2;
				CurrentBoss->CurrentState = Jab;
				CurrentBoss->InnerState = Start;
				CurrentBoss->cooldownTimer = 0.0f;
				//CurrentBoss->BodySprite->SpriteTexture = LoadTexture("TextureFiles/TempHandGuy.png");
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
			//CurrentBoss->BodySprite->SpriteTexture = LoadTexture("TextureFiles/TempHandGuy.png");

			CurrentBoss->cooldownTimer += GetDeltaTime();
			if(CurrentBoss->cooldownTimer > 2.0f)
			{
				CurrentBoss->cooldownTimer = 0.0f;
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
			CurrentBoss->numProjectiles = 0;
			projectileAngle = 0;
			CurrentBoss->InnerState = Attack;
			break;
		case Attack:
			//printf("QUESTION TIME START\n");
			CurrentBoss->cooldownTimer += GetDeltaTime();
			// Throws out ? every 0.75s
			
			if(CurrentBoss->cooldownTimer >= 0.7f)
			{
				if (CurrentBoss->QuestionAttackAnimation == FALSE)
				{
					CurrentBoss->HandGuySpriteParts.AttackRotationArm = 0;
					CurrentBoss->HandGuySpriteParts.AttackRotationArmLower = 0;
					CurrentBoss->HandGuySpriteParts.AttackRotationArm2 = 0;
					CurrentBoss->HandGuySpriteParts.AttackRotationArmLower2 = 0;
				}
				CurrentBoss->QuestionAttackAnimation = TRUE;
				PlayAudio(CurrentBoss->SwooshSFX);
				
				
			}
			
			if(CurrentBoss->cooldownTimer >= 0.75f)
			{
				
				// Creates the projectile and aims to in the direction of the player in a cone
				if(CurrentPlayer.Position.x > CurrentBoss->Position.x)
				{
					projectileAngle = 0;
					CurrentProjectile = CreateProjectile("TextureFiles/QuestionProjectile.png", 120, 120, CurrentBoss->HandGuySpriteParts.ArmLower->Position.x, CurrentBoss->HandGuySpriteParts.ArmLower->Position.y, HandGuyProjectile, WeaponEnemy, CurrentBoss->QuestionDamage, 750, projectileAngle + (CurrentBoss->numProjectiles * 12 * FOX_PI / 180.0f));
					FreeSprite(CurrentProjectile->ProjectileSprite);
					CurrentProjectile->ProjectileSprite = (Sprite *)CreateSprite("TextureFiles/QuestionProjectile.png", 120, 120, 36, 1, 1, CurrentBoss->Position.x, CurrentBoss->Position.y);
					CurrentProjectile->ProjectileSprite->AnimationSpeed = 4;
				}
				// Creates the projectile and aims to in the direction of the player in a cone
				else
				{
					projectileAngle = FOX_PI;
					CurrentProjectile = CreateProjectile("TextureFiles/QuestionProjectile.png", 120, 120, CurrentBoss->HandGuySpriteParts.ArmLower->Position.x, CurrentBoss->HandGuySpriteParts.ArmLower->Position.y, HandGuyProjectile, WeaponEnemy, CurrentBoss->QuestionDamage, 750, projectileAngle - (float)(CurrentBoss->numProjectiles * 12 * FOX_PI / 180.0f));
					FreeSprite(CurrentProjectile->ProjectileSprite);
					CurrentProjectile->ProjectileSprite = (Sprite *)CreateSprite("TextureFiles/QuestionProjectile.png", 120, 120, 36, 1, 1, CurrentBoss->Position.x, CurrentBoss->Position.y);
					CurrentProjectile->ProjectileSprite->AnimationSpeed = 4;
				}

				CurrentBoss->cooldownTimer = 0.0f;
				++CurrentBoss->numProjectiles;
			}

			// Change to end once enough projectiles are created
			if(CurrentBoss->numProjectiles >= 5)
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
			CurrentBoss->Speed = 750.0f;
			//printf("MOVE TIME START\n");
			switch(CurrentBoss->PositionState)
			{
			// Left side
			case A:
				CurrentBoss->dropDown = TRUE;
				if(CurrentBoss->Position.x > -775.0f)
					CurrentBoss->Position.x -= CurrentBoss->Speed * GetDeltaTime();
				else
					CurrentBoss->InnerState = End;
				break;
			// Right side
			case B:
				CurrentBoss->dropDown = TRUE;
				if(CurrentBoss->Position.x < 775.0f)
					CurrentBoss->Position.x += CurrentBoss->Speed * GetDeltaTime();
				else
					CurrentBoss->InnerState = End;
				break;
			// Under platform
			case C:
				CurrentBoss->dropDown = TRUE;

				// Move to the middle
				if(CurrentBoss->Position.x < -20.0f)
					CurrentBoss->Position.x += CurrentBoss->Speed * GetDeltaTime();
				else if(CurrentBoss->Position.x > 20.0f)
					CurrentBoss->Position.x -= CurrentBoss->Speed * GetDeltaTime();
				else
					CurrentBoss->InnerState = End;
				break;
			// On platform
			case D:
				CurrentBoss->dropDown = FALSE;

				// Move to the middle
				if(CurrentBoss->Position.x < -20.0f)
					CurrentBoss->Position.x += CurrentBoss->Speed * GetDeltaTime();
				else if(CurrentBoss->Position.x > 20.0f)
					CurrentBoss->Position.x -= CurrentBoss->Speed * GetDeltaTime();
				else if(CurrentBoss->Position.y > -100 && CurrentBoss->HandGuyRigidBody.onGround)
					CurrentBoss->InnerState = End;

				// Jump up to the platform if needed
				if(CurrentBoss->Position.y <= GROUNDLEVEL && (CurrentBoss->Position.x < 200 && CurrentBoss->Position.x > -200))
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

	// Particle positioning
	CurrentBoss->AOEParticle->Position.x = CurrentBoss->Position.x;
	CurrentBoss->AOEParticle->Position.y = CurrentBoss->Position.y + 175.0f;
	CurrentBoss->DamageParticle->Position.x = CurrentBoss->Position.x;
	CurrentBoss->DamageParticle->Position.y = CurrentBoss->Position.y + 75.0f;


	if (CurrentBoss->Speed < 0.001f)
		CurrentBoss->LegSinValue = 0;

	HandGuyAnimation(CurrentBoss);

	// Set acceleration to zero
	ZeroAcceleration(&CurrentBoss->HandGuyRigidBody);

	//Brings the player back to the surface if something bad happens
	if(CurrentBoss->Position.y < GROUNDLEVEL)
	{
		CurrentBoss->Position.y = GROUNDLEVEL;
	}
	//Stop vertical velocity and acceleration when the player lands on the floor
	if(CurrentBoss->Position.y <= GROUNDLEVEL || CurrentBoss->HandGuyRigidBody.onGround)
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
	CurrentBoss->JabSprite->Position.y = CurrentBoss->Position.y + 130.0f;

	CurrentBoss->HandGuySpriteParts.ArmLower->Visible  = !CurrentBoss->JabSprite->Visible;
	CurrentBoss->HandGuySpriteParts.ArmLower2->Visible = !CurrentBoss->JabSprite->Visible;

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
			if(hit && CurrentPlayer.isAttacking && CurrentPlayer.AttackType == 0)
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
	
	//bool for sounds
	int SoundIsPlaying = FALSE;
	int i;
	int randNum = ((int)((rand() / (float)RAND_MAX) * 60)) % 6;


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

	CurrentBoss->CurrentHealth -= damageDealt;
	sprintf(num, "-%d", damageDealt);
	// Create Floating Combat Text
	FirstLetter = CreateText(num, (CurrentBoss->Position.x + rand() % 81 - 40), (CurrentBoss->Position.y + CurrentBoss->BodySprite->Height / 2), 80, textColor, Center, Border);
	AddFloatingText(FirstLetter);
	ChangeTextVisibility(FirstLetter);
	ChangeTextZIndex(FirstLetter, 201);

	//Voice Hit Reponse
	//Sound overlap checking
	for(i = 0; i < 6; i++)
	{
		if(FoxSoundCheckIsPlaying(CurrentBoss->HandGuyHit[i]))
			SoundIsPlaying = TRUE;
	}
	if(FoxSoundCheckIsPlaying(CurrentBoss->HandGuyYell) || FoxSoundCheckIsPlaying(CurrentBoss->HandGuyPhrase))
		SoundIsPlaying = TRUE;

	if(!SoundIsPlaying)
		PlayAudio(CurrentBoss->HandGuyHit[randNum]);
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

	if(CurrentPlayer.CurrentPlayerStats.CurrentHealth > 0)
	{
		//Voice Hit Reponse
		for(i = 0; i < 5; i++)
		{
			if(FoxSoundCheckIsPlaying(CurrentPlayer.CurrentPlayerSounds.VoiceHit[i]))
				SoundIsPlaying = TRUE;
		}

		if(!SoundIsPlaying)
			PlayAudio(CurrentPlayer.CurrentPlayerSounds.VoiceHit[randNum]);
	}

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

/*************************************************************************/
/*!
	\brief
	Deals player damage resolution
	
	\param CurrentBoss
	Pointer to the current boss
*/
/*************************************************************************/
void FreeHandGuyBoss(HandGuyBoss* CurrentBoss)
{	
	PoofSelf(CurrentBoss->BodySprite);

	FreeParticleSystem(CurrentBoss->AOEParticle);
	FreeParticleSystem(CurrentBoss->DamageParticle);

	FreeSprite(CurrentBoss->JabSprite);
	FreeSprite(CurrentBoss->HandGuySpriteParts.LegUpper);
	FreeSprite(CurrentBoss->HandGuySpriteParts.LegLower);
	FreeSprite(CurrentBoss->HandGuySpriteParts.LegUpper2);
	FreeSprite(CurrentBoss->HandGuySpriteParts.LegLower2);
	FreeSprite(CurrentBoss->HandGuySpriteParts.Body);
	FreeSprite(CurrentBoss->HandGuySpriteParts.Tail);
	FreeSprite(CurrentBoss->HandGuySpriteParts.ArmUpper);
	FreeSprite(CurrentBoss->HandGuySpriteParts.ArmLower);
	FreeSprite(CurrentBoss->HandGuySpriteParts.ArmUpper2);
	FreeSprite(CurrentBoss->HandGuySpriteParts.ArmLower2);
}

