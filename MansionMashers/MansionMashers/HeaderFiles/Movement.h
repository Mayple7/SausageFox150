/*****************************************************************************/
/*!
\file				Movement.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef FOX_MOVEMENT
#define FOX_MOVEMENT

#include "Vector2.h"
#include "FoxObjects.h"
#include "HandGuyBoss.h"

#define BALLISTA_DEVISOR 1.4f

enum Direction{ UP, LEFT, DOWN, RIGHT };

void MoveObject(Vec2 *Pos, enum Direction dirMove, float move_dist);

#endif