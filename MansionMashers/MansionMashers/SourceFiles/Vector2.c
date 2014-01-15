/*
File:				Vector2.c
Author:				Dan Muller (d.muller)
Creation Date:		Jan 12, 2014

Purpose:			Has all of the vector 2D functions.

Functions:			Vec2Zero - Zeros a vector
					Vec2Set - Sets a vector to x, y

					Vec2Negate - Negates a vector to -x, -y
					Vec2Add - Adds 2 vectors
					Vec2Subtract - Subtracts 2 vectors
					Vec2Normalize - Normalizes a vector

					Vec2Scale - Scales a vector
					Vec2Project - Vector projection

					Vec2Length - Returns length of a vector
					Vec2SquareLength - Returns the square of the length of a vector
					Vec2Distance - Returns the distance between 2 vectors
					Vec2SquareDistance - Returns the square of the distance between 2 vectors

					Vec2DotProduct - Returns the dot product of 2 vectors
					Vec2CrossProductMag - Returns the magnitude of the cross product of 2 vectors
 
Copyright (C) 2014 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited. 
*/ 

// ---------------------------------------------------------------------------
// Includes
#include "../HeaderFiles/Vector2.h"
#include <math.h>

// ---------------------------------------------------------------------------
// Globals

// ---------------------------------------------------------------------------
// Function Prototypes

// ---------------------------------------------------------------------------
// Main Program

//Zeros a vector
void Vec2Zero(Vec2* Result)
{
	Result->x = 0;
	Result->y = 0;
}

//Sets a vector to x, y
void Vec2Set(Vec2* Result, float x, float y)
{
	Result->x = x;
	Result->y = y;
}

//Negates a vector to -x, -y
void Vec2Negate(Vec2* Result, Vec2* Operand)
{
	Result->x = -(Operand->x);
	Result->y = -(Operand->y);
}

//Adds 2 vectors
void Vec2Add(Vec2* Result, Vec2* Operand1, Vec2* Operand2)
{
	Result->x = Operand1->x + Operand2->x;
	Result->y = Operand1->y + Operand2->y;
}

//Subtracts 2 vectors
void Vec2Subtract(Vec2* Result, Vec2* Operand1, Vec2* Operand2)
{
	Result->x = Operand1->x - Operand2->x;
	Result->y = Operand1->x - Operand2->y;
}

//Normalizes a vector
void Vec2Normalize(Vec2* Result, Vec2* Operand)
{
	float length = Vec2Length(Operand);

	Result->x = Operand->x / length;
	Result->y = Operand->y / length;
}


//Scales a vector
void Vec2Scale(Vec2* Result, Vec2* Operand, float Scalar)
{
	Result->x = Scalar * Operand->x;
	Result->y = Scalar * Operand->y;
}

//Vector projection
void Vec2Project(Vec2* Result, Vec2* Operand1, Vec2* Operand2)
{
	//Vector b onto a
	// b = Op1, a = Op2

	float scalar; // (a . b) / (a . a)

	scalar = Vec2DotProduct(Operand2, Operand1) / Vec2DotProduct(Operand2, Operand2);

	Vec2Scale(Result, Operand2, scalar);
}

//Returns length of a vector
float Vec2Length(Vec2* Operand)
{
	return (sqrt(Vec2SquareLength(Operand)));
}

//Returns the square of the length of a vector
float Vec2SquareLength(Vec2* Operand)
{
	return (Operand->x * Operand->x + Operand->y * Operand->y);
}

// Returns the distance between 2 vectors
float Vec2Distance(Vec2* Operand1, Vec2* Operand2)
{	
	return (sqrt(Vec2SquareDistance(Operand1, Operand2)));
}

//Returns the square of the distance between 2 vectors
float Vec2SquareDistance(Vec2* Operand1, Vec2* Operand2)
{
	float x_square;
	float y_square;

	x_square = (Operand1->x - Operand2->x) * (Operand1->x - Operand2->x);
	y_square = (Operand1->y - Operand2->y) * (Operand1->y - Operand2->y);

	return (x_square + y_square);
}

//Returns the dot product of 2 vectors
float Vec2DotProduct(Vec2* Operand1, Vec2* Operand2)
{
	return (Operand1->x * Operand2->x + Operand1->y * Operand2->y);
}

//Returns the magnitude of the cross product of 2 vectors
float Vec2CrossProductMag(Vec2* Operand1, Vec2* Operand2)
{
	return (Operand1->x * Operand2->y - Operand1->y * Operand2->x);
}

void Vec2RotateDegrees(Vec2* Result, float Degrees)
{
	//float length = Vec2Length(Result);
	float radians = 2 * M_PI * Degrees / 360.0f;
	Result->x = Result->x * cos(radians) - Result->y * sin(radians);
	Result->y = Result->x * sin(radians) + Result->y * cos(radians);
}
