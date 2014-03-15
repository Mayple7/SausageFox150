/*****************************************************************************/
/*!
\file				Enemy.c
\author				Dan Muller (d.muller)
\date				Jan 20, 2014

\brief				Enemy components and logic code

\par				Functions:
\li					InitializeEnemy
\li					UpdateEnemy
\li					EnemyLogic
  
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

void LoadEnemy(int enemyType)
{
	switch(enemyType)
	{
	case Dummy:
		LoadTexture("TextureFiles/StrawDummy.png");
		break;
	case BasicMelee:
		LoadTexture("TextureFiles/StrawDummy.png");
		LoadTexture("TextureFiles/BodyDog.png");
		LoadTexture("TextureFiles/ArmUpperDog.png");
		LoadTexture("TextureFiles/ArmUpperDog.png");
		LoadTexture("TextureFiles/ArmLowerDog.png");
		LoadTexture("TextureFiles/LegUpperDog.png");
		LoadTexture("TextureFiles/LegLowerDog.png");
		LoadTexture("TextureFiles/LegUpperDog.png");
		LoadTexture("TextureFiles/LegLowerDog.png");
		LoadTexture("TextureFiles/TailDog.png");
		LoadTexture("TextureFiles/ArmLowerDog.png");
		LoadTexture("TextureFiles/SkirtDog.png");

		//Weapon, may change
		LoadTexture("TextureFiles/BattleAxe.png");
		break;
	case BasicRanged:
		break;
	}
}


/*************************************************************************/
/*!
	\brief
	Initializes and creates the enemy
	  
	\param CurrentEnemy
	A pointer to the enemy object to be initialized
*/
/*************************************************************************/
Enemy* CreateEnemy(int enemyType, int collisionGroup, int objID, float xPos, float yPos, int panelId)
{
	float width, height;
	Vec2 position;
	Enemy *CurrentEnemy = AddEnemy();
	int i;

	EnemyPanelNumber[panelId]++;

	CurrentEnemy->panelId = panelId;

	for(i = 0; i < COLLIDEAMOUNT; i++)
	{
		CurrentEnemy->CollisionData[i] = -1;
	}

	switch(enemyType)
	{
	case Dummy:
		width = 261.0f;
		height = 373.0f;
		Vec2Set(&position, xPos, yPos);
		Vec2Scale(&CurrentEnemy->Position, &position, GetLoadRatio());
		CurrentEnemy->EnemyType = Dummy;
		CurrentEnemy->objID = objID;
		//Creates the enemy sprite
		CurrentEnemy->EnemySprite = (Sprite *) CreateSprite("TextureFiles/StrawDummy.png", width, height, 8, 1, 1, xPos, yPos);
		
		InitializeRigidBody(&CurrentEnemy->EnemyRigidBody, TRUE, width, height);

		InitializeEnemyStats(CurrentEnemy, 50, 0, 0, 0, 0, 0, 10);

		CurrentEnemy->EnemyParticleSystem = CreateFoxParticleSystem("TextureFiles/StrawParticle.png", CurrentEnemy->Position.x / GetLoadRatio(), CurrentEnemy->Position.y / GetLoadRatio(), CurrentEnemy->EnemySprite->ZIndex + 1, 0, 5, 0.0f, 270, 90, 1.0f, -5.0f, 25, 24, 50, 2.0f, 1.0f);

		CreateCollisionBox(&CurrentEnemy->EnemyCollider, &position, EnemyType, width / 2, height / 2, objID);
		CurrentEnemy->EnemyCollider.Offset.y = -CurrentEnemy->EnemyCollider.height / 6;

		break;
	case BasicMelee:
		width = 261.0f;
		height = 373.0f;
		Vec2Set(&position, xPos, yPos);
		Vec2Scale(&CurrentEnemy->Position, &position, GetLoadRatio());
		CurrentEnemy->EnemyType = BasicMelee;
		CurrentEnemy->objID = objID;

		//Creates the enemy sprite
		CurrentEnemy->EnemySprite = (Sprite *) CreateSprite("TextureFiles/StrawDummy.png", width, height, 8, 1, 1, xPos, yPos);
		CurrentEnemy->EnemySprite->Visible		= FALSE;
		
		InitializeRigidBody(&CurrentEnemy->EnemyRigidBody, FALSE, PLAYER_WIDTH, PLAYER_HEIGHT);
		CurrentEnemy->EnemyRigidBody.onGround	= FALSE;
		CurrentEnemy->dropDown					= FALSE;

		InitializeEnemyStats(CurrentEnemy, 50, (float)(250 + 10 * (rand() % 10)), 15.0f, 0, 10, 10, 10);

		CurrentEnemy->EnemyParticleSystem = CreateFoxParticleSystem("TextureFiles/Particle.png", CurrentEnemy->Position.x / GetLoadRatio(), CurrentEnemy->Position.y / GetLoadRatio(), CurrentEnemy->EnemySprite->ZIndex + 5, 0, 5, 0.0f, 0, 360, 1.0f, -5.0f, 25, 24, 20, 2.0f, 0.5f);

		CreateCollisionBox(&CurrentEnemy->EnemyCollider, &CurrentEnemy->Position, EnemyType, 100, 200, objID);
		CurrentEnemy->EnemyCollider.Offset.y = 20 * GetLoadRatio();
		CurrentEnemy->EnemyCollider.width = CurrentEnemy->EnemyCollider.width - 20 * GetLoadRatio();
		UpdateCollider(&CurrentEnemy->EnemyCollider, CurrentEnemy->EnemyCollider.width, CurrentEnemy->EnemyCollider.height);

		CurrentEnemy->EnemyWeapon = CreateWeapon("Sausage sausage of sausage", "TextureFiles/BattleAxe.png", Sword, Common, WeaponEnemy, 256, 256, objID++);
		CurrentEnemy->EnemySpriteParts.Weapon = CurrentEnemy->EnemyWeapon->WeaponSprite;
		CurrentEnemy->EnemyWeapon->WeaponFOF = EnemyWeapon;

		CreateEnemySprites(CurrentEnemy);
		CurrentEnemy->Speed				= 0;
		CurrentEnemy->LegSinValue		= 0;
		CurrentEnemy->isAttacking		= FALSE;
		CurrentEnemy->EnemyDirection	= LEFT;

		CurrentEnemy->isMoveRight		= FALSE;
		CurrentEnemy->isMoveLeft		= FALSE;
		CurrentEnemy->isJumping			= FALSE;
		CurrentEnemy->jumpTimer			= 0;
		CurrentEnemy->isDropDown		= FALSE;
		CurrentEnemy->canDropDownTimer	= 300;
		CurrentEnemy->Attack			= FALSE;
		CurrentEnemy->StateTimer		= 0;
		CurrentEnemy->EnemyState		= AIIdle;
		CurrentEnemy->idleMove			= 0;
		CurrentEnemy->idleTimer			= rand() % 60;
		CurrentEnemy->canAttack			= FALSE;
		CurrentEnemy->canAttackTimer	= 150;
		CurrentEnemy->findHome			= FALSE;
		CurrentEnemy->HomePos			= CurrentEnemy->Position;
		EnemyAnimation(CurrentEnemy);

		break;
	case BasicRanged:
		break;
	default:
		break;
	}

	CurrentEnemy->CurrentEnemySounds.Poof = CreateSound(rand() % 2 ? "Sounds/Poof1.wav" : "Sounds/Poof2.wav", SmallSnd);
	CurrentEnemy->CurrentEnemySounds.GetHit1 = CreateSound("Sounds/SwordHit1.wav", SmallSnd);
	CurrentEnemy->CurrentEnemySounds.GetHit2 = CreateSound("Sounds/SwordHit2.wav", SmallSnd);
	CurrentEnemy->CurrentEnemySounds.Swing1 = CreateSound("Sounds/SwordSwing1.wav", SmallSnd);
	CurrentEnemy->CurrentEnemySounds.Swing2 = CreateSound("Sounds/SwordSwing2.wav", SmallSnd);

	return CurrentEnemy;
}

/*************************************************************************/
/*!
	\brief
	Updates the enemy's position and sets the enemy sprite's position
	  
	\param CurrentEnemy
	The pointer to the enemy struct
*/
/*************************************************************************/
void UpdateEnemy(Enemy *CurrentEnemy)
{

	Vec2 velocityTime;

	switch(CurrentEnemy->EnemyType)
	{
	case Dummy:
		// Too dummy to do anything lololol
		//Still need to check collision :-/
		DetectEnemyCollision(CurrentEnemy);
		break;
	case BasicMelee:
		// Call enemy logic here
		DetectEnemyCollision(CurrentEnemy);
		EnemyAIUpdate(CurrentEnemy);
		EnemyBasicMeleeUpdate(CurrentEnemy);
		EnemyAnimation(CurrentEnemy);
		UpdateCollisionPosition(&CurrentEnemy->EnemyWeapon->WeaponAttack, &CurrentEnemy->EnemyWeapon->WeaponAttackPosition);
		break;
	case BasicRanged:
		// Call enemy logic here
		break;
	default:
		break;
	}

	CurrentEnemy->EnemyParticleSystem->Position.x = CurrentEnemy->Position.x / GetLoadRatio();
	CurrentEnemy->EnemyParticleSystem->Position.y = CurrentEnemy->Position.y / GetLoadRatio();

	if(CurrentEnemy->CurrentEnemyStats.CurrentHealth <= 0)
	{
		char num[10];
		Vec3 textColor;
		TextGlyphs *FirstLetter;

		//Give the player thier loot!
		CurrentPlayer.CurrentPlayerStats.Money += CurrentEnemy->CurrentEnemyStats.Money;
		
		Vec3Set(&textColor, 1.0f, 1.0f, 0.0f);
		sprintf(num, "+%d", CurrentEnemy->CurrentEnemyStats.Money);
		// Create Floating Coin Text
		FirstLetter = CreateText(num, (CurrentEnemy->Position.x + rand() % 81 - 40) / GetLoadRatio(), (CurrentPlayer.Position.y + CurrentPlayer.PlayerSpriteParts.Body->Height / 2) / GetLoadRatio(), 80, textColor, Center, Border);
		AddFloatingText(FirstLetter);
		ChangeTextVisibility(FirstLetter);
		ChangeTextZIndex(FirstLetter, 201);

		// Run on death stuff here
		CurrentEnemy->EnemyParticleSystem->emitAngleRandom = 360;
		CurrentEnemy->EnemyParticleSystem->amountTotal = 20;
		CurrentEnemy->EnemyParticleSystem->emitAmount = 20;
		CurrentEnemy->EnemyParticleSystem->emitDisplacementX = 100;
		CurrentEnemy->EnemyParticleSystem->emitDisplacementY = 101;
		strcpy(CurrentEnemy->EnemyParticleSystem->ParticleSprite, "TextureFiles/Particle.png");
		CurrentEnemy->EnemyParticleSystem->emitScale = 2.0f;
		CurrentEnemy->EnemyParticleSystem->emitLife = 1.0f;
		PlayAudio(CurrentEnemy->CurrentEnemySounds.Poof);
		EnemyPanelNumber[CurrentEnemy->panelId]--;
		FreeEnemy(CurrentEnemy);
	}

	if(CurrentEnemy->Position.y < GROUNDLEVEL * GetLoadRatio() && CurrentEnemy->EnemyState != AINone)
	{
		CurrentEnemy->Position.y = GROUNDLEVEL * GetLoadRatio();
	}
	//Stop velocity and acceleration when the player lands on the floor
	if(CurrentEnemy->Position.y <= GROUNDLEVEL * GetLoadRatio() || CurrentEnemy->EnemyRigidBody.onGround)
	{
		Vec2Zero(&CurrentEnemy->EnemyRigidBody.Acceleration);
		Vec2Zero(&CurrentEnemy->EnemyRigidBody.Velocity);
		ZeroGravity(&CurrentEnemy->EnemyRigidBody);
	}
	//Set gravity if not on floor or on a platform
	else
	{
		SetGravity(&CurrentEnemy->EnemyRigidBody, 0.0f, FOX_GRAVITY_Y * GetLoadRatio());
	}
	/*/Player position updated when dropping down from a platform
	if(CurrentEnemy->dropDown)
	{
		CurrentEnemy->Position.y -= 1200.0f * GetDeltaTime() * GetLoadRatio();
		if(CurrentEnemy->EnemyRigidBody.Velocity.y < 0)
		{
			CurrentEnemy->EnemyRigidBody.Velocity.y = -1800.0f * GetDeltaTime() * GetLoadRatio();
			CurrentEnemy->dropDown = FALSE;
		}
	}*/

	if(CurrentEnemy->dropDown)
	{
		CurrentEnemy->dropdownTimer -= GetDeltaTime();
		//Once gravity takes control stop the drop down stuffs
		if(CurrentEnemy->dropdownTimer <= 0.0f)
		{
			CurrentEnemy->dropDown = FALSE;
		}
	}

	if(CurrentEnemy->KnockBack)
	{
		CurrentEnemy->KnockBackTime--;

		MoveObject(&CurrentEnemy->Position, CurrentEnemy->KnockBackDir, 8.0f);

		if(CurrentEnemy->KnockBackTime <= 0 && (CurrentEnemy->EnemyRigidBody.onGround || CurrentEnemy->Position.y <= GROUNDLEVEL * GetLoadRatio()))
			CurrentEnemy->KnockBack = FALSE;
	}

	//Update velocity and acceleration
	UpdateVelocity(&CurrentEnemy->EnemyRigidBody);
	Vec2Scale(&velocityTime, &CurrentEnemy->EnemyRigidBody.Velocity, GetDeltaTime());
	
	Vec2Add(&CurrentEnemy->Position, &CurrentEnemy->Position, &velocityTime);

	//Updates the collision box
	UpdateCollisionPosition(&CurrentEnemy->EnemyCollider, &CurrentEnemy->Position);
	CurrentEnemy->EnemyRigidBody.onGround = FALSE;
	ZeroAcceleration(&CurrentEnemy->EnemyRigidBody);
}

void EnemyBasicMeleeUpdate(Enemy *CurrentEnemy)
{
	if (CurrentEnemy->Attack && !CurrentEnemy->isAttacking && CurrentEnemy->canAttack)
	{
		CurrentEnemy->isAttacking								= TRUE;
		CurrentEnemy->Attack									= FALSE;
		CurrentEnemy->EnemySpriteParts.AttackRotation			= 0;
		CurrentEnemy->EnemySpriteParts.AttackRotationArm		= 0;
		CurrentEnemy->EnemySpriteParts.AttackRotationArmLower	= 0;
		UpdateCollider(&CurrentEnemy->EnemyCollider,CurrentEnemy->EnemyCollider.width, CurrentEnemy->EnemyCollider.height);
		PlayAudio(rand() % 2 ? CurrentEnemy->CurrentEnemySounds.Swing1 : CurrentEnemy->CurrentEnemySounds.Swing1);
	}
	// Move left if A is pressed
	if(CurrentEnemy->isMoveLeft)
	{
		CurrentEnemy->EnemySprite->FlipX	= FALSE;
		CurrentEnemy->EnemyDirection		= LEFT;
		CurrentEnemy->Speed					= CurrentEnemy->CurrentEnemyStats.MoveSpeed * GetLoadRatio() * GetDeltaTime();
	}
	// Move right if D is pressed
	else if(CurrentEnemy->isMoveRight)
	{
		CurrentEnemy->EnemySprite->FlipX	= TRUE;
		CurrentEnemy->EnemyDirection		= RIGHT;
		CurrentEnemy->Speed					= CurrentEnemy->CurrentEnemyStats.MoveSpeed * GetLoadRatio() * GetDeltaTime();
	}
	// not key press for direction then slow down!
	else
	{
		if (!(CurrentEnemy->Position.y > GROUNDLEVEL * GetLoadRatio()) && !CurrentEnemy->EnemyRigidBody.onGround)
		{
			if (CurrentEnemy->Speed - 48.0f * GetDeltaTime() >= 0.0f)
			{
				CurrentEnemy->Speed -= 48.0f * GetDeltaTime();
			}
			else
			{
				CurrentEnemy->Speed			= 0.0f;
				CurrentEnemy->LegSinValue	= 0;
			}
		}
		else
		{
			if (CurrentEnemy->Speed - 48.0f * GetDeltaTime() >= 0.0f)
			{
				CurrentEnemy->Speed -= 48.0f * GetDeltaTime();
			}
			else
			{
				CurrentEnemy->Speed			= 0.0f;
				CurrentEnemy->LegSinValue	= 0;
			}
		}
	}
		
	//Jump when space is pushed
	if(CurrentEnemy->isJumping && CurrentEnemy->jumpTimer <= 0)
	{
		Vec2 velocity;
		CurrentEnemy->isJumping = FALSE;
		CurrentEnemy->jumpTimer = (int)(0.5f / GetDeltaTime());
		
		Vec2Set(&velocity, 0.0f, 1080.0f * GetLoadRatio());
		if(CurrentEnemy->Position.y <= GROUNDLEVEL * GetLoadRatio() || CurrentEnemy->EnemyRigidBody.onGround)
		{
			if(CurrentEnemy->Position.y <= GROUNDLEVEL * GetLoadRatio())
				Vec2Set(&CurrentEnemy->Position, CurrentEnemy->Position.x, GROUNDLEVEL * GetLoadRatio() + 0.1f);
			CurrentEnemy->EnemyRigidBody.onGround = FALSE;
			ApplyVelocity(&CurrentEnemy->EnemyRigidBody, &velocity);
		}
	}
	//Drop down when told to
	if(CurrentEnemy->isDropDown)
	{
		CurrentEnemy->EnemyRigidBody.onGround = FALSE;
		CurrentEnemy->isDropDown = FALSE;
		CurrentEnemy->dropDown = TRUE;
		CurrentEnemy->dropdownTimer = 0.25f;
	}

	if (CurrentEnemy->jumpTimer > 0)
		CurrentEnemy->jumpTimer--;

	if (!CurrentEnemy->KnockBack)
		MoveObject(&CurrentEnemy->Position, CurrentEnemy->EnemyDirection, CurrentEnemy->Speed);
}

void EnemyAIUpdate(Enemy *CurrentEnemy)
{
	switch(CurrentEnemy->EnemyState)
	{
		case AINone:
			CurrentEnemy->isMoveRight	= FALSE;
			CurrentEnemy->isMoveLeft	= FALSE;
			break;
		case AIAggressive:
			CurrentEnemy->isJumping			= FALSE;

			if (CurrentEnemy->Position.x < CurrentPlayer.Position.x - 100 * GetLoadRatio())
			{
				CurrentEnemy->isMoveRight	= TRUE;
				CurrentEnemy->isMoveLeft	= FALSE;
			}
			else if (CurrentEnemy->Position.x > CurrentPlayer.Position.x + 100 * GetLoadRatio())
			{
				CurrentEnemy->isMoveRight	= FALSE;
				CurrentEnemy->isMoveLeft	= TRUE;
			}
			else
			{
				CurrentEnemy->isMoveRight	= FALSE;
				CurrentEnemy->isMoveLeft	= FALSE;
				
			}

			if (fabs(CurrentEnemy->Position.x - CurrentPlayer.Position.x) < 200 * GetLoadRatio())
			{
				if (CurrentEnemy->canAttackTimer > 0)
				{
					CurrentEnemy->canAttack = FALSE;
					CurrentEnemy->canAttackTimer--;
				}
				else
					CurrentEnemy->canAttack = TRUE;
			}

			if (CurrentEnemy->Position.y < CurrentPlayer.Position.y - 60 * GetLoadRatio())
			{
				int i;
				for (i = 0; i < COLLIDEAMOUNT; i++)
				{
					if (platformList[i].objID == 0)
						continue;
					if (platformList[i].objID == -1)
						break;
					if (CurrentEnemy->Position.x > platformList[i].Position.x - (CurrentEnemy->Speed * (CurrentEnemy->EnemyDirection == RIGHT ? 1 : 0) / GetDeltaTime()) - 60.0f * GetLoadRatio() && 
						CurrentEnemy->Position.x < platformList[i].Position.x + (CurrentEnemy->Speed * (CurrentEnemy->EnemyDirection == LEFT ? 1 : 0) / GetDeltaTime()) + 60.0f * GetLoadRatio() &&
						CurrentEnemy->Position.y < platformList[i].Position.y + 40 * GetLoadRatio())
					{
						CurrentEnemy->isJumping			= TRUE;
						CurrentEnemy->canDropDownTimer	= (int)(2 / GetDeltaTime());
						break;
					}
				}
			}

			if (CurrentEnemy->Position.y < CurrentPlayer.Position.y - 10 * GetLoadRatio())
			{
				int i;
				for (i = 0; i < COLLIDEAMOUNT; i++)
				{
					if (wallList[i].objID == 0)
						continue;
					if (wallList[i].objID == -1)
						break;
					
					if (CurrentEnemy->Position.x > wallList[i].Position.x - (CurrentEnemy->Speed * (CurrentEnemy->EnemyDirection == RIGHT ? 1 : 0) / GetDeltaTime()) - 60.0f * GetLoadRatio() && 
						CurrentEnemy->Position.x < wallList[i].Position.x + (CurrentEnemy->Speed * (CurrentEnemy->EnemyDirection == LEFT ? 1 : 0) / GetDeltaTime()) + 60.0f * GetLoadRatio() &&
						wallList[i].Position.y + CurrentEnemy->EnemyCollider.height * 2 > CurrentEnemy->Position.y - CurrentEnemy->EnemyCollider.height)
					{
						CurrentEnemy->isJumping			= TRUE;
						CurrentEnemy->canDropDownTimer	= (int)(2 / GetDeltaTime());
						break;
					}
				}
			}
			else if (CurrentEnemy->EnemyRigidBody.onGround && CurrentEnemy->canDropDownTimer <= 0 && CurrentEnemy->Position.y > CurrentPlayer.Position.y + 10 * GetLoadRatio())
			{
				int i;
				for (i = 0; i < COLLIDEAMOUNT; i++)
				{
					if (platformList[i].objID == 0)
						continue;
					if (platformList[i].objID == -1)
						break;
				if (CurrentEnemy->Position.x > platformList[i].Position.x - (CurrentEnemy->Speed * (CurrentEnemy->EnemyDirection == RIGHT ? 1 : 0.25f) / GetDeltaTime()) - 60.0f * GetLoadRatio() && 
					CurrentEnemy->Position.x < platformList[i].Position.x + (CurrentEnemy->Speed * (CurrentEnemy->EnemyDirection == LEFT ? 1 : 0.25f) / GetDeltaTime()) + 60.0f * GetLoadRatio() &&
					platformList[i].Position.y + platformList[i].PlatformCollider.height / 2 > CurrentEnemy->Position.y - CurrentEnemy->EnemyCollider.height / 1.5f)
					CurrentEnemy->isDropDown		= TRUE;
				}
			}
			
			if (CurrentEnemy->canDropDownTimer > 0)
				CurrentEnemy->canDropDownTimer--;

			if (Vec2Distance(&CurrentEnemy->Position, &CurrentPlayer.Position) < 150 * GetLoadRatio() && !CurrentEnemy->isAttacking && !CurrentEnemy->Attack)
			{
				CurrentEnemy->canAttackTimer	= (int)(1.5f / GetDeltaTime());
				CurrentEnemy->Attack			= TRUE;
			}
			break;
		case AIPassive:

			if (CurrentEnemy->Position.x < CurrentPlayer.Position.x)
			{
				CurrentEnemy->isMoveRight	= FALSE;
				CurrentEnemy->isMoveLeft	= TRUE;
			}
			else if (CurrentEnemy->Position.x > CurrentPlayer.Position.x)
			{
				CurrentEnemy->isMoveRight	= TRUE;
				CurrentEnemy->isMoveLeft	= FALSE;
			}

			CurrentEnemy->StateTimer--;

			if (CurrentEnemy->StateTimer <= 0)
				CurrentEnemy->EnemyState = AIIdle;
			break;
			
		case AIIdle:
			//printf("%f\n", Vec2Distance(&CurrentEnemy->Position, &CurrentPlayer.Position));
			if (Vec2Distance(&CurrentEnemy->Position, &CurrentPlayer.Position) < 500.0f * GetLoadRatio())
			{
				CurrentEnemy->EnemyState = AIAggressive;
			}
			if ((float)fabs((float)(CurrentEnemy->Position.x - CurrentEnemy->HomePos.x)) > 500.0f * GetLoadRatio())
				CurrentEnemy->findHome = TRUE;
			switch(CurrentEnemy->findHome)
			{
				case FALSE:
					if (CurrentEnemy->idleMove == 1)
					{
						CurrentEnemy->isMoveRight	= TRUE;
						CurrentEnemy->isMoveLeft	= FALSE;
					}
					else if (CurrentEnemy->idleMove == -1)
					{
						CurrentEnemy->isMoveRight	= FALSE;
						CurrentEnemy->isMoveLeft	= TRUE;
					}
					else
					{
						CurrentEnemy->isMoveRight	= FALSE;
						CurrentEnemy->isMoveLeft	= FALSE;
					}

					if (CurrentEnemy->idleTimer > 0)
						CurrentEnemy->idleTimer--;
					else
					{
						CurrentEnemy->idleTimer = rand() % 60 + 10;
						CurrentEnemy->idleMove	= rand() % 3 - 1;
					}
					break;
				case TRUE:
					CurrentEnemy->isJumping			= FALSE;
					if (CurrentEnemy->Position.x < CurrentEnemy->HomePos.x - 60 * GetLoadRatio())
					{
						CurrentEnemy->isMoveRight	= TRUE;
						CurrentEnemy->isMoveLeft	= FALSE;
					}
					else if (CurrentEnemy->Position.x > CurrentEnemy->HomePos.x + 60 * GetLoadRatio())
					{
						CurrentEnemy->isMoveRight	= FALSE;
						CurrentEnemy->isMoveLeft	= TRUE;
					}
					else
					{
						CurrentEnemy->findHome = FALSE;
						CurrentEnemy->idleTimer = (int)(0.5f / GetDeltaTime());
						CurrentEnemy->idleMove	= 0;
					}

					if (CurrentEnemy->Position.y < CurrentEnemy->HomePos.y - 20 * GetLoadRatio())
					{
						int i;
						for (i = 0; i < COLLIDEAMOUNT; i++)
						{
							if (platformList[i].objID == 0)
								continue;
							if (platformList[i].objID == -1)
								break;
							if (CurrentEnemy->Position.x > platformList[i].Position.x - (CurrentEnemy->Speed * (CurrentEnemy->EnemyDirection == RIGHT ? 1 : 0) / GetDeltaTime()) - 60.0f * GetLoadRatio() && 
								CurrentEnemy->Position.x < platformList[i].Position.x + (CurrentEnemy->Speed * (CurrentEnemy->EnemyDirection == LEFT ? 1 : 0) / GetDeltaTime()) + 60.0f * GetLoadRatio() &&
								CurrentEnemy->Position.y < platformList[i].Position.y + 40 * GetLoadRatio())
							{
								CurrentEnemy->isJumping			= TRUE;
								CurrentEnemy->canDropDownTimer	= (int)(2.0f / GetDeltaTime());
								break;
							}
						}

						for (i = 0; i < COLLIDEAMOUNT; i++)
						{
							if (wallList[i].objID == 0)
								continue;
							if (wallList[i].objID == -1)
								break;
							if (CurrentEnemy->Position.x > wallList[i].Position.x - (CurrentEnemy->Speed * (CurrentEnemy->EnemyDirection == RIGHT ? 1 : 0) / GetDeltaTime()) - 60.0f * GetLoadRatio() && 
								CurrentEnemy->Position.x < wallList[i].Position.x + (CurrentEnemy->Speed * (CurrentEnemy->EnemyDirection == LEFT ? 1 : 0) / GetDeltaTime()) + 60.0f * GetLoadRatio() &&
								wallList[i].Position.y + CurrentEnemy->EnemyCollider.height * 2 > CurrentEnemy->Position.y - CurrentEnemy->EnemyCollider.height)
							{
								CurrentEnemy->isJumping			= TRUE;
								CurrentEnemy->canDropDownTimer	= (int)(2 / GetDeltaTime());
								break;
							}
						}
					}
					else if (CurrentEnemy->EnemyRigidBody.onGround && CurrentEnemy->canDropDownTimer <= 0 && CurrentEnemy->Position.y > CurrentEnemy->HomePos.y + 10 * GetLoadRatio())
					{
						CurrentEnemy->isDropDown		= TRUE;
					}
			
					if (CurrentEnemy->canDropDownTimer > 0)
						CurrentEnemy->canDropDownTimer--;
					break;
			}

			break;
	}
}


void InitializeEnemyStats(Enemy *CurrentEnemy, int maxHP, float movSpeed, float atkSpeed, float dmgReduction, int dmg, int money, int exp)
{
	CurrentEnemy->CurrentEnemyStats.MaxHealth = maxHP;
	CurrentEnemy->CurrentEnemyStats.CurrentHealth = maxHP;

	CurrentEnemy->CurrentEnemyStats.MoveSpeed = movSpeed;
	CurrentEnemy->CurrentEnemyStats.AttackSpeed = atkSpeed;

	CurrentEnemy->CurrentEnemyStats.DamageReduction = dmgReduction;
	CurrentEnemy->CurrentEnemyStats.Damage = dmg;

	CurrentEnemy->CurrentEnemyStats.Money = money;
	CurrentEnemy->CurrentEnemyStats.Experience = exp;
}

void DetectEnemyCollision(Enemy *CurrentEnemy)
{
	Weapon* wList = weaponList;
	Platform* pList = platformList;
	Wall* walls = wallList;
	int hit = 0;
	int hitPrev = 0;
	


	while(wList->objID != -1)
	{
		if(wList->objID > 0 && wList->WeaponFOF == PlayerWeapon)
		{
			hit = CollisionRectangles(&CurrentEnemy->EnemyCollider, &wList->WeaponAttack);
			hitPrev = searchHitArray(CurrentEnemy->CollisionData, COLLIDEAMOUNT, wList->WeaponAttack.collisionID);
			if(hit && CurrentPlayer.isAttacking)
			{
				// New target, on start collision
				if(hitPrev < 0)
				{
					CurrentEnemy->CollisionData[-hitPrev] = wList->WeaponAttack.collisionID * 10 + 1;
					//printf("NOT FOUND: %i\n", -hitPrev);
					EnemyCollideWeapon(CurrentEnemy);
				}
				// Found target, hit previous frame, on persistant
				else if(CurrentEnemy->CollisionData[hitPrev] % 10 == 1)
				{
					//printf("FOUND PERSISTANT: %i\n", CurrentEnemy.CollisionData[hitPrev]);
				}
				// Found target, did not hit previous frame, on start collision
				else if(CurrentEnemy->CollisionData[hitPrev] % 10 == 0)
				{
					//printf("FOUND NEW COLLISION: %i\n", CurrentEnemy.CollisionData[hitPrev]);
					CurrentEnemy->CollisionData[hitPrev] = wList->WeaponPickup.collisionID * 10 + 1;
					EnemyCollideWeapon(CurrentEnemy);
				}
			}
			else if(hitPrev > 0 && !CurrentPlayer.isAttacking)
			{
				CurrentEnemy->CollisionData[hitPrev] = 0;
			}
			else
			{
				if(hitPrev < 0 || CurrentEnemy->CollisionData[hitPrev] % 10 == 0)
				{
					// NEVER COLLIDED OR DIDNT COLLIDE PREV FRAME
					AE_ASSERT_MESG("No collision and not colliding, should never be here.");
				}
				// Found target, collision ended
				else if(CurrentEnemy->CollisionData[hitPrev] % 10 == 1)
				{
					//printf("END COLLISION: %i\n", CurrentEnemy.CollisionData[hitPrev]);
					//CurrentEnemy->CollisionData[hitPrev] = 0;
				}
			}
		}
		wList++;
	}

	while(pList->objID != -1)
	{
		if(pList->objID > 0)
		{
			hit = CollisionRectangles(&CurrentEnemy->EnemyCollider, &pList->PlatformCollider);
			hitPrev = searchHitArray(CurrentEnemy->CollisionData, COLLIDEAMOUNT, pList->PlatformCollider.collisionID);
			if(hit)
			{
				// New target, on start collision
				if(hitPrev < 0)
				{
					CurrentEnemy->CollisionData[-hitPrev] = pList->PlatformCollider.collisionID * 10 + 1;
					//printf("NOT FOUND: %i\n", -hitPrev);
					EnemyCollidePlatform(CurrentEnemy, pList);
				}
				// Found target, hit previous frame, on persistant
				else if(CurrentEnemy->CollisionData[hitPrev] % 10 == 1)
				{
					//printf("FOUND PERSISTANT: %i\n", CurrentEnemy->CollisionData[hitPrev]);
					EnemyCollidePlatform(CurrentEnemy, pList);
				}
				// Found target, did not hit previous frame, on start collision
				else if(CurrentEnemy->CollisionData[hitPrev] % 10 == 0)
				{
					//printf("FOUND NEW COLLISION: %i\n", CurrentEnemy->CollisionData[hitPrev]);
					CurrentEnemy->CollisionData[hitPrev] = pList->PlatformCollider.collisionID * 10 + 1;
					EnemyCollidePlatform(CurrentEnemy, pList);
				}
			}
			else
			{
				if(hitPrev < 0 || CurrentEnemy->CollisionData[hitPrev] % 10 == 0)
				{
					// NEVER COLLIDED OR DIDNT COLLIDE PREV FRAME
					AE_ASSERT_MESG("No collision and not colliding, should never be here.");
				}
				// Found target, collision ended
				else if(CurrentEnemy->CollisionData[hitPrev] % 10 == 1)
				{
					//printf("END COLLISION: %i\n", CurrentEnemy->CollisionData[hitPrev]);
					CurrentEnemy->CollisionData[hitPrev] = 0;
				}
			}
		}
		pList++;
	}

	while(walls->objID != -1)
	{
		if(walls->objID > 0)
		{
			hit = CollisionRectangles(&CurrentEnemy->EnemyCollider, &walls->WallCollider);
			hitPrev = searchHitArray(CurrentEnemy->CollisionData, COLLIDEAMOUNT, walls->WallCollider.collisionID);
			if(hit)
			{
				// New target, on start collision
				if(hitPrev < 0)
				{
					CurrentEnemy->CollisionData[-hitPrev] = walls->WallCollider.collisionID * 10 + 1;
					//printf("NOT FOUND: %i\n", -hitPrev);
					EnemyCollideWall(CurrentEnemy, walls);
				}
				// Found target, hit previous frame, on persistant
				else if(CurrentEnemy->CollisionData[hitPrev] % 10 == 1)
				{
					//printf("FOUND PERSISTANT: %i\n", CurrentEnemy.CollisionData[hitPrev]);
					EnemyCollideWall(CurrentEnemy, walls);
				}
				// Found target, did not hit previous frame, on start collision
				else if(CurrentEnemy->CollisionData[hitPrev] % 10 == 0)
				{
					//printf("FOUND NEW COLLISION: %i\n", CurrentEnemy.CollisionData[hitPrev]);
					CurrentEnemy->CollisionData[hitPrev] = walls->WallCollider.collisionID * 10 + 1;
					EnemyCollideWall(CurrentEnemy, walls);
				}
			}
			else
			{
				if(hitPrev < 0 || CurrentEnemy->CollisionData[hitPrev] % 10 == 0)
				{
					// NEVER COLLIDED OR DIDNT COLLIDE PREV FRAME
					AE_ASSERT_MESG("No collision and not colliding, should never be here.");
				}
				// Found target, collision ended
				else if(CurrentEnemy->CollisionData[hitPrev] % 10 == 1)
				{
					//printf("END COLLISION: %i\n", CurrentEnemy.CollisionData[hitPrev]);
					CurrentEnemy->CollisionData[hitPrev] = 0;
				}
			}
		}
		walls++;
	}
}

void CreateEnemySprites(Enemy *Object)
{
	Object->EnemySpriteParts.Body = (Sprite *) CreateSprite("TextureFiles/BodyDog.png", 300.0f, 300.0f, Object->EnemySprite->ZIndex, 4, 1, 0, 0);

	Object->EnemySpriteParts.ArmUpper = (Sprite *) CreateSprite("TextureFiles/ArmUpperDog.png", 128.0f, 128.0f, Object->EnemySprite->ZIndex + 2, 1, 1, 0, 0);

	Object->EnemySpriteParts.ArmUpper2 = (Sprite *) CreateSpriteNoMesh("TextureFiles/ArmUpperDog.png", 128.0f, 128.0f, Object->EnemySprite->ZIndex - 2, 1, 1, 0, 0);
	Object->EnemySpriteParts.ArmUpper2->SpriteMesh = Object->EnemySpriteParts.ArmUpper->SpriteMesh;

	Object->EnemySpriteParts.ArmLower2 = (Sprite *) CreateSpriteNoMesh("TextureFiles/ArmLowerDog.png", 128.0f, 128.0f, Object->EnemySprite->ZIndex - 2, 1, 1, 0, 0);
	Object->EnemySpriteParts.ArmLower2->SpriteMesh = Object->EnemySpriteParts.ArmUpper->SpriteMesh;

	Object->EnemySpriteParts.LegUpper = (Sprite *) CreateSpriteNoMesh("TextureFiles/LegUpperDog.png", 128.0f, 128.0f, Object->EnemySprite->ZIndex, 1, 1, 0, 0);
	Object->EnemySpriteParts.LegUpper->SpriteMesh = Object->EnemySpriteParts.ArmUpper->SpriteMesh;

	Object->EnemySpriteParts.LegLower = (Sprite *) CreateSpriteNoMesh("TextureFiles/LegLowerDog.png", 128.0f, 128.0f, Object->EnemySprite->ZIndex, 1, 1, 0, 0);
	Object->EnemySpriteParts.LegLower->SpriteMesh = Object->EnemySpriteParts.ArmUpper->SpriteMesh;

	Object->EnemySpriteParts.LegUpper2 = (Sprite *) CreateSpriteNoMesh("TextureFiles/LegUpperDog.png", 128.0f, 128.0f, Object->EnemySprite->ZIndex, 1, 1, 0, 0);
	Object->EnemySpriteParts.LegUpper2->SpriteMesh = Object->EnemySpriteParts.ArmUpper->SpriteMesh;

	Object->EnemySpriteParts.LegLower2 = (Sprite *) CreateSpriteNoMesh("TextureFiles/LegLowerDog.png", 128.0f, 128.0f, Object->EnemySprite->ZIndex, 1, 1, 0, 0);
	Object->EnemySpriteParts.LegLower2->SpriteMesh = Object->EnemySpriteParts.ArmUpper->SpriteMesh;

	Object->EnemySpriteParts.Body->AnimationSpeed = 3;

	Object->EnemySpriteParts.BlinkTimer = 0;

	Object->EnemySpriteParts.Tail = (Sprite *) CreateSpriteNoMesh("TextureFiles/TailDog.png", 300.0f, 300.0f, Object->EnemySprite->ZIndex, 1, 1, 0, 0);
	Object->EnemySpriteParts.Tail->SpriteMesh = Object->EnemySpriteParts.Body->SpriteMesh;

	Object->EnemySpriteParts.Tail->AnimationSpeed = (Object->Speed * GetLoadRatio())/2 + 3;

	Object->EnemySpriteParts.ArmLower = (Sprite *) CreateSpriteNoMesh("TextureFiles/ArmLowerDog.png", 128.0f, 128.0f, Object->EnemySprite->ZIndex + 2, 1, 1, 0, 0);
	Object->EnemySpriteParts.ArmLower->SpriteMesh = Object->EnemySpriteParts.ArmUpper->SpriteMesh;

	Object->EnemySpriteParts.Skirt = (Sprite *) CreateSpriteNoMesh("TextureFiles/SkirtDog.png", 300.0f, 300.0f, Object->EnemySprite->ZIndex + 1, 4, 1, 0, 0);
	Object->EnemySpriteParts.Skirt->SpriteMesh = Object->EnemySpriteParts.Body->SpriteMesh;

	Object->EnemySpriteParts.Skirt->AnimationActive = 0;
}

/*************************************************************************/
/*!
	\brief
	Animates the players legs.
*/
/*************************************************************************/
void EnemyAnimation(Enemy *Object)
{
	
	float LegDistance = ((Object->CurrentEnemyStats.MoveSpeed * GetDeltaTime() * GetLoadRatio()) + (2.3f * GetLoadRatio() / (((Object->Speed) * 0.075f + 0.5f * GetLoadRatio())) ))-(Object->Speed);
	float LegUpperDirection = (float)sin(Object->LegSinValue)/(LegDistance);
	float LegLowerDirection;
	float LegUpperDirection2 = (float)sin(Object->LegSinValue)/(LegDistance);
	float LegLowerDirection2;
	
	Sprite *LegUpr = Object->EnemySpriteParts.LegUpper;
	Sprite *LegUpr2 = Object->EnemySpriteParts.LegUpper2;
	Sprite *LegLwr = Object->EnemySpriteParts.LegLower;
	Sprite *LegLwr2 = Object->EnemySpriteParts.LegLower2;
	Sprite *ArmUpr = Object->EnemySpriteParts.ArmUpper;
	Sprite *ArmUpr2 = Object->EnemySpriteParts.ArmUpper2;
	Sprite *ArmLwr = Object->EnemySpriteParts.ArmLower;
	Sprite *ArmLwr2 = Object->EnemySpriteParts.ArmLower2;
	Sprite *Bdy = Object->EnemySpriteParts.Body;
	Sprite *Weap = Object->EnemySpriteParts.Weapon;
	Sprite *Tail = Object->EnemySpriteParts.Tail;
	Sprite *Skrt = Object->EnemySpriteParts.Skirt;

	Object->LegSinValue += 10.0f * GetDeltaTime() * (Object->Speed * 0.25f); 

	Object->EnemySpriteParts.BlinkTimer += 1;

	if (Object->EnemySpriteParts.BlinkTimer <= 150)
	{
		Object->EnemySpriteParts.Body->AnimationActive = 0;
	}
	else
	{
		Object->EnemySpriteParts.Body->AnimationActive = 1;
		if (Object->EnemySpriteParts.Body->CurrentFrame == 3)
		{
			Object->EnemySpriteParts.BlinkTimer = 0;
			Object->EnemySpriteParts.Body->CurrentFrame = 0;
		}
	}

	Bdy->Position.x = Object->Position.x;
	Bdy->Position.y = Object->Position.y - ((float)sin(-Object->LegSinValue*2)*5/(LegDistance)) * GetLoadRatio();
	Skrt->Position = Bdy->Position;
	if (Object->EnemyRigidBody.onGround || Object->Position.y <= GROUNDLEVEL * GetLoadRatio())
		Skrt->CurrentFrame = (int)floor(fabs(LegUpperDirection*4));
	else
		Skrt->CurrentFrame = 3;
	Tail->Position.y = Bdy->Position.y + (Bdy->Height/30);
	Tail->Rotation = (float)sin(Object->TailSinValue*1.25f)/4;

	if (Object->Speed * GetLoadRatio() > 90.0f * GetDeltaTime() * GetLoadRatio())
	{
		Tail->SpriteTexture = LoadTexture("TextureFiles/TailDog.png");
		Object->TailSinValue += 6.0f * GetDeltaTime();
		Object->EnemySpriteParts.Tail->AnimationSpeed = (Object->Speed * GetLoadRatio())/2 + 3 * FRAMERATE / 60;
	}
	else
	{
		Tail->SpriteTexture = LoadTexture("TextureFiles/TailDog.png");
		Object->TailSinValue = 0;
		Object->EnemySpriteParts.Tail->AnimationSpeed = 4 * FRAMERATE / 60;
	}

	if (Object->EnemyRigidBody.onGround || Object->Position.y <= GROUNDLEVEL * GetLoadRatio())
	{
		if (LegUpperDirection < 0)
			LegLowerDirection = ((float)sin(Object->LegSinValue)/1.25f + (float)sin(Object->LegSinValue) * -0.1f)/(LegDistance);
		else
			LegLowerDirection = (LegUpperDirection + (float)sin(Object->LegSinValue) + (float)sin(Object->LegSinValue) * 0.4f)/(LegDistance);

		if (LegUpperDirection2 > 0)
			LegLowerDirection2 = ((float)sin(Object->LegSinValue)/1.25f + (float)sin(Object->LegSinValue) * -0.1f)/(LegDistance);
		else
			LegLowerDirection2 = (LegUpperDirection2 + (float)sin(Object->LegSinValue) + (float)sin(Object->LegSinValue) * 0.4f)/(LegDistance);
	}
	else
	{
		LegUpperDirection = (float)sin(LegDistance/10) - 1.0f;
		LegUpperDirection2 = (float)sin(LegDistance/10) - 1.0f;//60.0f * GetDeltaTime();
		LegLowerDirection = LegUpperDirection + 0.5f;//30.0f * GetDeltaTime();
		LegLowerDirection2 = LegUpperDirection2 - 0.5f;//30.0f * GetDeltaTime();
	}
	LegUpr->FlipX = Object->EnemySprite->FlipX;
	LegLwr->FlipX = Object->EnemySprite->FlipX;
	LegUpr2->FlipX = Object->EnemySprite->FlipX;
	LegLwr2->FlipX = Object->EnemySprite->FlipX;
	Bdy->FlipX = Object->EnemySprite->FlipX;
	Skrt->FlipX = Object->EnemySprite->FlipX;
	Tail->FlipX = Object->EnemySprite->FlipX;
	ArmUpr->FlipX = Object->EnemySprite->FlipX;
	ArmLwr->FlipX = Object->EnemySprite->FlipX;
	ArmUpr2->FlipX = Object->EnemySprite->FlipX;
	ArmLwr2->FlipX = Object->EnemySprite->FlipX;
	Weap->FlipX = Object->EnemySprite->FlipX;

	if (Object->EnemySprite->FlipX == FALSE)
	{
		Tail->Position.x = Bdy->Position.x+(Bdy->Width/20);
		
		LegUpr->Rotation = LegUpperDirection;
		LegUpr->Position.x = Object->Position.x;
		if (Object->EnemyRigidBody.onGround || Object->Position.y <= GROUNDLEVEL * GetLoadRatio())
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*-8/(LegDistance) * GetLoadRatio();
		}
		LegUpr->Position.y = Object->Position.y + ((float)sin(Object->LegSinValue*2)*5/(LegDistance)) * GetLoadRatio();
		LegLwr->Position.x = (float)cos(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.x;
		LegLwr->Position.y = (float)sin(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.y;
		LegLwr->Rotation = LegLowerDirection;
		
		LegUpr2->Rotation = -LegUpperDirection2;
		LegUpr2->Position.x = Object->Position.x;
		if (Object->EnemyRigidBody.onGround || Object->Position.y <= GROUNDLEVEL * GetLoadRatio())
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*8/(LegDistance) * GetLoadRatio();
		}
		LegUpr2->Position.y = Object->Position.y + ((float)sin(Object->LegSinValue*2)*5/(LegDistance)) * GetLoadRatio();
		LegLwr2->Position.x = (float)cos(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.x;
		LegLwr2->Position.y = (float)sin(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.y;
		LegLwr2->Rotation = -LegLowerDirection2;
		
		ArmUpr->Rotation = LegUpperDirection/1.5f + 1.5f;
		ArmLwr->Rotation = ArmUpr->Rotation - 1.25f + LegUpperDirection/2.0f;
		ArmUpr->Position.x = Bdy->Position.x;
		ArmUpr->Position.y = Bdy->Position.y + (Bdy->Width/5.25f);
		ArmLwr->Position.x = ArmUpr->Position.x - (float)cos(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);
		ArmLwr->Position.y = ArmUpr->Position.y - (float)sin(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);
		
		// Attacking! -----------------------------------------------------------------------------------------
		if (Object->isAttacking)
		{
			Object->EnemySpriteParts.AttackRotation = RotateToAngle(Object->EnemySpriteParts.AttackRotation, 0, 0.2f);
			Object->EnemySpriteParts.AttackRotationArm = RotateToAngle(Object->EnemySpriteParts.AttackRotationArm, (float)FOX_PI, Object->CurrentEnemyStats.AttackSpeed * GetDeltaTime());
			Object->EnemySpriteParts.AttackRotationArmLower = RotateToAngle(Object->EnemySpriteParts.AttackRotationArmLower, (float)FOX_PI/2, Object->CurrentEnemyStats.AttackSpeed * GetDeltaTime());
			ArmUpr2->Rotation = (float)FOX_PI * 1.5f + 30.0f * GetLoadRatio() - Object->EnemySpriteParts.AttackRotationArm;
			ArmLwr2->Rotation = ArmUpr2->Rotation - (float)FOX_PI/2 + Object->EnemySpriteParts.AttackRotationArmLower;
			if (Object->EnemySpriteParts.AttackRotationArm == (float)FOX_PI)
			{
				Object->isAttacking		= FALSE;
				Object->canAttack		= FALSE;
				Object->canAttackTimer	= (int)(1.5f / GetDeltaTime());
				if (rand() % 2)
				{
					Object->EnemyState = AIPassive;
					Object->StateTimer = (int)(1.0f / GetDeltaTime());
				}
			}
		}
		else
		{
			ArmUpr2->Rotation = -LegUpperDirection/1.5f + 1.5f;
			ArmLwr2->Rotation = -(ArmUpr->Rotation - 1.75f + LegUpperDirection/2.0f);
		}
		Weap->Rotation = ArmLwr2->Rotation;
		// -----------------------------------------------------------------------------------------

		ArmUpr2->Position.x = Bdy->Position.x;
		ArmUpr2->Position.y = Bdy->Position.y + (Bdy->Width/5.25f);
		ArmLwr2->Position.x = ArmUpr2->Position.x - (float)cos(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);
		ArmLwr2->Position.y = ArmUpr2->Position.y - (float)sin(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);
		
		if ((Object->Speed * GetLoadRatio()) < 0.6f * GetDeltaTime() * GetLoadRatio())
		{
			if (!Object->isAttacking)
			{
				ArmLwr->Rotation = ArmUpr->Rotation - 0.2f;
				ArmLwr2->Rotation = ArmUpr2->Rotation - 0.5f;
				Weap->Rotation = ArmLwr2->Rotation;
			}
		}
		
		Weap->Position.x = ArmLwr2->Position.x - (float)cos(ArmLwr2->Rotation) * (ArmLwr2->Width/3.5f);
		Weap->Position.y = ArmLwr2->Position.y - (float)sin(ArmLwr2->Rotation) * (ArmLwr2->Width/3.5f);
		Weap->ZIndex = Object->EnemySprite->ZIndex - 1;
		
	}
	else
	{
		Tail->Position.x = Bdy->Position.x-(Bdy->Width/20);
		
		LegUpr->Rotation = -LegUpperDirection;
		LegUpr->Position.x = Object->Position.x;
		if (Object->EnemyRigidBody.onGround || Object->Position.y <= GROUNDLEVEL * GetLoadRatio())
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*-8/(LegDistance) * GetLoadRatio();
		}
		LegUpr->Position.y = Object->Position.y + ((float)sin(Object->LegSinValue*2)*5/(LegDistance)) * GetLoadRatio();
		LegLwr->Position.x = (float)cos(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.x;
		LegLwr->Position.y = (float)sin(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.y;
		LegLwr->Rotation = -LegLowerDirection;
		
		LegUpr2->Rotation = LegUpperDirection2;
		LegUpr2->Position.x = Object->Position.x;
		if (Object->EnemyRigidBody.onGround || Object->Position.y <= GROUNDLEVEL * GetLoadRatio())
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*8/(LegDistance) * GetLoadRatio();
		}
		LegUpr2->Position.y = Object->Position.y + ((float)sin(Object->LegSinValue*2)*5/(LegDistance)) * GetLoadRatio();
		LegLwr2->Position.x = (float)cos(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.x;
		LegLwr2->Position.y = (float)sin(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.y;
		LegLwr2->Rotation = LegLowerDirection2;
		
		
		// Attacking! -----------------------------------------------------------------------------------------
		if (Object->isAttacking)
		{
			Object->EnemySpriteParts.AttackRotation = RotateToAngle(Object->EnemySpriteParts.AttackRotation, (float)FOX_PI/6, Object->CurrentEnemyStats.AttackSpeed * GetDeltaTime());
			Object->EnemySpriteParts.AttackRotationArm = RotateToAngle(Object->EnemySpriteParts.AttackRotationArm, (float)FOX_PI, Object->CurrentEnemyStats.AttackSpeed * GetDeltaTime());
			Object->EnemySpriteParts.AttackRotationArmLower = RotateToAngle(Object->EnemySpriteParts.AttackRotationArmLower, (float)FOX_PI/2, Object->CurrentEnemyStats.AttackSpeed * GetDeltaTime());
			ArmUpr->Rotation = (float)FOX_PI / 2 - 30.0f * GetLoadRatio() + Object->EnemySpriteParts.AttackRotationArm;
			ArmLwr->Rotation = ArmUpr->Rotation + (float)FOX_PI/2 - Object->EnemySpriteParts.AttackRotationArmLower;
			if (Object->EnemySpriteParts.AttackRotationArm == (float)FOX_PI)
			{
				Object->isAttacking		= FALSE;
				Object->canAttack		= FALSE;
				Object->canAttackTimer	= (int)(1.5f / GetDeltaTime());
				if (rand() % 2)
				{
					Object->EnemyState = AIPassive;
					Object->StateTimer = (int)(1.0f / GetDeltaTime());
				}
			}
		}
		else
		{
			ArmUpr->Rotation = -LegUpperDirection/1.5f - 1.5f;
			ArmLwr->Rotation = ArmUpr->Rotation + 1.25f - LegUpperDirection/2.0f;
		}
		Weap->Rotation = ArmLwr->Rotation;
		// -----------------------------------------------------------------------------------------

		ArmUpr->Position.x = Bdy->Position.x;
		ArmUpr->Position.y = Bdy->Position.y + (Bdy->Width/5.25f);
		ArmLwr->Position.x = ArmUpr->Position.x + (float)cos(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);
		ArmLwr->Position.y = ArmUpr->Position.y + (float)sin(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);

		ArmUpr2->Rotation = LegUpperDirection/1.5f - 1.5f;
		ArmLwr2->Rotation = ArmUpr2->Rotation + 1.25f + LegUpperDirection/2.0f;
		ArmUpr2->Position.x = Bdy->Position.x;
		ArmUpr2->Position.y = Bdy->Position.y + (Bdy->Width/5.25f);
		ArmLwr2->Position.x = ArmUpr2->Position.x + (float)cos(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);
		ArmLwr2->Position.y = ArmUpr2->Position.y + (float)sin(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);

		if ((Object->Speed * GetLoadRatio()) < 0.6f * GetDeltaTime() * GetLoadRatio())
		{
			if (!Object->isAttacking)
			{
				ArmLwr->Rotation = ArmUpr->Rotation + 0.5f;
				ArmLwr2->Rotation = ArmUpr2->Rotation + 0.2f;
				Weap->Rotation = ArmLwr->Rotation;
			}
		}
		
		
		Weap->Position.x = ArmLwr->Position.x + (float)cos(ArmLwr->Rotation) * (ArmLwr->Width/3.5f);
		Weap->Position.y = ArmLwr->Position.y + (float)sin(ArmLwr->Rotation) * (ArmLwr->Width/3.5f);
		Weap->ZIndex = Object->EnemySprite->ZIndex;
		
	}

	Object->EnemyWeapon->WeaponAttackPosition.x = Weap->Position.x + (cosf(Weap->Rotation + FOX_PI / 2) * Object->EnemyWeapon->WeaponLength);
	Object->EnemyWeapon->WeaponAttackPosition.y = Weap->Position.y + (sinf(Weap->Rotation + FOX_PI / 2) * Object->EnemyWeapon->WeaponLength);

	//*************************************************************************************************
}

void ResetEnemyPanelNumber(void)
{
	MemsetMyAlloc(PANELAMOUNT, sizeof(*EnemyPanelNumber), 0, EnemyPanelNumber);
}
