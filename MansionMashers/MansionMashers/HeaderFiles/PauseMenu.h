#ifndef FOX_PAUSE
#define FOX_PAUSE

void LoadPause(void);
void InitializePause(void (*DrawLevel)());
void UpdatePause(void);
void DrawPause(void);
void FreePause(void);
void UnloadPause(void);

#endif