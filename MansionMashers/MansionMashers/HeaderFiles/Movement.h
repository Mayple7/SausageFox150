#ifndef FOX_MOVEMENT
#define FOX_MOVEMENT

#include "Vector2.h"
#include "FoxObjects.h"
#include "HandGuyBoss.h"

#define BALLISTA_DEVISOR 1.4f

enum Direction{ UP, LEFT, DOWN, RIGHT };

void MoveObject(Vec2 *Pos, enum Direction dirMove, float move_dist);

#endif