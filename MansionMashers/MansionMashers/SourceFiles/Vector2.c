/*
File:				Main.c
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
// ---------------------------------------------------------------------------
// Globals

// ---------------------------------------------------------------------------
// Function Prototypes

// ---------------------------------------------------------------------------
// Main Program
void Vec2Zero(Vec2* Result);
void Vec2Set(Vec2* Result, float x, float y);

void Vec2Negate(Vec2* Result, Vec2* Operand);
void Vec2Add(Vec2* Result, Vec2* Operand1, Vec2* Operand2);
void Vec2Subtract(Vec2* Result, Vec2* Operand1, Vec2* Operand2);
void Vec2Normalize(Vec2* Result, Vec2* Operand);

void Vec2Scale(Vec2* Result, Vec2* Operand, float Scalar);
void Vec2Project(Vec2* Result, Vec2* Operand1, Vec2* Operand2);

float Vec2Length(Vec2* Operand);
float Vec2SquareLength(Vec2* Operand);
float Vec2Distance(Vec2* Operand1, Vec2* Operand2);
float Vec2SquareDistance(Vec2* pVec0, Vec2* pVec1);

float Vec2DotProduct(Vec2* Operand1, Vec2* Operand2);
float Vec2CrossProductMag(Vec2* Operand1, Vec2* Operand2);