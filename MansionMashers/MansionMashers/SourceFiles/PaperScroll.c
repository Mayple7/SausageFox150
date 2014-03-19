#include "../AEEngine.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxObjects.h"

static Sprite *TxtScrollRight;
static Sprite *TxtScrollLeft;
static Sprite *TxtScrollMiddle;

void CreatePaperScroll(float height)
{
	TxtScrollMiddle = (Sprite*) CreateSprite("TextureFiles/scrollbar.png", 50, 400, 400, 1, 1, -650, height);
	TxtScrollMiddle->Visible = FALSE;

	TxtScrollRight = (Sprite*) CreateSprite("TextureFiles/rightscroll.png", 300, 400, 400, 1, 1, -625, height);
	TxtScrollRight->Visible = FALSE;

	TxtScrollLeft = (Sprite*) CreateSprite("TextureFiles/leftscroll.png", 300, 400, 400, 1, 1, -650, height);
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
