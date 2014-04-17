/*****************************************************************************/
/*!
\file				WeaponNames.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_WEAPONNAMES
#define FOX_WEAPONNAMES

#define NUM_COMMON_START 12
#define NUM_UNCOMMON_START 12
#define NUM_RARE_START 12

#define NUM_SWORD_SYNONYMS 15
#define NUM_AXE_SYNONYMS 8
#define NUM_HAMMER_SYNONYMS 8
#define NUM_SPEAR_SYNONYMS 10

#define NUM_COMMON_END 9
#define NUM_UNCOMMON_END 10
#define NUM_RARE_END 10

char* GetCommonStart(void);
char* GetUncommonStart(void);
char* GetRareStart(void);

char* GetSwordSynonym(void);
char* GetAxeSynonym(void);
char* GetHammerSynonym(void);
char* GetSpearSynonym(void);

char* GetCommonEnd(void);
char* GetUncommonEnd(void);
char* GetRareEnd(void);

#endif