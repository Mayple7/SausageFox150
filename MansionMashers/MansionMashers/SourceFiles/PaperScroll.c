#include "../AEEngine.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxObjects.h"

static Sprite *TxtScrollRight;
static Sprite *TxtScrollLeft;
static Sprite *TxtScrollMiddle;

void CreatePaperScroll(float height)
{
	TxtScrollMiddle = (Sprite*) CreateSprite("TextureFiles/scrollbar.png", 50, 400, 400, 1, 1, -780, height);
	TxtScrollMiddle->Visible = FALSE;

	TxtScrollRight = (Sprite*) CreateSprite("TextureFiles/rightscroll.png", 300, 400, 400, 1, 1, -760, height);
	TxtScrollRight->Visible = FALSE;

	TxtScrollLeft = (Sprite*) CreateSprite("TextureFiles/leftscroll.png", 300, 400, 400, 1, 1, -800, height);
	TxtScrollLeft->Visible = FALSE;
}

void ScrollPaperScroll(void)
{
	TxtScrollMiddle->Visible = TRUE;
	TxtScrollRight->Visible = TRUE;
	TxtScrollLeft->Visible = TRUE;

	if(TxtScrollMiddle->ScaleX < 30)
	{
		TxtScrollMiddle->ScaleX += .40f;
		TxtScrollMiddle->Position.x += 10 * GetLoadRatio();
		TxtScrollRight->Position.x += 20 * GetLoadRatio();
	}
}