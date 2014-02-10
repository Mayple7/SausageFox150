#ifndef FOX_EPMENU
#define FOX_EPMENU

void LoadEPMenu(void);
void InitializeEPMenu(void);
void UpdateEPMenu(void);
void DrawEPMenu(void);
void FreeEPMenu(void);
void UnloadEPMenu(void);

void EPMenuInput(void);
void UpdateEPSelector(struct Sprite* Selector);

#endif