/*****************************************************************************/
/*!
\file				Credits.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_CREDITS
#define FOX_CREDITS

void LoadCredits(void);
void InitializeCredits(void);
void UpdateCredits(void);
void DrawCredits(void);
void FreeCredits(void);
void UnloadCredits(void);

static void EventLevel(void);

#endif