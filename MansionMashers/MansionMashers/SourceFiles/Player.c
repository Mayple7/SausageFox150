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
void InitializePlayer(struct Player *CurrentPlayer)
{
	//Creates the sprite for the player
	CurrentPlayer->PlayerSprite = CreateSprite("Player", "TextureFiles/SausageFox.png", 250.0f, 150.0f, 10, 4, 2, PlayerType);

	//Default position of the player
	CurrentPlayer->Position.x = 0.0f;
	CurrentPlayer->Position.y = 0.0f;

	//Animation properties
	CurrentPlayer->PlayerSprite->AnimationSpeed = 4; // STOP CHANGING HIS LEG SPEED -The Supreme Sausage

	//Collision properties
	CurrentPlayer->PlayerSprite->CollideSize.x   = 2 * CurrentPlayer->PlayerSprite->Width  / 3;
	CurrentPlayer->PlayerSprite->CollideSize.y   = CurrentPlayer->PlayerSprite->Height / 2;
	CurrentPlayer->PlayerSprite->CollideOffset.x =  0.0f;
	CurrentPlayer->PlayerSprite->CollideOffset.y =  -20.0f;

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
	//Move left when A is pushed
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
		MoveObject(&CurrentPlayer->Position, LEFT, 8.0f);
		CurrentPlayer->PlayerSprite->FlipX = 0;
	}
	//Move right when D is pushed
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
		MoveObject(&CurrentPlayer->Position, RIGHT, 8.0f);
		CurrentPlayer->PlayerSprite->FlipX = 1;
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
		if(CurrentPlayer->Position.y < -225)
		{
			Vec2Set(&CurrentPlayer->Position, CurrentPlayer->Position.x, -224.9f);
		}
		ApplyForce(&CurrentPlayer->PlayerRigidBody, &force);
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
	Handles a collision with the player
	
	\param objHit
	Object that hit the player
*/
/*************************************************************************/
void HandleCollision(Sprite *objHit)
{
	//If the object is Ham
	if (objHit->CollisionGroup == FoodType)
	{
		printf("YUM YUM YUM YUM  DELICIOUSO\n");
		freeObject(objHit);
	}
	//If the object is an enemy
	else if (objHit->CollisionGroup == EnemyType)
	{
		if((objHit->Position.y + (objHit->Height / 3.0f) < CurrentPlayer.Position.y - (CurrentPlayer.PlayerSprite->CollideSize.y / 2.0f)) && CurrentPlayer.PlayerRigidBody.Velocity.y < 0)
		{
			printf("BOOP!\n");
			freeObject(objHit);
			SetVelocity(&CurrentPlayer.PlayerRigidBody, 0.0f, 10.0f);
		}
	}
	//If the object is a platform and you're landing on it
	else if(objHit->CollisionGroup == PlatformType && CurrentPlayer.PlayerRigidBody.Velocity.y <= 0)
	{
		if(CurrentPlayer.Position.y + CurrentPlayer.PlayerSprite->CollideOffset.y - CurrentPlayer.PlayerSprite->CollideSize.y / 2.0f > objHit->Position.y + objHit->CollideOffset.y)
		{
			if(CurrentPlayer.PlayerRigidBody.Velocity.y != 0)
			{
				CurrentPlayer.Position.y = objHit->Position.y + objHit->CollideOffset.y + objHit->CollideSize.y / 2 - CurrentPlayer.PlayerSprite->CollideOffset.y + (CurrentPlayer.PlayerSprite->CollideSize.y / 2 - 0.01);
			}
			CurrentPlayer.PlayerRigidBody.onGround = TRUE;
		}
	}
	//If you are not landing on the platform
	else if(objHit->CollisionGroup == PlatformType)
	{
		CurrentPlayer.PlayerRigidBody.onGround = FALSE;
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

void DetectPlayerCollision(void)
{
	// Check Platform collisions
	//	-> Handle collision if true
	// Check projectile collisions
	//	-> Handle collision if true
	// other collisions!
}
