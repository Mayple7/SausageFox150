#ifndef FOX_MAINMENU
#define FOX_MAINMENU

void InitializeMainMenu(void);
void FreeMainMenu(void);
void DrawMainMenu(void);
static void InputHandling(void);
void UnloadMainMenu(void);
void LoadMainMenu(void);
void UpdateMainMenu(void);

void BackgroundAnimation(void);
void SetStartLocation(float *xPos, float *yPos, int startNum);

#endif