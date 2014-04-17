/*****************************************************************************/
/*!
\file				PaperScroll.c
\author				Juli Gregg (j.gregg)	
\date				Mar 8, 2014

\brief				Functions

\par				Functions:
					- CreatePaperScroll
					- ScrollPaperScroll
					- RescrollPaperScroll
					- FadeScroll
					- ResetScrollObjects
					- UpdateScrollObjects
					- SetUpScrollWithText
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

#include "../AEEngine.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/TextCreation.h"
#include "../HeaderFiles/PaperScroll.h"

static Sprite *TxtScrollRight;
static Sprite *TxtScrollLeft;
static Sprite *TxtScrollMiddle;

/*************************************************************************/
/*!
	\brief
	Sets up the scroll objects

	\param posx
	X Position to set

	\param height
	Y position to set
*/
/*************************************************************************/
void CreatePaperScroll(float posx, float height)
{

	TxtScrollMiddle = (Sprite*) CreateSprite("TextureFiles/scrollbar.png", 50, 400, 400, 1, 1, posx - 650, height);
	TxtScrollRight = (Sprite*) CreateSprite("TextureFiles/rightscroll.png", 300, 400, 400, 1, 1, posx - 635, height);
	TxtScrollLeft = (Sprite*) CreateSprite("TextureFiles/leftscroll.png", 300, 400, 400, 1, 1, posx - 650, height);

	TxtScrollRight->Visible = FALSE;
	TxtScrollMiddle->Visible = FALSE;
	TxtScrollLeft->Visible = FALSE;

	Scroll = TRUE;
	ReScroll = FALSE;
}

/*************************************************************************/
/*!
	\brief
	Scrolls/Expands scroll

	\param Speed
	Mulitplier to set speed at which scroll expands
*/
/*************************************************************************/
void ScrollPaperScroll(float Speed)
{
	TxtScrollMiddle->Visible = TRUE;
	TxtScrollRight->Visible = TRUE;
	TxtScrollLeft->Visible = TRUE;

	//Scale and move over scroll
	if(TxtScrollMiddle->ScaleX < 50)
	{
		TxtScrollMiddle->ScaleX += .40f * Speed;
		TxtScrollRight->Position.x += 10 * Speed;
	}
	//When done set bools
	else
	{
		Scroll = FALSE;
		ReScroll = TRUE;
	}
}

/*************************************************************************/
/*!
	\brief
	ReScrolls/UnExpands scroll

	\param Speed
	Mulitplier to set speed at which scroll recedes
*/
/*************************************************************************/
void ReScrollPaperScroll(float Speed)
{
	//Scale and move over objects if needed
	if(TxtScrollMiddle->ScaleX > 1)
	{
		TxtScrollMiddle->ScaleX -= .40f * Speed;
		TxtScrollRight->Position.x -= 10 * Speed;
	}

	//At certain point make objects invisible
	if(TxtScrollMiddle->ScaleX <= 1)
	{
		TxtScrollMiddle->Visible = FALSE;
		TxtScrollRight->Visible = FALSE;
		TxtScrollLeft->Visible = FALSE;
		ReScroll = FALSE;
	}
}

/*************************************************************************/
/*!
	\brief
	Fades scroll out
*/
/*************************************************************************/
void FadeScroll(void)
{
	//Decrease alpha each game loop
	TxtScrollMiddle->Alpha -= .05f;
	TxtScrollRight->Alpha -= .05f;
	TxtScrollLeft->Alpha -= .05f;
}

/*************************************************************************/
/*!
	\brief
	Resets objects to original states

	\param posX
	Reset x position to
*/
/*************************************************************************/
void ResetScrollObjects(float posx)
{
	TxtScrollMiddle->ScaleX = 1;
	Scroll = TRUE;
	ReScroll = FALSE;
	TxtScrollMiddle->Alpha = 1.0f;
	TxtScrollRight->Alpha = 1.0f;
	TxtScrollLeft->Alpha = 1.0f;
	TxtScrollMiddle->Position.x = posx - 650;
	TxtScrollRight->Position.x = posx - 635;
	TxtScrollLeft->Position.x = posx - 650;
}

/*************************************************************************/
/*!
	\brief
	Updates scroll objects to move with camera
*/
/*************************************************************************/
void UpdateSrollObjects(void)
{
	TxtScrollMiddle->Position.x = (GetCameraXPosition() - 650);
	//TxtScrollRight->Position.x =  TxtScrollRight->Position.x - GetCameraXPosition() - 635;
	TxtScrollLeft->Position.x = GetCameraXPosition() - 650;
}

/*************************************************************************/
/*!
	\brief
	Expands Scroll and places text on top of it

	\param string
	String to print with text

	\param counter
	Pointer to a counter for how long until scroll fades
*/
/*************************************************************************/
void SetUpScrollWithText(TextGlyphs* string, int* counter)
{
	//Can scroll so scroll
	if(Scroll == TRUE)
	{
		ScrollPaperScroll(2);
	}
	//Scroll is out so roll out that text 
	//but not if were trying to get rid of text
	else if(!GetTextToDisappear())
		TextProgressiveVisible(string, 2);
	//Time to rescroll and the text is up
	if(ReScroll == TRUE && !GetTextInProgress())
	{
		//wait a bit people need to read that stuff
		if(*counter > 0)
			*counter -= 1;
		//get rid of that text and scroll
		else
		{
			TextProgressiveDisappear(string, 1);
			FadeScroll();
		}
	}
}
