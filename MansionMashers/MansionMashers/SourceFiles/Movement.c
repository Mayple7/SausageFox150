/*****************************************************************************/
/*!
\file				Movement.c
\author				Juli Gregg (j.gregg)
\date				Jan 11, 2014

\brief				Object movement function

\par				Functions:
\li					MoveObject
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// includes
#include "../HeaderFiles/Movement.h"
#include "../AEEngine.h"
#include "../HeaderFiles/FoxMath.h"

/*************************************************************************/
/*!
	\brief
	Moves an object a specific direction and distance
	
	\param Pos
	The position to be moved

	\param dirMove
	An enum of the direction to move

	\param move_dist
	The distance to move the object
*/
/*************************************************************************/
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
