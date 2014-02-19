#ifndef FOX_UIBUTTON
#define FOX_UIBUTTON

#include "Vector2.h"
#include "Sprite.h"
#include "CollisionBox.h"

typedef struct Button
{
	Sprite *ButtonSprite;
	CollisionBox ButtonCollider;

	int objID;
	Vec2 Position;

}Button;

Button* CreateButton(char* texture, float xPos, float yPos, float width, float height, int objID);

#endif