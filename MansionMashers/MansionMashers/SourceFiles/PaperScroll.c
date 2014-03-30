#include "../AEEngine.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/TextCreation.h"
#include "../HeaderFiles/PaperScroll.h"

static Sprite *TxtScrollRight;
static Sprite *TxtScrollLeft;
static Sprite *TxtScrollMiddle;

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

void ScrollPaperScroll(float Speed)
{
	TxtScrollMiddle->Visible = TRUE;
	TxtScrollRight->Visible = TRUE;
	TxtScrollLeft->Visible = TRUE;

	if(TxtScrollMiddle->ScaleX < 50)
	{
		TxtScrollMiddle->ScaleX += .40f * Speed;
		TxtScrollRight->Position.x += 10 * Speed;
	}
	else
	{
		Scroll = FALSE;
		ReScroll = TRUE;
	}
}

void ReScrollPaperScroll(float Speed)
{
	if(TxtScrollMiddle->ScaleX > 1)
	{
		TxtScrollMiddle->ScaleX -= .40f * Speed;
		TxtScrollRight->Position.x -= 10 * Speed;
	}

	if(TxtScrollMiddle->ScaleX <= 1)
	{
		TxtScrollMiddle->Visible = FALSE;
		TxtScrollRight->Visible = FALSE;
		TxtScrollLeft->Visible = FALSE;
		ReScroll = FALSE;
	}
}

void FadeScroll(void)
{
	TxtScrollMiddle->Alpha -= .05f;
	TxtScrollRight->Alpha -= .05f;
	TxtScrollLeft->Alpha -= .05f;
}

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

void UpdateSrollObjects(void)
{
	TxtScrollMiddle->Position.x = (GetCameraXPosition() - 650);
	//TxtScrollRight->Position.x =  TxtScrollRight->Position.x - GetCameraXPosition() - 635;
	TxtScrollLeft->Position.x = GetCameraXPosition() - 650;
}


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