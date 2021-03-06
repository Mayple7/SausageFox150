/*****************************************************************************/
/*!
\file				PauseMenu.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_PAUSE
#define FOX_PAUSE

void LoadPause(void);
void InitializePause(void (*DrawLevel)());
void UpdatePause(void);
void DrawPause(void);
void FreePause(void);
void UnloadPause(void);
void EventPause(void);

#endif