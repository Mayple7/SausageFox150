/*
File:				Player.c
Author:				Dan Muller (d.muller)
Creation Date:		Jan 8, 2014

Purpose:			All the player components and code are here.

Functions:			MenuLoop - Main loop for the main menu
 
Copyright (C) 2014 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited. 
*/ 



// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/Sprite.h"
#include "../HeaderFiles/Player.h"

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
	CreateSprite(&Player->PlayerSprite, 250.0f, 150.0f, 4, 2, "TextureFiles/SausageFox.png");

	Player->PlayerSprite.XPosition = 0.0f;
	Player->PlayerSprite.YPosition = 0.0f;

	Player->PlayerSprite.TotalFrames = 8;
	Player->PlayerSprite.AnimationActive = 1;
	Player->PlayerSprite.AnimationSpeed = 6;
}

void DrawPlayer(struct Player *Player)
{
	DrawSprite(&Player->PlayerSprite);
}

