/*****************************************************************************/
/*!
\file				MainMenu.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_MAINMENU
#define FOX_MAINMENU

#include "Sprite.h"
#include "../HeaderFiles/Sound.h"

void InitializeMainMenu(void);
void FreeMainMenu(void);
void DrawMainMenu(void);
static void InputHandling(void);
void UnloadMainMenu(void);
void LoadMainMenu(void);
void UpdateMainMenu(void);

void BackgroundAnimation(void);
void SetStartLocation(float *xPos, float *yPos, int startNum);
int RandomNewTexture(Sprite* CurrentSprite, int prevTexture);

struct FoxSound MenuBackSnd;

#endif