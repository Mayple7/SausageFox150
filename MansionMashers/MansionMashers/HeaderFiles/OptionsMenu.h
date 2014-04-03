#ifndef FOX_OPTIONS
#define FOX_OPTIONS

void LoadOptions(void);
void InitializeOptions(void);
void UpdateOptions(void);
void DrawOptions(void);
void FreeOptions(void);
void UnloadOptions(void);
void EventOptions(void);

void BackgroundOptionsAnimation(void);
void SetOptionsStartLocation(float *xPos, float *yPos, int startNum);
int RandomNewOptionsTexture(Sprite* CurrentSprite, int prevTexture);

#endif