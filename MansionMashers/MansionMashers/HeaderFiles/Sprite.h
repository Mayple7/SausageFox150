AEGfxVertexList* CreateSpriteTexture(float width, float height);
int UpdateFrame(int totalFrames, int currentFrame, int frameUpdate, float *offsetX, float *offsetY);

struct Sprite
{
	//Sprite Graphics Properties
	AEGfxVertexList* SpriteMesh;
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

	//Texture Properties
	char TextureName[256];
	int NumHeightFrames;
	int NumWidthFrames;

	float Alpha;
	int Visible;
	int FlipX;
	int FlipY;
};

void CreateSprite(struct Sprite *CurrentSprite, float width, float height, char* texture);
struct Sprite CreateAnimation(struct Sprite currentSprite, int verticalFrames, int horizontalFrames, int framesPerSecond);
void DrawSprite(struct Sprite currentSprite);