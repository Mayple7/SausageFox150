/*****************************************************************************/
/*!
\file				UIButton.c
\author				Dan Muller (d.muller)
\date				Feb 16, 2014

\brief				UI Button functions and creation

\par				Functions:
\li					CreateButton
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes

#include "../AEEngine.h"
#include "../HeaderFiles/FoxMath.h"
#include "../HeaderFiles/FoxEngine.h"
#include "../HeaderFiles/UIButton.h"

// ---------------------------------------------------------------------------
// globals
//static int LogicTimer = 0;

/*************************************************************************/
/*!
	\brief
	Initializes and creates the enemy
	  
	\param CurrentEnemy
	A pointer to the enemy object to be initialized
*/
/*************************************************************************/
Button *CreateButton(char *texture, float xPos, float yPos, float width, float height, int objID)
{
	Button *CurrentButton = AddButton();

	if(CurrentButton)
	{
		Vec2Set(&CurrentButton->Position, xPos, yPos);
		CurrentButton->objID = objID;
		CurrentButton->ButtonSprite = (Sprite *)CreateSprite(texture, width, height, 30, 1, 1, xPos, yPos);
		CreateCollisionBox(&CurrentButton->ButtonCollider, &CurrentButton->Position, ButtonType, width, height, objID);

		return CurrentButton;
	}
	else
		return NULL;
}

/*************************************************************************/
/*!
	\brief
	Changes the button's position
*/
/*************************************************************************/
void UpdateButtonPosition(Button *CurrentButton, float PosX, float PosY)
{
	Vec2Set(&CurrentButton->Position, PosX, PosY);
	Vec2Set(&CurrentButton->ButtonSprite->Position, PosX, PosY);
}
