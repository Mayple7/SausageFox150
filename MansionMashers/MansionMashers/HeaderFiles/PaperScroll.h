/*****************************************************************************/
/*!
\file				PaperScroll.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_PAPERSCROLL
#define FOX_PAPERSCROLL

#include "../HeaderFiles/TextCreation.h"


void CreatePaperScroll(float posx, float height);
void ScrollPaperScroll(float Speed);
void ReScrollPaperScroll(float Speed);
void FadeScroll(void);
void ResetScrollObjects(float posx);
void UpdateSrollObjects(void);
void SetUpScrollWithText(struct TextGlyphs* string, int* counter);

//Bool Vaulues
int Scroll;
int ReScroll;

#endif