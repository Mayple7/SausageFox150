#include "../AEEngine.h"
#include "../HeaderFiles/Level1.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"

//Bounding Boxes
Sprite *BoundTop;
Sprite *BoundBottom;
Sprite *BoundLeft;
Sprite *BoundRight;

void CreateBoundingBoxes(void)
{
	Vec3 BoundingTint;

	Vec3Set(&BoundingTint, 0.0f, 0.0f, 0.0f);

	BoundTop = (Sprite *) CreateSprite("TextureFiles/BoundingBox.png", 1920, 1080, 5000, 1, 1, 0, 1080);
	BoundBottom = (Sprite *) CreateSprite("TextureFiles/BoundingBox.png", 1920, 1080, 5000, 1, 1, 0, -1080);
	BoundLeft = (Sprite *) CreateSprite("TextureFiles/BoundingBox.png", 1920, 1080, 5000, 1, 1, -1920, 0);
	BoundRight = (Sprite *) CreateSprite("TextureFiles/BoundingBox.png", 1920, 1080, 5000, 1, 1, 1920, 0);

	BoundTop->Tint = BoundingTint;
	BoundBottom->Tint = BoundingTint;
	BoundLeft->Tint = BoundingTint;
	BoundRight->Tint = BoundingTint;
}

void BoundingBoxUpdate(void)
{
	float camX, camY;	

	//Get camera position
	AEGfxGetCamPosition(&camX, &camY);


	BoundTop->Position.x = camX;
	BoundBottom->Position.x = camX;
	BoundRight->Position.x = camX + 1920 * GetLoadRatio();
	BoundLeft->Position.x = camX - 1920 * GetLoadRatio();

}

