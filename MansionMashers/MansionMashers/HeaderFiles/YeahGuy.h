/*****************************************************************************/
/*!
\file				YeahGuy.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_YEAHGUY_LEVEL
#define FOX_YEAHGUY_LEVEL

void LoadYeahGuy(void);
void InitializeYeahGuy(void);
void UpdateYeahGuy(void);
void DrawYeahGuy(void);
void FreeYeahGuy(void);
void UnloadYeahGuy(void);

void EventYeahGuy(void);
void ShowBuffs(void);
void HideBuffs(void);
void UpdatePlayerBuff(void);
void MoveHPBars(void);

#endif