#include "../HeaderFiles/Movement.h"
#include "../AEEngine.h"
#include "../HeaderFiles/Vector2.h"

void obj_move(Vec2 *Pos, int direction, float move_dist)
{
	switch (direction)
	{
		case LEFT:
			Pos->x -= move_dist;
			break;
		case RIGHT:
			Pos->x += move_dist;
			break;
		case UP:
			Pos->y += move_dist;				
			break;
		case DOWN:
			Pos->y -= move_dist;
			break;
	}
}
