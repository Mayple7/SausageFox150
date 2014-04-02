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

static Sprite *DeathConfirm;
static Button *RestartButton;
static Button *MainMapButton;

static float confirmTimer;

/*************************************************************************/
/*!
	\brief
	Creates objects for menu when player dies

	\param newID
	ID for objects with collision
*/
/*************************************************************************/
void CreateDeathConfirmObjects(int *newID)
{
	Vec3 TextTint;

	Vec3Set(&TextTint, 0, 0, 0);
	DeathConfirm = (Sprite *) CreateSprite("TextureFiles/DeathConfirm.png", 639, 204, 4020, 1, 1, -1920, 100);

	MainMapButton = CreateButton("TextureFiles/GoToMapButton.png", -1920, -130, 300, 112.5f, (*newID)++);
	MainMapButton->ButtonSprite->ZIndex = 4021;

	RestartButton = CreateButton("TextureFiles/RestartButton.png", -1920, -130, 300, 112.5f, (*newID)++);
	RestartButton->ButtonSprite->ZIndex = 4022;

	confirmTimer = 0;
}

/*************************************************************************/
/*!
	\brief
	Updates objects when player has died 
*/
/*************************************************************************/
void UpdateDeathConfirmObjects(void)
{
	int worldX, worldY;
	Vec2 MouseClick;

	if(confirmTimer < 1.0f)
	{
		confirmTimer += GetDeltaTime();
		MainMapButton->ButtonSprite->Visible = FALSE;
		RestartButton->ButtonSprite->Visible = FALSE;
	}
	else
	{
		MainMapButton->ButtonSprite->Visible = TRUE;
		RestartButton->ButtonSprite->Visible = TRUE;
	}

	CurrentPlayer.Position.x = -1920;
	UpdateCollisionPosition(&CurrentPlayer.PlayerCollider, &CurrentPlayer.Position);

	FoxInput_GetWorldPosition(&worldX, &worldY);
	Vec2Set(&MouseClick, (float)worldX, (float)worldY);

	UpdateButtonPosition(RestartButton, (-250) + GetCameraXPosition(), -130);
	UpdateCollisionPosition(&RestartButton->ButtonCollider, &RestartButton->ButtonSprite->Position);

	UpdateButtonPosition(MainMapButton, (250) + GetCameraXPosition(), -130);
	UpdateCollisionPosition(&MainMapButton->ButtonCollider, &MainMapButton->ButtonSprite->Position);

	DeathConfirm->Position.x = GetCameraXPosition();

	if(PointRectCollision(&RestartButton->ButtonCollider, &MouseClick))
	{
		RestartButton->ButtonSprite->ScaleX = 1.2f;
		RestartButton->ButtonSprite->ScaleY = 1.2f;

		if(confirmTimer > 1.0f && FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			FreeButton(RestartButton);
			FreeButton(MainMapButton);
			FreeSprite(DeathConfirm);
			if(GetCurrentState() == GS_Level31)
				SetNextState(GS_Level3);
			else
				SetNextState(GS_Restart);
		}
	}
	else
	{
		RestartButton->ButtonSprite->ScaleX = 1.0f;
		RestartButton->ButtonSprite->ScaleY = 1.0f;
	}

	if(PointRectCollision(&MainMapButton->ButtonCollider, &MouseClick))
	{
		if(confirmTimer > 1.0f && FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			FreeButton(RestartButton);
			FreeButton(MainMapButton);
			FreeSprite(DeathConfirm);
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

