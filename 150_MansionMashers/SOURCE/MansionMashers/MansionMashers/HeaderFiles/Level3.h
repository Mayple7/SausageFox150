/*****************************************************************************/
/*!
\file				Level3.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_LEVEL3
#define FOX_LEVEL3

#include "../HeaderFiles/Sound.h"

void LoadLevel3(void);
void InitializeLevel3(void);
void UpdateLevel3(void);
void DrawLevel3(void);
void FreeLevel3(void);
void UnloadLevel3(void);

void EventLevel3(void);

struct FoxSound Level3BackSnd;

#endif