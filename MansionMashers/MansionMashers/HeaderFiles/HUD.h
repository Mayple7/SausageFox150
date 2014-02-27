#ifndef FOX_HUD
#define FOX_HUD

#include "FoxMath.h"
#include "FoxEngine.h"
#include "Sprite.h"

typedef struct HUD
{
	Sprite* HUDBackground;
	Sprite* HUDItem[4];
}HUD;

HUD* CreateHUD(Player* CurrentPlayer);
void UpdateHUDPosition(HUD* CurrentHUD);
void UpdateHUDItems(HUD* CurrentHUD, Player* CurrentPlayer);
void FreeHUD(HUD* CurrentHUD);

#endif