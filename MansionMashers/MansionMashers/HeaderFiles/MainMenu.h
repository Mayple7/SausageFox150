#ifndef FOX_MAINMENU
#define FOX_MAINMENU

int MenuLoop(void);
void InitizalizeMainMenu(void);
void FreeMainMenu(void);
void DrawMenu(void);
int InputHandling(void);
void UpdateSelector(struct Sprite* Selector);

#endif