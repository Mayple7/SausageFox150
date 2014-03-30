#ifndef FOX_PAPERSCROLL
#define FOX_PAPERSCROLL

void CreatePaperScroll(float posx, float height);
void ScrollPaperScroll(float Speed);
void ReScrollPaperScroll(float Speed);
void FadeScroll(void);
void ResetScrollObjects(float posx);
void UpdateSrollObjects(void);

//Bool Vaulues
int Scroll;
int ReScroll;

#endif