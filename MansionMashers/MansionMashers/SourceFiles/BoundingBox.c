#include "../AEEngine.h"
#include "../HeaderFiles/Level1.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxObjects.h"
#include "../HeaderFiles/GameStateManager.h"
#include "../HeaderFiles/GameStateList.h"

//Bounding Boxes
static Sprite *BoundTop;
static Sprite *BoundBottom;
static Sprite *BoundLeft;
static Sprite *BoundRight;

static Wall *BBWallLeft;
static Wall *BBWallRight;

void CreateBoundingBoxes(void)
{
	Vec3 BoundingTint;

	Vec3Set(&BoundingTint, 0.0f, 0.0f, 0.0f);

	BoundTop = (Sprite *) CreateSprite("TextureFiles/BoundingBox.png", 1920, 1080, 5000, 1, 1, 0, 1080);
	BoundBottom = (Sprite *) CreateSprite("TextureFiles/BoundingBox.png", 1920, 1080, 5000, 1, 1, 0, -1080);
	BoundLeft = (Sprite *) CreateSprite("TextureFiles/BoundingBox.png", 1920, 2080, 5000, 1, 1, -1920, 0);
	BoundRight = (Sprite *) CreateSprite("TextureFiles/BoundingBox.png", 1920, 2080, 5000, 1, 1, 1920, 0);

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

	BoundTop->Position.x = camX / GetLoadRatio();
	BoundBottom->Position.x = camX / GetLoadRatio();
	BoundRight->Position.x = camX / GetLoadRatio() + 1920;
	BoundLeft->Position.x = camX / GetLoadRatio() - 1920;
}

void CreateBlockerBoxes(int *newID)
{
	// Bounding Box Walls
	BBWallLeft = CreateWall("TextureFiles/BlankPlatform.png", 200.0f, 1080.0f, (*newID)++, 0, 0);
	BBWallLeft->WallSprite->Visible = FALSE;
	BBWallLeft->enemyNotCollidable = TRUE;
	BBWallRight = CreateWall("TextureFiles/BlankPlatform.png", 200.0f, 1080.0f, (*newID)++, 0, 0);
	BBWallRight->WallSprite->Visible = FALSE;
	BBWallRight->enemyNotCollidable = TRUE;
}

void UpdateBlockerBoxes(float panelsize)
{
	BBWallLeft->Position.y = -1080.0f + 1080.0f * GetCameraLockState();
	BBWallLeft->Position.x = GetCameraXPosition() - (panelsize / 2);
	UpdateCollisionPosition(&BBWallLeft->WallCollider, &BBWallLeft->Position);
	BBWallRight->Position.y = -1080.0f + 1080.0f * GetCameraLockState();
	BBWallRight->Position.x = GetCameraXPosition() + (panelsize / 2);
	UpdateCollisionPosition(&BBWallRight->WallCollider, &BBWallRight->Position);
}