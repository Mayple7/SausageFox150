typedef struct Sprite
{
	//Bools
	int Created;
	int CanCollide;

	//Sprite Graphics Properties
	AEGfxVertexList *SpriteMesh;
	AEGfxTexture *SpriteTexture;

	// Size of the sprite
	float Width;
	float Height;

	//Position of the sprite
	float XPosition;
	float YPosition;

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
}Sprite;

void CreateSprite(Sprite *CurrentSprite, float width, float height, int xFrames, int yFrames, char* texture);
Sprite CreateAnimation(Sprite currentSprite, int verticalFrames, int horizontalFrames, int framesPerSecond);
void DrawSprite(Sprite *currentSprite);