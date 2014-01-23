#ifndef FOX_MAINMENU
#define FOX_MAINMENU

int MenuLoop(void);
void InitizalizeMainMenu(void);
void FreeMainMenu(void);
void DrawMainMenu(void);
int InputHandling(void);
void UpdateSelector(struct Sprite* Selector);
void UnloadMainMenu(void);
void LoadMainMenu(void);
void UpdateMainMenu(void);

#endif