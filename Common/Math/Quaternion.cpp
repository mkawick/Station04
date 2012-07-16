/*
 *  Quaternion.cpp
 *  Station05
 */

#include <math.h>
#include <assert.h>
#include "mathfunctions.h"
#include "Quaternion.h"

#include "angle.h"
#include "vector.h"

//----------------------------------------------------------------------

inline Quaternion::Quaternion () : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{
}


inline Quaternion::Quaternion (const Vector& v, float w) : x(v.X()), y(v.Y()), z(v.Z()), w(w)
{
}

inline Quaternion::Quaternion (float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
{}

inline Quaternion::Quaternion (Angle yaw, Angle pitch, Angle roll)
{
	SetYawPitchRoll(yaw,pitch,roll);
}

inline Quaternion::Quaternion (const Quaternion& q) : x(q.x), y(q.y), z(q.z), w(q.w)
{}

inline Quaternion& Quaternion::operator = (const Quaternion& q)
{
	x = q.x;
	y = q.y;
	z = q.z;
	w = q.w;
	return *this;
}

inline Quaternion& Quaternion::operator += (const Quaternion& q)
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	return *this;
}

inline Quaternion& Quaternion::operator -= (const Quaternion& q)
{
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
	return *this;
}
/*
inline Quaternion& Quaternion::operator *= (const Quaternion& q)
{
	Vector a (q.X(),q.Y(),q.Z());
	const Vector b (X(),Y(),Z());
	float dot = a.Dot(b);
	a = q.w*b + w*a + a.LeftHandedCross(b);
	x = a.X();
	y = a.Y();
	z = a.Z();
	w = w*q.w - dot;
	return *this;
}*/

inline Quaternion& Quaternion::operator *= (float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

inline Quaternion& Quaternion::operator /= (float f)
{
	assert(f != 0.0f);//, "Division by zero");
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return *this;
}

inline Quaternion Quaternion::operator - () const
{
	return Quaternion(*this) *= -1.f;
}

inline Quaternion Quaternion::operator + (const Quaternion& q) const
{
	return Quaternion(*this) += q;
}

inline Quaternion Quaternion::operator - (const Quaternion& q) const
{
	return Quaternion(*this) -= q;
}

inline Quaternion Quaternion::operator * (const Quaternion& q) const
{
	return Quaternion(*this) *= q;
}

inline Quaternion Quaternion::operator * (float f) const
{
	return Quaternion(*this) *= f;
}

inline Quaternion Quaternion::operator / (float f) const
{
	assert (f != 0.0f);//, "Division by zero");
	return Quaternion(*this) *= 1.0f/f;
}

inline bool Quaternion::operator == (const Quaternion& q) const
{
	return q.x == x && q.y == y && q.z == z && q.w == w;
}

inline bool Quaternion::operator != (const Quaternion& q) const
{
	return !(*this == q);
}

float				Quaternion::Dot					(const Quaternion&rhs) const
{
	return x*rhs.x + y*rhs.y + z*rhs.z + w*rhs.w;
}

inline Quaternion& Quaternion::Set (float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
	return *this;
}

inline Quaternion& Quaternion::AxisAngle (const Vector& axis, Angle angle)
{
	register float halfAngle = angle.AsRadians() * 0.5f;
	register float sinHalfAngle = sin (halfAngle);
	x = sinHalfAngle*axis.X();
	y = sinHalfAngle*axis.Y();
	z = sinHalfAngle*axis.Z();
	w = cos (halfAngle);
	
	assert (fabs (SquareMagnitude() - 1.f) <= EPSILON);//, "Quaternion::AxisAngle: conversion resultin a non-normalised quaternion, probably because of a non-normalised axis being passed in!");
	return *this;
}

inline Quaternion& Quaternion::SetYawPitchRoll (Angle yaw, Angle pitch, Angle roll)
{
	Quaternion r0, r1;
	r0.AxisAngle(Vector(1.0f,0.0f,0.0f),pitch);
	r1.AxisAngle(Vector(0.0f,1.0f,0.0f),roll);
	
	AxisAngle(Vector(0.0f,0.0f,1.0f),yaw);// call member function to set the basic quaternion values.
	
	*this *= r0;
	*this *= r1;
	return *this;
}

inline Quaternion& Quaternion::SetIdentity ()
{
	x = y = z = 0.0f;
	w = 1.0f;
	return *this;
}

inline float Quaternion::Magnitude () const
{
	return sqrt (SquareMagnitude());
}

inline float Quaternion::SquareMagnitude () const
{
	return (x*x + y*y + z*z + w*w);
}

inline Quaternion Quaternion::AsNormal () const
{
	float len = Magnitude();
	assert (len > 0);//, "division by zero");
	return (Quaternion(*this) *= 1.f/len);
}

inline Quaternion& Quaternion::Normalize ()
{
	float len = Magnitude();
	assert (len > 0);//, "division by zero");
	return ((*this) *= 1.f/len);
}

inline Quaternion& Quaternion::Invert ()
{
	*this = AsConjugate();
	float mag = SquareMagnitude();
	assert (mag > 0);//, "division by zero");
	*this *= 1.f / mag;
	return *this;
}

inline bool Quaternion::IsNormal () const
{
	return Equal(SquareMagnitude(),1.0f);
}

inline float Quaternion::X() const
{
	return x;
}

inline float Quaternion::Y() const
{
	return y;
}

inline float Quaternion::Z() const
{
	return z;
}

inline float Quaternion::W() const
{
	return w;
}

inline void Quaternion::X (float _x)
{
	x = _x;
}

inline void Quaternion::Y (float _y)
{
	y = _y;
}

inline void Quaternion::Z (float _z)
{
	z = _z;
}

inline void Quaternion::W (float _w)
{
	w = _w;
}

inline void Quaternion::Validate() const
{ 
	assert ((X()==X() && Y()==Y() && Z()==Z() && W()==W()));
}

inline Quaternion Quaternion::AsConjugate () const
{
	return Quaternion(-x,-y,-z,w);
}

inline Quaternion Quaternion::AsInverse () const
{
	Quaternion result = AsConjugate();
	float mag = result.SquareMagnitude();
	assert (mag > 0);//, "division by zero");
	result *= 1.f / mag;
	return result;
}

inline Quaternion& Quaternion::ReflectYawPitchRoll(float yaw, float pitch, float roll) 
{ 
	return SetYawPitchRoll(yaw, pitch, roll); 
}


//---------------------------------------------------------------------------------------------------------------------------------
// Friend functions
//---------------------------------------------------------------------------------------------------------------------------------


inline Quaternion operator * (float f, const Quaternion& q)
{
	return Quaternion(q) *= f;
}

inline Vector operator * (const Vector& v, const Quaternion& q)
{
	Quaternion result = q.AsConjugate();
	result *= Quaternion(v,0.f);
	result *= q;
	return Vector (result.x, result.y, result.z);
}


inline Quaternion SlerpPrecise (float fraction, const Quaternion& q1, const Quaternion& q2)
{
	assert (fraction>=0 && fraction <=1.0);
	return Slerp(fraction, q1, q2);
}

Quaternion	Slerp (Quaternion A, Quaternion B, float Percentage)
{
	assert (Percentage>=0 && Percentage <=1.0);
	float Mult1, Mult2;
	
	float CosTheta = A.Dot (B);// dot product
	if (CosTheta < 0.0f)
	{
		A = -A;// inverse quat
		CosTheta = -CosTheta;
	}
	
	// If the quaternions are close in angle, use LERP
	if ((CosTheta + 1.0f) > 0.05f)
	{
		if ((1.0f - CosTheta) < 0.05f)
		{
			Mult1 = 1.0f - Percentage;
			Mult2 = Percentage;
			Quaternion quat (Mult1 * A.x + Mult2 * B.x,
							 Mult1 * A.y + Mult2 * B.y,
							 Mult1 * A.z + Mult2 * B.z,
							 Mult1 * A.w + Mult2 * B.w);
			return quat.Normalize ();
		}
		else
		{
			// do SLERP
			float Theta = acos (CosTheta);// angle
			float SinTheta = sin (Theta);
			Mult1 = sin (Theta * (1.0f-Percentage)) / SinTheta;
			Mult2 = sin (Theta * Percentage) / SinTheta;
		}
	}
	else
	{
		// this will fall through below
		B.Set (-A.y, A.x, -A.w, A.z);
		Mult1 = sin (M_PI * (0.5f - Percentage));
		Mult2 = sin (M_PI * Percentage);
	}
	
	// return the final LERP/SLERP
	return Quaternion (Mult1 * A.x + Mult2 * B.x,
					   Mult1 * A.y + Mult2 * B.y,
					   Mult1 * A.z + Mult2 * B.z,
					   Mult1 * A.w + Mult2 * B.w);
}

inline Quaternion CreateQuaternionFromVectors(const Vector& v1, const Vector& v2)
{
	assert((v1.SquareMagnitude() - 1.0f) <= EPSILON);//, "Quaternion::AxisAngle: conversion resultin a non-normalised quaternion, probably because of a non-normalised axis being passed in!");
	assert((v2.SquareMagnitude() - 1.0f) <= EPSILON);//, "Quaternion::AxisAngle: conversion resultin a non-normalised quaternion, probably because of a non-normalised axis being passed in!");
	
	Vector axis = v1.Cross(v2);
	if(axis.SquareMagnitude() <= EPSILON)
	{
		// If the vectors are a negation, we'll get a zero vector in the cross product.
		// Get an arbitrary axis that's orthoganal to the lines in this case
		axis = v1.Cross(Vector(0.66f - v2.X(), 0.0f, 0.33f + v2.Y() * 2.0f)); // Cross with arbitrary different vector
	}
	axis.Normalize();
	float fAngle = v1.Dot(v2);
	Angle angle = acos (fAngle);
	
	// Determine angle direction
	Vector testAxis = v1.Cross(axis);
	if(testAxis.Dot(v2) < 0.0f)
	{
		angle *= -1.0f;
	}
	
	// Create quaternion
	Quaternion r;
	r.AxisAngle(axis,angle);
	return r;
}

