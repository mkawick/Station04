#include <cmath>
#include <cassert>
#include "Math/mathfunctions.h"
#include "Math/Quaternion.h"

#include "Math/angle.h"
#include "Math/vector3.h"

//----------------------------------------------------------------------

Quaternion::Quaternion () : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{
}


Quaternion::Quaternion (const Vector3& v, float w) : x(v.X()), y(v.Y()), z(v.Z()), w(w)
{
}

Quaternion::Quaternion (float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
{}

Quaternion::Quaternion (Angle yaw, Angle pitch, Angle roll)
{
	SetYawPitchRoll(yaw,pitch,roll);
}

Quaternion::Quaternion (float yaw, float pitch, float roll)
{
	SetYawPitchRoll(yaw,pitch,roll);
}

Quaternion::Quaternion (const Quaternion& q) : x(q.x), y(q.y), z(q.z), w(q.w)
{}

Quaternion& Quaternion::operator = (const Quaternion& q)
{
	x = q.x;
	y = q.y;
	z = q.z;
	w = q.w;
	return *this;
}

Quaternion& Quaternion::operator += (const Quaternion& q)
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	return *this;
}

Quaternion& Quaternion::operator -= (const Quaternion& q)
{
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
	return *this;
}

Quaternion& Quaternion::operator *= (const Quaternion& q)
{
	Vector3 a (q.X(),q.Y(),q.Z());
	const Vector3 b (X(),Y(),Z());
	float dot = a.Dot(b);
	a = q.w*b + w*a + a.Cross(b);
	x = a.X();
	y = a.Y();
	z = a.Z();
	w = w*q.w - dot;
	return *this;
}

Quaternion& Quaternion::operator *= (float f)
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

Quaternion& Quaternion::operator /= (float f)
{
	assert(f != 0.0f);//, "Division by zero");
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return *this;
}

Quaternion Quaternion::operator - () const
{
	return Quaternion(*this) *= -1.f;
}

Quaternion Quaternion::operator + (const Quaternion& q) const
{
	return Quaternion(*this) += q;
}

Quaternion Quaternion::operator - (const Quaternion& q) const
{
	return Quaternion(*this) -= q;
}

Quaternion Quaternion::operator * (const Quaternion& q) const
{
	return Quaternion(*this) *= q;
}

Quaternion Quaternion::operator * (float f) const
{
	return Quaternion(*this) *= f;
}

Quaternion Quaternion::operator / (float f) const
{
	assert (f != 0.0f);//, "Division by zero");
	return Quaternion(*this) *= 1.0f/f;
}

bool Quaternion::operator == (const Quaternion& q) const
{
	return q.x == x && q.y == y && q.z == z && q.w == w;
}

bool Quaternion::operator != (const Quaternion& q) const
{
	return !(*this == q);
}

float				Quaternion::Dot					(const Quaternion&rhs) const
{
	return x*rhs.x + y*rhs.y + z*rhs.z + w*rhs.w;
}

Quaternion& Quaternion::Set (float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
	return *this;
}

Quaternion& Quaternion::AxisAngle (const Vector3& axis, Angle angle)
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

Quaternion& Quaternion::SetYawPitchRoll (Angle yaw, Angle pitch, Angle roll)
{
	Quaternion r0, r1;
	r0.AxisAngle(Vector3(1.0f,0.0f,0.0f),pitch);
	r1.AxisAngle(Vector3(0.0f,1.0f,0.0f),roll);
	
	AxisAngle(Vector3(0.0f,0.0f,1.0f),yaw);// call member function to set the basic quaternion values.
	
	*this *= r0;
	*this *= r1;
	return *this;
}

Quaternion& Quaternion::SetYawPitchRoll (float yaw, float pitch, float roll)
{
	Quaternion r0, r1;
	r0.AxisAngle(Vector3(1.0f,0.0f,0.0f),pitch);
	r1.AxisAngle(Vector3(0.0f,1.0f,0.0f),roll);
	
	AxisAngle(Vector3(0.0f,0.0f,1.0f),yaw);// call member function to set the basic quaternion values.
	
	*this *= r0;
	*this *= r1;
	return *this;
}

Quaternion& Quaternion::SetIdentity ()
{
	x = y = z = 0.0f;
	w = 1.0f;
	return *this;
}

float Quaternion::Magnitude () const
{
	return sqrt (SquareMagnitude());
}

float Quaternion::SquareMagnitude () const
{
	return (x*x + y*y + z*z + w*w);
}

Quaternion Quaternion::AsNormal () const
{
	float len = Magnitude();
	assert (len > 0);//, "division by zero");
	return (Quaternion(*this) *= 1.f/len);
}

Quaternion& Quaternion::Normalize ()
{
	float len = Magnitude();
	assert (len > 0);//, "division by zero");
	return ((*this) *= 1.f/len);
}

Quaternion& Quaternion::Invert ()
{
	*this = AsConjugate();
	float mag = SquareMagnitude();
	assert (mag > 0);//, "division by zero");
	*this *= 1.f / mag;
	return *this;
}

bool Quaternion::IsNormal () const
{
	return Equal(SquareMagnitude(),1.0f);
}

/*float Quaternion::X() const
{
	return x;
}

float Quaternion::Y() const
{
	return y;
}

float Quaternion::Z() const
{
	return z;
}

float Quaternion::W() const
{
	return w;
}

void Quaternion::X (float _x)
{
	x = _x;
}

void Quaternion::Y (float _y)
{
	y = _y;
}

void Quaternion::Z (float _z)
{
	z = _z;
}

void Quaternion::W (float _w)
{
	w = _w;
}*/

void Quaternion::Validate() const
{ 
	assert ((X()==X() && Y()==Y() && Z()==Z() && W()==W()));
}

Quaternion Quaternion::AsConjugate () const
{
	return Quaternion(-x,-y,-z,w);
}

Quaternion Quaternion::AsInverse () const
{
	Quaternion result = AsConjugate();
	float mag = result.SquareMagnitude();
	assert (mag > 0);//, "division by zero");
	result *= 1.f / mag;
	return result;
}

Quaternion& Quaternion::ReflectYawPitchRoll(float yaw, float pitch, float roll) 
{ 
	return SetYawPitchRoll(yaw, pitch, roll); 
}


//---------------------------------------------------------------------------------------------------------------------------------
// Friend functions
//---------------------------------------------------------------------------------------------------------------------------------


Quaternion operator * (float f, const Quaternion& q)
{
	return Quaternion(q) *= f;
}

Vector3 operator * (const Vector3& v, const Quaternion& q)
{
	Quaternion result = q.AsConjugate();
	result *= Quaternion(v,0.f);
	result *= q;
	return Vector3 (result.x, result.y, result.z);
}


Quaternion SlerpPrecise (const Quaternion& q1, const Quaternion& q2, float fraction)
{
	assert (fraction>=0 && fraction <=1.0);
	return Slerp(q1, q2, fraction);
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

Quaternion CreateQuaternionFromVectors(const Vector3& v1, const Vector3& v2)
{
	assert((v1.SquareMagnitude() - 1.0f) <= EPSILON);//, "Quaternion::AxisAngle: conversion resultin a non-normalised quaternion, probably because of a non-normalised axis being passed in!");
	assert((v2.SquareMagnitude() - 1.0f) <= EPSILON);//, "Quaternion::AxisAngle: conversion resultin a non-normalised quaternion, probably because of a non-normalised axis being passed in!");
	
	Vector3 axis = v1.Cross(v2);
	if(axis.SquareMagnitude() <= EPSILON)
	{
		// If the vectors are a negation, we'll get a zero vector in the cross product.
		// Get an arbitrary axis that's orthoganal to the lines in this case
		axis = v1.Cross(Vector3(0.66f - v2.X(), 0.0f, 0.33f + v2.Y() * 2.0f)); // Cross with arbitrary different vector
	}
	axis.Normalize();
	float fAngle = v1.Dot(v2);
	Angle angle = acos (fAngle);
	
	// Determine angle direction
	Vector3 testAxis = v1.Cross(axis);
	if(testAxis.Dot(v2) < 0.0f)
	{
		angle *= -1.0f;
	}
	
	// Create quaternion
	Quaternion r;
	r.AxisAngle(axis,angle);
	return r;
}

