#include "../HeaderFiles/Movement.h"
#include "../AEEngine.h"
#include "../HeaderFiles/Vector2.h"

void MoveObject(Vec2 *Pos, enum Direction dirMove, float move_dist)
{
	switch (dirMove)
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
