/*****************************************************************************/
/*!
\file				WeaponNames.c
\author				Dan Muller (d.muller)
\date				Feb 4, 2014

\brief				Functions for naming weapons

\par				Functions:
\li					GetCommonStart
\li					GetUncommonStart
\li					GetRareStart
\li					GetSwordSynonym
\li					GetAxeSynonym
\li					GetHammerSynonym
\li					GetSpearSynonym
\li					GetCommonEnd
\li					GetUncommonEnd
\li					GetRareEnd

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

#include "../HeaderFiles/WeaponNames.h"
#include "../HeaderFiles/FoxEngine.h"

char* GetCommonStart(void)
{
	int randomName = rand() % NUM_COMMON_START;

	switch(randomName)
	{
	case 0:
		return "Dull ";
	case 1:
		return "Broken ";
	case 2:
		return "Sausage ";
	case 3:
		return "Chipped ";
	case 4:
		return "Flat ";
	case 5:
		return "Blunt ";
	case 6:
		return "Pointless ";
	case 7:
		return "Wooden ";
	case 8:
		return "Soft ";
	case 9:
		return "Mushy ";
	case 10:
		return "Cloth ";
	default:
		return "Common ";
	}
}

char* GetUncommonStart(void)
{
	int randomName = rand() % NUM_COMMON_START;

	switch(randomName)
	{
	case 0:
		return "Iron ";
	case 1:
		return "Heavy ";
	case 2:
		return "Swift ";
	case 3:
		return "Treasured ";
	case 4:
		return "Adequate ";
	case 5:
		return "Soldiers ";
	case 6:
		return "Guards ";
	case 7:
		return "Special ";
	case 8:
		return "Sausage ";
	case 9:
		return "Sharp ";
	case 10:
		return "Quality ";
	default:
		return "Uncommon ";
	}
}

char* GetRareStart(void)
{
	int randomName = rand() % NUM_COMMON_START;

	switch(randomName)
	{
	case 0:
		return "Epic ";
	case 1:
		return "Legendary ";
	case 2:
		return "Fantastic ";
	case 3:
		return "Sausage ";
	case 4:
		return "Kings ";
	case 5:
		return "Amazing ";
	case 6:
		return "Prestigious ";
	case 7:
		return "Esteemed ";
	case 8:
		return "Renowned ";
	case 9:
		return "Impressive ";
	case 10:
		return "Illustrious ";
	default:
		return "Rare ";
	}
}


char* GetSwordSynonym(void)
{
	int randomName = rand() % NUM_SWORD_SYNONYMS;

	switch(randomName)
	{
	case 0:
		return "Shortsword ";
	case 1:
		return "Longsword ";
	case 2:
		return "Dagger ";
	case 3:
		return "Katana ";
	case 4:
		return "Broadsword ";
	case 5:
		return "Dirk ";
	case 6:
		return "Claymore ";
	case 7:
		return "Blade ";
	case 8:
		return "Saber ";
	case 9:
		return "Cutlass ";
	case 10:
		return "Kris ";
	case 11:
		return "Rapier ";
	case 12:
		return "Scimitar ";
	case 13:
		return "Sausage ";
	default:
		return "Sword ";
	}
}

char* GetAxeSynonym(void)
{
	int randomName = rand() % NUM_AXE_SYNONYMS;

	switch(randomName)
	{
	case 0:
		return "Hatchet ";
	case 1:
		return "Tomahawk ";
	case 2:
		return "Chopper ";
	case 3:
		return "Battleaxe ";
	case 4:
		return "Broad Axe ";
	case 5:
		return "Ono ";
	case 6:
		return "Sausage ";
	default:
		return "Axe ";
	}
}

char* GetHammerSynonym(void)
{
	int randomName = rand() % NUM_HAMMER_SYNONYMS;

	switch(randomName)
	{
	case 0:
		return "Warhammer ";
	case 1:
		return "Maul ";
	case 2:
		return "Mace ";
	case 3:
		return "Club ";
	case 4:
		return "Morningstar ";
	case 5:
		return "Sausage ";
	case 6:
		return "Stick ";
	default:
		return "Hammer ";
	}
}

char* GetSpearSynonym(void)
{
	int randomName = rand() % NUM_SPEAR_SYNONYMS;

	switch(randomName)
	{
	case 0:
		return "Spear ";
	case 1:
		return "Lance ";
	case 2:
		return "Halberd ";
	case 3:
		return "Pike ";
	case 4:
		return "Pitchfork ";
	case 5:
		return "Trident ";
	case 6:
		return "Sausage ";
	case 7:
		return "Scythe ";
	case 8:
		return "Harpoon ";
	default:
		return "Polearm ";
	}
}


char* GetCommonEnd(void)
{
	int randomName = rand() % NUM_COMMON_END;

	switch(randomName)
	{
	case 0:
		return "of peasants";
	case 1:
		return "of peons";
	case 2:
		return "of sausage";
	case 3:
		return "of farmers";
	case 4:
		return "of losers";
	case 5:
		return "of villagers";
	case 6:
		return "of chefs";
	case 7:
		return "of commoners";
	default:
		return "";
	}
}

char* GetUncommonEnd(void)
{
	int randomName = rand() % NUM_UNCOMMON_END;

	switch(randomName)
	{
	case 0:
		return "of knights";
	case 1:
		return "of the legion";
	case 2:
		return "of the mayor";
	case 3:
		return "of warriors";
	case 4:
		return "of the guard";
	case 5:
		return "of the fox";
	case 6:
		return "of dogs";
	case 7:
		return "of intensity";
	case 8:
		return "of sausage";
	default:
		return "";
	}
}

char* GetRareEnd(void)
{
	int randomName = rand() % NUM_RARE_END;

	switch(randomName)
	{
	case 0:
		return "of nobility";
	case 1:
		return "of heroes";
	case 2:
		return "of champions";
	case 3:
		return "of conquorers";
	case 4:
		return "of victory";
	case 5:
		return "of paladins";
	case 6:
		return "of immortality";
	case 7:
		return "of the stars";
	case 8:
		return "of sausage";
	default:
		return "";
	}
}
