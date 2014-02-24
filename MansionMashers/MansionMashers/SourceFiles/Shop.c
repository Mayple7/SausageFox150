// ---------------------------------------------------------------------------
// includes
#include "../AEEngine.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"

// ---------------------------------------------------------------------------
// Globals

/*************************************************************************/
/*!
	\brief
	Creates a Weapon Shop from given variables
*/
/*************************************************************************/
void CreateWeaponShop(float shopPosX, float shopPosY, Weapon *Weapon, int *newID, int collisionGroup, int weaponRarity)
{
	Vec3 TextTint;
	WeaponShop *newShop = AddWeaponShop();

	//Sign
	CreateSprite("TextureFiles/WoodSign.png", 512, 512, 5, 1, 1, shopPosX, shopPosY);

	//Text
	Vec3Set(&TextTint, 0, 0, 0);
	newShop->ItemName = CreateText("SWORD", shopPosX + 20, shopPosY + 120, 80, TextTint, Center);
	ChangeTextVisibility((TextGlyphs *) newShop->ItemName);
	Vec3Set(&TextTint, 0.2f, 0.2f, 0.2f);
	newShop->ItemCoin = CreateText("5 coin", shopPosX + 10, shopPosY + 60, 60, TextTint, Center);
	ChangeTextVisibility((TextGlyphs *) newShop->ItemCoin);

	//Weapon
	Weapon = CreateDroppedWeapon(collisionGroup, weaponRarity, 250, 250, *newID++, -350, -140);
	Weapon->WeaponSprite->Rotation = (float)FOX_PI / 2;

	//Gongratz
	newShop->Created = TRUE;
}
