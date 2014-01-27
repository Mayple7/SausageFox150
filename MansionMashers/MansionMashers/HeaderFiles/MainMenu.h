#ifndef FOX_MAINMENU
#define FOX_MAINMENU

void InitializeMainMenu(void);
void FreeMainMenu(void);
void DrawMainMenu(void);
void InputHandling(void);
void UpdateSelector(struct Sprite* Selector);
void UnloadMainMenu(void);
void LoadMainMenu(void);
void UpdateMainMenu(void);

#endif