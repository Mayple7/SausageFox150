/*****************************************************************************/
/*!
\file				Movement.c
\author				Lucas Conlon (l.conlon)
\date				Jan 11, 2014

\brief				Object movement function, and animations for movement

\par				Functions:
\li					MoveObject
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include "../HeaderFiles/Movement.h"
#include "../AEEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/HandGuyBoss.h"
#include "../HeaderFiles/KevinBoss.h"

static enum HeadColor {RedHead, GreenHead, BlueHead };
static enum YeahGuyState { CooldownYeah, AOE, ProjectYeah, Pound, Vault };
static enum HandGuyState { CooldownHand, Shout, Question, Jab, Move };
static enum InnerState { Start, Attack, End };

/*************************************************************************/
/*!
	\brief
	Moves an object a specific direction and distance
	
	\param Pos
	The position to be moved

	\param dirMove
	An enum of the direction to move

	\param move_dist
	The distance to move the object
*/
/*************************************************************************/
void MoveObject(Vec2 *Pos, enum Direction dirMove, float move_dist)
{
	switch (dirMove)
	{
		case LEFT:
			Pos->x -= move_dist;
			break;
		case RIGHT:
			Pos->x += move_dist;
			break;
		case UP:
			Pos->y += move_dist;
			break;
		case DOWN:
			Pos->y -= move_dist;
			break;
	}
}


/*************************************************************************/
/*!
	\brief
	Animates the Ballista.
*/
/*************************************************************************/
void EnemyAnimationBasicRanged(Enemy *Object)
{
	Object->EnemySprite->Position.x = Object->Position.x;
	Object->EnemySprite->Position.y = Object->Position.y;

	//Arrow position
	if (Object->EnemySprite->FlipX)
	{
		Object->EnemyWeapon->Position.x = Object->Position.x + 12 / BALLISTA_DEVISOR;
		Object->EnemyWeapon->Position.y = Object->Position.y + 56 / BALLISTA_DEVISOR;

		//Draw back the arrow
		if (Object->TailSinValue < 0.5)
			Object->EnemyWeapon->Position.x += 70 - Object->TailSinValue * 140;
	}
	else
	{
		Object->EnemyWeapon->Position.x = Object->Position.x - 12 / BALLISTA_DEVISOR;
		Object->EnemyWeapon->Position.y = Object->Position.y + 56 / BALLISTA_DEVISOR;

		//Draw back the arrow
		if (Object->TailSinValue < 0.5)
			Object->EnemyWeapon->Position.x -= 70 - Object->TailSinValue * 140;
	}

	Object->EnemySpriteParts.Weapon->FlipX = Object->EnemySprite->FlipX;
	Object->EnemySpriteParts.Weapon->Position.x = Object->EnemyWeapon->Position.x;
	Object->EnemySpriteParts.Weapon->Position.y = Object->EnemyWeapon->Position.y;
}

/*************************************************************************/
/*!
	\brief
	Animates the enemies legs and arms.
*/
/*************************************************************************/
void EnemyAnimation(Enemy *Object)
{
	float LegDistance = Object->CurrentEnemyStats.MoveSpeed * (1 / 60.0f) + (2.3f / (((Object->CurrentEnemyStats.MoveSpeed * (1 / 60.0f)) * 0.075f + 0.5f)) ) - (Object->Speed / GetDeltaTime()) * (1 / 60.0f);
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

	Object->LegSinValue += (Object->Speed) / 45.0f; 

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

	Object->TailSinValue += GetDeltaTime() * 5;

	Bdy->Position.x = Object->Position.x;
	Bdy->Position.y = Object->Position.y - ((float)sin(-Object->LegSinValue*2)*5/(LegDistance));
	Skrt->Position = Bdy->Position;
	if (Object->EnemyRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
		Skrt->CurrentFrame = (int)floor(fabs(LegUpperDirection*4));
	else
		Skrt->CurrentFrame = 3;
	Tail->Position.y = Bdy->Position.y + (Bdy->Height/30);
	Tail->Rotation = (float)sin(Object->TailSinValue*1.25f)/4;

	if (Object->EnemyRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
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
		if (Object->EnemyRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*-8/(LegDistance);
		}
		LegUpr->Position.y = Object->Position.y + ((float)sin(Object->LegSinValue*2)*5/(LegDistance));
		LegLwr->Position.x = (float)cos(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.x;
		LegLwr->Position.y = (float)sin(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.y;
		LegLwr->Rotation = LegLowerDirection;
		
		LegUpr2->Rotation = -LegUpperDirection2;
		LegUpr2->Position.x = Object->Position.x;
		if (Object->EnemyRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*8/(LegDistance);
		}
		LegUpr2->Position.y = Object->Position.y + ((float)sin(Object->LegSinValue*2)*5/(LegDistance));
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
			Object->EnemySpriteParts.AttackRotationArm = RotateToAngle(Object->EnemySpriteParts.AttackRotationArm, FOX_PI, Object->CurrentEnemyStats.AttackSpeed * GetDeltaTime());
			Object->EnemySpriteParts.AttackRotationArmLower = RotateToAngle(Object->EnemySpriteParts.AttackRotationArmLower, FOX_PI/2, Object->CurrentEnemyStats.AttackSpeed * GetDeltaTime());
			ArmUpr2->Rotation = FOX_PI * 1.5f + 0.5f - Object->EnemySpriteParts.AttackRotationArm;
			ArmLwr2->Rotation = ArmUpr2->Rotation - FOX_PI/2 + Object->EnemySpriteParts.AttackRotationArmLower;
			if (Object->EnemySpriteParts.AttackRotationArm == FOX_PI)
			{
				Object->isAttacking		= FALSE;
				Object->canAttack		= FALSE;
				Object->canAttackTimer	= (int)(0.5f / GetDeltaTime());
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
		
		if ((Object->Speed) < 0.6f * GetDeltaTime())
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
		if (Object->EnemyRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*-8/(LegDistance);
		}
		LegUpr->Position.y = Object->Position.y + ((float)sin(Object->LegSinValue*2)*5/(LegDistance));
		LegLwr->Position.x = (float)cos(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.x;
		LegLwr->Position.y = (float)sin(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.y;
		LegLwr->Rotation = -LegLowerDirection;
		
		LegUpr2->Rotation = LegUpperDirection2;
		LegUpr2->Position.x = Object->Position.x;
		if (Object->EnemyRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*8/(LegDistance);
		}
		LegUpr2->Position.y = Object->Position.y + ((float)sin(Object->LegSinValue*2)*5/(LegDistance));
		LegLwr2->Position.x = (float)cos(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.x;
		LegLwr2->Position.y = (float)sin(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.y;
		LegLwr2->Rotation = LegLowerDirection2;
		
		
		// Attacking! -----------------------------------------------------------------------------------------
		if (Object->isAttacking)
		{
			Object->EnemySpriteParts.AttackRotation = RotateToAngle(Object->EnemySpriteParts.AttackRotation, FOX_PI/6, Object->CurrentEnemyStats.AttackSpeed * GetDeltaTime());
			Object->EnemySpriteParts.AttackRotationArm = RotateToAngle(Object->EnemySpriteParts.AttackRotationArm, FOX_PI, Object->CurrentEnemyStats.AttackSpeed * GetDeltaTime());
			Object->EnemySpriteParts.AttackRotationArmLower = RotateToAngle(Object->EnemySpriteParts.AttackRotationArmLower, FOX_PI/2, Object->CurrentEnemyStats.AttackSpeed * GetDeltaTime());
			ArmUpr->Rotation = FOX_PI / 2 - 0.5f + Object->EnemySpriteParts.AttackRotationArm;
			ArmLwr->Rotation = ArmUpr->Rotation + FOX_PI/2 - Object->EnemySpriteParts.AttackRotationArmLower;
			if (Object->EnemySpriteParts.AttackRotationArm == FOX_PI)
			{
				Object->isAttacking		= FALSE;
				Object->canAttack		= FALSE;
				Object->canAttackTimer	= (int)(0.5f / GetDeltaTime());
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

		if ((Object->Speed) < 0.6f * GetDeltaTime())
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


/*************************************************************************/
/*!
	\brief
	Animates the players legs.

	\param Object
	The player to animate
*/
/*************************************************************************/
void Animation(Player *Object)
{
	float DT1 = FRAMERATE * GetDeltaTime(); //Just so it isn't hard-coded too much

	float sinOfLegValue = (float)sin(Object->LegSinValue);
	float sinOfTwoLegValue = (float)sin(Object->LegSinValue * 2);
	float LegDistance = Object->CurrentPlayerStats.MoveSpeed * (1 / 60.0f) + (2.3f / (((Object->CurrentPlayerStats.MoveSpeed * (1 / 60.0f)) * 0.15f + 0.15f)) ) - (Object->Speed / GetDeltaTime()) * (1 / 60.0f);
	float LegUpperDirection = sinOfLegValue / (LegDistance);
	float LegLowerDirection;
	float LegUpperDirection2 = sinOfLegValue / (LegDistance);
	float LegLowerDirection2;
	
	Sprite *LegUpr = Object->PlayerSpriteParts.LegUpper;
	Sprite *LegUpr2 = Object->PlayerSpriteParts.LegUpper2;
	Sprite *LegLwr = Object->PlayerSpriteParts.LegLower;
	Sprite *LegLwr2 = Object->PlayerSpriteParts.LegLower2;
	Sprite *ArmUpr = Object->PlayerSpriteParts.ArmUpper;
	Sprite *ArmUpr2 = Object->PlayerSpriteParts.ArmUpper2;
	Sprite *ArmLwr = Object->PlayerSpriteParts.ArmLower;
	Sprite *ArmLwr2 = Object->PlayerSpriteParts.ArmLower2;
	Sprite *Skrt = Object->PlayerSpriteParts.Skirt;
	Sprite *Bdy = Object->PlayerSpriteParts.Body;
	Sprite *Weap = Object->PlayerSpriteParts.Weapon;
	Sprite *Tail = Object->PlayerSpriteParts.Tail;

	
	Object->LegSinValue += (Object->Speed) / 75.0f; 

	Object->PlayerSpriteParts.BlinkTimer += 1;

	if (Object->PlayerSpriteParts.BlinkTimer <= 150)
	{
		Object->PlayerSpriteParts.Body->AnimationActive = 0;
	}
	else
	{
		Object->PlayerSpriteParts.Body->AnimationActive = 1;
		if (Object->PlayerSpriteParts.Body->CurrentFrame == 3)
		{
			Object->PlayerSpriteParts.BlinkTimer = 0;
			Object->PlayerSpriteParts.Body->CurrentFrame = 0;
		}
	}

	Bdy->Position.x = Object->Position.x;
	Bdy->Position.y = Object->Position.y - ((float)sin(-Object->LegSinValue * 2) * 5 / (LegDistance));
	Skrt->Position = Bdy->Position;
	if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
		Skrt->CurrentFrame = (int)floor(fabs(LegUpperDirection * 4));
	else
		Skrt->CurrentFrame = 3;
	Tail->Position.y = Bdy->Position.y + (Bdy->Height / 30);
	Tail->Rotation = (float)sin(Object->TailSinValue * 1.25f) / 4;

	if (Object->Speed > 90.0f * GetDeltaTime())
	{
		Tail->SpriteTexture = LoadTexture("TextureFiles/TailRun.png");
		Object->TailSinValue += 6.0f * GetDeltaTime();
		Object->PlayerSpriteParts.Tail->AnimationSpeed = (Object->Speed) / 2 + 3 * DT1;
	}
	else
	{
		Tail->SpriteTexture = LoadTexture("TextureFiles/TailIdle.png");
		Object->TailSinValue = 0;
		if(Object->Princess == Mayple)
			Object->PlayerSpriteParts.Tail->AnimationSpeed = 2 * DT1;
		else
			Object->PlayerSpriteParts.Tail->AnimationSpeed = 4 * DT1;
	}

	if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
	{
		if (LegUpperDirection < 0)
			LegLowerDirection = (sinOfLegValue/1.25f + sinOfLegValue * -0.1f)/(LegDistance);
		else
			LegLowerDirection = (LegUpperDirection + sinOfLegValue + sinOfLegValue * 0.4f)/(LegDistance);

		if (LegUpperDirection2 > 0)
			LegLowerDirection2 = (sinOfLegValue/1.25f + sinOfLegValue * -0.1f)/(LegDistance);
		else
			LegLowerDirection2 = (LegUpperDirection2 + sinOfLegValue + sinOfLegValue * 0.4f)/(LegDistance);
	}
	else
	{
		float sinLegOverTen = (float)sin(LegDistance / 10);
		LegUpperDirection  = sinLegOverTen - 1.0f;
		LegUpperDirection2 = sinLegOverTen - 1.0f;
		LegLowerDirection  = LegUpperDirection + 0.5f;
		LegLowerDirection2 = LegUpperDirection2 - 0.5f;
	}
	LegUpr->FlipX = Object->FlipX;
	LegLwr->FlipX = Object->FlipX;
	LegUpr2->FlipX = Object->FlipX;
	LegLwr2->FlipX = Object->FlipX;
	Bdy->FlipX = Object->FlipX;
	Skrt->FlipX = Object->FlipX;
	Tail->FlipX = Object->FlipX;
	ArmUpr->FlipX = Object->FlipX;
	ArmLwr->FlipX = Object->FlipX;
	ArmUpr2->FlipX = Object->FlipX;
	ArmLwr2->FlipX = Object->FlipX;
	Weap->FlipX = Object->FlipX;

	if (Object->FlipX == FALSE)
	{
		Tail->Position.x = Bdy->Position.x+(Bdy->Width/20);
		
		LegUpr->Rotation = LegUpperDirection;
		LegUpr->Position.x = Object->Position.x;
		if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
		{
			LegUpr2->Position.x += sinOfLegValue*-8/(LegDistance);
		}
		LegUpr->Position.y = Object->Position.y + (sinOfTwoLegValue*5/(LegDistance));
		LegLwr->Position.x = (float)cos(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.x;
		LegLwr->Position.y = (float)sin(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.y;
		LegLwr->Rotation = LegLowerDirection;
		
		LegUpr2->Rotation = -LegUpperDirection2;
		LegUpr2->Position.x = Object->Position.x;
		if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
		{
			LegUpr2->Position.x += sinOfLegValue*8/(LegDistance);
		}
		LegUpr2->Position.y = Object->Position.y + (sinOfTwoLegValue*5/(LegDistance));
		LegLwr2->Position.x = (float)cos(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.x;
		LegLwr2->Position.y = (float)sin(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.y;
		LegLwr2->Rotation = -LegLowerDirection2;
		
		ArmUpr->Rotation = LegUpperDirection/1.5f + 1.5f;
		ArmLwr->Rotation = ArmUpr->Rotation - 1.25f + LegUpperDirection/2.0f;
		ArmUpr->Position.x = Bdy->Position.x;
		ArmUpr->Position.y = Bdy->Position.y + (Bdy->Width/5.25f);
		ArmLwr->Position.x = ArmUpr->Position.x - (float)cos(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);
		ArmLwr->Position.y = ArmUpr->Position.y - (float)sin(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);

		// Attacking!
		if (Object->isAttacking)
		{
			if (Object->PlayerWeapon->WeaponType != Spear || (Object->PlayerWeapon->WeaponType == Spear && Object->AttackType == 1))
			{
				Object->PlayerSpriteParts.AttackRotation = RotateToAngle(Object->PlayerSpriteParts.AttackRotation, 0, 0.2f);
				Object->PlayerSpriteParts.AttackRotationArm = RotateToAngle(Object->PlayerSpriteParts.AttackRotationArm, FOX_PI, Object->CurrentPlayerStats.AttackSpeed * GetDeltaTime());
				Object->PlayerSpriteParts.AttackRotationArmLower = RotateToAngle(Object->PlayerSpriteParts.AttackRotationArmLower, FOX_PI/2, Object->CurrentPlayerStats.AttackSpeed * GetDeltaTime());
				ArmUpr2->Rotation = FOX_PI * 1.5f + 0.5f - Object->PlayerSpriteParts.AttackRotationArm;
				ArmLwr2->Rotation = ArmUpr2->Rotation - FOX_PI/2 + Object->PlayerSpriteParts.AttackRotationArmLower;
				Weap->Rotation = ArmLwr2->Rotation;
				if (Object->PlayerSpriteParts.AttackRotationArm == FOX_PI)
				Object->isAttacking = FALSE;
			}
			else 
			{
				Object->PlayerSpriteParts.AttackRotation = RotateToAngle(Object->PlayerSpriteParts.AttackRotation, FOX_PI / 2.1f, Object->CurrentPlayerStats.AttackSpeed * GetDeltaTime());
				Object->PlayerSpriteParts.AttackRotationArm = RotateToAngle(Object->PlayerSpriteParts.AttackRotationArm, FOX_PI * 0.9f, Object->CurrentPlayerStats.AttackSpeed * GetDeltaTime());
				Object->PlayerSpriteParts.AttackRotationArmLower = RotateToAngle(Object->PlayerSpriteParts.AttackRotationArmLower, FOX_PI/2, Object->CurrentPlayerStats.AttackSpeed * GetDeltaTime());
				ArmUpr2->Rotation = FOX_PI * 0.75f - Object->PlayerSpriteParts.AttackRotationArm;
				ArmLwr2->Rotation = ArmUpr2->Rotation - Object->PlayerSpriteParts.AttackRotationArmLower / 2;
				Weap->Rotation = ArmLwr2->Rotation + Object->PlayerSpriteParts.AttackRotation;
				if (Object->PlayerSpriteParts.AttackRotationArm == FOX_PI * 0.9f)
					Object->isAttacking = FALSE;
			}
		}
		else
		{
			ArmUpr2->Rotation = -LegUpperDirection/1.5f + 1.5f;
			ArmLwr2->Rotation = -(ArmUpr->Rotation - 1.75f + LegUpperDirection/2.0f);
			Weap->Rotation = ArmLwr2->Rotation;
		}
		
		ArmUpr2->Position.x = Bdy->Position.x;
		ArmUpr2->Position.y = Bdy->Position.y + (Bdy->Width/5.25f);
		ArmLwr2->Position.x = ArmUpr2->Position.x - (float)cos(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);
		ArmLwr2->Position.y = ArmUpr2->Position.y - (float)sin(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);

		if ((Object->Speed) < 0.6f * GetDeltaTime())
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
		Weap->ZIndex = Object->Zindex - 1;
	}
	else
	{
		Tail->Position.x = Bdy->Position.x-(Bdy->Width/20);
		
		LegUpr->Rotation = -LegUpperDirection;
		LegUpr->Position.x = Object->Position.x;
		if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
		{
			LegUpr2->Position.x += sinOfLegValue*-8/(LegDistance);
		}
		LegUpr->Position.y = Object->Position.y + (sinOfTwoLegValue*5/(LegDistance));
		LegLwr->Position.x = (float)cos(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.x;
		LegLwr->Position.y = (float)sin(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.y;
		LegLwr->Rotation = -LegLowerDirection;
		
		LegUpr2->Rotation = LegUpperDirection2;
		LegUpr2->Position.x = Object->Position.x;
		if (Object->PlayerRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
		{
			LegUpr2->Position.x += sinOfLegValue*8/(LegDistance);
		}
		LegUpr2->Position.y = Object->Position.y + (sinOfTwoLegValue*5/(LegDistance));
		LegLwr2->Position.x = (float)cos(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.x;
		LegLwr2->Position.y = (float)sin(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.y;
		LegLwr2->Rotation = LegLowerDirection2;
		
		// Attacking!
		if (Object->isAttacking)
		{
			if (Object->PlayerWeapon->WeaponType != Spear || (Object->PlayerWeapon->WeaponType == Spear && Object->AttackType == 1))
			{
				Object->PlayerSpriteParts.AttackRotation = RotateToAngle(Object->PlayerSpriteParts.AttackRotation, FOX_PI/6, Object->CurrentPlayerStats.AttackSpeed * GetDeltaTime());
				Object->PlayerSpriteParts.AttackRotationArm = RotateToAngle(Object->PlayerSpriteParts.AttackRotationArm, FOX_PI, Object->CurrentPlayerStats.AttackSpeed * GetDeltaTime());
				Object->PlayerSpriteParts.AttackRotationArmLower = RotateToAngle(Object->PlayerSpriteParts.AttackRotationArmLower, FOX_PI/2, Object->CurrentPlayerStats.AttackSpeed * GetDeltaTime());
				ArmUpr->Rotation = FOX_PI / 2 - 0.5f + Object->PlayerSpriteParts.AttackRotationArm;
				ArmLwr->Rotation = ArmUpr->Rotation + FOX_PI/2 - Object->PlayerSpriteParts.AttackRotationArmLower;
				Weap->Rotation = ArmLwr->Rotation;
				if (Object->PlayerSpriteParts.AttackRotationArm == FOX_PI)
					Object->isAttacking = FALSE;
			}
			else
			{
				Object->PlayerSpriteParts.AttackRotation = RotateToAngle(Object->PlayerSpriteParts.AttackRotation, FOX_PI / 2.1f, Object->CurrentPlayerStats.AttackSpeed * GetDeltaTime());
				Object->PlayerSpriteParts.AttackRotationArm = RotateToAngle(Object->PlayerSpriteParts.AttackRotationArm, FOX_PI * 0.9f, Object->CurrentPlayerStats.AttackSpeed * GetDeltaTime());
				Object->PlayerSpriteParts.AttackRotationArmLower = RotateToAngle(Object->PlayerSpriteParts.AttackRotationArmLower, FOX_PI/2, Object->CurrentPlayerStats.AttackSpeed * GetDeltaTime());
				ArmUpr->Rotation = FOX_PI * 1.25f + Object->PlayerSpriteParts.AttackRotationArm;
				ArmLwr->Rotation = ArmUpr->Rotation + Object->PlayerSpriteParts.AttackRotationArmLower / 2;
				Weap->Rotation = ArmLwr->Rotation - Object->PlayerSpriteParts.AttackRotation;
				if (Object->PlayerSpriteParts.AttackRotationArm == FOX_PI * 0.9f)
					Object->isAttacking = FALSE;
			}
		}
		else
		{
			ArmUpr->Rotation = -LegUpperDirection/1.5f - 1.5f;
			ArmLwr->Rotation = ArmUpr->Rotation + 1.25f - LegUpperDirection/2.0f;
			Weap->Rotation = ArmLwr->Rotation;
		}
		
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

		if ((Object->Speed) < 0.6f * GetDeltaTime())
		{
			if (!Object->isAttacking)
			{
				ArmLwr->Rotation = ArmUpr->Rotation + 0.5f;
				ArmLwr2->Rotation = ArmUpr2->Rotation + 0.2f;
				Weap->Rotation = ArmLwr->Rotation;
			}
		}

		Weap->Position.x = ArmLwr->Position.x + (float)cos(ArmLwr->Rotation) * (ArmLwr->Width / 3.5f);
		Weap->Position.y = ArmLwr->Position.y + (float)sin(ArmLwr->Rotation) * (ArmLwr->Width / 3.5f);
		Weap->ZIndex = Object->Zindex + 2;
	}

	Object->PlayerWeapon->WeaponAttackPosition.x = Weap->Position.x + (cosf(Weap->Rotation + FOX_PI / 2) * Object->PlayerWeapon->WeaponLength);
	Object->PlayerWeapon->WeaponAttackPosition.y = Weap->Position.y + (sinf(Weap->Rotation + FOX_PI / 2) * Object->PlayerWeapon->WeaponLength);

	//*************************************************************************************************
}


/*************************************************************************/
/*!
	\brief
	Animates the enemies legs and arms.
*/
/*************************************************************************/
void HandGuyAnimation(HandGuyBoss *Object)
{
	float LegDistance = (2.3f / (((Object->Speed * (1 / 60.0f)) * 0.075f + 0.5f)) );
	float LegUpperDirection = (float)sin(Object->LegSinValue)/(LegDistance);
	float LegLowerDirection;
	float LegUpperDirection2 = (float)sin(Object->LegSinValue)/(LegDistance);
	float LegLowerDirection2;
	

	Sprite *LegUpr = Object->HandGuySpriteParts.LegUpper;
	Sprite *LegUpr2 = Object->HandGuySpriteParts.LegUpper2;
	Sprite *LegLwr = Object->HandGuySpriteParts.LegLower;
	Sprite *LegLwr2 = Object->HandGuySpriteParts.LegLower2;
	Sprite *ArmUpr = Object->HandGuySpriteParts.ArmUpper;
	Sprite *ArmUpr2 = Object->HandGuySpriteParts.ArmUpper2;
	Sprite *ArmLwr = Object->HandGuySpriteParts.ArmLower;
	Sprite *ArmLwr2 = Object->HandGuySpriteParts.ArmLower2;
	Sprite *Bdy = Object->HandGuySpriteParts.Body;
	Sprite *Tail = Object->HandGuySpriteParts.Tail;

	Object->LegSinValue += (Object->Speed * GetDeltaTime()) / 75.0f; 

	Bdy->Position.x = Object->Position.x;
	Bdy->Position.y = Object->Position.y + (Bdy->Height / 10) - ((float)sin(-Object->LegSinValue*2)*5/(LegDistance));

	Tail->Position.y = Bdy->Position.y + (Bdy->Height/30);
	Tail->Rotation = (float)sin(Object->TailSinValue*1.25f)/4;

	if (Object->Speed > 90.0f * GetDeltaTime())
	{
		Tail->SpriteTexture = LoadTexture("TextureFiles/TailDog.png");
		Object->TailSinValue += 6.0f * GetDeltaTime();
		Object->HandGuySpriteParts.Tail->AnimationSpeed = (Object->Speed)/2 + 3;
	}
	else
	{
		Tail->SpriteTexture = LoadTexture("TextureFiles/TailDog.png");
		Object->TailSinValue = 0;
		Object->HandGuySpriteParts.Tail->AnimationSpeed = 4;
	}

	if (Object->HandGuyRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
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
	LegUpr->FlipX = !Object->BodySprite->FlipX;
	LegLwr->FlipX = !Object->BodySprite->FlipX;
	LegUpr2->FlipX = !Object->BodySprite->FlipX;
	LegLwr2->FlipX = !Object->BodySprite->FlipX;
	Bdy->FlipX = !Object->BodySprite->FlipX;
	Tail->FlipX = !Object->BodySprite->FlipX;
	ArmUpr->FlipX = !Object->BodySprite->FlipX;
	ArmLwr->FlipX = !Object->BodySprite->FlipX;
	ArmUpr2->FlipX = !Object->BodySprite->FlipX;
	ArmLwr2->FlipX = !Object->BodySprite->FlipX;

	if (Object->BodySprite->FlipX != FALSE)
	{
		Tail->Position.x = Bdy->Position.x+(Bdy->Width/20);
		
		LegUpr->Rotation = LegUpperDirection;
		LegUpr->Position.x = Object->Position.x;
		if (Object->HandGuyRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*-8/(LegDistance);
		}
		LegUpr->Position.y = Bdy->Position.y;
		LegLwr->Position.x = (float)cos(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.x;
		LegLwr->Position.y = (float)sin(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.y;
		LegLwr->Rotation = LegLowerDirection;
		
		LegUpr2->Rotation = -LegUpperDirection2;
		LegUpr2->Position.x = Object->Position.x;
		if (Object->HandGuyRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*8/(LegDistance);
		}
		LegUpr2->Position.y = Bdy->Position.y;
		LegLwr2->Position.x = (float)cos(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.x;
		LegLwr2->Position.y = (float)sin(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.y;
		LegLwr2->Rotation = -LegLowerDirection2;
		
		
		// Attacking! -----------------------------------------------------------------------------------------
		if (Object->QuestionAttackAnimation)
		{
			Object->HandGuySpriteParts.AttackRotationArm = RotateToAngle(Object->HandGuySpriteParts.AttackRotationArm, FOX_PI * 0.9f, 15.0f * GetDeltaTime());
			Object->HandGuySpriteParts.AttackRotationArmLower = RotateToAngle(Object->HandGuySpriteParts.AttackRotationArmLower, FOX_PI / 2, 15.0f * GetDeltaTime());
			if (fabs(Object->HandGuySpriteParts.AttackRotationArm) > FOX_PI / 1.5f)
			{
				Object->HandGuySpriteParts.AttackRotationArm2 = RotateToAngle(Object->HandGuySpriteParts.AttackRotationArm2, FOX_PI * 0.9f, 15.0f * GetDeltaTime());
				Object->HandGuySpriteParts.AttackRotationArmLower2 = RotateToAngle(Object->HandGuySpriteParts.AttackRotationArmLower2, FOX_PI / 2, 15.0f * GetDeltaTime());
			}
			ArmUpr2->Rotation = -FOX_PI / 12 + Object->HandGuySpriteParts.AttackRotationArm2 / 6 - (Object->numProjectiles * 12 * FOX_PI / 180.0f);
			ArmLwr2->Rotation = ArmUpr2->Rotation - FOX_PI / 2 + Object->HandGuySpriteParts.AttackRotationArmLower2;
			ArmUpr->Rotation = -FOX_PI / 12 + Object->HandGuySpriteParts.AttackRotationArm / 6 - (Object->numProjectiles * 12 * FOX_PI / 180.0f);
			ArmLwr->Rotation = ArmUpr->Rotation - FOX_PI / 2 + Object->HandGuySpriteParts.AttackRotationArmLower;

			if (Object->HandGuySpriteParts.AttackRotationArm2 == FOX_PI * 0.9f)
				Object->QuestionAttackAnimation = FALSE;

		}
		else
		{
			ArmUpr2->Rotation = -LegUpperDirection/1.5f + 1.5f;
			ArmLwr2->Rotation = -(ArmUpr->Rotation - 1.75f + LegUpperDirection/2.0f);
			ArmUpr->Rotation = LegUpperDirection/1.5f + 1.5f;
			ArmLwr->Rotation = ArmUpr->Rotation - 1.25f + LegUpperDirection/2.0f;

			if (Object->JabSprite->Visible)
			{
				ArmUpr2->Rotation = -FOX_PI / 12;
				ArmUpr->Rotation = -FOX_PI / 12;
			}
		}
		// -----------------------------------------------------------------------------------------

		ArmUpr2->Position.x = Bdy->Position.x;
		ArmUpr2->Position.y = Bdy->Position.y + (Bdy->Width/5.25f);
		ArmLwr2->Position.x = ArmUpr2->Position.x - (float)cos(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);
		ArmLwr2->Position.y = ArmUpr2->Position.y - (float)sin(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);
		
		ArmUpr->Position.x = Bdy->Position.x;
		ArmUpr->Position.y = Bdy->Position.y + (Bdy->Width/5.25f);
		ArmLwr->Position.x = ArmUpr->Position.x - (float)cos(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);
		ArmLwr->Position.y = ArmUpr->Position.y - (float)sin(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);
		
		if ((Object->Speed * GetDeltaTime()) < 0.6f * GetDeltaTime())
		{
			if (!(Object->CurrentState == Question && Object->InnerState == Attack && Object->cooldownTimer >= 0.5f))
			{
				ArmLwr->Rotation = ArmUpr->Rotation - 0.2f;
				ArmLwr2->Rotation = ArmUpr2->Rotation - 0.5f;
			}
		}
		
	}
	else
	{
		Tail->Position.x = Bdy->Position.x-(Bdy->Width/20);
		
		LegUpr->Rotation = -LegUpperDirection;
		LegUpr->Position.x = Object->Position.x;
		if (Object->HandGuyRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*-8/(LegDistance);
		}
		LegUpr->Position.y = Bdy->Position.y;
		LegLwr->Position.x = (float)cos(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.x;
		LegLwr->Position.y = (float)sin(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.y;
		LegLwr->Rotation = -LegLowerDirection;
		
		LegUpr2->Rotation = LegUpperDirection2;
		LegUpr2->Position.x = Object->Position.x;
		if (Object->HandGuyRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*8/(LegDistance);
		}
		LegUpr2->Position.y = Bdy->Position.y;
		LegLwr2->Position.x = (float)cos(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.x;
		LegLwr2->Position.y = (float)sin(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.y;
		LegLwr2->Rotation = LegLowerDirection2;
		
		
		// Attacking! -----------------------------------------------------------------------------------------
		if (Object->QuestionAttackAnimation)
		{
			Object->HandGuySpriteParts.AttackRotationArm = RotateToAngle(Object->HandGuySpriteParts.AttackRotationArm, FOX_PI * 0.9f, 15.0f * GetDeltaTime());
			Object->HandGuySpriteParts.AttackRotationArmLower = RotateToAngle(Object->HandGuySpriteParts.AttackRotationArmLower, FOX_PI / 2, 15.0f * GetDeltaTime());
			if (fabs(Object->HandGuySpriteParts.AttackRotationArm) > FOX_PI / 1.5f)
			{
				Object->HandGuySpriteParts.AttackRotationArm2 = RotateToAngle(Object->HandGuySpriteParts.AttackRotationArm2, FOX_PI * 0.9f, 15.0f * GetDeltaTime());
				Object->HandGuySpriteParts.AttackRotationArmLower2 = RotateToAngle(Object->HandGuySpriteParts.AttackRotationArmLower2, FOX_PI / 2, 15.0f * GetDeltaTime());
			}

			ArmUpr2->Rotation = FOX_PI / 12 - Object->HandGuySpriteParts.AttackRotationArm2 / 6 + (Object->numProjectiles * 12 * FOX_PI / 180.0f);
			ArmLwr2->Rotation = ArmUpr2->Rotation + FOX_PI / 2 - Object->HandGuySpriteParts.AttackRotationArmLower2;
			ArmUpr->Rotation = FOX_PI / 12 - Object->HandGuySpriteParts.AttackRotationArm / 6 + (Object->numProjectiles * 12 * FOX_PI / 180.0f);
			ArmLwr->Rotation = ArmUpr->Rotation + FOX_PI / 2 - Object->HandGuySpriteParts.AttackRotationArmLower;

			if (Object->HandGuySpriteParts.AttackRotationArm2 == FOX_PI * 0.9f)
				Object->QuestionAttackAnimation = FALSE;
		}
		else
		{
			ArmUpr->Rotation = -LegUpperDirection/1.5f - 1.5f;
			ArmLwr->Rotation = ArmUpr->Rotation + 1.25f - LegUpperDirection/2.0f;
			ArmUpr2->Rotation = LegUpperDirection/1.5f - 1.5f;
			ArmLwr2->Rotation = ArmUpr2->Rotation + 1.25f + LegUpperDirection/2.0f;

			if (Object->JabSprite->Visible)
			{
				ArmUpr2->Rotation = FOX_PI / 12;
				ArmUpr->Rotation = FOX_PI / 12;
			}
		}
		// -----------------------------------------------------------------------------------------

		ArmUpr->Position.x = Bdy->Position.x;
		ArmUpr->Position.y = Bdy->Position.y + (Bdy->Width/5.25f);
		ArmLwr->Position.x = ArmUpr->Position.x + (float)cos(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);
		ArmLwr->Position.y = ArmUpr->Position.y + (float)sin(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);

		
		ArmUpr2->Position.x = Bdy->Position.x;
		ArmUpr2->Position.y = Bdy->Position.y + (Bdy->Width/5.25f);
		ArmLwr2->Position.x = ArmUpr2->Position.x + (float)cos(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);
		ArmLwr2->Position.y = ArmUpr2->Position.y + (float)sin(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);

		
		if ((Object->Speed * GetDeltaTime()) < 0.6f * GetDeltaTime())
		{
			if (!(Object->CurrentState == Question && Object->InnerState == Attack && Object->cooldownTimer >= 0.5f))
			{
				ArmLwr->Rotation = ArmUpr->Rotation + 0.5f;
				ArmLwr2->Rotation = ArmUpr2->Rotation + 0.2f;
			}
		}

	}

	//*************************************************************************************************
}


/*************************************************************************/
/*!
	\brief
	Animates the enemies legs and arms.
*/
/*************************************************************************/
void YeahGuyAnimation(YeahGuyBoss *Object)
{
	float LegDistance = (2.3f / (((Object->Speed * (1 / 60.0f)) * 0.075f + 0.5f)) );
	float LegUpperDirection = (float)sin(Object->LegSinValue)/(LegDistance);
	float LegLowerDirection;
	float LegUpperDirection2 = (float)sin(Object->LegSinValue)/(LegDistance);
	float LegLowerDirection2;
	

	Sprite *LegUpr = Object->YeahGuySpriteParts.LegUpper;
	Sprite *LegUpr2 = Object->YeahGuySpriteParts.LegUpper2;
	Sprite *LegLwr = Object->YeahGuySpriteParts.LegLower;
	Sprite *LegLwr2 = Object->YeahGuySpriteParts.LegLower2;
	Sprite *Bdy = Object->YeahGuySpriteParts.Body;
	Sprite *Tail = Object->YeahGuySpriteParts.Tail;
	Sprite *Red = Object->YeahGuySpriteParts.HeadRed;
	Sprite *Blue = Object->YeahGuySpriteParts.HeadBlue;
	Sprite *Green = Object->YeahGuySpriteParts.HeadGreen;

	Object->LegSinValue += (Object->Speed * GetDeltaTime()) / 75.0f; 

	Bdy->Position.x = Object->Position.x;
	Bdy->Position.y = Object->Position.y - 75.0f + (Bdy->Height / 10) - ((float)sin(-Object->LegSinValue*2)*5/(LegDistance));

	Red->Position = Bdy->Position;
	Green->Position = Bdy->Position;
	Blue->Position = Bdy->Position;

	Red->Visible = Object->redHead;
	Green->Visible = Object->greenHead;
	Blue->Visible = Object->blueHead;

	Tail->Position.y = Bdy->Position.y + (Bdy->Height/30);
	Tail->Rotation = (float)sin(Object->TailSinValue*1.25f)/4;

	if (Object->Speed > 90.0f * GetDeltaTime())
	{
		Tail->SpriteTexture = LoadTexture("TextureFiles/TailDog.png");
		Object->TailSinValue += 6.0f * GetDeltaTime();
		Tail->AnimationSpeed = (Object->Speed)/2 + 3;
	}
	else
	{
		Tail->SpriteTexture = LoadTexture("TextureFiles/TailDog.png");
		Object->TailSinValue = 0;
		Tail->AnimationSpeed = 4;
	}

	if (Object->YeahGuyRigidBody.onGround || Object->Position.y - Object->BodySprite->Height / 4 <= GROUNDLEVEL)
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
	LegUpr->FlipX = !Object->BodySprite->FlipX;
	LegLwr->FlipX = !Object->BodySprite->FlipX;
	LegUpr2->FlipX = !Object->BodySprite->FlipX;
	LegLwr2->FlipX = !Object->BodySprite->FlipX;
	Bdy->FlipX = !Object->BodySprite->FlipX;
	Tail->FlipX = !Object->BodySprite->FlipX;
	Red->FlipX = !Object->BodySprite->FlipX;
	Green->FlipX = !Object->BodySprite->FlipX;
	Blue->FlipX = !Object->BodySprite->FlipX;

	if (Object->BodySprite->FlipX != FALSE)
	{
		Tail->Position.x = Bdy->Position.x+(Bdy->Width/20);
		
		LegUpr->Rotation = LegUpperDirection;
		LegUpr->Position.x = Object->Position.x;
		if (Object->YeahGuyRigidBody.onGround || Object->Position.y - Object->BodySprite->Height / 4 <= GROUNDLEVEL)
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*-8/(LegDistance);
		}
		LegUpr->Position.y = Bdy->Position.y;
		LegLwr->Position.x = (float)cos(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.x;
		LegLwr->Position.y = (float)sin(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.y;
		LegLwr->Rotation = LegLowerDirection;
		
		LegUpr2->Rotation = -LegUpperDirection2;
		LegUpr2->Position.x = Object->Position.x;
		if (Object->YeahGuyRigidBody.onGround || Object->Position.y - Object->BodySprite->Height / 4 <= GROUNDLEVEL)
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*8/(LegDistance);
		}
		LegUpr2->Position.y = Bdy->Position.y;
		LegLwr2->Position.x = (float)cos(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.x;
		LegLwr2->Position.y = (float)sin(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.y;
		LegLwr2->Rotation = -LegLowerDirection2;
		
		
		// Attacking! -----------------------------------------------------------------------------------------
		if (Object->CurrentState == ProjectYeah && (Object->InnerState == Attack || Object->InnerState == Start))
		{
			if (Object->HeadAttacking == RedHead)
			{
				Object->YeahGuySpriteParts.HeadRed->CurrentFrame = 1;
				Object->YeahGuySpriteParts.HeadRed->Position.x -= (1 - (Object->cooldownTimer > 0 ? Object->cooldownTimer : 0)) * 15;
			}
			else
				Object->YeahGuySpriteParts.HeadRed->CurrentFrame = 0;

			if (Object->HeadAttacking == GreenHead)
			{
				Object->YeahGuySpriteParts.HeadGreen->CurrentFrame = 1;
				Object->YeahGuySpriteParts.HeadGreen->Position.x -= (1 - (Object->cooldownTimer > 0 ? Object->cooldownTimer : 0)) * 15;
			}
			else
				Object->YeahGuySpriteParts.HeadGreen->CurrentFrame = 0;

			if (Object->HeadAttacking == BlueHead)
			{
				Object->YeahGuySpriteParts.HeadBlue->CurrentFrame = 1;
				Object->YeahGuySpriteParts.HeadBlue->Position.x -= (1 - (Object->cooldownTimer > 0 ? Object->cooldownTimer : 0)) * 15;
			}
			else
				Object->YeahGuySpriteParts.HeadBlue->CurrentFrame = 0;
		}
		else if (Object->CurrentState == AOE)
		{
			Object->YeahGuySpriteParts.HeadRed->CurrentFrame = 1;
			Object->YeahGuySpriteParts.HeadGreen->CurrentFrame = 1;
			Object->YeahGuySpriteParts.HeadBlue->CurrentFrame = 1;
		}
		else
		{
			Object->YeahGuySpriteParts.HeadRed->CurrentFrame = 0;
			Object->YeahGuySpriteParts.HeadGreen->CurrentFrame = 0;
			Object->YeahGuySpriteParts.HeadBlue->CurrentFrame = 0;
		}
		// -----------------------------------------------------------------------------------------

	}
	else
	{
		Tail->Position.x = Bdy->Position.x-(Bdy->Width/20);
		
		LegUpr->Rotation = -LegUpperDirection;
		LegUpr->Position.x = Object->Position.x;
		if (Object->YeahGuyRigidBody.onGround || Object->Position.y - Object->BodySprite->Height / 4 <= GROUNDLEVEL)
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*-8/(LegDistance);
		}
		LegUpr->Position.y = Bdy->Position.y;
		LegLwr->Position.x = (float)cos(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.x;
		LegLwr->Position.y = (float)sin(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.y;
		LegLwr->Rotation = -LegLowerDirection;
		
		LegUpr2->Rotation = LegUpperDirection2;
		LegUpr2->Position.x = Object->Position.x;
		if (Object->YeahGuyRigidBody.onGround || Object->Position.y - Object->BodySprite->Height / 4 <= GROUNDLEVEL)
		{
			LegUpr2->Position.x += (float)sin(Object->LegSinValue)*8/(LegDistance);
		}
		LegUpr2->Position.y = Bdy->Position.y;
		LegLwr2->Position.x = (float)cos(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.x;
		LegLwr2->Position.y = (float)sin(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.y;
		LegLwr2->Rotation = LegLowerDirection2;
		
		
		// Attacking! -----------------------------------------------------------------------------------------
		if (Object->CurrentState == ProjectYeah && (Object->InnerState == Attack || Object->InnerState == Start))
		{
			if (Object->HeadAttacking == RedHead)
			{
				Object->YeahGuySpriteParts.HeadRed->CurrentFrame = 1;
				Object->YeahGuySpriteParts.HeadRed->Position.x += (1 - (Object->cooldownTimer > 0 ? Object->cooldownTimer : 0)) * 15;
			}
			else
				Object->YeahGuySpriteParts.HeadRed->CurrentFrame = 0;

			if (Object->HeadAttacking == GreenHead)
			{
				Object->YeahGuySpriteParts.HeadGreen->CurrentFrame = 1;
				Object->YeahGuySpriteParts.HeadGreen->Position.x += (1 - (Object->cooldownTimer > 0 ? Object->cooldownTimer : 0)) * 15;
			}
			else
				Object->YeahGuySpriteParts.HeadGreen->CurrentFrame = 0;

			if (Object->HeadAttacking == BlueHead)
			{
				Object->YeahGuySpriteParts.HeadBlue->CurrentFrame = 1;
				Object->YeahGuySpriteParts.HeadBlue->Position.x += (1 - (Object->cooldownTimer > 0 ? Object->cooldownTimer : 0)) * 15;
			}
			else
				Object->YeahGuySpriteParts.HeadBlue->CurrentFrame = 0;
		}
		else if (Object->CurrentState == AOE)
		{
			Object->YeahGuySpriteParts.HeadRed->CurrentFrame = 1;
			Object->YeahGuySpriteParts.HeadGreen->CurrentFrame = 1;
			Object->YeahGuySpriteParts.HeadBlue->CurrentFrame = 1;
		}
		else
		{
			Object->YeahGuySpriteParts.HeadRed->CurrentFrame = 0;
			Object->YeahGuySpriteParts.HeadGreen->CurrentFrame = 0;
			Object->YeahGuySpriteParts.HeadBlue->CurrentFrame = 0;
		}
		// -----------------------------------------------------------------------------------------

	}

	//*************************************************************************************************
}


/*************************************************************************/
/*!
	\brief
	Animates the enemies legs and arms.
*/
/*************************************************************************/
void KevinAnimation(KevinBoss *Object)
{
	float LegDistance = (2.3f / (((Object->Speed * (1 / 60.0f)) * 0.075f + 0.5f)) );
	float LegUpperDirection = (float)sin(Object->LegSinValue)/(LegDistance);
	float LegLowerDirection;
	float LegUpperDirection2 = (float)sin(Object->LegSinValue)/(LegDistance);
	float LegLowerDirection2;
	

	Sprite *LegUpr = Object->KevinSpriteParts.LegUpper;
	Sprite *LegUpr2 = Object->KevinSpriteParts.LegUpper2;
	Sprite *LegLwr = Object->KevinSpriteParts.LegLower;
	Sprite *LegLwr2 = Object->KevinSpriteParts.LegLower2;
	Sprite *ArmUpr = Object->KevinSpriteParts.ArmUpper;
	Sprite *ArmUpr2 = Object->KevinSpriteParts.ArmUpper2;
	Sprite *ArmLwr = Object->KevinSpriteParts.ArmLower;
	Sprite *ArmLwr2 = Object->KevinSpriteParts.ArmLower2;
	Sprite *Bdy = Object->KevinSpriteParts.Body;

	Object->LegSinValue += (Object->Speed * GetDeltaTime()) / 75.0f; 

	Bdy->Position.x = Object->Position.x;
	Bdy->Position.y = Object->Position.y + (Bdy->Height / 10) - ((float)sin(-Object->LegSinValue*2)*5/(LegDistance));


	if (Object->KevinRigidBody.onGround || Object->Position.y <= GROUNDLEVEL)
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
	LegUpr->FlipX = Object->BodySprite->FlipX;
	LegLwr->FlipX = Object->BodySprite->FlipX;
	LegUpr2->FlipX = Object->BodySprite->FlipX;
	LegLwr2->FlipX = Object->BodySprite->FlipX;
	Bdy->FlipX = Object->BodySprite->FlipX;
	ArmUpr->FlipX = Object->BodySprite->FlipX;
	ArmLwr->FlipX = Object->BodySprite->FlipX;
	ArmUpr2->FlipX = Object->BodySprite->FlipX;
	ArmLwr2->FlipX = Object->BodySprite->FlipX;

	if (Object->BodySprite->FlipX == FALSE)
	{
		
		LegUpr->Rotation = LegUpperDirection;
		LegUpr->Position.x = Object->Position.x;
		
		LegUpr->Position.y = Bdy->Position.y;
		LegLwr->Position.x = (float)cos(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.x;
		LegLwr->Position.y = (float)sin(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.y;
		LegLwr->Rotation = LegLowerDirection;
		
		LegUpr2->Rotation = -LegUpperDirection2;
		LegUpr2->Position.x = Object->Position.x;
		
		LegUpr2->Position.y = Bdy->Position.y;
		LegLwr2->Position.x = (float)cos(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.x;
		LegLwr2->Position.y = (float)sin(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.y;
		LegLwr2->Rotation = -LegLowerDirection2;
		
		
		// Attacking! -----------------------------------------------------------------------------------------
		ArmUpr2->Rotation = -LegUpperDirection/1.5f + 1.5f;
		ArmLwr2->Rotation = -(ArmUpr->Rotation - 1.75f + LegUpperDirection/2.0f);
		ArmUpr->Rotation = LegUpperDirection/1.5f + 1.5f;
		ArmLwr->Rotation = ArmUpr->Rotation - 1.25f + LegUpperDirection/2.0f;

		if (Object->JabSprite->Visible)
		{
			ArmUpr2->Rotation = -FOX_PI / 12;
			ArmUpr->Rotation = -FOX_PI / 12;
		}
		// -----------------------------------------------------------------------------------------

		ArmUpr2->Position.x = Bdy->Position.x;
		ArmUpr2->Position.y = Bdy->Position.y + (Bdy->Width/5.25f);
		ArmLwr2->Position.x = ArmUpr2->Position.x - (float)cos(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);
		ArmLwr2->Position.y = ArmUpr2->Position.y - (float)sin(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);
		
		ArmUpr->Position.x = Bdy->Position.x;
		ArmUpr->Position.y = Bdy->Position.y + (Bdy->Width/5.25f);
		ArmLwr->Position.x = ArmUpr->Position.x - (float)cos(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);
		ArmLwr->Position.y = ArmUpr->Position.y - (float)sin(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);
		
	}
	else
	{
		
		LegUpr->Rotation = -LegUpperDirection;
		LegUpr->Position.x = Object->Position.x;
		
		LegUpr->Position.y = Bdy->Position.y;
		LegLwr->Position.x = (float)cos(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.x;
		LegLwr->Position.y = (float)sin(LegUpr->Rotation-(FOX_PI/2)) * (LegLwr->Width/4.2f) + LegUpr->Position.y;
		LegLwr->Rotation = -LegLowerDirection;
		
		LegUpr2->Rotation = LegUpperDirection2;
		LegUpr2->Position.x = Object->Position.x;
		
		LegUpr2->Position.y = Bdy->Position.y;
		LegLwr2->Position.x = (float)cos(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.x;
		LegLwr2->Position.y = (float)sin(LegUpr2->Rotation-(FOX_PI/2)) * (LegLwr2->Width/4.2f) + LegUpr2->Position.y;
		LegLwr2->Rotation = LegLowerDirection2;
		
		
		// Attacking! -----------------------------------------------------------------------------------------
		ArmUpr->Rotation = -LegUpperDirection/1.5f - 1.5f;
		ArmLwr->Rotation = ArmUpr->Rotation + 1.25f - LegUpperDirection/2.0f;
		ArmUpr2->Rotation = LegUpperDirection/1.5f - 1.5f;
		ArmLwr2->Rotation = ArmUpr2->Rotation + 1.25f + LegUpperDirection/2.0f;

		if (Object->JabSprite->Visible)
		{
			ArmUpr2->Rotation = FOX_PI / 12;
			ArmUpr->Rotation = FOX_PI / 12;
		}
		// -----------------------------------------------------------------------------------------

		ArmUpr->Position.x = Bdy->Position.x;
		ArmUpr->Position.y = Bdy->Position.y + (Bdy->Width/5.25f);
		ArmLwr->Position.x = ArmUpr->Position.x + (float)cos(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);
		ArmLwr->Position.y = ArmUpr->Position.y + (float)sin(ArmUpr->Rotation) * (ArmLwr->Width/3.2f);

		
		ArmUpr2->Position.x = Bdy->Position.x;
		ArmUpr2->Position.y = Bdy->Position.y + (Bdy->Width/5.25f);
		ArmLwr2->Position.x = ArmUpr2->Position.x + (float)cos(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);
		ArmLwr2->Position.y = ArmUpr2->Position.y + (float)sin(ArmUpr2->Rotation) * (ArmLwr2->Width/3.2f);

	}

	//*************************************************************************************************
}

/*************************************************************************/
/*!
	\brief
	Rotates something to a certain angle

	\param angle
	Current angle

	\param angleTo
	Angle to rotate to

	\param speed
	Speed of the rotation

	\return
	The new resulting angle
*/
/*************************************************************************/
float RotateToAngle(float angle, float angleTo, float speed)
{
	int diff;
	
	diff = (((( (int)(angleTo / FOX_PI * 180) - (int)(angle / FOX_PI * 180) ) % 360) + 540) % 360) - 180;
	
	if (diff > 0)
	{
		if ((diff * FOX_PI / 180) < speed)
			return angleTo;
	}
	else
	{
		if (-(diff * FOX_PI / 180) < speed)
			return angleTo;
	}
	if (abs(diff) > 0)
		diff = diff / abs(diff);
	else
		return angleTo;
	return angle + diff * speed;	
}