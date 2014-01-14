/*
File:				Player.c
Author:				Dan Muller (d.muller)
Creation Date:		Jan 8, 2014

Purpose:			All the player components and code are here.

Functions:			InitializePlayer - Creates the player object and sprites.
					DrawPlayer - Draws the player based on the sprite.
 
Copyright (C) 2014 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited. 
*/ 



// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/Vector2.h"
#include "../HeaderFiles/Sprite.h"
#include "../HeaderFiles/Player.h"
#include "../HeaderFiles/RigidBody.h"


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
	CurrentPlayer->PlayerSprite = *CreateSprite(250.0f, 150.0f, 4, 2, "TextureFiles/SausageFox.png");

	CurrentPlayer->PlayerSprite.Position.x = 0.0f;
	CurrentPlayer->PlayerSprite.Position.y = 0.0f;

	CurrentPlayer->PlayerSprite.TotalFrames = 8;
	CurrentPlayer->PlayerSprite.AnimationActive = 1;
	CurrentPlayer->PlayerSprite.AnimationSpeed = 6;

	CurrentPlayer->Position.x = 0.0f;
	CurrentPlayer->Position.y = 0.0f;
}

void DrawPlayer(struct Player *CurrentPlayer)
{
	CurrentPlayer->PlayerSprite.Position.x = CurrentPlayer->Position.x;
	CurrentPlayer->PlayerSprite.Position.y = CurrentPlayer->Position.y;
	DrawSprite(&CurrentPlayer->PlayerSprite);
}


void Input_Player(int key)
{
	/*
	switch (key)
	{
		case 'A':
			obj_move
	}*/
}