 //Matrix3 

/*
    struct  
    {
      float m00, m01, m02,
          m10, m11, m12,
          m20, m21, m22,

    };

	  /* 
    This union lets us access the data in multiple ways
    All of these are modifying the same location in memory
  
    Matrix3 mtx;
    mtx.m[2][2] = 1.0f;
    mtx.v[10] = 2.0f;
    mtx.m22 = 3.0f;
 
  
*/

#include "../HeaderFiles/FoxMath.h"



//Set matrix to zero
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

//Copy Matrix
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

//Manually set a matrix
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

//Mulltiply a matrix by a vector
void Matrix3MultVec3(Vec3* Result, Matrix3* Operand1, Vec3* Operand2)
{
	Result->x = Operand1->m00 * Operand2->x + Operand1->m01 * Operand2->y + Operand1->m02 * Operand2->z;
	Result->y = Operand1->m10 * Operand2->x + Operand1->m11 * Operand2->y + Operand1->m12 * Operand2->z;
	Result->z = Operand1->m20 * Operand2->x + Operand1->m21 * Operand2->y + Operand1->m22 * Operand2->z;
}

//Add two matrixes
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

//Subtract a matrix from another matrix
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

//Multiply two matrixes together
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

//Multiply matrix by a float
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

//Divide matrix by a float
void Matrix3Div(Matrix3* Result, Matrix3* Operand, float Divisor)
{
	Result->m00 = Operand->m00 / Divisor;
	Result->m01 = Operand->m01 / Divisor;
	Result->m02 = Operand->m02 / Divisor;
	Result->m10 = Operand->m10 / Divisor;
	Result->m11 = Operand->m11 / Divisor;
	Result->m12 = Operand->m12 / Divisor;
	Result->m20 = Operand->m20 / Divisor;
	Result->m21 = Operand->m21 / Divisor;
	Result->m22 = Operand->m22 / Divisor;
}


//Builds Identity matrix
void Identity(Matrix3* Result)
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