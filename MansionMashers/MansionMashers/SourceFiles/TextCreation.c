/*****************************************************************************/
/*!
\file				TextCreation.c
\author				Dan Muller (d.muller)
\date				Jan 31, 2014

\brief				Text creation and drawing functions

\par				Functions:
\li					CreateText
\li					ConvertToGlyph
\li					DrawGlyphs
\li					FreeText

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

/*************************************************************************/
/*!
	\brief
	Creates the string of text and sprites in the correct positions
	
	\param string
	The string to make text of

	\param xPos
	The starting X position of the first letter

	\param yPos
	The starting Y position of the first letter

	\param fontSize
	The size of the text

	\param TextColor
	Vec3 of the text color to create

	\return
	Returns a pointer to the first element in the text linked list
*/
/*************************************************************************/
TextGlyphs* CreateText(char *string, float xPos, float yPos, int fontSize, Vec3 TextColor)
{
	TextGlyphs *textString, *firstLetter, *nextLetter;
	int length, i, character;
	length = strlen(string);
	
	firstLetter = (TextGlyphs *) CallocMyAlloc(1, sizeof(TextGlyphs));
	// Uppercases the letters then creates a glyph
	for(i = 0; i < length; i++)
	{
		if(i != 0)
			nextLetter = (TextGlyphs *) CallocMyAlloc(1, sizeof(TextGlyphs));
		else
			nextLetter = firstLetter;

		if(string[i] >= 'a' && string[i] <= 'z')
			character = string[i] - 32;
		else
			character = string[i];
		
		nextLetter->letter = character;
		nextLetter->Glyph = ConvertToGlyph(character, fontSize, xPos + (i * fontSize * 0.4f), yPos);
		nextLetter->NextLetter = NULL;
		if(nextLetter->Glyph)
		{
			nextLetter->Glyph->Tint = TextColor;
			//nextLetter->Glyph->isHUD = TRUE;
		}
		if(i != 0)
			textString->NextLetter = nextLetter;

		textString = nextLetter;
	}
	return firstLetter;
}

/*************************************************************************/
/*!
	\brief
	Converts a character in the string to a sprite glyph
	
	\param character
	The letter to make a glyph of

	\param fontsize
	The size of the letters
*/
/*************************************************************************/
Sprite* ConvertToGlyph(char character, int fontSize, float xPos, float yPos)
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
		temp = CreateSprite("TextureFiles/Rumple_TextSheet_White.png", ((float)fontSize * 92.0f / 100.0f), (float)fontSize, 101, 11, 4, xPos, yPos);
		temp->Visible = FALSE;
		temp->AnimationActive = FALSE;
		temp->CurrentFrame = frame;
		return temp;
	}
	else
		return NULL;

}

/*************************************************************************/
/*!
	\brief
	Draws the list of glyphs to the screen
	
	\param FirstLetter
	A pointer to the first letter in the text.
*/
/*************************************************************************/
void DrawGlyphs(TextGlyphs *FirstLetter)
{
	TextGlyphs* nextLetter = FirstLetter;

	while(nextLetter)
	{
		if(nextLetter->Glyph)
			DrawSprite(nextLetter->Glyph);
		nextLetter = nextLetter->NextLetter;
	}
}

/*************************************************************************/
/*!
	\brief
	Frees the entire linked list of Text objects
	
	\param FirstLetter
	A pointer to the first letter in the text.
*/
/*************************************************************************/
void FreeText(TextGlyphs *FirstLetter)
{
	TextGlyphs *NextLetter;

	while(FirstLetter)
	{
		NextLetter = FirstLetter->NextLetter;
		FreeMyAlloc(FirstLetter);
		FirstLetter = NextLetter;
	}
}

/*************************************************************************/
/*!
	\brief
	Changes the position of the text
	
	\param firstLetter
	A pointer to the first letter in the text.

	\param newPosition
	New position to move the text to.
*/
/*************************************************************************/
void ChangeTextPosition(TextGlyphs* FirstLetter, Vec2 newPosition)
{
	TextGlyphs* nextLetter = FirstLetter;
	int i = 0;

	while(nextLetter)
	{
		if(nextLetter->Glyph)
			Vec2Set(&nextLetter->Glyph->Position, newPosition.x + (i * nextLetter->Glyph->Height * 0.4f), newPosition.y);
		nextLetter = nextLetter->NextLetter;
		i++;
	}
}

/*************************************************************************/
/*!
	\brief
	Frees the entire linked list of Text objects
	
	\param FirstLetter
	A pointer to the first letter in the text.
*/
/*************************************************************************/
void ChangeTextVisibility(TextGlyphs* FirstLetter)
{
	TextGlyphs* nextLetter = FirstLetter;

	while(nextLetter)
	{
		if(nextLetter->Glyph)
			nextLetter->Glyph->Visible = !(nextLetter->Glyph->Visible);
		nextLetter = nextLetter->NextLetter;
	}
}
