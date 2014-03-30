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