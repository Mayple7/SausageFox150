#ifndef FOX_TEXT
#define FOX_TEXT

#include "FoxEngine.h"
#include "FoxMath.h"
#include "Sprite.h"

typedef struct TextGlyphs
{
	Sprite *Glyph;
	int textTexture;
	char letter;
	struct TextGlyphs *NextLetter;
}TextGlyphs;

enum TextAlignment { Left, Center, Right };
enum TextTexture { Plain, Border };

TextGlyphs* CreateText(char *string, float xPos, float yPos, int fontSize, Vec3 TextColor, int alignment, int textSheet);
Sprite* ConvertToGlyph(char character, int fontSize, float xPos, float yPos, int textSheet);
void FreeText(TextGlyphs *FirstLetter);

void ChangeTextPosition(TextGlyphs* FirstLetter, Vec2 Offset, int alignment);
void ChangeTextVisibility(TextGlyphs* FirstLetter);
void ChangeTextString(TextGlyphs* FirstLetter, char* newString);

void UpdateFloatingText(void);
void AnimateFloatingText(TextGlyphs *FirstLetter);

void ChangeTextZIndex(TextGlyphs* FirstLetter, int newIndex);
void TextAllVisible(TextGlyphs* FirstLetter);
void TextAllNotVisible(TextGlyphs* FirstLetter);

void TextProgressiveInit(TextGlyphs* FirstLetter);
void TextProgressiveVisible(TextGlyphs* FirstLetter, int fadeinSpeed);
void TextProgressiveEnd(TextGlyphs* FirstLetter);

void TextDisappearInit(TextGlyphs* FirstLetter);
void TextProgressiveDisappear(TextGlyphs* FirstLetter, int fadeinSpeed);
void TextDisappearEnd(TextGlyphs* FirstLetter);

int GetTextInProgress(void);
int GetTextToDisappear(void);

#endif