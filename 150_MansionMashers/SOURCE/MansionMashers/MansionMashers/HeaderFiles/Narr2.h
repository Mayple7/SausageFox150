/*****************************************************************************/
/*!
\file				Narr2.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
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