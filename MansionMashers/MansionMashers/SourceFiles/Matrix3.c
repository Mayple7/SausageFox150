/*****************************************************************************/
/*!
\file				Matrix3.c
\author				Dan Muller (d.muller)
\date				Jan 13, 2014

\brief				Math functions for a 3x3 matrix

\par				Functions:
\li					Matrix3Zero
\li					Matrix3Copy
\li					Matrix3Set
\li					Matrix3MultVec3
\li					Matrix3Add
\li					Matrix3Sub
\li					Matrix3Mult
\li					Matrix3Scale
\li					Matrix3Identity
  
\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/

// ---------------------------------------------------------------------------
// Includes
#include "../HeaderFiles/FoxMath.h"

/*************************************************************************/
/*!
	\brief
	Zeros a 3x3 matrix

	\param Result
	Pointer to the zeroed 3x3 matrix
*/
/*************************************************************************/
void Matrix3Zero(Matrix3* Result)
{
	Result->m00 = 0.0f; 
	Result->m01 = 0.0f;
	Result->m02 = 0.0f; 
	Result->m10 = 0.0f;
	Result->m11 = 0.0f; 
	Result->m12 = 0.0f;
	Result->m20 = 0.0f; 
	Result->m21 = 0.0f;
	Result->m22 = 0.0f; 
}

/*************************************************************************/
/*!
	\brief
	Copies one matrix to another

	\param Result
	Copy of the matrix

	\param Operand
	Matrix to make a copy of
*/
/*************************************************************************/
void Matrix3Copy(Matrix3* Result, Matrix3* Operand)
{
	Result->m00 = Operand->m00;
	Result->m01 = Operand->m01;
	Result->m02 = Operand->m02;
	Result->m10 = Operand->m10;
	Result->m11 = Operand->m11;
	Result->m12 = Operand->m12;
	Result->m20 = Operand->m20;
	Result->m21 = Operand->m21;
	Result->m22 = Operand->m22;
}

/*************************************************************************/
/*!
	\brief
	Sets a 3x3 matrix

	\param Result
	Pointer to the set 3x3 matrix

	\param mm00
	Value for slot 00

	\param mm01
	Value for slot 01

	\param mm02
	Value for slot 02

	\param mm10
	Value for slot 10

	\param mm11
	Value for slot 11

	\param mm12
	Value for slot 12

	\param mm20
	Value for slot 20

	\param mm21
	Value for slot 21

	\param mm22
	Value for slot 22
*/
/*************************************************************************/
void Matrix3Set(Matrix3* Result, float mm00, float mm01, float mm02,
          float mm10, float mm11, float mm12,
          float mm20, float mm21, float mm22)
{	
	Result->m00 = mm00;
	Result->m01 = mm01;
	Result->m02 = mm02;
	Result->m10 = mm10;
	Result->m11 = mm11;
	Result->m12 = mm12;
	Result->m20 = mm20;
	Result->m21 = mm21;
	Result->m22 = mm22;
}

/*************************************************************************/
/*!
	\brief
	Multiply a matrix with a Vector3

	\param Result
	Result of the multiplication

	\param Operand1
	Matrix to multiply

	\param Operand2
	Vector to multiply
*/
/*************************************************************************/
void Matrix3MultVec3(Vec3* Result, Matrix3* Operand1, Vec3* Operand2)
{
	Result->x = Operand1->m00 * Operand2->x + Operand1->m01 * Operand2->y + Operand1->m02 * Operand2->z;
	Result->y = Operand1->m10 * Operand2->x + Operand1->m11 * Operand2->y + Operand1->m12 * Operand2->z;
	Result->z = Operand1->m20 * Operand2->x + Operand1->m21 * Operand2->y + Operand1->m22 * Operand2->z;
}

/*************************************************************************/
/*!
	\brief
	Adds 2 matricies together

	\param Result
	Matrix of the addition result

	\param Operand1
	First matrix to add

	\param Operand2
	Second matrix to add
*/
/*************************************************************************/
void Matrix3Add(Matrix3* Result, Matrix3* Operand1, Matrix3* Operand2)
{
	Result->m00 = Operand1->m00 + Operand2->m00;
	Result->m01 = Operand1->m01 + Operand2->m01;
	Result->m02 = Operand1->m02 + Operand2->m02;

	Result->m10 = Operand1->m10 + Operand2->m10;
	Result->m11 = Operand1->m11 + Operand2->m11;
	Result->m12 = Operand1->m12 + Operand2->m12;

	Result->m20 = Operand1->m20 + Operand2->m20;
	Result->m21 = Operand1->m21 + Operand2->m21;
	Result->m22 = Operand1->m22 + Operand2->m22;
}

/*************************************************************************/
/*!
	\brief
	Subtracts one matrix from another

	\param Result
	Matrix of the subtraction result

	\param Operand1
	Matrix to subtract from

	\param Operand2
	Matrix to subtract
*/
/*************************************************************************/
void Matrix3Sub(Matrix3* Result, Matrix3* Operand1, Matrix3* Operand2)
{
	Result->m00 = Operand1->m00 - Operand2->m00;
	Result->m01 = Operand1->m01 - Operand2->m01;
	Result->m02 = Operand1->m02 - Operand2->m02;

	Result->m10 = Operand1->m10 - Operand2->m10;
	Result->m11 = Operand1->m11 - Operand2->m11;
	Result->m12 = Operand1->m12 - Operand2->m12;

	Result->m20 = Operand1->m20 - Operand2->m20;
	Result->m21 = Operand1->m21 - Operand2->m21;
	Result->m22 = Operand1->m22 - Operand2->m22;
}

/*************************************************************************/
/*!
	\brief
	Multiplies 2 matricies together

	\param Result
	Matrix of the multiplication result

	\param Operand1
	Matrix to multiply

	\param Operand2
	Matrix to multiply
*/
/*************************************************************************/
void Matrix3Mult(Matrix3* Result, Matrix3* Operand1, Matrix3* Operand2)
{
	Result->m00 = Operand1->m00 * Operand2->m00 + Operand1->m01 * Operand2->m10 + Operand1->m02 * Operand2->m20;
	Result->m01 = Operand1->m00 * Operand2->m01 + Operand1->m01 * Operand2->m11 + Operand1->m02 * Operand2->m21;
	Result->m02 = Operand1->m00 * Operand2->m02 + Operand1->m01 * Operand2->m22 + Operand1->m02 * Operand2->m22;

	Result->m10 = Operand1->m10 * Operand2->m00 + Operand1->m11 * Operand2->m10 + Operand1->m12 * Operand2->m20;
	Result->m11 = Operand1->m10 * Operand2->m01 + Operand1->m11 * Operand2->m11 + Operand1->m12 * Operand2->m21;
	Result->m12 = Operand1->m10 * Operand2->m02 + Operand1->m11 * Operand2->m22 + Operand1->m12 * Operand2->m22;

	Result->m20 = Operand1->m20 * Operand2->m00 + Operand1->m21 * Operand2->m10 + Operand1->m22 * Operand2->m20;
	Result->m21 = Operand1->m20 * Operand2->m01 + Operand1->m21 * Operand2->m11 + Operand1->m22 * Operand2->m21;
	Result->m22 = Operand1->m20 * Operand2->m02 + Operand1->m21 * Operand2->m22 + Operand1->m22 * Operand2->m22;
}

/*************************************************************************/
/*!
	\brief
	Scales a matrix by a value

	\param Result
	Matrix of the resulting scaling

	\param Operand
	Matrix to scale

	\param Scalar
	Value to scale the matrix with
*/
/*************************************************************************/
void Matrix3Scale(Matrix3* Result, Matrix3* Operand, float Scalar)
{
	Result->m00 = Operand->m00 * Scalar;
	Result->m01 = Operand->m01 * Scalar;
	Result->m02 = Operand->m02 * Scalar;
	Result->m10 = Operand->m10 * Scalar;
	Result->m11 = Operand->m11 * Scalar;
	Result->m12 = Operand->m12 * Scalar;
	Result->m20 = Operand->m20 * Scalar;
	Result->m21 = Operand->m21 * Scalar;
	Result->m22 = Operand->m22 * Scalar;
}

/*************************************************************************/
/*!
	\brief
	Creates a 3x3 identity matrix

	\param Result
	Identity matrix
*/
/*************************************************************************/
void Matrix3Identity(Matrix3* Result)
{
	Result->m00 = 1.0f; 
	Result->m01 = 0.0f;
	Result->m02 = 0.0f; 

	Result->m10 = 0.0f;
	Result->m11 = 1.0f; 
	Result->m12 = 0.0f;

	Result->m20 = 0.0f; 
	Result->m21 = 0.0f;
	Result->m22 = 1.0f; 
}