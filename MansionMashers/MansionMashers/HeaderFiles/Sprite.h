#ifndef FOX_SPRITE
#define FOX_SPRITE

#include "Vector2.h"
#include "../HeaderFiles/CollisionManager.h"

enum spriteType{ Player, Enemy, Part };

typedef struct Sprite
{
	//Created (BOOL 1 or 0 only)
	int Created;
	int SpriteType;

	//Collision
	int CanCollide;
	int Ghost;
	int IsHit;
	int SensorType;

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

Sprite * CreateSprite(float width, float height, int xFrames, int yFrames, char* texture);
Sprite CreateAnimation(Sprite currentSprite, int verticalFrames, int horizontalFrames, int framesPerSecond);
void DrawSprite(Sprite *currentSprite);

#endif