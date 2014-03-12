/*****************************************************************************/
/*!
\file				Death.c
\author				Juli Gregg
\date				Mar 11, 2014

\brief				

\par				Functions:
\li					
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#include "../AEEngine.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"

static Sprite* DeathConfirm;
static Button* RestartButton;
static Button* MainMapButton;

void CreateDeathConfirmObjects(int* newID)
{
	Vec3 TextTint;

	RestartButton = CreateButton("TextureFiles/RestartButton.png", -1920, -130, 300, 112.5f, *newID++);
	RestartButton->ButtonSprite->ZIndex = 1502;
	MainMapButton = CreateButton("TextureFiles/GoToMapButton.png", -1920, -130, 300, 112.5f, *newID++);
	MainMapButton->ButtonSprite->ZIndex = 1502;
	Vec3Set(&TextTint, 0, 0, 0);
	DeathConfirm = (Sprite *) CreateSprite("TextureFiles/DeathConfirm.png", 639, 204, 500, 1, 1, -1920, 100);
}

void UpdateDeathConfirmObjects(void)
{
	int worldX, worldY;
	Vec2 MouseClick;

	FoxInput_GetWorldPosition(&worldX, &worldY);
	Vec2Set(&MouseClick, (float)worldX, (float)worldY);

	RestartButton->ButtonSprite->Position.x = (-250 * GetLoadRatio()) + GetCameraXPosition();
	UpdateCollisionPosition(&RestartButton->ButtonCollider, &RestartButton->ButtonSprite->Position);
	MainMapButton->ButtonSprite->Position.x = (250 * GetLoadRatio()) + GetCameraXPosition();
	UpdateCollisionPosition(&MainMapButton->ButtonCollider, &MainMapButton->ButtonSprite->Position);
	DeathConfirm->Position.x = GetCameraXPosition();

	if(PointRectCollision(&RestartButton->ButtonCollider, &MouseClick))
	{
		RestartButton->ButtonSprite->ScaleX = 1.2f;
		RestartButton->ButtonSprite->ScaleY = 1.2f;

		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
			SetNextState(GS_Restart);
	}
	else
	{
		RestartButton->ButtonSprite->ScaleX = 1.0f;
		RestartButton->ButtonSprite->ScaleY = 1.0f;
	}

	if(PointRectCollision(&MainMapButton->ButtonCollider, &MouseClick))
	{
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
				SetNextState(GS_MapLevel);
		}
		MainMapButton->ButtonSprite->ScaleX = 1.2f;
		MainMapButton->ButtonSprite->ScaleY = 1.2f;
	}
	else
	{
		MainMapButton->ButtonSprite->ScaleX = 1.0f;
		MainMapButton->ButtonSprite->ScaleY = 1.0f;
	}
}

