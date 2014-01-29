/*****************************************************************************/
/*!
\file				Vector2.c
\author				Juli Gregg (j.gregg)
\date				Jan 12, 2014

\brief				All Vector2 functions

\par				Functions:
\li					Vec2Zero
\li					Vec2Set
\li					Vec2Negate
\li					Vec2Add
\li					Vec2Subtract
\li					Vec2Normalize
\li					Vec2Scale
\li					Vec2Project
\li					Vec2Length
\li					Vec2SquareLength
\li					Vec2Distance
\li					Vec2SquareDistance
\li					Vec2DotProduct
\li					Vec2CrossProductMag
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// Includes
#include "../HeaderFiles/Vector2.h"
#include <math.h>

// ---------------------------------------------------------------------------
// Main Program

/*************************************************************************/
/*!
	\brief
	Zeroes a vector

	\param Result
	Vector to be zeroed
*/
/*************************************************************************/
void Vec2Zero(Vec2* Result)
{
	Result->x = 0;
	Result->y = 0;
}

/*************************************************************************/
/*!
	\brief
	Sets a vector to specific x and y values

	\param Result
	Vector to be set

	\param x
	X value for the vector

	\param y
	Y value for the vector
*/
/*************************************************************************/
void Vec2Set(Vec2* Result, float x, float y)
{
	Result->x = x;
	Result->y = y;
}

/*************************************************************************/
/*!
	\brief
	Negates a vector

	\param Result
	Pointer to the negated vector

	\param Operand
	Vector to be negated
*/
/*************************************************************************/
void Vec2Negate(Vec2* Result, Vec2* Operand)
{
	Result->x = -(Operand->x);
	Result->y = -(Operand->y);
}

/*************************************************************************/
/*!
	\brief
	Adds 2 Vectors

	\param Result
	Vector 2 result of the addition

	\param Operand1
	First operand for the addition

	\param Operand2
	Second operand for the addition
*/
/*************************************************************************/
void Vec2Add(Vec2* Result, Vec2* Operand1, Vec2* Operand2)
{
	Result->x = Operand1->x + Operand2->x;
	Result->y = Operand1->y + Operand2->y;
}

/*************************************************************************/
/*!
	\brief
	Subtracts 2 vectors

	\param Result
	Result of the subtraction

	\param Operand1
	Vector to subtract from

	\param Operand2
	Vector to subtract
*/
/*************************************************************************/
void Vec2Subtract(Vec2* Result, Vec2* Operand1, Vec2* Operand2)
{
	Result->x = Operand1->x - Operand2->x;
	Result->y = Operand1->x - Operand2->y;
}

/*************************************************************************/
/*!
	\brief
	Normalizes a vector

	\param Result
	Pointer to the resulting normalized vector

	\param Operand
	Vector to be normalized
*/
/*************************************************************************/
void Vec2Normalize(Vec2* Result, Vec2* Operand)
{
	float length = Vec2Length(Operand);

	Result->x = Operand->x / length;
	Result->y = Operand->y / length;
}


/*************************************************************************/
/*!
	\brief
	Scale a vector by a scalar

	\param Result
	Vector of the result of the scaling

	\param Operand
	Vector to be scaled

	\param Scalar
	Value to scale the vector with
*/
/*************************************************************************/
void Vec2Scale(Vec2* Result, Vec2* Operand, float Scalar)
{
	Result->x = Scalar * Operand->x;
	Result->y = Scalar * Operand->y;
}

/*************************************************************************/
/*!
	\brief
	Projects one vector onto another

	\param Result
	Vector of the result of the projection

	\param Operand1
	Vector to project

	\param Operand2
	Vector to project onto
*/
/*************************************************************************/
void Vec2Project(Vec2* Result, Vec2* Operand1, Vec2* Operand2)
{
	//Vector b onto a
	// b = Op1, a = Op2

	float scalar; // (a . b) / (a . a)

	scalar = Vec2DotProduct(Operand2, Operand1) / Vec2DotProduct(Operand2, Operand2);

	Vec2Scale(Result, Operand2, scalar);
}

/*************************************************************************/
/*!
	\brief
	Calculates the length of a vector

	\param Operand
	Vector whose length will be calculated

	\return
	The value of the length of the Operand vector
*/
/*************************************************************************/
float Vec2Length(Vec2* Operand)
{
	return (float)sqrt(Vec2SquareLength(Operand));
}

/*************************************************************************/
/*!
	\brief
	Calculates the square of the vector length

	\param Operand
	Vector to calculate the square length of

	\return
	Value of the square length of the Operand vector
*/
/*************************************************************************/
float Vec2SquareLength(Vec2* Operand)
{
	return (Operand->x * Operand->x + Operand->y * Operand->y);
}

/*************************************************************************/
/*!
	\brief
	Calculates the distance between 2 vectors

	\param Operand1
	Vector to calculate the distance of

	\param Operand2
	Vector to calculate the distance of

	\return
	Value of the distance between the 2 vectors
*/
/*************************************************************************/
float Vec2Distance(Vec2* Operand1, Vec2* Operand2)
{	
	return (float)sqrt(Vec2SquareDistance(Operand1, Operand2));
}

/*************************************************************************/
/*!
	\brief
	Calculates the square of the distance between 2 vectors

	\param Operand1
	Vector to calculate the distance of

	\param Operand2
	Vector to calculate the distance of

	\return
	Value of the square distance between the 2 vectors
*/
/*************************************************************************/
float Vec2SquareDistance(Vec2* Operand1, Vec2* Operand2)
{
	float x_square;
	float y_square;

	x_square = (Operand1->x - Operand2->x) * (Operand1->x - Operand2->x);
	y_square = (Operand1->y - Operand2->y) * (Operand1->y - Operand2->y);

	return (x_square + y_square);
}

/*************************************************************************/
/*!
	\brief
	Calculates the dot product of 2 vectors

	\param Operand1
	Vector to calculate the dot product with

	\param Operand2
	Vector to calculate the dot product with

	\return
	Value of the dot product calculated
*/
/*************************************************************************/
float Vec2DotProduct(Vec2* Operand1, Vec2* Operand2)
{
	return (Operand1->x * Operand2->x + Operand1->y * Operand2->y);
}

/*************************************************************************/
/*!
	\brief
	Calculates the magnitude of the cross product

	\param Operand1
	Vector to calculate the cross product magnitude with

	\param Operand2
	Vector to calculate the cross product magnitude with
*/
/*************************************************************************/
float Vec2CrossProductMag(Vec2* Operand1, Vec2* Operand2)
{
	return (Operand1->x * Operand2->y - Operand1->y * Operand2->x);
}

/*************************************************************************/
/*!
	\brief
	Rotates a vector by a specified degrees

	\param Result
	Vector to be rotated

	\param Degrees
	Number of degrees to be rotated
*/
/*************************************************************************/
void Vec2RotateDegrees(Vec2* Result, float Degrees)
{
	//float length = Vec2Length(Result);
	float radians = 2 * (float)M_PI * Degrees / 360.0f;
	float resultX = Result->x * (float)cos(radians) - Result->y * (float)sin(radians);
	float resultY = Result->x * (float)sin(radians) + Result->y * (float)cos(radians);

	Vec2Set(Result, resultX, resultY);
}
