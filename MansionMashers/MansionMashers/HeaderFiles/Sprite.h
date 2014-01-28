#ifndef FOX_SPRITE
#define FOX_SPRITE

#include "Vector2.h"
#include "CollisionManager.h"

enum collisionGroup{ PlayerType, EnemyType, PartType, BackgroundType, FoodType, HudType, PlatformType, BounceType, ButtonType};

typedef struct Sprite
{
	//Created (BOOL 1 or 0 only)
	int Created;

	//Name
	char SpriteName[24];
	//Collision Group
	int CollisionGroup;

	//Collision
	int CanCollide;
	int Ghost;
	int SensorType;
	int CollideDebug;
	Vec2 CollideSize;
	Vec2 CollideOffset;

	//Sprite Graphics Properties
	AEGfxVertexList *SpriteMesh;
	AEGfxTexture *SpriteTexture;

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

	float Alpha;
	int Visible;
	int FlipX;
	int FlipY;
	int FlipXPrev;
	int FlipYPrev;
	float Rotation;
	float RotationPrev;
}Sprite;

Sprite* CreateSprite(char SpriteName[], char* texture, float width, float height, unsigned short ZIndex, int xFrames, int yFrames, int newGroup);
Sprite CreateAnimation(Sprite currentSprite, int verticalFrames, int horizontalFrames, int framesPerSecond);
void UpdateMesh(Sprite *currentSprite);
void DrawSprite(Sprite *currentSprite);

#endif