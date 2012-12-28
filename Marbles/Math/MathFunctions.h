#pragma once
#include "vector3.h"

class Angle;
//-----------------------------------------------------------

float	ConvertToDegrees (float);
float	ConvertToRadians (float);
unsigned int BitCount (unsigned int v);
template <typename T>
T Select( T iCompare, T iValA, T iValB )
{
	return ((iCompare >= 0) ? iValA : iValB);
}

//Matrix44   	LookAt (const Axiom::Math::Vector4& from, const Axiom::Math::Vector4& to, const Axiom::Math::Vector4& up);

//extern const float PI;
extern const float TWO_PI;
extern const float PI_DIV_TWO;
extern const float PI_DIV_FOUR;
extern const float TO_RADIANS;
extern const float TO_DEGREES;
extern const float EPSILON;
extern const float FLOAT_MAX;
extern const float FLOAT_MIN;
//-----------------------------------------------------------

float FClamp( float val, float min, float max );
//float FMin( float lhs, float rhs )								{return (lhs < rhs) ? lhs:rhs;}
//float FMax( float lhs, float rhs )								{return (lhs > rhs) ? lhs:rhs;}
//float Fabs( float val )											{return (val<0) ? -val:val;}

Angle Fabs( const Angle& val );

inline float FloatMax					(float a, float b)					{ return a < b ? b : a; }
inline float FloatMin					(float a, float b)					{ return a < b ? a : b; }
inline float FloatAbs					(float a)							{ return a < 0.0f ? -a : a; }

// Floating point functions for ranges of -1 to +1
inline bool Equal						(float a, float b)					{ return (FloatAbs (a -b) < EPSILON); }
inline bool Less						(float a, float b)					{ return (a <= (b-EPSILON)); }
inline bool Greater						(float a, float b)					{ return (a >= (b+EPSILON)); }
inline bool LessEqual					(float a, float b)					{ return (a < (b+EPSILON)); }
inline bool GreaterEqual				(float a, float b)					{ return (a > (b-EPSILON)); }

inline bool EqualRelative				(float a, float b)					{ float eps = EPSILON * FloatMax(FloatAbs(a),FloatAbs(b)); return (a >= (b-eps)) && (a <= (b+eps)); }
inline bool LessRelative				(float a, float b)					{ float eps = EPSILON * FloatMax(FloatAbs(a),FloatAbs(b)); return (a <= (b-eps)); }
inline bool GreaterRelative				(float a, float b)					{ float eps = EPSILON * FloatMax(FloatAbs(a),FloatAbs(b)); return (a >= (b+eps)); }
inline bool LessEqualRelative			(float a, float b)					{ float eps = EPSILON * FloatMax(FloatAbs(a),FloatAbs(b)); return (a < (b+eps)); }
inline bool GreaterEqualRelative		(float a, float b)					{ float eps = EPSILON * FloatMax(FloatAbs(a),FloatAbs(b)); return (a > (b-eps)); }

inline bool Equal						(float a, float b, float epsilon)	{ return (FloatAbs (a - b) < epsilon); }
inline bool Less						(float a, float b, float epsilon)	{ return (a <= (b-epsilon)); }
inline bool Greater						(float a, float b, float epsilon)	{ return (a >= (b+epsilon)); }
inline bool LessEqual					(float a, float b, float epsilon)	{ return (a < (b+epsilon)); }
inline bool GreaterEqual				(float a, float b, float epsilon)	{ return (a > (b-epsilon)); }

inline bool EqualRelative				(float a, float b, float epsilon)	{ float eps = epsilon * FloatMax(FloatAbs(a),FloatAbs(b)); return (a >= (b-eps)) && (a <= (b+eps)); }
inline bool LessRelative				(float a, float b, float epsilon)	{ float eps = epsilon * FloatMax(FloatAbs(a),FloatAbs(b)); return (a <= (b-eps)); }
inline bool GreaterRelative				(float a, float b, float epsilon)	{ float eps = epsilon * FloatMax(FloatAbs(a),FloatAbs(b)); return (a >= (b+eps)); }
inline bool LessEqualRelative			(float a, float b, float epsilon)	{ float eps = epsilon * FloatMax(FloatAbs(a),FloatAbs(b)); return (a <= (b+eps)); }
inline bool GreaterEqualRelative		(float a, float b, float epsilon)	{ float eps = epsilon * FloatMax(FloatAbs(a),FloatAbs(b)); return (a >= (b-eps)); }

inline bool InRange						(float minVal, float maxVal, float f, float epsilon = EPSILON)	{ return (GreaterEqual(f, minVal, epsilon) && LessEqual(f, maxVal, epsilon)); }
inline bool InRangeRelative				(float minVal, float maxVal, float f, float epsilon = EPSILON)	{ return (GreaterEqualRelative(f, minVal, epsilon) && LessEqualRelative(f, maxVal, epsilon)); }

// -------------------------------------------------------------------------

//Copied from //http://www.topcoder.com/tc?module=Static&d1=tutorials&d2=math_for_topcoders
inline //assume that a and b cannot both be 0
int GCD(int a, int b) // greatest common denominator
{
	if (b==0) return a;
	return GCD(b,a%b);
}

inline
int LCM(int a, int b) // least common multiplier
{
	return b*a/GCD(a,b);
}
inline
int toDecimal(int n, int b)
{
	int result=0;
	int multiplier=1;
	
	while(n>0)
	{
		result+=n%10*multiplier;
		multiplier*=b;
		n/=10;
	}
	
	return result;
}
inline
int fromDecimal(int n, int b)
{
	int result=0;
	int multiplier=1;
	
	while(n>0)
	{
		result+=n%b*multiplier;
		multiplier*=10;
		n/=b;
	}
	
	return result;
}


//--------------------------------------------------------------------------

Vector3	GetPointClosestToLine (const Vector3& l1, // origin point
							   const Vector3& l2,
							   const Vector3& p); //tested point
Vector3	GetPointInTriangleClosestToPoint (const Vector3& point,
										  const Vector3& pointInTriangle,
										  const Vector3& normalOfTriangle);

bool	IsPointInTriangle (Vector3 a, Vector3 b, Vector3 c, Vector3 point);

float	GetSquareDistanceFromPointToLine (Vector3 v1, Vector3 v2, Vector3 pt);// shortest
float	GetDistanceFromLineToLine (Vector3 v1, Vector3 v2, Vector3 pt);// shortest

float	GetDistanceFromPointToLine2D (Vector3 v1, Vector3 v2, Vector3 pt);// shortest
Vector3	GetInPlanePortionOfVector (const Vector3& vect, const Vector3& PlaneNormal);
//-----------------------------------------------------------------------

Vector3	CreateVectorUpInHex (Vector3 startposition, float distance);
Vector3	CreateVectorUpLeftInHex (Vector3 startposition, float distance);
Vector3	CreateVectorUpRightInHex (Vector3 startposition, float distance);
Vector3	CreateVectorDownInHex (Vector3 startposition, float distance);
Vector3	CreateVectorDownLeftInHex (Vector3 startposition, float distance);
Vector3	CreateVectorDownRightInHex (Vector3 startposition, float distance);
void	Random2dVector( Vector3& v, float magnitude = 1.0f );

//-----------------------------------------------------------------------

#ifndef M_PI
#define M_PI 3.1415926535F
#endif
