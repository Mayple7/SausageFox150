// ---------------------------------------------------------------------------
// includes
#include "../AEEngine.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"
#include "../HeaderFiles/CharacterSelect.h"

// ---------------------------------------------------------------------------
// Globals
Player CharacterMayple;
Player CharacterGinko;
Player CharacterHolly;
Player CharacterKaya;

Button* MaypleButton;
Button* GinkoButton;
Button* HollyButton;
Button* KayaButton;

Sprite* BackgroundColor;
Sprite* BackgroundText;

Sprite* MaypleSelected;
Sprite* GinkoSelected;
Sprite* KayaSelected;
Sprite* HollySelected;

static int newID;
static int selected;

void LoadCharacterSelect(void)
{
	FILE *fp;

	fp = fopen("GameData.cfg", "wt");
	if(fp)
		fclose(fp);

	//Allocate space for a large texture
	CreateTextureList();
}

void InitializeCharacterSelect(void)
{
	Vec3 Tint;
	selected = Mayple;
	newID = 10;
	ResetObjectList();
	ResetSoundList();
	Vec3Set(&Tint, 0.475f, 0.463f, 0.311f);

	BackgroundColor = (Sprite *) CreateSprite("TextureFiles/BlankPlatform.png", 1920, 1080, 1, 1, 1, 0, 0);
	BackgroundColor->Tint = Tint;

	BackgroundText = (Sprite *) CreateSprite("TextureFiles/CharacterSelectText.png", 1920, 1080, 2, 1, 1, 0, 0);
	
	MaypleSelected = (Sprite *) CreateSprite("TextureFiles/MaypleSelect.png", 1920, 1080, 2, 1, 1, 0, 0);
	GinkoSelected = (Sprite *) CreateSprite("TextureFiles/GinkoSelect.png", 1920, 1080, 2, 1, 1, 0, 0);
	GinkoSelected->Alpha = 0.0f;
	KayaSelected = (Sprite *) CreateSprite("TextureFiles/KayaSelect.png", 1920, 1080, 2, 1, 1, 0, 0);
	KayaSelected->Alpha = 0.0f;
	HollySelected = (Sprite *) CreateSprite("TextureFiles/HollySelect.png", 1920, 1080, 2, 1, 1, 0, 0);
	HollySelected->Alpha = 0.0f;

	MaypleButton = CreateButton("TextureFiles/BlankPlatform.png", -712, -62, 405.0f, 724.0f, newID++);
	MaypleButton->ButtonSprite->Visible = FALSE;
	GinkoButton = CreateButton("TextureFiles/BlankPlatform.png", -235, -62, 405.0f, 724.0f, newID++);
	GinkoButton->ButtonSprite->Visible = FALSE;
	KayaButton = CreateButton("TextureFiles/BlankPlatform.png", 235, -62, 405.0f, 724.0f, newID++);
	KayaButton->ButtonSprite->Visible = FALSE;
	HollyButton = CreateButton("TextureFiles/BlankPlatform.png", 712, -62, 405.0f, 724.0f, newID++);
	HollyButton->ButtonSprite->Visible = FALSE;

	if(remove("../GameData.cfg"))
	{
		printf("FAILED");
	}
	//Create Mayple
	InitializePlayer(&CharacterMayple, Mayple, -712, -80);
	CharacterMayple.Speed = 0.0f;
	CharacterMayple.FlipX = TRUE;
	UpdatePlayerPosition(&CharacterMayple);
	CharacterMayple.PlayerSpriteParts.Tail->CurrentFrame = 3;
	if(remove("../GameData.cfg"))
	{
		printf("FAILED");
	}

	//Create Ginko
	InitializePlayer(&CharacterGinko, Ginko, -235, -80);
	CharacterGinko.Speed = 0.0f;
	CharacterGinko.FlipX = TRUE;
	UpdatePlayerPosition(&CharacterGinko);
	CharacterGinko.PlayerSpriteParts.Tail->CurrentFrame = 9;
	if(remove("../GameData.cfg"))
	{
		printf("FAILED");
	}

	//Create Kaya
	InitializePlayer(&CharacterKaya, Kaya, 235, -80);
	CharacterKaya.Speed = 0.0f;
	CharacterKaya.FlipX = FALSE;
	UpdatePlayerPosition(&CharacterKaya);
	CharacterKaya.PlayerSpriteParts.Tail->CurrentFrame = 6;
	if(remove("../GameData.cfg"))
	{
		printf("FAILED");
	}

	//Create Holly
	InitializePlayer(&CharacterHolly, Holly, 712, -80);
	CharacterHolly.Speed = 0.0f;
	CharacterHolly.FlipX = FALSE;
	UpdatePlayerPosition(&CharacterHolly);
	CharacterHolly.PlayerSpriteParts.Tail->CurrentFrame = 14;
	if(remove("../GameData.cfg"))
	{
		printf("FAILED");
	}
}

void UpdateCharacterSelect(void)
{
	EventCharacterSelect();

	CharacterMayple.PlayerRigidBody.onGround = TRUE;
	CharacterGinko.PlayerRigidBody.onGround = TRUE;
	CharacterKaya.PlayerRigidBody.onGround = TRUE;
	CharacterHolly.PlayerRigidBody.onGround = TRUE;

	//Mayple not selected
	if(selected != Mayple)
	{
		if (CharacterMayple.Speed - 48.0f * GetDeltaTime() >= 0.0f)
		{
			CharacterMayple.Speed -= 48.0f * GetDeltaTime();
		}
		else
		{
			CharacterMayple.Speed = 0.0f;
			CharacterMayple.LegSinValue = 0;
		}
	}

	//Ginko not selected
	if(selected != Ginko)
	{
		if (CharacterGinko.Speed - 48.0f * GetDeltaTime() >= 0.0f)
		{
			CharacterGinko.Speed -= 48.0f * GetDeltaTime();
		}
		else
		{
			CharacterGinko.Speed = 0.0f;
			CharacterGinko.LegSinValue = 0;
		}
	}

	//Kaya not selected
	if(selected != Kaya)
	{
		if (CharacterKaya.Speed - 48.0f * GetDeltaTime() >= 0.0f)
		{
			CharacterKaya.Speed -= 48.0f * GetDeltaTime();
		}
		else
		{
			CharacterKaya.Speed = 0.0f;
			CharacterKaya.LegSinValue = 0;
		}
	}

	//Holly not selected
	if(selected != Holly)
	{
		if (CharacterHolly.Speed - 48.0f * GetDeltaTime() >= 0.0f)
		{
			CharacterHolly.Speed -= 48.0f * GetDeltaTime();
		}
		else
		{
			CharacterHolly.Speed = 0.0f;
			CharacterHolly.LegSinValue = 0;
		}
	}

	//Do if selected
	switch(selected)
	{
	case Mayple:
		CharacterMayple.Speed = CharacterMayple.CurrentPlayerStats.MoveSpeed * GetLoadRatio() * GetDeltaTime();
		break;
	case Ginko:
		CharacterGinko.Speed = CharacterGinko.CurrentPlayerStats.MoveSpeed * GetLoadRatio() * GetDeltaTime();
		break;
	case Kaya:
		CharacterKaya.Speed = CharacterKaya.CurrentPlayerStats.MoveSpeed * GetLoadRatio() * GetDeltaTime();
		break;
	case Holly:
		CharacterHolly.Speed = CharacterHolly.CurrentPlayerStats.MoveSpeed * GetLoadRatio() * GetDeltaTime();
		break;
	}
	Animation(&CharacterMayple);
	Animation(&CharacterGinko);
	Animation(&CharacterKaya);
	Animation(&CharacterHolly);
}

void DrawCharacterSelect(void)
{
	DrawObjectList();
	DrawCollisionList();
}

void FreeCharacterSelect(void)
{
	FreeAllLists();
}

void UnloadCharacterSelect(void)
{
	DestroyTextureList();
	DestroyPlayer(&CharacterMayple);
	DestroyPlayer(&CharacterGinko);
	DestroyPlayer(&CharacterHolly);
	DestroyPlayer(&CharacterKaya);
}

void EventCharacterSelect(void)
{
	int worldX, worldY;
	Vec2 MouseHover;

	FoxInput_GetWorldPosition(&worldX, &worldY);
	Vec2Set(&MouseHover, (float)worldX, (float)worldY);

	if(PointRectCollision(&MaypleButton->ButtonCollider, &MouseHover) || FoxInput_KeyTriggered('1'))
	{
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			SavePlayer(&CharacterMayple);
			SetNextState(GS_Tutorial);
		}
		selected = Mayple;
		MaypleSelected->Alpha = 1.0f;
		GinkoSelected->Alpha = 0.0f;
		KayaSelected->Alpha = 0.0f;
		HollySelected->Alpha = 0.0f;
	}
	else if(PointRectCollision(&GinkoButton->ButtonCollider, &MouseHover) || FoxInput_KeyTriggered('2'))
	{
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			SavePlayer(&CharacterGinko);
			SetNextState(GS_Tutorial);
		}
		selected = Ginko;
		MaypleSelected->Alpha = 0.0f;
		GinkoSelected->Alpha = 1.0f;
		KayaSelected->Alpha = 0.0f;
		HollySelected->Alpha = 0.0f;
	}
	else if(PointRectCollision(&KayaButton->ButtonCollider, &MouseHover) || FoxInput_KeyTriggered('3'))
	{
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			SavePlayer(&CharacterKaya);
			SetNextState(GS_Tutorial);
		}
		selected = Kaya;
		MaypleSelected->Alpha = 0.0f;
		GinkoSelected->Alpha = 0.0f;
		KayaSelected->Alpha = 1.0f;
		HollySelected->Alpha = 0.0f;
	}
	else if(PointRectCollision(&HollyButton->ButtonCollider, &MouseHover) || FoxInput_KeyTriggered('4'))
	{
		if(FoxInput_MouseTriggered(MOUSE_BUTTON_LEFT))
		{
			SavePlayer(&CharacterHolly);
			SetNextState(GS_Tutorial);
		}
		selected = Holly;
		MaypleSelected->Alpha = 0.0f;
		GinkoSelected->Alpha = 0.0f;
		KayaSelected->Alpha = 0.0f;
		HollySelected->Alpha = 1.0f;
	}

	if(FoxInput_KeyTriggered(VK_ESCAPE))
	{
		SetNextState(GS_MainMenu);
	}
}
