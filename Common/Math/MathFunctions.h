/*
 *  MathFunctions.h
 *  SDL04_Station
 */

#pragma once
#include "vector.h"

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
int GCD(int a, int b)
{
	if (b==0) return a;
	return GCD(b,a%b);
}

inline
int LCM(int a, int b)
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

Vector	GetPointClosestToLine (const Vector& l1, // origin point
							   const Vector& l2,
							   const Vector& p); //tested point
Vector	GetPointInTriangleClosestToPoint (const Vector& point,
										  const Vector& pointInTriangle,
										  const Vector& normalOfTriangle);

bool	IsPointInTriangle (Vector a, Vector b, Vector c, Vector point);

float	GetSquareDistanceFromPointToLine (Vector v1, Vector v2, Vector pt);// shortest
float	GetDistanceFromLineToLine (Vector v1, Vector v2, Vector pt);// shortest

float	GetDistanceFromPointToLine2D (Vector v1, Vector v2, Vector pt);// shortest
Vector	GetInPlanePortionOfVector (const Vector& vect, const Vector& PlaneNormal);
//-----------------------------------------------------------------------

Vector	CreateVectorUpInHex (Vector startposition, float distance);
Vector	CreateVectorUpLeftInHex (Vector startposition, float distance);
Vector	CreateVectorUpRightInHex (Vector startposition, float distance);
Vector	CreateVectorDownInHex (Vector startposition, float distance);
Vector	CreateVectorDownLeftInHex (Vector startposition, float distance);
Vector	CreateVectorDownRightInHex (Vector startposition, float distance);

//-----------------------------------------------------------------------

#ifndef M_PI
#define M_PI 3.1415926535F
#endif
