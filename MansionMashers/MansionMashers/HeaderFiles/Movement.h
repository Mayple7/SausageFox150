#ifndef FOX_MOVEMENT
#define FOX_MOVEMENT

#include "Vector2.h"

enum Direction{ UP, LEFT, DOWN, RIGHT };

void MoveObject(Vec2 *Pos, enum Direction dirMove, float move_dist);

#endif