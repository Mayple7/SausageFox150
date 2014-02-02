/*****************************************************************************/
/*!
\file				TextCreation.c
\author				Dan Muller (d.muller)
\date				Jan 31, 2014

\brief				Text creation and drawing functions

\par				Functions:
\li					CreateText

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include "../HeaderFiles/TextCreation.h"
#include <stdio.h>

Text* CreateText(char *string, float xPos, float yPos, int fontSize, Vec3 TextColor)
{
	Text *textString, *firstLetter, *nextLetter;
	int length, i, character;
	length = strlen(string);
	
	firstLetter = (Text *) calloc(1, sizeof(Text));
	// Uppercases the letters then creates a glyph
	for(i = 0; i < length; i++)
	{
		if(i != 0)
			nextLetter = (Text *) calloc(1, sizeof(Text));
		else
			nextLetter = firstLetter;

		if(string[i] >= 'a' && string[i] <= 'z')
			character = string[i] - 32;
		else
			character = string[i];
		
		nextLetter->letter = character;
		nextLetter->Glyph = ConvertToGlyph(character, fontSize);
		nextLetter->NextLetter = NULL;
		if(nextLetter->Glyph)
		{
			nextLetter->Glyph->Position.x = xPos + (i * fontSize * 0.45f);
			nextLetter->Glyph->Position.y = yPos;
			nextLetter->Glyph->Tint = TextColor;
			nextLetter->Glyph->isHUD = TRUE;
		}
		if(i != 0)
			textString->NextLetter = nextLetter;

		textString = nextLetter;
	}
	return firstLetter;
}

Sprite* ConvertToGlyph(char character, int fontSize)
{
	int frame = -1;
	Sprite *temp;

	if(character >= 'A' && character <= 'Z')
		frame = (int)character - 65;
	else if(character >= '0' && character <= '9')
		frame = (int)character - 22;
	else
	{
		switch(character)
		{
			case '-':
				frame = 36;
				break;
			case '+':
				frame = 37;
				break;
			case '!':
				frame = 38;
				break;
			case '.':
				frame = 39;
				break;
			case ',':
				frame = 40;
				break;
			case '%':
				frame = 41;
				break;
			case '(':
				frame = 42;
				break;
			case ')':
				frame = 43;
				break;
		}
	}
	if(frame >= 0)
	{
		temp = CreateSprite("TextureFiles/Rumple_TextSheet_White.png", ((float)fontSize * 92.0f / 100.0f), (float)fontSize, 101, 11, 4);
		temp->AnimationActive = FALSE;
		temp->CurrentFrame = frame;
		return temp;
	}
	else
		return NULL;

}

void DrawGlyphs(Text *FirstLetter)
{
	Text* nextLetter = FirstLetter;

	while(nextLetter)
	{
		if(nextLetter->Glyph)
			DrawSprite(nextLetter->Glyph);
		nextLetter = nextLetter->NextLetter;
	}
}
/*
void FreeText(Text *FirstLetter)
{

}*/
