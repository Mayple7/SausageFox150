#ifndef FOX_NARR2
#define FOX_NARR2

#include "../HeaderFiles/Sound.h"

void LoadNarr2(void);
void InitializeNarr2(void);
void UpdateNarr2(void);
void DrawNarr2(void);
void FreeNarr2(void);
void UnloadNarr2(void);

static void EventNarr2(void);

struct FoxSound WinBackSnd;

#endif