/*****************************************************************************/
/*!
\file				OptionsMenu.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
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