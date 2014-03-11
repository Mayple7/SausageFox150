#ifndef FOX_HUD
#define FOX_HUD

#include "FoxMath.h"
#include "FoxEngine.h"
#include "Sprite.h"
#include "TextCreation.h"

typedef struct HUD
{
	Sprite* HUDBackground;
	Sprite* HUDItem[6];

	struct TextGlyphs *CoinText;
	int currentHUDCoinValue;
}HUD;

HUD* CreateHUD(Player* CurrentPlayer);
void UpdateHUDPosition(HUD* CurrentHUD);
void UpdateHUDItems(HUD* CurrentHUD, Player* CurrentPlayer);
void FreeHUD(HUD* CurrentHUD);

#endif