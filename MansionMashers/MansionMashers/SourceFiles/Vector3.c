/*****************************************************************************/
/*!
\file				Vector3.c
\author				Dan Muller (d.muller)
\date				Jan 13, 2014

\brief				All Vector3 functions

\par				Functions:
\li					Vec3Zero
\li					Vec3Set
\li					Vec3Negate
\li					Vec3Add
\li					Vec3Subtract
\li					Vec3Normalize
\li					Vec3Scale
\li					Vec3Project
\li					Vec3Length
\li					Vec3SquareLength
\li					Vec3Distance
\li					Vec3SquareDistance
\li					Vec3DotProduct
\li					Vec3CrossProductMag
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 

// ---------------------------------------------------------------------------
// Includes
#include "../HeaderFiles/Vector3.h"
#include <math.h>

/*************************************************************************/
/*!
	\brief
	Zeroes a vector

	\param Result
	Vector to be zeroed
*/
/*************************************************************************/
void Vec3Zero(Vec3* Result)
{
	Result->x = 0.0f;
	Result->y = 0.0f;
	Result->z = 0.0f;
}

/*************************************************************************/
/*!
	\brief
	Sets a vector to specific x, y and z values

	\param Result
	Vector to be set

	\param x
	X value for the vector

	\param y
	Y value for the vector

	\param z
	Z value for the vector
*/
/*************************************************************************/
void Vec3Set(Vec3* Result, float x, float y, float z)
{
	Result->x = x;
	Result->y = y;
	Result->z = z;
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
void Vec3Negate(Vec3* Result, Vec3* Operand)
{
	Result->x = -Operand->x;
	Result->y = -Operand->y;
	Result->z = -Operand->z;
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
void Vec3Add(Vec3* Result, Vec3* Operand1, Vec3* Operand2)
{
	Result->x = Operand1->x + Operand2->x;
	Result->y = Operand1->y + Operand2->y;
	Result->z = Operand1->z + Operand2->z;
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
void Vec3Subtract(Vec3* Result, Vec3* Operand1, Vec3* Operand2)
{
	Result->x = Operand1->x - Operand2->x;
	Result->y = Operand1->y - Operand2->y;
	Result->z = Operand1->z - Operand2->z;
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
void Vec3Normalize(Vec3* Result, Vec3* Operand)
{
	float length = Vec3Length(Operand);

	Result->x = Operand->x / length;
	Result->y = Operand->y / length;
	Result->z = Operand->z / length;
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
void Vec3Scale(Vec3* Result, Vec3* Operand, float Scalar)
{
	Result->x = Operand->x * Scalar;
	Result->y = Operand->y * Scalar;
	Result->z = Operand->z * Scalar;
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
void Vec3Project(Vec3* Result, Vec3* Operand1, Vec3* Operand2)
{
	float scalar;

	scalar = Vec3DotProduct(Operand1, Operand2) / Vec3DotProduct(Operand2, Operand2);

	Vec3Scale(Result, Operand2, scalar);
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
float Vec3Length(Vec3* Operand)
{
	return (float)sqrt(Vec3SquareLength(Operand));
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
float Vec3SquareLength(Vec3* Operand)
{
	return (Operand->x * Operand->x) + (Operand->y * Operand->y) + (Operand->z * Operand->z);
}

//float Vec3Distance(Vec3* Operand1, Vec3* Operand2);
//float Vec3SquareDistance(Vec3* Operand1, Vec3* Operand2);

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
float Vec3DotProduct(Vec3* Operand1, Vec3* Operand2)
{
	return (Operand1->x * Operand2->x) + (Operand1->y * Operand2->y) + (Operand1->z * Operand2->z);
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
float Vec3CrossProductMag(Vec3* Operand1, Vec3* Operand2)
{
	float i, j, k;
	
	i = (Operand1->y * Operand2->z - Operand1->z * Operand2->y);
	j = -(Operand1->x * Operand2->z - Operand1->z * Operand2->x);
	k = (Operand1->x * Operand2->y - Operand1->y * Operand2->x);

	return (float)sqrt(i * i + j * j + k * k);
}

