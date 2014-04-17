/*****************************************************************************/
/*!
\file				UIButton.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
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

Button *CreateButton(char* texture, float xPos, float yPos, float width, float height);
void UpdateButtonPosition(Button *CurrentButton, float PosX, float PosY);

#endif
