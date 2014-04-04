#ifndef FOX_HUD
#define FOX_HUD

#include "FoxMath.h"
#include "FoxEngine.h"
#include "Sprite.h"
#include "TextCreation.h"

typedef struct HUD
{
	Sprite* HUDBackground;
	Sprite* HUDItem[4];

	Sprite* HealthBar;
	Sprite* HealthBarEnd;

	Sprite* AbilityBar;
	Sprite* AbilityBarEnd;

	Sprite* ExpBar;
	Sprite* ExpBarEnd;

	struct TextGlyphs *FPSText;

	struct TextGlyphs *StatusText;
	struct TextGlyphs *CoinText;
	struct TextGlyphs *RankText;
	int currentHUDCoinValue;
	int currentHUDRankValue;
}HUD;

HUD* CreateHUD(Player* CurrentPlayer);
void UpdateHUDPosition(HUD* CurrentHUD);
void UpdateHUDItems(HUD* CurrentHUD, Player* CurrentPlayer);
void FreeHUD(HUD* CurrentHUD);

#endif