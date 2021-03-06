/*****************************************************************************/
/*!
\file				Level.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_LEVEL
#define FOX_LEVEL

void CreateUpgradeScreenObjects(void);
void UpdateUpgradeScreenObjects(void);
void InitializePlayerHurt(Player *CurrentPlayer);
void UpdatePlayerHurt(Player *CurrentPlayer);
void InitializePlayerRank(Player *CurrentPlayer);
void UpdatePlayerRank(Player *CurrentPlayer);
int GetMaxExperience(Player *CurrentPlayer);
void LevelCompletion(void);
void GameCompletion(void);

int GetObjectID(void);

int UpgradeComplete; //Global for telling when the player is done upgrading
int ObjectID;        //Global object ID, call "GetObjID"

#endif