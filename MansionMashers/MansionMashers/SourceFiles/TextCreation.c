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
TextGlyphs* CreateText(char *string, float xPos, float yPos, int fontSize, Vec3 TextColor, int alignment, int textSheet)
{
	TextGlyphs *textString, *firstLetter, *nextLetter;
	int length, i;
	float firstXPos;
	length = strlen(string);

	if(alignment == Center)
	{
		firstXPos = xPos - (length * fontSize * 0.4f) / 2;
	}
	else if(alignment == Right)
	{
		firstXPos = xPos - (length * fontSize * 0.4f);
	}
	else
	{
		firstXPos = xPos;
	}

	firstLetter = (TextGlyphs *) CallocMyAlloc(1, sizeof(TextGlyphs));
	AddStaticText(firstLetter);
	//Uppercases the letters then creates a glyph
	for(i = 0; i < length; i++)
	{
		if(i != 0)
			nextLetter = (TextGlyphs *) CallocMyAlloc(1, sizeof(TextGlyphs));
		else
			nextLetter = firstLetter;
		
		nextLetter->letter = string[i];
		nextLetter->textTexture = textSheet;
		nextLetter->Glyph = ConvertToGlyph(string[i], fontSize, firstXPos + (i * fontSize * 0.4f), yPos, textSheet);
		nextLetter->NextLetter = NULL;
		if(nextLetter->Glyph)
		{
			nextLetter->Glyph->Tint = TextColor;
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
Sprite* ConvertToGlyph(char character, int fontSize, float xPos, float yPos, int textSheet)
{
	int frame = -1;
	Sprite *temp;

	if(character >= 'A' && character <= 'Z')
		frame = (int)character - 65;
	else if(character >= '0' && character <= '9')
		frame = (int)character - 22;
	else if(character >= 'a' && character <= 'z')
		frame = (int)character - 97;
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
		switch(textSheet)
		{
		case Plain:
			temp = (Sprite *) CreateSprite("TextureFiles/Rumple_TextSheet_White.png", ((float)fontSize * 92.0f / 100.0f), (float)fontSize, 11, 11, 4, xPos, yPos);
			break;
		case Border:
			temp = (Sprite *) CreateSprite("TextureFiles/Rumple_TextSheet_Border.png", ((float)fontSize * 92.0f / 100.0f), (float)fontSize, 11, 11, 4, xPos, yPos);
			break;
		}

		
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
	Frees the entire linked list of Text objects
	
	\param FirstLetter
	A pointer to the first letter in the text.
*/
/*************************************************************************/
void FreeText(TextGlyphs *FirstLetter)
{
	int i;
	TextGlyphs *NextLetter = FirstLetter;

	//Find the pointer in the list that is the first letter
	for(i = 0; i < COLLIDEAMOUNT; i++)
		if (staticTextList[i] == FirstLetter)
			break;

	//Go through all the letters and free my people
	while(NextLetter)
	{
		NextLetter = FirstLetter->NextLetter;
		if(FirstLetter->Glyph)
			FreeSprite(FirstLetter->Glyph);
		FreeMyAlloc(FirstLetter);
		FirstLetter = NextLetter;
	}

	//Now remove that pointer.
	if(i < COLLIDEAMOUNT)
		staticTextList[i] = NULL;
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
void ChangeTextPosition(TextGlyphs* FirstLetter, Vec2 newPosition, int alignment)
{
	TextGlyphs* nextLetter = FirstLetter;
	int i = 0;
	while(nextLetter)
	{
		nextLetter = nextLetter->NextLetter;
		i++;
	}


	if(alignment == Center)
	{
		newPosition.x = newPosition.x - (i * FirstLetter->Glyph->Height * 0.4f) / 2;
	}
	else if(alignment == Right)
	{
		newPosition.x = newPosition.x - (i * FirstLetter->Glyph->Height * 0.4f);
	}

	i = 0;
	nextLetter = FirstLetter;
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

/*************************************************************************/
/*!
	\brief
	Changes the Zindex of the text string
	
	\param FirstLetter
	A pointer to the first letter in the text.

	\param newIndex
	The new Zindex for the text
*/
/*************************************************************************/
void ChangeTextZIndex(TextGlyphs* FirstLetter, int newIndex)
{
	TextGlyphs* nextLetter = FirstLetter;

	while(nextLetter)
	{
		if(nextLetter->Glyph)
			nextLetter->Glyph->ZIndex = newIndex;
		nextLetter = nextLetter->NextLetter;
	}
}

/*************************************************************************/
/*!
	\brief
	Changes the letters in the text
	
	\param FirstLetter
	A pointer to the first letter in the text.

	\param newString
	The string to create text for
*/
/*************************************************************************/
void ChangeTextString(TextGlyphs* FirstLetter, char* newString)
{
	TextGlyphs* nextLetter = FirstLetter;
	TextGlyphs* prevLetter = FirstLetter;
	int i = 0;
	while(*newString)
	{
		if(nextLetter && nextLetter->Glyph)
		{
			nextLetter->letter = *newString;
			if(*newString >= 'A' && *newString <= 'Z')
				nextLetter->Glyph->CurrentFrame = (int)*newString - 65;
			else if(*newString >= '0' && *newString <= '9')
				nextLetter->Glyph->CurrentFrame = (int)*newString - 22;
			else if(*newString >= 'a' && *newString <= 'z')
				nextLetter->Glyph->CurrentFrame = (int)*newString - 97;
			else
			{
				switch(*newString)
				{
					case '-':
						nextLetter->Glyph->CurrentFrame = 36;
						break;
					case '+':
						nextLetter->Glyph->CurrentFrame = 37;
						break;
					case '!':
						nextLetter->Glyph->CurrentFrame = 38;
						break;
					case '.':
						nextLetter->Glyph->CurrentFrame = 39;
						break;
					case ',':
						nextLetter->Glyph->CurrentFrame = 40;
						break;
					case '%':
						nextLetter->Glyph->CurrentFrame = 41;
						break;
					case '(':
						nextLetter->Glyph->CurrentFrame = 42;
						break;
					case ')':
						nextLetter->Glyph->CurrentFrame = 43;
						break;
					default:
						FreeSprite(nextLetter->Glyph);
						nextLetter->Glyph = NULL;
						break;
				}
			}
		}
		else if(nextLetter)
		{
			nextLetter->letter = *newString;
			nextLetter->Glyph = ConvertToGlyph(*newString, (int)(FirstLetter->Glyph->Height), FirstLetter->Glyph->Position.x + (i * FirstLetter->Glyph->Height * 0.4f), FirstLetter->Glyph->Position.y, FirstLetter->textTexture);
		}
		else
		{
			nextLetter = (TextGlyphs *) CallocMyAlloc(1, sizeof(TextGlyphs));
			
			prevLetter->NextLetter = nextLetter;
			nextLetter->letter = *newString;
			nextLetter->Glyph = ConvertToGlyph(*newString, (int)(FirstLetter->Glyph->Height), FirstLetter->Glyph->Position.x + (i * FirstLetter->Glyph->Height * 0.4f), FirstLetter->Glyph->Position.y, FirstLetter->textTexture);
			nextLetter->NextLetter = NULL;
			if(nextLetter->Glyph)
				nextLetter->Glyph->ZIndex = FirstLetter->Glyph->ZIndex;
		}
		if(nextLetter->Glyph)
		{
			nextLetter->Glyph->Position.x = FirstLetter->Glyph->Position.x + (i * nextLetter->Glyph->Height * 0.4f);
			nextLetter->Glyph->Tint = FirstLetter->Glyph->Tint;
			nextLetter->Glyph->Visible = FirstLetter->Glyph->Visible;
			nextLetter->Glyph->AnimationActive = FALSE;
		}
		i++;
		prevLetter = nextLetter;
		nextLetter = nextLetter->NextLetter;
		*(newString++);
	}
	if(nextLetter)
	{
		prevLetter->NextLetter = NULL;
		FreeText(nextLetter);
	}
}

/*************************************************************************/
/*!
	\brief
	Updates the floating text
*/
/*************************************************************************/
void UpdateFloatingText(void)
{
	int i = 0;

	while(i < FLOATINGTEXTAMOUNT)
	{
		if(floatTextList[i] > 0)
			AnimateFloatingText(floatTextList[i]);
		i++;
	}
}

/*************************************************************************/
/*!
	\brief
	Animates the floating text
	
	\param FirstLetter
	A pointer to the first letter in the text.
*/
/*************************************************************************/
void AnimateFloatingText(TextGlyphs *FirstLetter)
{
	TextGlyphs *nextLetter;
	nextLetter = FirstLetter;

	if(nextLetter && nextLetter->Glyph && nextLetter->Glyph->Alpha <= 0.0f)
	{
		FreeFloatingText(FirstLetter);
		return;
	}

	while(nextLetter)
	{
		if(nextLetter->letter == ' ')
		{
			nextLetter = nextLetter->NextLetter;
			continue;
		}
		if(!(nextLetter->Glyph))
			break;
		if(nextLetter->Glyph->Alpha > 0.9f)
		{
			nextLetter->Glyph->Alpha -= 0.06f * GetDeltaTime();
			nextLetter->Glyph->Position.y += 90.0f * GetDeltaTime();
		}
		else if(nextLetter->Glyph->Alpha > 0.3f)
		{
			nextLetter->Glyph->Alpha -= 1.2f * GetDeltaTime();
			nextLetter->Glyph->Position.y += 90.0f * GetDeltaTime();
		}
		else
		{
			nextLetter->Glyph->Alpha -= 2.4f * GetDeltaTime();
			nextLetter->Glyph->Position.y += 90.0f * GetDeltaTime();
		}
		nextLetter = nextLetter->NextLetter;
	}
}

/*************************************************************************/
/*!
	\brief
	Makes all the text visible
	
	\param FirstLetter
	A pointer to the first letter in the text.
*/
/*************************************************************************/
void TextAllVisible(TextGlyphs* FirstLetter)
{
	TextGlyphs* nextLetter = FirstLetter;

	while(nextLetter)
	{
		if(nextLetter->Glyph)
			nextLetter->Glyph->Visible = TRUE;
		nextLetter = nextLetter->NextLetter;
	}
}

/*************************************************************************/
/*!
	\brief
	Makes all the text invisible
	
	\param FirstLetter
	A pointer to the first letter in the text.
*/
/*************************************************************************/
void TextAllNotVisible(TextGlyphs* FirstLetter)
{
	TextGlyphs* nextLetter = FirstLetter;

	while(nextLetter)
	{
		if(nextLetter->Glyph)
			nextLetter->Glyph->Visible = FALSE;
		nextLetter = nextLetter->NextLetter;
	}
}

/*************************************************************************/
/*!
	\brief
	Initializes text for progressive visibility
	
	\param FirstLetter
	A pointer to the first letter in the text.
*/
/*************************************************************************/
void TextProgressiveInit(TextGlyphs* FirstLetter)
{
	TextGlyphs* nextLetter = FirstLetter;

	while(nextLetter)
	{
		if(nextLetter->Glyph)
		{
			nextLetter->Glyph->Visible = TRUE;
			nextLetter->Glyph->Alpha = 0.0f;
		}
		nextLetter = nextLetter->NextLetter;
	}

}

/*************************************************************************/
/*!
	\brief
	Makes all the text invisible
	
	\param FirstLetter
	A pointer to the first letter in the text.

	\param fadeinSpeed
	How many frames for a glyph to go from fully invisible to fully visible
*/
/*************************************************************************/
void TextProgressiveVisible(TextGlyphs* FirstLetter, int fadeinSpeed)
{
	TextGlyphs* nextLetter = FirstLetter;

	while(nextLetter)
	{
		if(nextLetter->Glyph && nextLetter->Glyph->Alpha >= 1.0f)
		{
			nextLetter->Glyph->Alpha = 1.0f;
			nextLetter = nextLetter->NextLetter;
		}
		else if(nextLetter->Glyph)
		{
			nextLetter->Glyph->Alpha += 1.0f / fadeinSpeed;
			return;
		}
		else
		{
			nextLetter = nextLetter->NextLetter;
		}
	}
}

/*************************************************************************/
/*!
	\brief
	Initializes text for progressive visibility
	
	\param FirstLetter
	A pointer to the first letter in the text.
*/
/*************************************************************************/
void TextProgressiveEnd(TextGlyphs* FirstLetter)
{
	TextGlyphs* nextLetter = FirstLetter;

	while(nextLetter)
	{
		if(nextLetter->Glyph)
		{
			nextLetter->Glyph->Visible = TRUE;
			nextLetter->Glyph->Alpha = 1.0f;
		}
		nextLetter = nextLetter->NextLetter;
	}

}
