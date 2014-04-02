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

#define PANELSIZE 1920.0f

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
		LoadTexture("TextureFiles/Ballista.png");
		LoadTexture("TextureFiles/BallistaArrow.png");
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
		Vec2Set(&CurrentEnemy->Position, xPos, yPos);
		CurrentEnemy->EnemyType = enemyType;
		CurrentEnemy->objID = objID;
		//Creates the enemy sprite
		CurrentEnemy->EnemySprite = (Sprite *) CreateSprite("TextureFiles/StrawDummy.png", width, height, 8, 1, 1, xPos, yPos);
		
		InitializeRigidBody(&CurrentEnemy->EnemyRigidBody, TRUE, width, height);

		InitializeEnemyStats(CurrentEnemy, 50, 0, 0, 0, 0, 5, 29);

		CurrentEnemy->EnemyParticleSystem = CreateFoxParticleSystem("TextureFiles/StrawParticle.png", CurrentEnemy->Position.x, CurrentEnemy->Position.y, CurrentEnemy->EnemySprite->ZIndex + 1, 0, 5, 0.0f, 270, 90, 1.0f, -5.0f, 25, 24, 50, 2.0f, 1.0f);

		CreateCollisionBox(&CurrentEnemy->EnemyCollider, &position, EnemyType, width / 2, height / 2, objID);
		CurrentEnemy->EnemyCollider.Offset.y = -CurrentEnemy->EnemyCollider.height / 6;

		break;
	case BasicMelee:
		width = 261.0f;
		height = 373.0f;
		Vec2Set(&CurrentEnemy->Position, xPos, yPos);
		CurrentEnemy->EnemyType = enemyType;
		CurrentEnemy->objID = objID;

		//Creates the enemy sprite
		CurrentEnemy->EnemySprite = (Sprite *) CreateSprite("TextureFiles/StrawDummy.png", width, height, 8, 1, 1, xPos, yPos);
		CurrentEnemy->EnemySprite->Visible		= FALSE;
		
		InitializeRigidBody(&CurrentEnemy->EnemyRigidBody, FALSE, PLAYER_WIDTH, PLAYER_HEIGHT);
		CurrentEnemy->EnemyRigidBody.onGround	= FALSE;
		CurrentEnemy->dropDown					= FALSE;

		InitializeEnemyStats(CurrentEnemy, 50, (float)(300 + 10 * (rand() % 10)), 15.0f, 0, 10, 10 + rand() % 10, 33);

		CurrentEnemy->EnemyParticleSystem = CreateFoxParticleSystem("TextureFiles/Particle.png", CurrentEnemy->Position.x, CurrentEnemy->Position.y, CurrentEnemy->EnemySprite->ZIndex + 5, 0, 5, 0.0f, 0, 360, 1.0f, -5.0f, 25, 24, 20, 2.0f, 0.5f);

		CreateCollisionBox(&CurrentEnemy->EnemyCollider, &CurrentEnemy->Position, EnemyType, 100, 200, objID);
		CurrentEnemy->EnemyCollider.Offset.y = 20;
		CurrentEnemy->EnemyCollider.width = CurrentEnemy->EnemyCollider.width - 20;
		UpdateCollider(&CurrentEnemy->EnemyCollider, CurrentEnemy->EnemyCollider.width, CurrentEnemy->EnemyCollider.height);

		CurrentEnemy->EnemyWeapon = CreateWeapon("Random Weapon", "TextureFiles/BattleAxe.png", (int)rand() % 4, Common, WeaponEnemy, 256, 256, objID++);
		CurrentEnemy->EnemyWeapon->WeaponSprite->Created = 0;
		CreateWeaponName(&CurrentEnemy->EnemyWeapon->WeaponName, CurrentEnemy->EnemyWeapon->WeaponType, CurrentEnemy->EnemyWeapon->WeaponRarity);
		CreateWeaponStats(CurrentEnemy->EnemyWeapon->WeaponType, CurrentEnemy->EnemyWeapon->WeaponRarity, &CurrentEnemy->EnemyWeapon->BonusStrength, &CurrentEnemy->EnemyWeapon->BonusAgility, &CurrentEnemy->EnemyWeapon->BonusDefense);
		CurrentEnemy->EnemyWeapon->WeaponSprite = CreateWeaponSprite(CurrentEnemy->EnemyWeapon->WeaponType, CurrentEnemy->EnemyWeapon->WeaponRarity, xPos, yPos);
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
		CurrentEnemy->canAttackTimer	= 30;
		CurrentEnemy->findHome			= FALSE;
		CurrentEnemy->HomePos			= CurrentEnemy->Position;
		EnemyAnimation(CurrentEnemy);
		break;
	case BasicRanged:
		width = 600.0f / BALLISTA_DEVISOR;
		height = 400.0f / BALLISTA_DEVISOR;
		Vec2Set(&CurrentEnemy->Position, xPos, yPos);
		CurrentEnemy->EnemyType = enemyType;
		CurrentEnemy->objID = objID;

		//Creates the enemy sprite
		CurrentEnemy->EnemySprite = (Sprite *) CreateSprite("TextureFiles/Ballista.png", width, height, 8, 1, 1, xPos, yPos);
		
		InitializeRigidBody(&CurrentEnemy->EnemyRigidBody, FALSE, PLAYER_WIDTH, PLAYER_HEIGHT); //Lulz, a ballista with the same weight as a fox
		CurrentEnemy->EnemyRigidBody.onGround	= FALSE;
		CurrentEnemy->dropDown					= FALSE;

		InitializeEnemyStats(CurrentEnemy, 80, (float)(150 + 10 * (rand() % 10)), 15.0f, 0, 10, 20 + rand() % 20, 47);

		CurrentEnemy->EnemyParticleSystem = CreateFoxParticleSystem("TextureFiles/Particle.png", CurrentEnemy->Position.x, CurrentEnemy->Position.y, CurrentEnemy->EnemySprite->ZIndex + 5, 0, 5, 0.0f, 0, 360, 1.0f, -5.0f, 25, 24, 20, 2.0f, 0.5f);

		CreateCollisionBox(&CurrentEnemy->EnemyCollider, &CurrentEnemy->Position, EnemyType, 500 / BALLISTA_DEVISOR, 200 / BALLISTA_DEVISOR, objID);
		CurrentEnemy->EnemyCollider.Offset.y = -40 / BALLISTA_DEVISOR;
		CurrentEnemy->EnemyCollider.width = CurrentEnemy->EnemyCollider.width - 60 * BALLISTA_DEVISOR;
		UpdateCollider(&CurrentEnemy->EnemyCollider, CurrentEnemy->EnemyCollider.width, CurrentEnemy->EnemyCollider.height);

		CurrentEnemy->EnemyWeapon = CreateWeapon("Ballista Arrow You Cannot Have", "TextureFiles/BallistaArrow.png", FoxWeapon, Common, WeaponEnemy, 360 / BALLISTA_DEVISOR, 100 / BALLISTA_DEVISOR, objID++);
		CurrentEnemy->EnemyWeapon->WeaponSprite->Width = 360 / BALLISTA_DEVISOR;
		CurrentEnemy->EnemyWeapon->WeaponSprite->Height = 100 / BALLISTA_DEVISOR;
		UpdateMesh(CurrentEnemy->EnemyWeapon->WeaponSprite);
		CurrentEnemy->EnemySpriteParts.Weapon = CurrentEnemy->EnemyWeapon->WeaponSprite;
		CurrentEnemy->EnemyWeapon->WeaponFOF = EnemyWeapon;

		CurrentEnemy->Speed				= 0;
		  //Object ID? Ovah here, we ain't be needin' any a that stuff, ya hear?
		CurrentEnemy->LegSinValue		= 0;
		  //Attack wait time in seconds, none of that per frame jazz, yo.
		CurrentEnemy->TailSinValue      = (rand() / (float)RAND_MAX) + 1; //Number between 1 and 2
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
		CurrentEnemy->canAttackTimer	= 30;
		CurrentEnemy->findHome			= FALSE;
		CurrentEnemy->HomePos			= CurrentEnemy->Position;
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
	No need to have this loop in every level
*/
/*************************************************************************/
void UpdateAllEnemies(void)
{
	int i;

	for(i = 0; i < COLLIDEAMOUNT; i++)
	{
		//Update the created enemies
		if (enemyList[i].objID == -1)
			break;
		if (enemyList[i].objID == 0)
			continue;

		UpdateEnemy(&enemyList[i]);
	}
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
		DetectEnemyCollision(CurrentEnemy);
		EnemyAIUpdate(CurrentEnemy);
		EnemyBasicRangedUpdate(CurrentEnemy);
		EnemyAnimationBasicRanged(CurrentEnemy);
		UpdateCollisionPosition(&CurrentEnemy->EnemyWeapon->WeaponAttack, &CurrentEnemy->EnemyWeapon->WeaponAttackPosition);
		break;
	default:
		break;
	}

	CurrentEnemy->EnemyParticleSystem->Position.x = CurrentEnemy->Position.x;
	CurrentEnemy->EnemyParticleSystem->Position.y = CurrentEnemy->Position.y;

	if(CurrentEnemy->CurrentEnemyStats.CurrentHealth <= 0)
	{
		char num[10];
		int dropWeapon;
		Vec3 textColor;
		TextGlyphs *FirstLetter;

		//Give the player thier loot!
		CurrentPlayer.CurrentPlayerStats.Money      += CurrentEnemy->CurrentEnemyStats.Money;
		CurrentPlayer.CurrentPlayerStats.Experience += CurrentEnemy->CurrentEnemyStats.Experience;
		
		Vec3Set(&textColor, 1.0f, 1.0f, 0.0f);
		sprintf(num, "+%d", CurrentEnemy->CurrentEnemyStats.Money);
		// Create Floating Coin Text
		FirstLetter = CreateText(num, (CurrentEnemy->Position.x + rand() % 81 - 40), (CurrentPlayer.Position.y + CurrentPlayer.PlayerSpriteParts.Body->Height / 2), 80, textColor, Center, Border);
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
		if (CurrentEnemy->EnemyType == BasicRanged)
			CurrentEnemy->EnemyParticleSystem->emitScale = 3.2f;
		else
			CurrentEnemy->EnemyParticleSystem->emitScale = 2.0f;
		CurrentEnemy->EnemyParticleSystem->emitLife = 1.0f;
		CurrentEnemy->EnemyParticleSystem->emitThenDestroy = TRUE;

		//Give the player a chance at the enemy's weapon sometimes
		if (CurrentEnemy->EnemyType == BasicMelee && GetCurrentState() != GS_Level7) //No weapon drop on the super spawn level
			dropWeapon = rand() % 2;
		else
			dropWeapon = 0;

		if (dropWeapon == 1)
		{
			Weapon *CurrentWeapon = CreateDroppedWeapon(CurrentEnemy->EnemyWeapon->WeaponType, CurrentEnemy->EnemyWeapon->WeaponRarity, 250, 250, CurrentEnemy->objID, CurrentEnemy->Position.x, CurrentEnemy->Position.y);
			CurrentWeapon->WeaponSprite->Rotation = FOX_PI / 2 + (rand() % 50 - 20) / 4; //A random angle for the dropped weapon
			CurrentWeapon->WeaponFalling = TRUE;
		}

		PlayAudio(CurrentEnemy->CurrentEnemySounds.Poof);
		EnemyPanelNumber[CurrentEnemy->panelId]--;

		// Free all the sounds
		freeSound(CurrentEnemy->CurrentEnemySounds.GetHit1);
		freeSound(CurrentEnemy->CurrentEnemySounds.GetHit2);
		freeSound(CurrentEnemy->CurrentEnemySounds.Poof);
		freeSound(CurrentEnemy->CurrentEnemySounds.Swing1);
		freeSound(CurrentEnemy->CurrentEnemySounds.Swing2);

		FreeEnemy(CurrentEnemy);
	}

	if(CurrentEnemy->Position.y < GROUNDLEVEL && CurrentEnemy->EnemyState != AINone)
	{
		CurrentEnemy->Position.y = GROUNDLEVEL;
	}
	//Stop velocity and acceleration when the player lands on the floor
	if(CurrentEnemy->Position.y <= GROUNDLEVEL || CurrentEnemy->EnemyRigidBody.onGround)
	{
		Vec2Zero(&CurrentEnemy->EnemyRigidBody.Acceleration);
		Vec2Zero(&CurrentEnemy->EnemyRigidBody.Velocity);
		ZeroGravity(&CurrentEnemy->EnemyRigidBody);
	}
	//Set gravity if not on floor or on a platform
	else
	{
		SetGravity(&CurrentEnemy->EnemyRigidBody, 0.0f, FOX_GRAVITY_Y);
	}

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

		if(CurrentEnemy->KnockBackTime <= 0 && (CurrentEnemy->EnemyRigidBody.onGround || CurrentEnemy->Position.y <= GROUNDLEVEL))
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
		CurrentEnemy->Speed					= CurrentEnemy->CurrentEnemyStats.MoveSpeed * GetDeltaTime();
	}
	// Move right if D is pressed
	else if(CurrentEnemy->isMoveRight)
	{
		CurrentEnemy->EnemySprite->FlipX	= TRUE;
		CurrentEnemy->EnemyDirection		= RIGHT;
		CurrentEnemy->Speed					= CurrentEnemy->CurrentEnemyStats.MoveSpeed * GetDeltaTime();
	}
	// not key press for direction then slow down!
	else
	{
		if (!(CurrentEnemy->Position.y > GROUNDLEVEL) && !CurrentEnemy->EnemyRigidBody.onGround)
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
		
		Vec2Set(&velocity, 0.0f, 1080.0f);
		if(CurrentEnemy->Position.y <= GROUNDLEVEL || CurrentEnemy->EnemyRigidBody.onGround)
		{
			if(CurrentEnemy->Position.y <= GROUNDLEVEL)
				Vec2Set(&CurrentEnemy->Position, CurrentEnemy->Position.x, GROUNDLEVEL + 0.1f);
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

void EnemyBasicRangedUpdate(Enemy *CurrentEnemy)
{
	//Do nothing if offline
	if (CurrentEnemy->EnemyState == AINone)
		return;

	// Move left if A is pressed
	if(CurrentEnemy->isMoveLeft)
	{
		CurrentEnemy->EnemySprite->FlipX	= TRUE;
		CurrentEnemy->EnemyDirection		= LEFT;
		CurrentEnemy->Speed					= CurrentEnemy->CurrentEnemyStats.MoveSpeed * GetDeltaTime();
	}
	// Move right if D is pressed
	else if(CurrentEnemy->isMoveRight)
	{
		CurrentEnemy->EnemySprite->FlipX	= FALSE;
		CurrentEnemy->EnemyDirection		= RIGHT;
		CurrentEnemy->Speed					= CurrentEnemy->CurrentEnemyStats.MoveSpeed * GetDeltaTime();
	}
	// not key press for direction then slow down!
	else
	{
		if (!(CurrentEnemy->Position.y > GROUNDLEVEL) && !CurrentEnemy->EnemyRigidBody.onGround)
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
		
		Vec2Set(&velocity, 0.0f, 1080.0f);
		if(CurrentEnemy->Position.y <= GROUNDLEVEL || CurrentEnemy->EnemyRigidBody.onGround)
		{
			if(CurrentEnemy->Position.y <= GROUNDLEVEL)
				Vec2Set(&CurrentEnemy->Position, CurrentEnemy->Position.x, GROUNDLEVEL + 0.1f);
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

	//Don't shoot if we are in idle or aren't in sight, that is just rude
	if (CurrentEnemy->EnemyState == AIIdle || CurrentEnemy->Position.y + 300 < CurrentPlayer.Position.y
		|| CurrentEnemy->Position.x > GetCameraXPosition() + (PANELSIZE + CurrentEnemy->EnemySprite->Width / 4) / 2
		|| CurrentEnemy->Position.x < GetCameraXPosition() - (PANELSIZE + CurrentEnemy->EnemySprite->Width / 4) / 2)
	{
		//Reverse the shoot cycle if needed
		if (CurrentEnemy->TailSinValue < 0.8)
			CurrentEnemy->TailSinValue += GetDeltaTime();
		return;
	}

	//Shooting
	if (!CurrentEnemy->canAttack)
	{
		//GRIMY H4X OVAH H3RRE
		if (CurrentEnemy->TailSinValue < 0)
			CurrentEnemy->canAttack = TRUE;

		//Arrow back yet dog?
		if (CurrentEnemy->TailSinValue < 0.8 && !CurrentEnemy->EnemySpriteParts.Weapon->Visible)
				CurrentEnemy->EnemySpriteParts.Weapon->Visible = TRUE;

		//Time ticks by
		CurrentEnemy->TailSinValue -= GetDeltaTime();
	}
	else
	{
		//Project this right bby gril
		Projectile *smexyArrow;
		float projectileSpeed = 1400;
		if (CurrentEnemy->EnemySpriteParts.Weapon->FlipX)
			projectileSpeed *= -1;

		//NASTY NASTY HACKKKK
		smexyArrow = CreateProjectile("TextureFiles/BallistaArrow.png", 
									  CurrentEnemy->EnemySpriteParts.Weapon->Width, CurrentEnemy->EnemySpriteParts.Weapon->Height, 
									  CurrentEnemy->EnemySpriteParts.Weapon->Position.x, CurrentEnemy->EnemySpriteParts.Weapon->Position.y, 
									  Arrow, WeaponEnemy, (CurrentEnemy->objID + 100) * 8 + (int)CurrentEnemy->LegSinValue++, 10, projectileSpeed, 0);

		if (smexyArrow->ProjectileSprite->FlipX)
			smexyArrow->ProjectileAttack.Offset.x = -smexyArrow->ProjectileAttack.width / 3;
		else
			smexyArrow->ProjectileAttack.Offset.x = smexyArrow->ProjectileAttack.width / 3;
		UpdateCollider(&smexyArrow->ProjectileAttack, smexyArrow->ProjectileAttack.width / 4, smexyArrow->ProjectileAttack.height / 4);
		
		//Sound off
		PlayAudio(rand() % 2 ? CurrentEnemy->CurrentEnemySounds.Swing1 : CurrentEnemy->CurrentEnemySounds.Swing1);

		//Arrow long gone
		CurrentEnemy->EnemySpriteParts.Weapon->Visible = FALSE;

		//Reset my timerz
		CurrentEnemy->canAttack = FALSE;
		CurrentEnemy->TailSinValue = (rand() / (float)RAND_MAX) + 2; //Number between 2 and 3
	}
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

			if (CurrentEnemy->Position.x < CurrentPlayer.Position.x - 100)
			{
				CurrentEnemy->isMoveRight	= TRUE;
				CurrentEnemy->isMoveLeft	= FALSE;
			}
			else if (CurrentEnemy->Position.x > CurrentPlayer.Position.x + 100)
			{
				CurrentEnemy->isMoveRight	= FALSE;
				CurrentEnemy->isMoveLeft	= TRUE;
			}
			else
			{
				CurrentEnemy->isMoveRight	= FALSE;
				CurrentEnemy->isMoveLeft	= FALSE;
			}

			if (CurrentEnemy->EnemyType != BasicRanged && fabs(CurrentEnemy->Position.x - CurrentPlayer.Position.x) < 200)
			{
				if (CurrentEnemy->canAttackTimer > 0)
				{
					CurrentEnemy->canAttack = FALSE;
					CurrentEnemy->canAttackTimer--;
				}
				else
					CurrentEnemy->canAttack = TRUE;
			}

			if (CurrentEnemy->Position.y < CurrentPlayer.Position.y - 60)
			{
				int i;
				for (i = 0; i < COLLIDEAMOUNT; i++)
				{
					if (platformList[i].objID == 0)
						continue;
					if (platformList[i].objID == -1)
						break;
					//Don't jump if you are a ballista  :'[
					if (CurrentEnemy->EnemyType != BasicRanged && CurrentEnemy->Position.y < platformList[i].Position.y + 40 &&
						CurrentEnemy->Position.x > platformList[i].Position.x - (CurrentEnemy->Speed * (CurrentEnemy->EnemyDirection == RIGHT ? 1 : 0) / GetDeltaTime()) - 60.0f && 
						CurrentEnemy->Position.x < platformList[i].Position.x + (CurrentEnemy->Speed * (CurrentEnemy->EnemyDirection == LEFT ? 1 : 0) / GetDeltaTime()) + 60.0f)
					{
						CurrentEnemy->isJumping			= TRUE;
						CurrentEnemy->canDropDownTimer	= (int)(2 / GetDeltaTime());
						break;
					}
				}
			}

			if (CurrentEnemy->Position.y < CurrentPlayer.Position.y - 10)
			{
				int i;
				for (i = 0; i < COLLIDEAMOUNT; i++)
				{
					if (wallList[i].objID == 0)
						continue;
					if (wallList[i].objID == -1)
						break;
					
					if (CurrentEnemy->Position.x > wallList[i].Position.x - (CurrentEnemy->Speed * (CurrentEnemy->EnemyDirection == RIGHT ? 1 : 0) / GetDeltaTime()) - 60.0f && 
						CurrentEnemy->Position.x < wallList[i].Position.x + (CurrentEnemy->Speed * (CurrentEnemy->EnemyDirection == LEFT ? 1 : 0) / GetDeltaTime()) + 60.0f &&
						CurrentEnemy->EnemyType != BasicRanged && wallList[i].Position.y + CurrentEnemy->EnemyCollider.height * 2 > CurrentEnemy->Position.y - CurrentEnemy->EnemyCollider.height)
					{
						CurrentEnemy->isJumping			= TRUE;
						CurrentEnemy->canDropDownTimer	= (int)(2 / GetDeltaTime());
						break;
					}
				}
			}
			else if (CurrentEnemy->EnemyRigidBody.onGround && CurrentEnemy->canDropDownTimer <= 0 && CurrentEnemy->Position.y > CurrentPlayer.Position.y + 10)
			{
				int i;
				for (i = 0; i < COLLIDEAMOUNT; i++)
				{
					if (platformList[i].objID == 0)
						continue;
					if (platformList[i].objID == -1)
						break;
				if (CurrentEnemy->Position.x > platformList[i].Position.x - (CurrentEnemy->Speed * (CurrentEnemy->EnemyDirection == RIGHT ? 1 : 0.25f) / GetDeltaTime()) - 60.0f && 
					CurrentEnemy->Position.x < platformList[i].Position.x + (CurrentEnemy->Speed * (CurrentEnemy->EnemyDirection == LEFT ? 1 : 0.25f) / GetDeltaTime()) + 60.0f &&
					platformList[i].Position.y + platformList[i].PlatformCollider.height / 2 > CurrentEnemy->Position.y - CurrentEnemy->EnemyCollider.height / 1.5f)
					CurrentEnemy->isDropDown		= TRUE;
				}
			}
			
			if (CurrentEnemy->canDropDownTimer > 0)
				CurrentEnemy->canDropDownTimer--;

			if (CurrentEnemy->EnemyType != BasicRanged && Vec2Distance(&CurrentEnemy->Position, &CurrentPlayer.Position) < 150 && !CurrentEnemy->isAttacking && !CurrentEnemy->Attack)
			{
				CurrentEnemy->canAttackTimer	= (int)(0.5f / GetDeltaTime());
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
			if (CurrentEnemy->EnemyType == BasicRanged && Vec2Distance(&CurrentEnemy->Position, &CurrentPlayer.Position) < 1600.0f
				&& CurrentEnemy->Position.x < GetCameraXPosition() + (PANELSIZE + CurrentEnemy->EnemySprite->Width) / 2
				&& CurrentEnemy->Position.x > GetCameraXPosition() - (PANELSIZE + CurrentEnemy->EnemySprite->Width) / 2)
			{
				CurrentEnemy->EnemyState = AIAggressive;
			}
			else if (CurrentEnemy->EnemyType != BasicRanged && Vec2Distance(&CurrentEnemy->Position, &CurrentPlayer.Position) < 500.0f)
			{
				CurrentEnemy->EnemyState = AIAggressive;
			}
			if ((float)fabs((float)(CurrentEnemy->Position.x - CurrentEnemy->HomePos.x)) > 500.0f)
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
					if (CurrentEnemy->Position.x < CurrentEnemy->HomePos.x - 60)
					{
						CurrentEnemy->isMoveRight	= TRUE;
						CurrentEnemy->isMoveLeft	= FALSE;
					}
					else if (CurrentEnemy->Position.x > CurrentEnemy->HomePos.x + 60)
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

					if (CurrentEnemy->Position.y < CurrentEnemy->HomePos.y - 20)
					{
						int i;
						for (i = 0; i < COLLIDEAMOUNT; i++)
						{
							if (platformList[i].objID == 0)
								continue;
							if (platformList[i].objID == -1)
								break;
							if (CurrentEnemy->EnemyType != BasicRanged && CurrentEnemy->Position.y < platformList[i].Position.y + 40 &&
								CurrentEnemy->Position.x > platformList[i].Position.x - (CurrentEnemy->Speed * (CurrentEnemy->EnemyDirection == RIGHT ? 1 : 0) / GetDeltaTime()) - 60.0f && 
								CurrentEnemy->Position.x < platformList[i].Position.x + (CurrentEnemy->Speed * (CurrentEnemy->EnemyDirection == LEFT ? 1 : 0) / GetDeltaTime()) + 60.0f)
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
							if (CurrentEnemy->Position.x > wallList[i].Position.x - (CurrentEnemy->Speed * (CurrentEnemy->EnemyDirection == RIGHT ? 1 : 0) / GetDeltaTime()) - 60.0f && 
								CurrentEnemy->Position.x < wallList[i].Position.x + (CurrentEnemy->Speed * (CurrentEnemy->EnemyDirection == LEFT ? 1 : 0) / GetDeltaTime()) + 60.0f &&
								CurrentEnemy->EnemyType != BasicRanged && wallList[i].Position.y + CurrentEnemy->EnemyCollider.height * 2 > CurrentEnemy->Position.y - CurrentEnemy->EnemyCollider.height)
							{
								CurrentEnemy->isJumping			= TRUE;
								CurrentEnemy->canDropDownTimer	= (int)(2 / GetDeltaTime());
								break;
							}
						}
					}
					else if (CurrentEnemy->EnemyRigidBody.onGround && CurrentEnemy->canDropDownTimer <= 0 && CurrentEnemy->Position.y > CurrentEnemy->HomePos.y + 10)
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

	Object->EnemySpriteParts.Tail->AnimationSpeed = (Object->Speed)/2 + 3;

	Object->EnemySpriteParts.ArmLower = (Sprite *) CreateSpriteNoMesh("TextureFiles/ArmLowerDog.png", 128.0f, 128.0f, Object->EnemySprite->ZIndex + 2, 1, 1, 0, 0);
	Object->EnemySpriteParts.ArmLower->SpriteMesh = Object->EnemySpriteParts.ArmUpper->SpriteMesh;

	Object->EnemySpriteParts.Skirt = (Sprite *) CreateSpriteNoMesh("TextureFiles/SkirtDog.png", 300.0f, 300.0f, Object->EnemySprite->ZIndex + 1, 4, 1, 0, 0);
	Object->EnemySpriteParts.Skirt->SpriteMesh = Object->EnemySpriteParts.Body->SpriteMesh;

	Object->EnemySpriteParts.Skirt->AnimationActive = 0;
}

void ResetEnemyPanelNumber(void)
{
	MemsetMyAlloc(PANELAMOUNT, sizeof(*EnemyPanelNumber), 0, EnemyPanelNumber);
}
