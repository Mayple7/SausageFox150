/*****************************************************************************/
/*!
\file				TemplateLevel.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_LEVEL
#define FOX_LEVEL

void LoadLevel(void);
void InitializeLevel(void);
void UpdateLevel(void);
void DrawLevel(void);
void FreeLevel(void);
void UnloadLevel(void);

static void EventLevel(void);

#endif