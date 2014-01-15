/*
File:				Vector3.c
Author:				Dan Muller (d.muller)
Creation Date:		Jan 13, 2014

Purpose:			Has all of the vector 2D functions.

Functions:			Vec3Zero - Zeros a vector
					Vec3Set - Sets a vector to x, y, z

					Vec3Negate - Negates a vector to -x, -y, -z
					Vec3Add - Adds 3 vectors
					Vec3Subtract - Subtracts 3 vectors
					Vec3Normalize - Normalizes a vector

					Vec3Scale - Scales a vector
					Vec3Project - Vector projection

					Vec3Length - Returns length of a vector
					Vec3SquareLength - Returns the square of the length of a vector
					Vec3Distance - Returns the distance between 2 vectors
					Vec3SquareDistance - Returns the square of the distance between 2 vectors

					Vec3DotProduct - Returns the dot product of 2 vectors
					Vec3CrossProductMag - Returns the magnitude of the cross product of 2 vectors
 
Copyright (C) 2014 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited. 
*/ 

#include "../HeaderFiles/Vector3.h"
#include <math.h>

void Vec3Zero(Vec3* Result)
{
	Result->x = 0.0f;
	Result->y = 0.0f;
	Result->z = 0.0f;
}

void Vec3Set(Vec3* Result, float x, float y, float z)
{
	Result->x = x;
	Result->y = y;
	Result->z = z;
}

void Vec3Negate(Vec3* Result, Vec3* Operand)
{
	Result->x = -Operand->x;
	Result->y = -Operand->y;
	Result->z = -Operand->z;
}

void Vec3Add(Vec3* Result, Vec3* Operand1, Vec3* Operand2)
{
	Result->x = Operand1->x + Operand2->x;
	Result->y = Operand1->y + Operand2->y;
	Result->z = Operand1->z + Operand2->z;
}

void Vec3Subtract(Vec3* Result, Vec3* Operand1, Vec3* Operand2)
{
	Result->x = Operand1->x - Operand2->x;
	Result->y = Operand1->y - Operand2->y;
	Result->z = Operand1->z - Operand2->z;
}

void Vec3Normalize(Vec3* Result, Vec3* Operand)
{
	float length = Vec3Length(Operand);

	Result->x = Operand->x / length;
	Result->y = Operand->y / length;
	Result->z = Operand->z / length;
}

void Vec3Scale(Vec3* Result, Vec3* Operand, float Scalar)
{
	Result->x = Operand->x * Scalar;
	Result->y = Operand->y * Scalar;
	Result->z = Operand->z * Scalar;
}

void Vec3Project(Vec3* Result, Vec3* Operand1, Vec3* Operand2)
{
	float scalar;

	scalar = Vec3DotProduct(Operand1, Operand2) / Vec3DotProduct(Operand2, Operand2);

	Vec3Scale(Result, Operand2, scalar);
}

//void Vec3ProjectPerp(Vec3* Result, Vec3* Operand1, Vec3* Operand2);

float Vec3Length(Vec3* Operand)
{
	return sqrt(Vec3SquareLength(Operand));
}

float Vec3SquareLength(Vec3* Operand)
{
	return (Operand->x * Operand->x) + (Operand->y * Operand->y) + (Operand->z * Operand->z);
}

//float Vec3Distance(Vec3* Operand1, Vec3* Operand2);
//float Vec3SquareDistance(Vec3* Operand1, Vec3* Operand2);

float Vec3DotProduct(Vec3* Operand1, Vec3* Operand2)
{
	return (Operand1->x * Operand2->x) + (Operand1->y * Operand2->y) + (Operand1->z * Operand2->z);
}

float Vec3CrossProductMag(Vec3* Operand1, Vec3* Operand2)
{
	float i, j, k;
	
	i = (Operand1->y * Operand2->z - Operand1->z * Operand2->y);
	j = -(Operand1->x * Operand2->z - Operand1->z * Operand2->x);
	k = (Operand1->x * Operand2->y - Operand1->y * Operand2->x);

	return sqrt(i * i + j * j + k * k);
}

