/*****************************************************************************/
/*!
\file				ArmGuyBoss.c
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
#include "../HeaderFiles/ArmGuyBoss.h"
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
	LoadTexture("TextureFiles/ArmGuy.png");
	LoadTexture("TextureFiles/ArmGuyArm.png");
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
ArmGuyBoss* CreateArmGuyBoss(float xPos, float yPos, int *objID)
{
	ArmGuyBoss *CurrentBoss = (ArmGuyBoss *) CallocMyAlloc(1, sizeof(ArmGuyBoss));

	//Initialize boss struct
	Vec2Set(&CurrentBoss->Position, 700, -200);
	CurrentBoss->BodySprite = (Sprite *) CreateSprite("TextureFiles/ArmGuyBody.png", 600, 600, 10, 3, 3, 700, -200);
	CurrentBoss->BodySprite->FlipX = TRUE;
	CurrentBoss->BodySprite->AnimationActive = FALSE;
	CurrentBoss->BodySprite->AnimationSpeed = 4;
	CurrentBoss->SpinSprite = (Sprite *) CreateSprite("TextureFiles/ArmGuySpin.png", 600, 600, 60, 2, 1, 700, -200);
	CurrentBoss->SpinSprite->Visible = FALSE;
	CurrentBoss->SpinSprite->AnimationSpeed = 8;
	CurrentBoss->ArmSprite = (Sprite *) CreateSprite("TextureFiles/ArmGuyArm.png", 600, 600, 11, 1, 1, 750, -140);
	CurrentBoss->ArmSprite->FlipX = TRUE;
	CurrentBoss->OffArmSprite = (Sprite *) CreateSprite("TextureFiles/ArmGuyOffArm.png", 600, 600, 9, 1, 1, 750, -140);
	CurrentBoss->OffArmSprite->FlipX = TRUE;
	CurrentBoss->ArmJabSprite = (Sprite *) CreateSprite("TextureFiles/ArmGuyArmPoke.png", 600, 600, 11, 3, 3, 750, -140);
	CurrentBoss->ArmJabSprite->FlipX = TRUE;
	CurrentBoss->ArmJabSprite->Visible = FALSE;
	CurrentBoss->ArmSmashSprite = (Sprite *) CreateSprite("TextureFiles/ArmGuyArmSmash.png", 600, 600, 11, 3, 3, 750, -140);
	CurrentBoss->ArmSmashSprite->FlipX = TRUE;
	CurrentBoss->ArmSmashSprite->Visible = FALSE;
	CurrentBoss->playerHit = 0;
	CurrentBoss->MaxHealth = 1000;
	CurrentBoss->CurrentHealth = 1000;
	CurrentBoss->CurrentState = Cooldown;
	CurrentBoss->InnerState = Start;

	// Particle System
	CurrentBoss->ArmGuyParticle = CreateFoxParticleSystem("TextureFiles/ArmGuyParticle.png", CurrentBoss->Position.x, CurrentBoss->Position.y - 260.0f, CurrentBoss->SpinSprite->ZIndex + 1, 0, 5, 0.0f, 90, 110, 1, 1, 90, 0, 750, 0.25f, 0.5f);

	// Armguy colliders
	CreateCollisionBox(&CurrentBoss->BossCollider, &CurrentBoss->Position, EnemyType, 150, 430, (*objID)++);
	CreateCollisionBox(&CurrentBoss->SpinAttack, &CurrentBoss->Position, WeaponEnemy, 300, 200, (*objID)++); 
	CreateCollisionBox(&CurrentBoss->JabAttack, &CurrentBoss->Position, WeaponEnemy, 200, 100, (*objID)++); 
	CreateCollisionBox(&CurrentBoss->SmashAttack, &CurrentBoss->Position, WeaponEnemy, 800, 200, (*objID)++);


	// Sets the initial position of all colliders
	CurrentBoss->BossCollider.Position.y -= CurrentBoss->BossCollider.height / 8;
	CurrentBoss->SpinAttack.Position.y -= CurrentBoss->SpinAttack.height / 2;
	CurrentBoss->JabAttack.Position.x = CurrentBoss->Position.x - 200;
	CurrentBoss->JabAttack.Position.y = -40;
	CurrentBoss->SmashAttack.Position.x = 640;
	CurrentBoss->SmashAttack.Position.y = -40;

	CurrentBoss->playerHit = -1; // No need for a collision list
	CurrentBoss->cooldownTimer = 0;
	CurrentBoss->SpinDamage = 30;
	CurrentBoss->JabDamage = 30;
	CurrentBoss->SmashDamage = 25;

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
		CurrentBoss->ArmGuyParticle->Position.x = CurrentBoss->JabAttack.Position.x;
		CurrentBoss->ArmGuyParticle->Position.y = CurrentBoss->JabAttack.Position.y;
		switch(CurrentBoss->InnerState)
		{
		case Start:
			//printf("JAB TIME START\n");
			// Boss is on the right

			CurrentBoss->ArmJabSprite->Visible = TRUE;
			CurrentBoss->ArmSprite->Visible = FALSE;
			CurrentBoss->BodySprite->AnimationActive = TRUE;

			if (CurrentBoss->BodySprite->CurrentFrame == 4)
				CurrentBoss->BodySprite->AnimationActive = FALSE;

			if (CurrentBoss->BodySprite->Position.y < 75.0f)
			{
				CurrentBoss->BodySprite->Position.y += 320.0f * GetDeltaTime();
				CurrentBoss->BossCollider.Position.y += 320.0f * GetDeltaTime();
			}
			else
			{
				CurrentBoss->BodySprite->Position.y = 75.0f;
				CurrentBoss->BossCollider.Position.y = 75.0f - CurrentBoss->BossCollider.height / 8;
			}

			if(CurrentBoss->Position.x > 0)
			{
				// Pull arm back
				CurrentBoss->JabAttack.Position.x += 250 * GetDeltaTime();
				// Its back enough, switch state
				if(CurrentBoss->JabAttack.Position.x >= CurrentBoss->Position.x + 100)
				{
					CurrentBoss->InnerState = Attack;
					playerHit = FALSE;
					CurrentBoss->ArmJabSprite->AnimationSpeed = 3;
					CurrentBoss->BodySprite->AnimationActive = FALSE;
				}
			}
			// Boss is on the left
			else
			{
				// Pull arm back
				CurrentBoss->JabAttack.Position.x -= 250 * GetDeltaTime();

				// Its back enough, switch state
				if(CurrentBoss->JabAttack.Position.x <= CurrentBoss->Position.x - 100)
				{
					CurrentBoss->InnerState = Attack;
					playerHit = FALSE;
					CurrentBoss->ArmJabSprite->AnimationSpeed = 3;
					CurrentBoss->BodySprite->AnimationActive = FALSE;
				}
			}
			break;
		case Attack:
			//printf("JAB TIME ATTACK\n");
			// Boss is on the right
			CurrentBoss->ArmGuyParticle->amountTotal = -1;
			CurrentBoss->ArmGuyParticle->emitAmount = 5;
			CurrentBoss->ArmGuyParticle->emitDisplacementX = 90;
			CurrentBoss->ArmGuyParticle->emitDisplacementY = 90;
			CurrentBoss->ArmGuyParticle->emitAngle = 0;

			if(CurrentBoss->Position.x > 0)
			{
				// Punch hard
				CurrentBoss->JabAttack.Position.x -= 2400 * GetDeltaTime();

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
					CurrentPlayer.Position.x -= 3200 * GetDeltaTime();
				}

				// Max range hit, switch state
				if(CurrentBoss->JabAttack.Position.x <= CurrentBoss->Position.x - 400)
				{
					CurrentBoss->InnerState = End;
					CurrentBoss->ArmJabSprite->AnimationSpeed = 24;
				}
			}
			// Boss is on the left
			else
			{
				// Punch hard
				CurrentBoss->JabAttack.Position.x += 2400 * GetDeltaTime();

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
					CurrentPlayer.Position.x += 3200 * GetDeltaTime();
				}

				// Max range hit, switch state
				if(CurrentBoss->JabAttack.Position.x >= CurrentBoss->Position.x + 400)
				{
					CurrentBoss->InnerState = End;
					CurrentBoss->ArmJabSprite->AnimationSpeed = 24;
				}
			}
			break;
		case End:
			//printf("JAB TIME END\n");
			// Reset the player hit fake bool
			CurrentBoss->ArmGuyParticle->amountTotal = 0;

			if (CurrentBoss->BodySprite->Position.y - 1024.0f * GetDeltaTime() > -200.0f)
			{
				CurrentBoss->BodySprite->Position.y -= 1024.0f * GetDeltaTime();
				CurrentBoss->BossCollider.Position.y -= 1024.0f * GetDeltaTime();
			}
			else
			{
				CurrentBoss->BodySprite->Position.y = -200.0f;
				CurrentBoss->BossCollider.Position.y = -200.0f - CurrentBoss->BossCollider.height / 8;
			}

			CurrentBoss->BodySprite->AnimationActive = TRUE;

			if (CurrentBoss->BodySprite->CurrentFrame == 8)
				CurrentBoss->BodySprite->AnimationActive = FALSE;

			// Boss is on the right
			if(CurrentBoss->Position.x > 0)
			{
				// Pull arm back into position
				CurrentBoss->JabAttack.Position.x += 400 * GetDeltaTime();

				// Player is still reeling back a bit from the blow
				if(playerHit)
				{
					CurrentPlayer.Position.x -= 1500 * GetDeltaTime();
				}

				// Back into position, onto the cooldown
				if(CurrentBoss->JabAttack.Position.x >= CurrentBoss->Position.x - 200)
				{
					CurrentBoss->CurrentState = Cooldown;
					CurrentBoss->cooldownTimer = 0;
					CurrentBoss->ArmJabSprite->Visible = FALSE;
					CurrentBoss->ArmSprite->Visible = TRUE;
					CurrentBoss->OffArmSprite->Visible = TRUE;
					CurrentBoss->BodySprite->AnimationActive = FALSE;
					playerHit = FALSE;
				}
			}
			// Boss is on the left
			else
			{
				// Pull arm back into position
				CurrentBoss->JabAttack.Position.x -= 400 * GetDeltaTime();

				// Player is still reeling back a bit from the blow
				if(playerHit)
				{
					CurrentPlayer.Position.x += 1500 * GetDeltaTime();
				}

				// Back into position, onto the cooldown
				if(CurrentBoss->JabAttack.Position.x <= CurrentBoss->Position.x + 200)
				{
					CurrentBoss->CurrentState = Cooldown;
					CurrentBoss->cooldownTimer = 0;
					CurrentBoss->ArmJabSprite->Visible = FALSE;
					CurrentBoss->ArmSprite->Visible = TRUE;
					CurrentBoss->OffArmSprite->Visible = TRUE;
					CurrentBoss->BodySprite->AnimationActive = FALSE;
					playerHit = FALSE;
				}
			}
			break;
		}
		break;
	case Smash:
		CurrentBoss->ArmGuyParticle->Position.x = CurrentBoss->Position.x + 250 * (CurrentBoss->Position.x < 0 ? 1 : -1);
		CurrentBoss->ArmGuyParticle->Position.y = CurrentBoss->SmashAttack.Position.y;
		switch(CurrentBoss->InnerState)
		{
		case Start:
			//printf("SMASH TIME START\n");
			if (CurrentBoss->BodySprite->Position.y + 480.0f * GetDeltaTime() < 250.0f)
			{
				CurrentBoss->BodySprite->Position.y += 480.0f * GetDeltaTime();
				CurrentBoss->BossCollider.Position.y += 480.0f * GetDeltaTime();
			}
			else
			{
				CurrentBoss->BodySprite->Position.y = 250.0f;
				CurrentBoss->BossCollider.Position.y = 250.0f - CurrentBoss->BossCollider.height / 8;
			}

			CurrentBoss->ArmSprite->Visible = FALSE;
			CurrentBoss->ArmSmashSprite->Visible = TRUE;
			CurrentBoss->ArmSmashSprite->AnimationActive = TRUE;
			CurrentBoss->BodySprite->AnimationActive = TRUE;

			if (CurrentBoss->BodySprite->CurrentFrame == 4)
				CurrentBoss->BodySprite->AnimationActive = FALSE;

			if (CurrentBoss->ArmSmashSprite->CurrentFrame == 4)
				CurrentBoss->ArmSmashSprite->AnimationActive = FALSE;

			// Raise the arm and perhaps the roof!
			CurrentBoss->SmashAttack.Position.y += 500 * GetDeltaTime();
			if(CurrentBoss->SmashAttack.Position.y > 550)
			{
				CurrentBoss->InnerState = Attack;
				playerHit = FALSE;
				CurrentBoss->BodySprite->AnimationActive = FALSE;
				CurrentBoss->ArmSmashSprite->AnimationSpeed = 4;
				CurrentBoss->ArmSmashSprite->AnimationActive = TRUE;
			}

			break;
		case Attack:
			//printf("SMASH TIME ATTACK\n");
			CurrentBoss->ArmGuyParticle->amountTotal = -1;
			CurrentBoss->ArmGuyParticle->emitAmount = 15;
			CurrentBoss->ArmGuyParticle->emitDisplacementX = 500;
			CurrentBoss->ArmGuyParticle->emitDisplacementY = 0;
			CurrentBoss->ArmGuyParticle->emitAngle = 90;

			if (CurrentBoss->BodySprite->Position.y - 2048.0f * GetDeltaTime() > -200.0f)
			{
				CurrentBoss->BodySprite->Position.y -= 2048.0f * GetDeltaTime();
				CurrentBoss->BossCollider.Position.y -= 2048.0f * GetDeltaTime();
			}
			else
			{
				CurrentBoss->BodySprite->Position.y = -200.0f;
				CurrentBoss->BossCollider.Position.y = -200.0f - CurrentBoss->BossCollider.height / 8;
			}
			// Bring down the arm and perhaps the house!
			CurrentBoss->SmashAttack.Position.y -= 3500 * GetDeltaTime();

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
				CurrentPlayer.Position.y -= 3500 * GetDeltaTime();
			}

			if(CurrentBoss->SmashAttack.Position.y < GROUNDLEVEL)
			{
				CurrentBoss->InnerState = End;
				CurrentBoss->ArmGuyParticle->amountTotal = 0;
				
			}
			break;
		case End:
			//printf("SMASH TIME END\n");
			if (CurrentBoss->BodySprite->Position.y - 960.0f * GetDeltaTime() > -200.0f)
			{
				CurrentBoss->BodySprite->Position.y -= 960.0f * GetDeltaTime();
				CurrentBoss->BossCollider.Position.y -= 960.0f * GetDeltaTime();
			}
			else
			{
				CurrentBoss->BodySprite->Position.y = -200.0f;
				CurrentBoss->BossCollider.Position.y = -200.0f - CurrentBoss->BossCollider.height / 8;
			}

			CurrentBoss->BodySprite->AnimationActive = TRUE;

			if (CurrentBoss->BodySprite->CurrentFrame >= 8)
				CurrentBoss->BodySprite->AnimationActive = FALSE;

			CurrentBoss->ArmSmashSprite->AnimationActive = TRUE;

			if (CurrentBoss->ArmSmashSprite->CurrentFrame >= 8)
				CurrentBoss->ArmSmashSprite->AnimationActive = FALSE;

			// Put things back in order
			CurrentBoss->SmashAttack.Position.y += 500 * GetDeltaTime();

			if(CurrentBoss->SmashAttack.Position.y > -40)
			{
				CurrentBoss->CurrentState = Cooldown;
				CurrentBoss->cooldownTimer = 0;
				playerHit = FALSE;
				CurrentBoss->ArmSprite->Visible = TRUE;
				CurrentBoss->OffArmSprite->Visible = TRUE;
				CurrentBoss->ArmSmashSprite->Visible = FALSE;
				CurrentBoss->BodySprite->AnimationActive = FALSE;
				
			}
			break;
		}
		break;
	case Spin:
		//printf("SPIN TIME START\n");
		CurrentBoss->ArmGuyParticle->Position.x = CurrentBoss->Position.x;
		CurrentBoss->ArmGuyParticle->Position.y = CurrentBoss->Position.y - 260.0f;
		switch(CurrentBoss->InnerState)
		{
		case Start:

			if (CurrentBoss->Position.x < 0)
				CurrentBoss->SpinSprite->FlipX = FALSE;
			else
				CurrentBoss->SpinSprite->FlipX = TRUE;

			CurrentBoss->cooldownTimer += GetDeltaTime();
			CurrentBoss->ArmGuyParticle->amountTotal = -1;
			CurrentBoss->ArmGuyParticle->emitAmount = 5;
			CurrentBoss->ArmGuyParticle->emitDisplacementX = 90;
			CurrentBoss->ArmGuyParticle->emitDisplacementY = 0;
			CurrentBoss->ArmGuyParticle->emitAngle = 90;
			// Switch to the spinning sprite
			CurrentBoss->BodySprite->Visible = FALSE;
			CurrentBoss->ArmSprite->Visible = FALSE;
			CurrentBoss->OffArmSprite->Visible = FALSE;
			CurrentBoss->ArmJabSprite->Visible = FALSE;
			CurrentBoss->SpinSprite->Visible = TRUE;
			
			// Boss is on the right and start up timer is done
			if(CurrentBoss->cooldownTimer > 1.0f && CurrentBoss->Position.x > 0)
			{
				CurrentBoss->InnerState = SpinL;
				playerHit = FALSE;
			}
			// Boss is on the left and start up timer is done
			else if(CurrentBoss->cooldownTimer > 1.0f)
			{
				CurrentBoss->InnerState = SpinR;
				playerHit = FALSE;
			}
			break;
		case SpinR:
			// Spin to the right
			CurrentBoss->Position.x += 1800 * GetDeltaTime();
			CurrentBoss->BodySprite->Position.x += 1800 * GetDeltaTime();
			CurrentBoss->SpinSprite->Position.x += 1800 * GetDeltaTime();
			CurrentBoss->SpinAttack.Position.x += 1800 * GetDeltaTime();
			CurrentBoss->BossCollider.Position.x += 1800 * GetDeltaTime();

			// Check for collision (special to boss)
			if(!playerHit && CollisionRectangles(&CurrentPlayer.PlayerCollider, &CurrentBoss->SpinAttack))
			{
				// Player gets shot up
				playerHit = TRUE;
				Vec2Set(&CurrentPlayer.PlayerRigidBody.Velocity, 0.0f, 1500.0f);
				CurrentPlayer.Position.y += 2.0f;
				PlayerDamageResult(CurrentBoss->SpinDamage);
			}

			if(CurrentBoss->Position.x >= 700)
			{
				CurrentBoss->BodySprite->Position.x = 700;
				CurrentBoss->SpinSprite->Position.x = 700;
				CurrentBoss->Position.x = 700;
				CurrentBoss->SpinAttack.Position.x = 700;
				CurrentBoss->BossCollider.Position.x = 700;
				CurrentBoss->InnerState = End;
				CurrentBoss->cooldownTimer = 0;
			}
			break;
		case SpinL:
			// Spin to the left
			CurrentBoss->Position.x -= 1800 * GetDeltaTime();
			CurrentBoss->BodySprite->Position.x -= 1800 * GetDeltaTime();
			CurrentBoss->SpinSprite->Position.x -= 1800 * GetDeltaTime();
			CurrentBoss->SpinAttack.Position.x -= 1800 * GetDeltaTime();
			CurrentBoss->BossCollider.Position.x -= 1800 * GetDeltaTime();

			// Check for collision (special to boss)
			if(!playerHit && CollisionRectangles(&CurrentPlayer.PlayerCollider, &CurrentBoss->SpinAttack))
			{
				// Player gets shot up
				playerHit = TRUE;
				Vec2Set(&CurrentPlayer.PlayerRigidBody.Velocity, 0.0f, 1500.0f);
				CurrentPlayer.Position.y += 2.0f;
				PlayerDamageResult(CurrentBoss->SpinDamage);
			}

			if(CurrentBoss->Position.x <= -700)
			{
				CurrentBoss->BodySprite->Position.x = -700;
				CurrentBoss->SpinSprite->Position.x = -700;
				CurrentBoss->Position.x = -700;
				CurrentBoss->SpinAttack.Position.x = -700;
				CurrentBoss->BossCollider.Position.x = -700;
				CurrentBoss->InnerState = End;
				CurrentBoss->cooldownTimer = 0;
			}
			break;
		case End:
			CurrentBoss->cooldownTimer += GetDeltaTime();
			if(CurrentBoss->cooldownTimer > 0.5f && CurrentBoss->Position.x > 0)
			{
				// Switch to the standing sprite
				CurrentBoss->BodySprite->Visible = TRUE;
				CurrentBoss->SpinSprite->Visible = FALSE;
				CurrentBoss->ArmJabSprite->Visible = FALSE;
				CurrentBoss->ArmSprite->Visible = TRUE;
				CurrentBoss->OffArmSprite->Visible = TRUE;

				CurrentBoss->BodySprite->FlipX = TRUE;
				CurrentBoss->ArmSprite->FlipX = TRUE;
				CurrentBoss->OffArmSprite->FlipX = TRUE;
				CurrentBoss->JabAttack.Position.x = CurrentBoss->Position.x - 200;
				CurrentBoss->SmashAttack.Position.x = 640;
				CurrentBoss->CurrentState = Cooldown;
				CurrentBoss->cooldownTimer = 0;
				playerHit = FALSE;
			}
			else if(CurrentBoss->cooldownTimer > 0.5f)
			{
				// Switch to the standing sprite
				CurrentBoss->BodySprite->Visible = TRUE;
				CurrentBoss->SpinSprite->Visible = FALSE;
				CurrentBoss->ArmSprite->Visible = TRUE;
				CurrentBoss->OffArmSprite->Visible = TRUE;

				CurrentBoss->BodySprite->FlipX = FALSE;
				CurrentBoss->ArmSprite->FlipX = FALSE;
				CurrentBoss->OffArmSprite->FlipX = FALSE;
				CurrentBoss->JabAttack.Position.x = CurrentBoss->Position.x + 200;
				CurrentBoss->SmashAttack.Position.x = -640;
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
		CurrentBoss->ArmGuyParticle->amountTotal = 0;
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
			else if(CurrentPlayer.Position.y > -150)
			{
				if(rand() % 2)
				{
					CurrentBoss->CurrentState = Smash;
					CurrentBoss->ArmSmashSprite->AnimationSpeed = 8;
					CurrentBoss->ArmSmashSprite->CurrentFrame = 0;
					CurrentBoss->InnerState = Start;
					CurrentBoss->cooldownTimer = 0;
					CurrentBoss->BodySprite->CurrentFrame = 1;
				}
				else
				{
					CurrentBoss->CurrentState = Jab;
					CurrentBoss->ArmJabSprite->AnimationSpeed = 24;
					CurrentBoss->ArmJabSprite->CurrentFrame = 0;
					CurrentBoss->InnerState = Start;
					CurrentBoss->cooldownTimer = 0;
					CurrentBoss->BodySprite->CurrentFrame = 1;
				}
			}
			// Player is low, smash or spin
			else
			{
				if(rand() % 2)
				{
					CurrentBoss->CurrentState = Smash;
					CurrentBoss->ArmSmashSprite->AnimationSpeed = 8;
					CurrentBoss->ArmSmashSprite->CurrentFrame = 0;
					CurrentBoss->InnerState = Start;
					CurrentBoss->cooldownTimer = 0;
					CurrentBoss->BodySprite->CurrentFrame = 1;
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
			else if(CurrentPlayer.Position.y > -150)
			{
				if(rand() % 2)
				{
					CurrentBoss->CurrentState = Smash;
					CurrentBoss->ArmSmashSprite->AnimationSpeed = 8;
					CurrentBoss->ArmSmashSprite->CurrentFrame = 0;
					CurrentBoss->InnerState = Start;
					CurrentBoss->cooldownTimer = 0;
				}
				else
				{
					CurrentBoss->CurrentState = Jab;
					CurrentBoss->InnerState = Start;
					CurrentBoss->ArmJabSprite->AnimationSpeed = 24;
					CurrentBoss->ArmJabSprite->CurrentFrame = 0;
					CurrentBoss->cooldownTimer = 0;
					CurrentBoss->BodySprite->CurrentFrame = 1;
				}
			}
			// Player is low, smash or spin
			else
			{
				if(rand() % 2)
				{
					CurrentBoss->CurrentState = Smash;
					CurrentBoss->ArmSmashSprite->AnimationSpeed = 8;
					CurrentBoss->ArmSmashSprite->CurrentFrame = 0;
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
	CurrentBoss->ArmSprite->Position.y = CurrentBoss->BodySprite->Position.y + 50.0f;
	CurrentBoss->ArmSprite->Position.x = CurrentBoss->BodySprite->Position.x + 50.0f * (CurrentBoss->BodySprite->Position.x > 0 ? 1 : -1);
	CurrentBoss->OffArmSprite->Position.y = CurrentBoss->BodySprite->Position.y;
	CurrentBoss->OffArmSprite->Position.x = CurrentBoss->Position.x;
	CurrentBoss->ArmJabSprite->Position.y = CurrentBoss->BodySprite->Position.y + 50.0f;
	CurrentBoss->ArmJabSprite->Position.x = CurrentBoss->BodySprite->Position.x + 50.0f * (CurrentBoss->BodySprite->Position.x > 0 ? 1 : -1);
	CurrentBoss->ArmJabSprite->FlipX = CurrentBoss->ArmSprite->FlipX;
	CurrentBoss->ArmSmashSprite->Position.y = CurrentBoss->BodySprite->Position.y + 50.0f;
	CurrentBoss->ArmSmashSprite->Position.x = CurrentBoss->BodySprite->Position.x + 50.0f * (CurrentBoss->BodySprite->Position.x > 0 ? 1 : -1);
	CurrentBoss->ArmSmashSprite->FlipX = CurrentBoss->ArmSprite->FlipX;
	

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
void DetectArmGuyBossCollision(ArmGuyBoss *CurrentBoss)
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
					ArmGuyBossCollideWeapon(CurrentBoss);
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
void ArmGuyBossCollideWeapon(ArmGuyBoss *CurrentBoss)
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
	if(CurrentPlayer.CurrentPlayerStats.StrengthTimer > 0)
		Vec3Set(&textColor, 0.0f, 0.9372f, 0.6f);
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
	{
		damageDealt = (int)(damage * (1.0f - CurrentPlayer.CurrentPlayerStats.DamageReduction));
		if(damageDealt == 0)
			damageDealt = 1;
	}
	
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
	Deals player damage resolution
	
	\param CurrentBoss
	Pointer to the current boss
*/
/*************************************************************************/
void FreeArmGuyBoss(ArmGuyBoss* CurrentBoss)
{
	FreeParticleSystem(CurrentBoss->ArmGuyParticle);
	FreeSprite(CurrentBoss->ArmJabSprite);
	FreeSprite(CurrentBoss->ArmSmashSprite);
	FreeSprite(CurrentBoss->ArmSprite);
	FreeSprite(CurrentBoss->BodySprite);
	FreeSprite(CurrentBoss->OffArmSprite);
	FreeSprite(CurrentBoss->SpinSprite);
	FreeMyAlloc(CurrentBoss);
}

