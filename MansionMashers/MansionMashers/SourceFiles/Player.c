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

struct Player Player;

// ---------------------------------------------------------------------------
// Static function protoypes

// ---------------------------------------------------------------------------
void InitializePlayer(struct Player *Player)
{
	Player->PlayerSprite = *CreateSprite(250.0f, 150.0f, 4, 2, "TextureFiles/SausageFox.png");

	Player->PlayerSprite.XPosition = 0.0f;
	Player->PlayerSprite.YPosition = 0.0f;

	Player->PlayerSprite.TotalFrames = 8;
	Player->PlayerSprite.AnimationActive = 1;
	Player->PlayerSprite.AnimationSpeed = 6;

	Player->XPos = 0.0f;
	Player->YPos = 0.0f;
}

void DrawPlayer(struct Player *Player)
{
	DrawSprite(&Player->PlayerSprite);
}


void MovePlayer(int key, struct Player *Obj, float move_dist, int direction)
{
	// Handling Input
	AEInputUpdate();

	switch (direction)
	{
		case LEFT:
			if(AEInputCheckCurr(key))
			{
				//Obj->XPos -= move_dist;
				Obj->PlayerSprite.XPosition -= move_dist;
			}
			break;
		case RIGHT:
			if(AEInputCheckCurr(key))
			{
				//Obj->XPos += move_dist;
				Obj->PlayerSprite.XPosition += move_dist;
			}
			break;
		case UP:
			if(AEInputCheckCurr(key))
			{
				//Obj->YPos += move_dist;
				Obj->PlayerSprite.YPosition += move_dist;
			}
			break;
		case DOWN:
			if(AEInputCheckCurr(key))
			{
				//Obj->YPos -= move_dist;
				Obj->PlayerSprite.YPosition -= move_dist;
			}
			break;
	}
}