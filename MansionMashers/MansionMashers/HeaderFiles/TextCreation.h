#ifndef FOX_TEXT
#define FOX_TEXT

#include "FoxEngine.h"
#include "FoxMath.h"
#include "Sprite.h"

typedef struct Text
{
	Sprite *Glyph;
	char letter;
	struct Text *NextLetter;
}Text;

Text* CreateText(char *string, float xPos, float yPos, int fontSize, Vec3 TextColor);
Sprite* ConvertToGlyph(char character, int fontSize, float xPos, float yPos);
void DrawGlyphs(Text *FirstLetter);
void FreeText(Text *FirstLetter);

#endif