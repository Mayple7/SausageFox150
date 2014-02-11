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

enum TextAlignment { Left, Center, Right };

TextGlyphs* CreateText(char *string, float xPos, float yPos, int fontSize, Vec3 TextColor, int alignment);
Sprite* ConvertToGlyph(char character, int fontSize, float xPos, float yPos);
void FreeText(TextGlyphs *FirstLetter);
void ChangeTextPosition(TextGlyphs* FirstLetter, Vec2 Offset, int alignment);
void ChangeTextVisibility(TextGlyphs* FirstLetter);
void ChangeTextString(TextGlyphs* FirstLetter, char* newString);
void UpdateFloatingText(TextGlyphs *FirstLetter);

#endif