#ifndef FOX_TEXT
#define FOX_TEXT

#include "FoxEngine.h"
#include "FoxMath.h"
#include "Sprite.h"

typedef struct TextGlyphs
{
	Sprite *Glyph;
	char letter;
	struct TextGlyphs *NextLetter;
}TextGlyphs;

TextGlyphs* CreateText(char *string, float xPos, float yPos, int fontSize, Vec3 TextColor);
Sprite* ConvertToGlyph(char character, int fontSize, float xPos, float yPos);
void DrawGlyphs(TextGlyphs *FirstLetter);
void FreeText(TextGlyphs *FirstLetter);
void ChangeTextPosition(TextGlyphs* FirstLetter, Vec2 Offset);
void ChangeTextVisibility(TextGlyphs* FirstLetter);
void ChangeTextString(TextGlyphs* FirstLetter, char* newString);

#endif