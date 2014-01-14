#ifndef FOX_MOVEMENT
#define FOX_MOVEMENT

typedef enum { UP, LEFT, DOWN, RIGHT } Direction;

void obj_move(Vec2 *Pos, int direction, float move_dist);

#endif