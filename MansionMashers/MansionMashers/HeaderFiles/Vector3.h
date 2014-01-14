#ifndef FOX_VECTOR3
#define FOX_VECTOR3

typedef struct Vector3
{
	float x, y, z;
}Vec3;

void Vec3Zero(Vec3* Result);
void Vec3Set(Vec3* Result, float x, float y, float z);

void Vec3Negate(Vec3* Result, Vec3* Operand);
void Vec3Add(Vec3* Result, Vec3* Operand1, Vec3* Operand2);
void Vec3Subtract(Vec3* Result, Vec3* Operand1, Vec3* Operand2);
void Vec3Normalize(Vec3* Result, Vec3* Operand);

void Vec3Scale(Vec3* Result, Vec3* Operand, float Scalar);

void Vec3Project(Vec3* Result, Vec3* Operand1, Vec3* Operand2);
void Vec3ProjectPerp(Vec3* Result, Vec3* Operand1, Vec3* Operand2);

float Vec3Length(Vec3* Operand);
float Vec3SquareLength(Vec3* Operand);
float Vec3Distance(Vec3* Operand1, Vec3* Operand2);
float Vec3SquareDistance(Vec3* Operand1, Vec3* Operand2);

float Vec3DotProduct(Vec3* Operand1, Vec3* Operand2);
float Vec3CrossProductMag(Vec3* Operand1, Vec3* Operand2);

#endif