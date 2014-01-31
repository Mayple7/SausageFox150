/*****************************************************************************/
/*!
\file				Player.c
\author				Dan Muller (d.muller)
\date				Jan 8, 2014

\brief				Player movement and logic functions

\par				Functions:
\li					InitializePlayer
\li					InputPlayer
\li					HandleCollision
\li					UpdatePlayerPosition
\li					updateMaxHealth
\li					updateMoveSpeed
\li					updateAttackSpeed
\li					updateDamageReduction
\li					DetectPlayerCollision
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxEngine.h"

// ---------------------------------------------------------------------------
// globals
Player CurrentPlayer;

/*************************************************************************/
/*!
	\brief
	Initializes the player
	
	\param CurrentPlayer
	A pointer to the player to be initialized
*/
/*************************************************************************/
void InitializePlayer(struct Player *CurrentPlayer, int newID)
{
	int i;

	for(i = 0; i < COLLIDEAMOUNT; i++)
	{
		CurrentPlayer->CollisionData[i] = -1;
	}

	//Creates the sprite for the player
	CurrentPlayer->PlayerSprite = CreateSprite("TextureFiles/SausageFox.png", 250.0f, 150.0f, 10, 4, 2);

	//Default position of the player
	CurrentPlayer->Position.x = 0.0f;
	CurrentPlayer->Position.y = 0.0f;

	CurrentPlayer->PlayerSprite->Visible = FALSE;

	//Animation properties
	CurrentPlayer->PlayerSprite->AnimationSpeed = 4; // STOP CHANGING HIS LEG SPEED -The Supreme Sausage

	//Collision properties
	CreateCollisionBox(&CurrentPlayer->PlayerCollider, &CurrentPlayer->Position, PlayerType, 2 * CurrentPlayer->PlayerSprite->Width / 3, CurrentPlayer->PlayerSprite->Height / 2, newID);
	CurrentPlayer->PlayerCollider.Offset.y = -20.0f;

	//Initialize rigidbody
	InitializeRigidBody(&CurrentPlayer->PlayerRigidBody, FALSE, CurrentPlayer->PlayerSprite->Width, CurrentPlayer->PlayerSprite->Height);
	CurrentPlayer->PlayerRigidBody.onGround = FALSE;
	CurrentPlayer->dropDown = FALSE;
}

/*************************************************************************/
/*!
	\brief
	Updates the player based on the input
	
	\param CurrentPlayer
	A pointer to the player to update
*/
/*************************************************************************/
void InputPlayer(struct Player *CurrentPlayer)
{
	// Move the direction based on the speed
	MoveObject(&CurrentPlayer->Position, CurrentPlayer->PlayerDirection, CurrentPlayer->Speed);

	// not key press for direction then slow down!
	if(!AEInputCheckCurr('D') && !AEInputCheckCurr('A'))
	{
		if (!(CurrentPlayer->Position.y > -225) && !CurrentPlayer->PlayerRigidBody.onGround)
		{
			if (CurrentPlayer->Speed - 0.8f >= 0.0f)
			{
				CurrentPlayer->Speed -= 0.8f;
			}
			else
			{
				CurrentPlayer->Speed = 0.0f;
				CurrentPlayer->LegSinValue = 0;
			}
		}
		else
		{
			if (CurrentPlayer->Speed - 0.8f >= 0.0f)
			{
				CurrentPlayer->Speed -= 0.8f;
			}
			else
			{
				CurrentPlayer->Speed = 0.0f;
				CurrentPlayer->LegSinValue = 0;
			}
		}
	}
	// Move left if A is pressed
	if(AEInputCheckCurr('A'))
	{
		if(CurrentPlayer->Position.y > -225 && !CurrentPlayer->PlayerRigidBody.onGround)
		{
			if(CurrentPlayer->PlayerSprite->CurrentFrame == 2)
				CurrentPlayer->PlayerSprite->AnimationActive = 0;
			else
				CurrentPlayer->PlayerSprite->AnimationActive = 1;
		}
		else
			CurrentPlayer->PlayerSprite->AnimationActive = 1;
		CurrentPlayer->PlayerSprite->FlipX = 0;
		CurrentPlayer->PlayerDirection = LEFT;
		CurrentPlayer->Speed = 8.0f;
	}
	// Move right if D is pressed
	else if(AEInputCheckCurr('D'))
	{
		if(CurrentPlayer->Position.y > -225 && !CurrentPlayer->PlayerRigidBody.onGround)
		{
			if(CurrentPlayer->PlayerSprite->CurrentFrame == 2)
				CurrentPlayer->PlayerSprite->AnimationActive = 0;
			else
				CurrentPlayer->PlayerSprite->AnimationActive = 1;
		}
		else
			CurrentPlayer->PlayerSprite->AnimationActive = 1;
		CurrentPlayer->PlayerSprite->FlipX = 1;
		CurrentPlayer->PlayerDirection = RIGHT;
		CurrentPlayer->Speed = 8.0f;
	}
	//Stop moving when A and D are not pushed, stop animation correctly as well
	else
	{
		if(CurrentPlayer->Position.y > -225 && !CurrentPlayer->PlayerRigidBody.onGround)
		{
			if(CurrentPlayer->PlayerSprite->CurrentFrame == 2)
				CurrentPlayer->PlayerSprite->AnimationActive = 0;
			else
				CurrentPlayer->PlayerSprite->AnimationActive = 1;
		}
		else
		{
			if(CurrentPlayer->PlayerSprite->CurrentFrame == 0)
				CurrentPlayer->PlayerSprite->AnimationActive = 0;
			else if(CurrentPlayer->PlayerSprite->CurrentFrame == 4)
				CurrentPlayer->PlayerSprite->CurrentFrame = 0;
			else
				CurrentPlayer->PlayerSprite->AnimationActive = 1;
		}
	}
	//Jump when space is pushed or drop down if S is pushed as well
	if(AEInputCheckTriggered(VK_SPACE))
	{
		Vec2 velocity;
		
		if(AEInputCheckCurr('S') && CurrentPlayer->PlayerRigidBody.onGround)
		{
			CurrentPlayer->PlayerRigidBody.onGround = FALSE;
			CurrentPlayer->dropDown = TRUE;
		}

		
		Vec2Set(&velocity, 0.0f, 12.0f);
		if(CurrentPlayer->Position.y < -225 || CurrentPlayer->PlayerRigidBody.onGround)
		{
			if(CurrentPlayer->Position.y < -225)
				Vec2Set(&CurrentPlayer->Position, CurrentPlayer->Position.x, -224.9f);
			CurrentPlayer->PlayerRigidBody.onGround = FALSE;
			ApplyVelocity(&CurrentPlayer->PlayerRigidBody, &velocity);
		}
	}
	//Throw an upward force on the fox when backspace is pushed
	if(AEInputCheckCurr(VK_BACK))
	{
		Vec2 force;
		RECT rect;
		if(GetClientRect(AESysGetWindowHandle(), &rect))
		{
		  int width = rect.right - rect.left;
		  int height = rect.bottom - rect.top;
		  printf("%d, %d\n", width, height);
		}
		CurrentPlayer->PlayerRigidBody.Acceleration.x = 0;
		CurrentPlayer->PlayerRigidBody.Acceleration.y = 0;
		Vec2Set(&force, 0.0f, 15.0f);
		if(CurrentPlayer->Position.y > -225)
		{
			ApplyForce(&CurrentPlayer->PlayerRigidBody, &force);
		}
	}
	//No acceleration when backspace is not being pushed
	else
	{
		CurrentPlayer->PlayerRigidBody.Acceleration.x = 0;
		CurrentPlayer->PlayerRigidBody.Acceleration.y = 0;
	}
}

/*************************************************************************/
/*!
	\brief
	Updates the player's position
	
	\param CurrentPlayer
	A pointer to the player to update
*/
/*************************************************************************/
void UpdatePlayerPosition(Player *CurrentPlayer)
{
	//Stop velocity and acceleration when the player lands on the floor
	if(CurrentPlayer->Position.y <= -225 || CurrentPlayer->PlayerRigidBody.onGround)
	{
		Vec2Zero(&CurrentPlayer->PlayerRigidBody.Acceleration);
		Vec2Zero(&CurrentPlayer->PlayerRigidBody.Velocity);
		ZeroGravity(&CurrentPlayer->PlayerRigidBody);
	}
	//Set gravity if not on floor or on a platform
	else
	{
		SetGravity(&CurrentPlayer->PlayerRigidBody, 0.0f, -15.0f);
	}
	//Player position updated when dropping down from a platform
	if(CurrentPlayer->dropDown)
	{
		CurrentPlayer->Position.y -= 5.0f;
		if(CurrentPlayer->PlayerRigidBody.Velocity.y < 0)
		{
			CurrentPlayer->PlayerRigidBody.Velocity.y = -5.0f;
			CurrentPlayer->dropDown = FALSE;
		}
	}

	//Update velocity and acceleration
	UpdateVelocity(&CurrentPlayer->PlayerRigidBody);
	Vec2Add(&CurrentPlayer->Position, &CurrentPlayer->Position, &CurrentPlayer->PlayerRigidBody.Velocity);

	//Updates the sprite
	CurrentPlayer->PlayerSprite->Position.x = CurrentPlayer->Position.x;
	CurrentPlayer->PlayerSprite->Position.y = CurrentPlayer->Position.y;

	//Updates the collision box
	UpdateCollisionPosition(&CurrentPlayer->PlayerCollider, &CurrentPlayer->Position);
	CurrentPlayer->PlayerRigidBody.onGround = FALSE;
}

/*************************************************************************/
/*!
	\brief
	Update the player's max HP based on their strength
	
	\param CurrentPlayerStats
	A pointer to the player's stats to update
*/
/*************************************************************************/
void updateMaxHealth(PlayerStats *CurrentPlayerStats)
{
	//Placeholder max health formula
	CurrentPlayerStats->MaxHealth = 5 + CurrentPlayerStats->Strength * 2;
}

/*************************************************************************/
/*!
	\brief
	Update the player's movespeed based on their agility
	
	\param CurrentPlayerStats
	A pointer to the player's stats to update
*/
/*************************************************************************/
void updateMoveSpeed(PlayerStats *CurrentPlayerStats)
{
	//Placeholder move speed formula
	CurrentPlayerStats->MoveSpeed = CurrentPlayerStats->Agility + 3.0f;
}

/*************************************************************************/
/*!
	\brief
	Update the player's attack speed based on their agility
	
	\param CurrentPlayerStats
	A pointer to the player's stats to update
*/
/*************************************************************************/
void updateAttackSpeed(PlayerStats *CurrentPlayerStats)
{
	//Placeholder attack speed formula
	CurrentPlayerStats->AttackSpeed = CurrentPlayerStats->Agility * 5.0f + 10.0f;
}

/*************************************************************************/
/*!
	\brief
	Update the player's damage reduction based on their defense
	
	\param CurrentPlayerStats
	A pointer to the player's stats to update
*/
/*************************************************************************/
void updateDamageReduction(PlayerStats *CurrentPlayerStats)
{
	//Placeholder damage reduction formula
	CurrentPlayerStats->DamageReduction = CurrentPlayerStats->Defense * 2.0f / 100.0f;
}

/*************************************************************************/
/*!
	\brief
	Detects collisions with the player and calls the correct resolution functions
*/
/*************************************************************************/
void DetectPlayerCollision(void)
{
	Platform* pList = platformList;
	Food* fList = foodList;
	int hit = 0;
	int hitPrev = 0;

	while(pList->objID != 0)
	{
		if(pList->objID > 0)
		{
			hit = CollisionRectangles(&CurrentPlayer.PlayerCollider, &pList->PlatformCollider);
			hitPrev = searchHitArray(CurrentPlayer.CollisionData, COLLIDEAMOUNT, pList->PlatformCollider.collisionID);
			if(hit)
			{
				// New target, on start collision
				if(hitPrev < 0)
				{
					CurrentPlayer.CollisionData[-hitPrev] = pList->PlatformCollider.collisionID * 10 + 1;
					//printf("NOT FOUND: %i\n", -hitPrev);
					PlayerCollidePlatform(&CurrentPlayer, pList);
				}
				// Found target, hit previous frame, on persistant
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 1)
				{
					//printf("FOUND PERSISTANT: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					PlayerCollidePlatform(&CurrentPlayer, pList);
				}
				// Found target, did not hit previous frame, on start collision
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 0)
				{
					//printf("FOUND NEW COLLISION: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					CurrentPlayer.CollisionData[hitPrev] = pList->PlatformCollider.collisionID * 10 + 1;
					PlayerCollidePlatform(&CurrentPlayer, pList);
				}
			}
			else
			{
				if(hitPrev < 0 || CurrentPlayer.CollisionData[hitPrev] % 10 == 0)
				{
					// NEVER COLLIDED OR DIDNT COLLIDE PREV FRAME
				}
				// Found target, collision ended
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 1)
				{
					//printf("END COLLISION: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					CurrentPlayer.CollisionData[hitPrev] = 0;
				}
			}
		}
		pList++;
	}
	while(fList->objID != 0)
	{
		if(fList->objID > 0)
		{
			hit = CollisionRectangles(&CurrentPlayer.PlayerCollider, &fList->FoodCollider);
			hitPrev = searchHitArray(CurrentPlayer.CollisionData, COLLIDEAMOUNT, fList->FoodCollider.collisionID);

			if(hit)
			{
				// New target, on start collision
				if(hitPrev < 0)
				{
					CurrentPlayer.CollisionData[-hitPrev] = fList->FoodCollider.collisionID * 10 + 1;
					//printf("NOT FOUND: %i\n", -hitPrev);
					PlayerCollideFood(&CurrentPlayer, fList);
					fList->objID = -1;
				}
				// Found target, hit previous frame, on persistant
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 1)
				{
					//printf("FOUND PERSISTANT: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					PlayerCollideFood(&CurrentPlayer, fList);
					fList->objID = -1;
				}
				// Found target, did not hit previous frame, on start collision
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 0)
				{
					//printf("FOUND NEW COLLISION: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					CurrentPlayer.CollisionData[hitPrev] = fList->FoodCollider.collisionID * 10 + 1;
					PlayerCollideFood(&CurrentPlayer, fList);
					fList->objID = -1;
				}
			}
			else
			{
				if(hitPrev < 0 || CurrentPlayer.CollisionData[hitPrev] % 10 == 0)
				{
					// NEVER COLLIDED OR DIDNT COLLIDE PREV FRAME
				}
				// Found target, collision ended
				else if(CurrentPlayer.CollisionData[hitPrev] % 10 == 1)
				{
					//printf("END COLLISION: %i\n", CurrentPlayer.CollisionData[hitPrev]);
					CurrentPlayer.CollisionData[hitPrev] = 0;
				}
			}
		}
		fList++;
	}

	// Check projectile collisions
	//	-> Handle collision if true
	// other collisions!
}

/*************************************************************************/
/*!
	\brief
	Animates the players legs.
*/
/*************************************************************************/
void LegAnimation(Player *Object, Sprite *LegUpr, Sprite *LegUpr2, Sprite *LegLwr, Sprite *LegLwr2, Sprite *Bdy, Sprite *ArmUpr, Sprite *ArmLwr)
{
	float LegDistance = 9.25f-Object->Speed;
	float LegUpperDirection = (float)sin(Object->LegSinValue)/(LegDistance);
	float LegLowerDirection;
	float LegUpperDirection2 = (float)sin(Object->LegSinValue)/(LegDistance);
	float LegLowerDirection2;

	Object->LegSinValue += 0.1f; 

	Bdy->Position.x = Object->Position.x;
	Bdy->Position.y = Object->Position.y + (float)sin(-Object->LegSinValue*2)*5/(LegDistance);

    if (LegUpperDirection < 0)
        LegLowerDirection = ((float)sin(Object->LegSinValue)/2 + (float)sin(Object->LegSinValue) * -0.8f)/(LegDistance);
    else
        LegLowerDirection = (LegUpperDirection + (float)sin(Object->LegSinValue) + (float)sin(Object->LegSinValue) * 0.4f)/(LegDistance);

    if (LegUpperDirection2 > 0)
        LegLowerDirection2 = ((float)sin(Object->LegSinValue)/2 + (float)sin(Object->LegSinValue) * -0.8f)/(LegDistance);
    else
        LegLowerDirection2 = (LegUpperDirection2 + (float)sin(Object->LegSinValue) + (float)sin(Object->LegSinValue) * 0.4f)/(LegDistance);
	
	LegUpr->FlipX = Object->PlayerSprite->FlipX;
	LegLwr->FlipX = Object->PlayerSprite->FlipX;
	LegUpr2->FlipX = Object->PlayerSprite->FlipX;
	LegLwr2->FlipX = Object->PlayerSprite->FlipX;
	Bdy->FlipX = Object->PlayerSprite->FlipX;
	ArmUpr->FlipX = Object->PlayerSprite->FlipX;
	ArmLwr->FlipX = Object->PlayerSprite->FlipX;


	if (Object->PlayerSprite->FlipX == FALSE)
	{
		LegUpr->Rotation = LegUpperDirection;
		LegUpr->Position.x = Object->Position.x + (float)sin(Object->LegSinValue)*8/(LegDistance);
		LegUpr->Position.y = Object->Position.y - (float)sin(Object->LegSinValue*2)*5/(LegDistance);
		LegLwr->Position.x = (float)cos(LegUpr->Rotation-(FOX_PI/2)) * 30 + LegUpr->Position.x;
		LegLwr->Position.y = (float)sin(LegUpr->Rotation-(FOX_PI/2)) * 30 + LegUpr->Position.y;
		LegLwr->Rotation = LegLowerDirection;

		LegUpr2->Rotation = -LegUpperDirection2;
		LegUpr2->Position.x = Object->Position.x + (float)sin(Object->LegSinValue)*-8/(LegDistance);
		LegUpr2->Position.y = Object->Position.y - (float)sin(Object->LegSinValue*2)*5/(LegDistance);
		LegLwr2->Position.x = (float)cos(LegUpr2->Rotation-(FOX_PI/2)) * 30 + LegUpr2->Position.x;
		LegLwr2->Position.y = (float)sin(LegUpr2->Rotation-(FOX_PI/2)) * 30 + LegUpr2->Position.y;
		LegLwr2->Rotation = -LegLowerDirection2;

		ArmUpr->Rotation = LegUpperDirection/1.5f + 1.5f;
		ArmLwr->Rotation = ArmUpr->Rotation - 1.0f + LegUpperDirection/2.0f;

		ArmUpr->Position.x = Bdy->Position.x;
		ArmUpr->Position.y = Bdy->Position.y + 50;

		ArmLwr->Position.x = ArmUpr->Position.x - (float)cos(ArmUpr->Rotation) * 40;
		ArmLwr->Position.y = ArmUpr->Position.y - (float)sin(ArmUpr->Rotation) * 40;
	}
	else
	{
		LegUpr->Rotation = -LegUpperDirection;
		LegUpr->Position.x = Object->Position.x + (float)sin(Object->LegSinValue)*-8/(LegDistance);
		LegUpr->Position.y = Object->Position.y - (float)sin(Object->LegSinValue*2)*5/(LegDistance);
		LegLwr->Position.x = (float)cos(LegUpr->Rotation-(FOX_PI/2)) * 30 + LegUpr->Position.x;
		LegLwr->Position.y = (float)sin(LegUpr->Rotation-(FOX_PI/2)) * 30 + LegUpr->Position.y;
		LegLwr->Rotation = -LegLowerDirection;

		LegUpr2->Rotation = LegUpperDirection2;
		LegUpr2->Position.x = Object->Position.x + (float)sin(Object->LegSinValue)*8/(LegDistance);
		LegUpr2->Position.y = Object->Position.y - (float)sin(Object->LegSinValue*2)*5/(LegDistance);
		LegLwr2->Position.x = (float)cos(LegUpr2->Rotation-(FOX_PI/2)) * 30 + LegUpr2->Position.x;
		LegLwr2->Position.y = (float)sin(LegUpr2->Rotation-(FOX_PI/2)) * 30 + LegUpr2->Position.y;
		LegLwr2->Rotation = LegLowerDirection2;

		ArmUpr->Rotation = -LegUpperDirection/1.5f - 1.5f;
		ArmLwr->Rotation = ArmUpr->Rotation + 1.0f - LegUpperDirection/2.0f;

		ArmUpr->Position.x = Bdy->Position.x;
		ArmUpr->Position.y = Bdy->Position.y + 50;

		ArmLwr->Position.x = ArmUpr->Position.x + (float)cos(ArmUpr->Rotation) * 40;
		ArmLwr->Position.y = ArmUpr->Position.y + (float)sin(ArmUpr->Rotation) * 40;
	}

	
	
	
	//*************************************************************************************************
}
