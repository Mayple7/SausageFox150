/*****************************************************************************/
/*!
\file				KevinBoss.c
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
#include "../HeaderFiles/KevinBoss.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxObjects.h"

// ---------------------------------------------------------------------------
// globals
static int LogicTimer = 0;
static int playerHit = FALSE;

static enum KevinState { Cooldown, Jab, Move };
static enum InnerState {Start, Attack, End };
static enum PositionState { A, B, C, D };

/*************************************************************************/
/*!
	\brief
	Handles the collision between an enemy and the attacking weapon
	
	\param CurrentEnemy
	Pointer to the enemy object
*/
/*************************************************************************/
void LoadKevinBoss(void)
{
	LoadTexture("TextureFiles/Kevin.png");
}

/*************************************************************************/
/*!
	\brief
	Handles the collision between an enemy and the attacking weapon
	
	\param CurrentEnemy
	Pointer to the enemy object
*/
/*************************************************************************/
KevinBoss* CreateKevinBoss(float xPos, float yPos)
{
	KevinBoss *CurrentBoss = (KevinBoss *) CallocMyAlloc(1, sizeof(KevinBoss));

	//Initialize boss struct
	Vec2Set(&CurrentBoss->Position, 700, -200);

	Vec2Set(&CurrentBoss->Position, 700, -200);
	CurrentBoss->BodySprite = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 477, 360, 20, 1, 1, 700, -200);
	CurrentBoss->BodySprite->FlipX = TRUE;
	CurrentBoss->BodySprite->Visible = FALSE;
	CurrentBoss->JabSprite = (Sprite *) CreateSprite("TextureFiles/KevinQuickJab.png", 64, 64, 21, 4, 4, 580, -120);
	CurrentBoss->JabSprite->FlipX = TRUE;
	CurrentBoss->JabSprite->AnimationSpeed = 3;
	CurrentBoss->JabSprite->Visible = FALSE;

	CurrentBoss->playerHit = 0;
	CurrentBoss->MaxHealth = 1000;
	CurrentBoss->CurrentHealth = 1000;
	CurrentBoss->CurrentState = Cooldown;
	CurrentBoss->InnerState = Start;
	CurrentBoss->PositionState = B;

	CurrentBoss->jumpTimer = 0.0f;

	// Kevin colliders
	CreateCollisionBox(&CurrentBoss->BossCollider, &CurrentBoss->Position, EnemyType, 100, 300, GetObjectID());
	CurrentBoss->BossCollider.Offset.y = CurrentBoss->BossCollider.height / 4;
	CreateCollisionBox(&CurrentBoss->JabAttack, &CurrentBoss->Position, WeaponEnemy, 200, 100, GetObjectID()); 


	// Sets the initial position of all colliders
	CurrentBoss->BossCollider.Position.y -= CurrentBoss->BossCollider.height / 8;
	CurrentBoss->JabAttack.Position.x = CurrentBoss->Position.x - 200;
	CurrentBoss->JabAttack.Position.y = -40;

	// Physics stuff
	InitializeRigidBody(&CurrentBoss->KevinRigidBody, FALSE, 150, 300);
	CurrentBoss->KevinRigidBody.Mass = 7;

	CurrentBoss->playerHit = -1; // No need for a collision list
	CurrentBoss->cooldownTimer = 0;
	CurrentBoss->JabDamage = 30;

	// All the sprites
	CreateKevinSprites(CurrentBoss);
	CurrentBoss->LegSinValue = 0;

	//Sounds
	CurrentBoss->KevinHit[0] = CreateSound("Sounds/KevinHit1.mp3", SmallSnd);
	CurrentBoss->KevinHit[1] = CreateSound("Sounds/KevinHit2.mp3", SmallSnd);
	CurrentBoss->KevinHit[2] = CreateSound("Sounds/KevinHit3.mp3", SmallSnd);
	CurrentBoss->KevinHit[3] = CreateSound("Sounds/KevinHit4.mp3", SmallSnd);

	CurrentBoss->KevinPhrase[0] = CreateSound("Sounds/KevinPhrase1.mp3", SmallSnd);
	CurrentBoss->KevinPhrase[1] = CreateSound("Sounds/KevinPhrase2.mp3", SmallSnd);
	CurrentBoss->KevinPhrase[2] = CreateSound("Sounds/KevinPhrase3.mp3", SmallSnd);
	CurrentBoss->KevinPhrase[3] = CreateSound("Sounds/KevinPhrase4.mp3", SmallSnd);

	CurrentBoss->KevinStart = CreateSound("Sounds/KevinStart.mp3", SmallSnd);

	CurrentBoss->KevinSoundsPlay = FALSE;


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
void UpdateKevinBoss(KevinBoss *CurrentBoss)
{
	Vec2 velocityTime;
	int movementPicker;

	if(CurrentBoss->CurrentState != Jab)
		CurrentBoss->cooldownTimer += GetDeltaTime();

	if(CurrentBoss->cooldownTimer > 15 && CollisionRectangles(&CurrentBoss->BossCollider, &CurrentPlayer.PlayerCollider))
	{
		CurrentBoss->CurrentState = Jab;
		CurrentBoss->InnerState = Attack;
		CurrentBoss->cooldownTimer = 0;
	}

	switch(CurrentBoss->CurrentState)
	{
	case Jab:
		switch(CurrentBoss->InnerState)
		{
		case Start:
			//printf("JAB TIME START\n");
			// This jab has no wind-up.
			break;
		case Attack:
			//printf("JAB TIME ATTACK\n");
			CurrentBoss->JabSprite->Visible = TRUE;
			CurrentBoss->cooldownTimer += GetDeltaTime();

			// Deal 1 damage if in range
			if(CollisionRectangles(&CurrentPlayer.PlayerCollider, &CurrentBoss->JabAttack))
			{
				PlayerDamageResult(2);
			}
			
			// Once all damage is dealt move to a new location
			if(CurrentBoss->cooldownTimer > GetDeltaTime() * CurrentBoss->JabDamage)
			{
				CurrentBoss->InnerState = End;
				CurrentBoss->cooldownTimer = 0.0f;
			}
			break;
			break;
		case End:
			//printf("JAB TIME END\n");
			CurrentBoss->JabSprite->Visible = FALSE;
			CurrentBoss->InnerState = Start;
			CurrentBoss->CurrentState = Cooldown;
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
				else if(movementPicker > 60)
					CurrentBoss->PositionState = D;		// 40% chance
				else
					CurrentBoss->PositionState = B;		// 40% chance;
				break;
			case B:
				// Choose a new state based on the random number
				if(movementPicker > 80)
					CurrentBoss->PositionState = C;		// 20% chance
				else if(movementPicker > 60)
					CurrentBoss->PositionState = D;		// 40% chance
				else
					CurrentBoss->PositionState = A;		// 40% chance
				break;
			case C:
				// Choose a new state based on the random number
				if(movementPicker > 80)
					CurrentBoss->PositionState = D;		// 20% chance
				else if(movementPicker > 40)
					CurrentBoss->PositionState = B;		// 40% chance
				else
					CurrentBoss->PositionState = A;		// 40% chance
				break;
			case D:
				// Choose a new state based on the random number
				if(movementPicker > 80)
					CurrentBoss->PositionState = C;		// 20% chance
				else if(movementPicker > 40)
					CurrentBoss->PositionState = B;		// 40% chance
				else
					CurrentBoss->PositionState = A;		// 40% chance
				break;
			}

			// Do the movement next
			CurrentBoss->InnerState = Attack;
			break;
		case Attack:
			CurrentBoss->Speed = 900.0f;
			//printf("MOVE TIME START\n");
			switch(CurrentBoss->PositionState)
			{
			// Left side
			case A:
				if(CurrentBoss->Position.x > -775.0f)
					CurrentBoss->Position.x -= CurrentBoss->Speed * GetDeltaTime();
				else
					CurrentBoss->InnerState = End;
				break;
			// Right side
			case B:
				if(CurrentBoss->Position.x < 2695.0f)
					CurrentBoss->Position.x += CurrentBoss->Speed * GetDeltaTime();
				else
					CurrentBoss->InnerState = End;
				break;
			// Under platform
			case C:
				// Move to the middle
				if(CurrentBoss->Position.x < 381.0f - CurrentBoss->Speed * GetDeltaTime())
					CurrentBoss->Position.x += CurrentBoss->Speed * GetDeltaTime();
				else if(CurrentBoss->Position.x > 381.0f + CurrentBoss->Speed * GetDeltaTime())
					CurrentBoss->Position.x -= CurrentBoss->Speed * GetDeltaTime();
				else
					CurrentBoss->InnerState = End;
				break;
			// On platform
			case D:
				// Move to the middle
				if(CurrentBoss->Position.x < 1536.0f - CurrentBoss->Speed * GetDeltaTime())
					CurrentBoss->Position.x += CurrentBoss->Speed * GetDeltaTime();
				else if(CurrentBoss->Position.x > 1536.0f + CurrentBoss->Speed * GetDeltaTime())
					CurrentBoss->Position.x -= CurrentBoss->Speed * GetDeltaTime();
				else
					CurrentBoss->InnerState = End;
				break;
			}
			break;
		case End:
			//printf("MOVE TIME START\n");
			CurrentBoss->InnerState = Start;
			CurrentBoss->CurrentState = Cooldown;
			break;
		}
		break;
	case Cooldown:
		//printf("CD TIME\n");
		CurrentBoss->CurrentState = Move;
		CurrentBoss->InnerState = Start;
		break;
	}

		// Jump Kevin Jump
	if(CurrentBoss->KevinRigidBody.Velocity.y >= 0 && CurrentBoss->Position.y <= GROUNDLEVEL && CurrentBoss->CurrentState != Jab && CurrentBoss->jumpTimer >= 0.5f)
	{
		// Set y velocity for jumping
		Vec2 velocity;
		CurrentBoss->Position.y += 3;
		Vec2Set(&velocity, 0.0f, 800.0f);
		ApplyVelocity(&CurrentBoss->KevinRigidBody, &velocity);
		CurrentBoss->jumpTimer = 0;
	}
	
	// Animation
	KevinAnimation(CurrentBoss);

	// Set acceleration to zero
	ZeroAcceleration(&CurrentBoss->KevinRigidBody);

	//Brings the player back to the surface if something bad happens
	if(CurrentBoss->Position.y < GROUNDLEVEL)
	{
		CurrentBoss->Position.y = GROUNDLEVEL;
	}
	//Stop vertical velocity and acceleration when the player lands on the floor
	if(CurrentBoss->Position.y <= GROUNDLEVEL || CurrentBoss->KevinRigidBody.onGround)
	{
		Vec2Zero(&CurrentBoss->KevinRigidBody.Acceleration);
		Vec2Zero(&CurrentBoss->KevinRigidBody.Velocity);
		ZeroGravity(&CurrentBoss->KevinRigidBody);
		CurrentBoss->jumpTimer += GetDeltaTime();
	}
	//Set gravity if not on floor or on a platform
	else
	{
		SetGravity(&CurrentBoss->KevinRigidBody, 0.0f, FOX_GRAVITY_Y);
	}

	//Update velocity and acceleration
	UpdateVelocity(&CurrentBoss->KevinRigidBody);
	Vec2Scale(&velocityTime, &CurrentBoss->KevinRigidBody.Velocity, GetDeltaTime());
	
	Vec2Add(&CurrentBoss->Position, &CurrentBoss->Position, &velocityTime);

	CurrentBoss->BodySprite->Position = CurrentBoss->Position;

	//Updates the collision box
	UpdateCollisionPosition(&CurrentBoss->BossCollider, &CurrentBoss->Position);
	CurrentBoss->KevinRigidBody.onGround = FALSE;

	CurrentBoss->JabAttack.Position.y = CurrentBoss->Position.y - CurrentBoss->BodySprite->Height / 2;

	CurrentBoss->JabSprite->Position.y = CurrentBoss->Position.y + 120.0f;

	if(CurrentPlayer.Position.x < CurrentBoss->Position.x)
	{
		// Flip to face player
		CurrentBoss->BodySprite->FlipX = FALSE;
		CurrentBoss->JabSprite->Position.x = CurrentBoss->Position.x - CurrentBoss->BossCollider.width / 2;
		CurrentBoss->JabSprite->FlipX = TRUE;
		CurrentBoss->JabAttack.Position.x = CurrentBoss->Position.x - CurrentBoss->BossCollider.width / 2;		
	}
	else
	{
		// Flip to face player
		CurrentBoss->BodySprite->FlipX = TRUE;
		CurrentBoss->JabSprite->Position.x = CurrentBoss->Position.x + CurrentBoss->BossCollider.width / 2;
		CurrentBoss->JabSprite->FlipX = FALSE;
		CurrentBoss->JabAttack.Position.x = CurrentBoss->Position.x + CurrentBoss->BossCollider.width / 2;
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
void DetectKevinBossCollision(KevinBoss *CurrentBoss)
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
			if(hit && CurrentPlayer.isAttacking && CurrentPlayer.AttackType == 0)
			{
				// On start collision
				if(hitPrev <= 0)
				{
					CurrentBoss->playerHit = wList->WeaponAttack.collisionID * 10 + 1;
					//printf("NOT FOUND: %i\n", -hitPrev);
					KevinBossCollideWeapon(CurrentBoss);
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
void KevinBossCollideWeapon(KevinBoss *CurrentBoss)
{
	int damageDealt;
	char num[10];

	//bool for sounds
	int i;
	int SoundIsPlaying = FALSE;
	int randNum = ((int)((rand() / (float)RAND_MAX) * 60)) % 4;


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
	if(CurrentPlayer.CurrentPlayerStats.StrengthTimer > 0)
		Vec3Set(&textColor, 0.0f, 0.9372f, 0.6f);
	FirstLetter = CreateText(num, (CurrentBoss->Position.x + rand() % 81 - 40), (CurrentBoss->Position.y + CurrentBoss->BodySprite->Height / 2), 80, textColor, Center, Border);
	AddFloatingText(FirstLetter);
	ChangeTextVisibility(FirstLetter);
	ChangeTextZIndex(FirstLetter, 201);

	//Voice Hit Reponse
	for(i = 0; i < 4; i++)
	{
		if(FoxSoundCheckIsPlaying(CurrentBoss->KevinHit[i]))
			SoundIsPlaying = TRUE;
		if(FoxSoundCheckIsPlaying(CurrentBoss->KevinPhrase[i]))
			SoundIsPlaying = TRUE;
	}

	if(!SoundIsPlaying)
		PlayAudio(CurrentBoss->KevinHit[randNum]);
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

/*************************************************************************/
/*!
	\brief
	Deals player damage resolution
	
	\param CurrentBoss
	Pointer to the current boss
*/
/*************************************************************************/
void FreeKevinBoss(KevinBoss* CurrentBoss)
{
	PoofSelf(CurrentBoss->BodySprite);
	FreeSprite(CurrentBoss->JabSprite);
	FreeSprite(CurrentBoss->BodySprite);
}

/*************************************************************************/
/*!
	\brief
	Creates the boss's sprites

	\param Object
	The boss to create
*/
/*************************************************************************/
void CreateKevinSprites(KevinBoss *Object)
{
	Object->KevinSpriteParts.Body = (Sprite *) CreateSprite("TextureFiles/KevinBody.png", 450.0f, 450.0f, Object->BodySprite->ZIndex, 1, 1, 0, 0);

	Object->KevinSpriteParts.ArmUpper = (Sprite *) CreateSprite("TextureFiles/KevinArmUpper.png", 192.0f, 192.0f, Object->BodySprite->ZIndex + 2, 1, 1, 0, 0);

	Object->KevinSpriteParts.ArmUpper2 = (Sprite *) CreateSpriteNoMesh("TextureFiles/KevinArmUpper.png", 192.0f, 192.0f, Object->BodySprite->ZIndex - 2, 1, 1, 0, 0);
	Object->KevinSpriteParts.ArmUpper2->SpriteMesh = Object->KevinSpriteParts.ArmUpper->SpriteMesh;

	Object->KevinSpriteParts.ArmLower2 = (Sprite *) CreateSpriteNoMesh("TextureFiles/KevinArmLower.png", 192.0f, 192.0f, Object->BodySprite->ZIndex - 2, 1, 1, 0, 0);
	Object->KevinSpriteParts.ArmLower2->SpriteMesh = Object->KevinSpriteParts.ArmUpper->SpriteMesh;

	Object->KevinSpriteParts.LegUpper = (Sprite *) CreateSpriteNoMesh("TextureFiles/KevinLegUpper.png", 192.0f, 192.0f, Object->BodySprite->ZIndex, 1, 1, 0, 0);
	Object->KevinSpriteParts.LegUpper->SpriteMesh = Object->KevinSpriteParts.ArmUpper->SpriteMesh;

	Object->KevinSpriteParts.LegLower = (Sprite *) CreateSpriteNoMesh("TextureFiles/KevinLegLower.png", 192.0f, 192.0f, Object->BodySprite->ZIndex, 1, 1, 0, 0);
	Object->KevinSpriteParts.LegLower->SpriteMesh = Object->KevinSpriteParts.ArmUpper->SpriteMesh;

	Object->KevinSpriteParts.LegUpper2 = (Sprite *) CreateSpriteNoMesh("TextureFiles/KevinLegUpper.png", 192.0f, 192.0f, Object->BodySprite->ZIndex, 1, 1, 0, 0);
	Object->KevinSpriteParts.LegUpper2->SpriteMesh = Object->KevinSpriteParts.ArmUpper->SpriteMesh;

	Object->KevinSpriteParts.LegLower2 = (Sprite *) CreateSpriteNoMesh("TextureFiles/KevinLegLower.png", 192.0f, 192.0f, Object->BodySprite->ZIndex, 1, 1, 0, 0);
	Object->KevinSpriteParts.LegLower2->SpriteMesh = Object->KevinSpriteParts.ArmUpper->SpriteMesh;

	Object->KevinSpriteParts.ArmLower = (Sprite *) CreateSpriteNoMesh("TextureFiles/KevinArmLower.png", 192.0f, 192.0f, Object->BodySprite->ZIndex + 2, 1, 1, 0, 0);
	Object->KevinSpriteParts.ArmLower->SpriteMesh = Object->KevinSpriteParts.ArmUpper->SpriteMesh;

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