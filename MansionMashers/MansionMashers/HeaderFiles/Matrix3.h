/*****************************************************************************/
/*!
\file				Matrix3.h
\author				Dan Muller (d.muller)

\par 
<b> Copyright (C) 2014 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior 
 written consent of DigiPen Institute of Technology is prohibited. </b>
*/ 
/*****************************************************************************/
#ifndef Matrix3_H
#define Matrix3_H

#include "FoxMath.h"

typedef struct Matrix3
{
  union
  {
    struct  
    {
      float m00, m01, m02,
          m10, m11, m12,
          m20, m21, m22;

    };
  
    float m[3][3];
    float v[9];
  };
} Matrix3;
  
  /* 
    This union lets us access the data in multiple ways
    All of these are modifying the same location in memory
  
    Matrix3 mtx;
    mtx.m[2][2] = 1.0f;
    mtx.v[10] = 2.0f;
    mtx.m22 = 3.0f;
  */
  
    // Default constructor should initialize to zeroes
	//void Matrix3Zero(void);
  
    // Copy constructor, copies every entry from the other matrix.
  //void Matrix3(Matrix3* Result, Matrix3* Operator);
  
    // Non-default constructor, self-explanatory
  /*Matrix3(float mm00, float mm01, float mm02, float mm03,
          float mm10, float mm11, float mm12, float mm13,
          float mm20, float mm21, float mm22, float mm23,
          float mm30, float mm31, float mm32, float mm33);
  
    // Assignment operator, does not need to handle self-assignment
  Matrix3& operator=(const Matrix3& rhs);
  
    // Multiplying a Matrix3 with a Vector4 or a Point4
  Vector3 operator*(const Vector3& rhs) const;
  
    // Basic Matrix arithmetic operations
  Matrix3 operator+(const Matrix3& rhs) const;
  Matrix3 operator-(const Matrix3& rhs) const;
  Matrix3 operator*(const Matrix3& rhs) const;
  
    // Similar to the three above except they modify
    // the original 
  Matrix3& operator+=(const Matrix3& rhs);
  Matrix3& operator-=(const Matrix3& rhs);
  Matrix3& operator*=(const Matrix3& rhs);
  
    // Scale/Divide the entire matrix by a float
  Matrix3 operator*(const float rhs) const;
  Matrix3 operator/(const float rhs) const;
  
    // Same as previous 
  Matrix3& operator*=(const float rhs);
  Matrix3& operator/=(const float rhs);
  
    // Comparison operators which should use an epsilon defined in 
    // Utilities.h to see if the value is within a certain range
    // in which case we say they are equivalent.
  bool operator==(const Matrix3& rhs) const;
  bool operator!=(const Matrix3& rhs) const;
  
    // Builds the identity matrix
  void Identity(void);
};
*/

  void Matrix3Zero(Matrix3* Result);
  void Matrix3Copy(Matrix3* Result, Matrix3* Operand);
  void Matrix3Set(Matrix3* Result, float mm00, float mm01, float mm02, 
		float mm10, float mm11, float mm12, 
		float mm20, float mm21, float mm22);

  void Matrix3Mult(Matrix3* Result, Matrix3* Operand1, Matrix3* Operand2);
  void Matrix3MultVec3(Vec3* Result, Matrix3* Operand1, Vec3* Operand2);
  void Matrix3Add(Matrix3* Result, Matrix3* Operand1, Matrix3* Operand2);
  void Matrix3Sub(Matrix3* Result, Matrix3* Operand1, Matrix3* Operand2);

  void Matrix3Scale(Matrix3* Result, Matrix3* Operand, float Scalar);
  void Matrix3Identity(Matrix3* Result);

  //Added from CS230
  void Matrix3Transpose(Matrix3 *Result, Matrix3 *Operand);
  void Matrix3Translate(Matrix3 *Result, float x, float y);
  void Matrix3ScaleMatrix(Matrix3 *Result, float x, float y);
  void Matrix3RotDeg(Matrix3 *Result, float Angle);
  void Matrix3RotRad(Matrix3 *Result, float Angle);



#endif