#ifndef FOX_MOVEMENT
#define FOX_MOVEMENT

typedef enum { UP, LEFT, DOWN, RIGHT } Direction;


void JG_move(int key, float *objX, float *objY, float move_dist, int direction);

#endif