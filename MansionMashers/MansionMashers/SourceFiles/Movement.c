#include "../HeaderFiles/Movement.h"
#include "../AEEngine.h"


void JG_move(int key, float *objX, float *objY, float move_dist, int direction)
{

	// Handling Input
	AEInputUpdate();

	switch (direction)
	{
		case LEFT:
			if(AEInputCheckCurr(key))
				*objX -= move_dist;
			break;
		case RIGHT:
			if(AEInputCheckCurr(key))
				*objX += move_dist;
			break;
		case UP:
			if(AEInputCheckCurr(key))
				*objY += move_dist;
			break;
		case DOWN:
			if(AEInputCheckCurr(key))
				*objY -= move_dist;
			break;
	}

	// AEGfxSetPosition(*objX, *objY); 
}