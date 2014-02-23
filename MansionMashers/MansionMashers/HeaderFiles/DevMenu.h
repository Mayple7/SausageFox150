#ifndef FOX_DEVMENU
#define FOX_DEVMENU

void InitializeDevMenu(void);
void FreeDevMenu(void);
void DrawDevMenu(void);
static void InputHandling(void);
static void UpdateSelector(struct Sprite* Selector);
void UnloadDevMenu(void);
void LoadDevMenu(void);
void UpdateDevMenu(void);

#endif