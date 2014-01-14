#ifndef FOX_VECTOR2
#define FOX_VECTOR2

typedef struct Vector2
{
	float x, y;
}Vec2;

void Vec2Zero(Vec2* Result);
void Vec2Set(Vec2* Result, float x, float y);

void Vec2Negate(Vec2* Result, Vec2* Operand);
void Vec2Add(Vec2* Result, Vec2* Operand1, Vec2* Operand2);
void Vec2Subtract(Vec2* Result, Vec2* Operand1, Vec2* Operand2);
void Vec2Normalize(Vec2* Result, Vec2* Operand);

void Vec2Scale(Vec2* Result, Vec2* Operand, float Scalar);

void Vec2Project(Vec2* Result, Vec2* Operand1, Vec2* Operand2);
void Vec2ProjectPerp(Vec2* Result, Vec2* Operand1, Vec2* Operand2);

float Vec2Length(Vec2* Operand);
float Vec2SquareLength(Vec2* Operand);
float Vec2Distance(Vec2* Operand1, Vec2* Operand2);
float Vec2SquareDistance(Vec2* pVec0, Vec2* pVec1);

float Vec2DotProduct(Vec2* Operand1, Vec2* Operand2);
float Vec2CrossProductMag(Vec2* Operand1, Vec2* Operand2);

void Vec2FromAngle(Vec2* Result, float Angle);

#endif