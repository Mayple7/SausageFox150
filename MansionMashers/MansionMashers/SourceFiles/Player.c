/*
File:				Player.c
Author:				Dan Muller (d.muller)
Creation Date:		Jan 8, 2014

Purpose:			All the player components and code are here.

Functions:			InitializePlayer - Creates the player object
					DrawPlayer - Draws the player based on the sprite.
					UpdatePosition - Updates the position of the player based on velocity and acceleration


Copyright (C) 2014 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited. 
*/ 



// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxEngine.h"


// ---------------------------------------------------------------------------

// Libraries

// ---------------------------------------------------------------------------
// globals

Player CurrentPlayer;

// ---------------------------------------------------------------------------
// Static function protoypes

// ---------------------------------------------------------------------------
void InitializePlayer(struct Player *CurrentPlayer)
{
	CurrentPlayer->PlayerSprite = *CreateSprite(128.0f, 128.0f, 4, 1, "TextureFiles/Bektor.png");

	CurrentPlayer->PlayerSprite.Position.x = 0.0f;
	CurrentPlayer->PlayerSprite.Position.y = 0.0f;

	CurrentPlayer->PlayerSprite.TotalFrames = 4;
	CurrentPlayer->PlayerSprite.AnimationActive = 1;
	CurrentPlayer->PlayerSprite.AnimationSpeed = 12;

	CurrentPlayer->Position.x = 0.0f;
	CurrentPlayer->Position.y = 0.0f;

	CurrentPlayer->PlayerRigidBody.Mass = 10;
	CurrentPlayer->PlayerRigidBody.Drag = 0.5;
	CurrentPlayer->PlayerRigidBody.Area = CurrentPlayer->PlayerSprite.Width * CurrentPlayer->PlayerSprite.Height;
	CurrentPlayer->PlayerRigidBody.Density = CurrentPlayer->PlayerRigidBody.Mass / CurrentPlayer->PlayerRigidBody.Area;

	ZeroAcceleration(&CurrentPlayer->PlayerRigidBody);
	SetVelocity(&CurrentPlayer->PlayerRigidBody, 0.0f, 0.0f);
}

void DrawPlayer(struct Player *CurrentPlayer)
{
	UpdatePosition(CurrentPlayer);
	CurrentPlayer->PlayerSprite.Position.x = CurrentPlayer->Position.x;
	CurrentPlayer->PlayerSprite.Position.y = CurrentPlayer->Position.y;
	DrawSprite(&CurrentPlayer->PlayerSprite);
}


void InputPlayer(struct Player *CurrentPlayer, int key)
{
	
	switch (key)
	{
		case 'A':
			if(AEInputCheckCurr('A'))
			{
				//MoveObject(&CurrentPlayer->Position, LEFT, 3.0f);
				//CurrentPlayer->PlayerSprite.FlipX = 0;
				CurrentPlayer->PlayerSprite.Rotation += 0.03;
			}
			break;
		case 'S':
			if(AEInputCheckCurr('S'))
			{
				//MoveObject(&CurrentPlayer->Position, DOWN, 3.0f);
				CurrentPlayer->Position.x -= cos(CurrentPlayer->PlayerSprite.Rotation+(PI/2)) * 1.5f;
				CurrentPlayer->Position.y -= sin(CurrentPlayer->PlayerSprite.Rotation+(PI/2)) * 1.5f;
			}
			break;
		case 'D':
			if(AEInputCheckCurr('D'))
			{
				//MoveObject(&CurrentPlayer->Position, RIGHT, 3.0f);
				//CurrentPlayer->PlayerSprite.FlipX = 1;
				CurrentPlayer->PlayerSprite.Rotation -= 0.03;
			}
			break;
		case 'W':
			if(AEInputCheckCurr('W'))
			{
				//MoveObject(&CurrentPlayer->Position, UP, 3.0f);
				CurrentPlayer->Position.x += cos(CurrentPlayer->PlayerSprite.Rotation+(PI/2)) * 3;
				CurrentPlayer->Position.y += sin(CurrentPlayer->PlayerSprite.Rotation+(PI/2)) * 3;
			}	
			break;
	}
}

void UpdatePosition(Player *CurrentPlayer)
{
	UpdateVelocity(&CurrentPlayer->PlayerRigidBody);
	Vec2Add(&CurrentPlayer->Position, &CurrentPlayer->Position, &CurrentPlayer->PlayerRigidBody.Velocity);
}
