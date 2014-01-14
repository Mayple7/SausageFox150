#include "../HeaderFiles/Movement.h"
#include "../AEEngine.h"
#include "../HeaderFiles/Sprite.h"

void JG_move(int key, Sprite *obj, float move_dist, int direction)
{

	// Handling Input
	AEInputUpdate();

	switch (direction)
	{
		case LEFT:
			if(AEInputCheckCurr(key))
				obj->XPosition -= move_dist;
			break;
		case RIGHT:
			if(AEInputCheckCurr(key))
				obj->XPosition += move_dist;
			break;
		case UP:
			if(AEInputCheckCurr(key))
				obj->YPosition += move_dist;
			break;
		case DOWN:
			if(AEInputCheckCurr(key))
				obj->YPosition -= move_dist;
			break;
	}

	// AEGfxSetPosition(*objX, *objY); 
}