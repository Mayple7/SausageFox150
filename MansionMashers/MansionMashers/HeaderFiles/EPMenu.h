/*****************************************************************************/
/*!
\file				EPMenu.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
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