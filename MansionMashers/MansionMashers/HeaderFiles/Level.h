#ifndef FOX_LEVEL
#define FOX_LEVEL

void CreateUpgradeScreenObjects(int *newID);
void UpdateUpgradeScreenConfirmObjects(void);
void InitializePlayerRank(Player *CurrentPlayer);
void UpdatePlayerRank(Player *CurrentPlayer);
int GetMaxExperience(Player *CurrentPlayer);
void LevelCompletion(Player *CurrentPlayer);


#endif