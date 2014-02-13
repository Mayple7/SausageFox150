#ifndef FOX_SPRITE
#define FOX_SPRITE

#include "../AEEngine.h"
#include "Vector2.h"
#include "Vector3.h"
#include "TextureManager.h"
#include "CollisionManager.h"

typedef struct Sprite
{
	//Created (BOOL 1 or 0 only)
	int Created;

	//Name
	char SpriteName[24];
	
	//Collision Group
	int isHUD;

	//Sprite Graphics Properties
	AEGfxVertexList *SpriteMesh;
	AEGfxTexture *SpriteTexture;

	int MeshOwner;

	// Size of the sprite
	float Width;
	float Height;

	//Position of the sprite
	Vec2 Position;
	unsigned short ZIndex;

	//Animation Properties
	int AnimationActive;
	int CurrentFrame;
	int TotalFrames;
	float AnimationSpeed;
	int AnimationTimer;
	float OffsetX;
	float OffsetY;

	int ItemType;
	//Texture Properties
	char TextureName[256];
	int NumHeightFrames;
	int NumWidthFrames;

	Vec3 Tint;

	float Alpha;
	int Visible;
	int FlipX;
	int FlipY;
	int FlipXPrev;
	int FlipYPrev;
	float Rotation;
	float RotationPrev;

	float ScaleX;
	float ScaleY;

}Sprite;

Sprite* CreateSprite(char texture[], float width, float height, unsigned short ZIndex, int xFrames, int yFrames, float xPos, float yPos);
Sprite* CreateSpriteNoMesh(char texture[], float width, float height, unsigned short ZIndex, int xFrames, int yFrames, float xPos, float yPos);
Sprite CreateAnimation(Sprite currentSprite, int verticalFrames, int horizontalFrames, int framesPerSecond);
Matrix3 CreateTranslationMtx(struct Sprite *CurrentSprite);
AEGfxVertexList* createMesh(float width, float height, float offsetX, float offsetY);
void UpdateMesh(Sprite *currentSprite);
void DrawSprite(Sprite *currentSprite);

#endif