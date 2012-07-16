/*
 *  Vector.cpp
 *  SDL04_Station
 */
/*
#ifndef __gl_h_
#include <SDL/SDL_opengl.h>
#include "glut.h"
#endif*/

#include <math.h>
#include <assert.h>
#include "Vector.h"
//#include "
#include "../struct.h"
#include "MathFunctions.h"


//---------------------------------------------------------

Vector :: Vector () :
x(0), y(0), z(0) 
{}

//---------------------------------------------------------

Vector :: Vector (float _x, float _y, float _z) :
x(_x), y(_y), z(_z)
{
}

//---------------------------------------------------------

Vector :: Vector (const Vector& v) : x(v.x), y(v.y), z(v.z)
{
}

//---------------------------------------------------------

// construct a vector from two points in space
Vector :: Vector (const Vector& v1, const Vector& v2) : x (v2.x-v1.x), y (v2.y-v1.y), z (v2.z-v1.z)
{
}

//---------------------------------------------------------

Vector&	Vector :: Set (float _x, float _y, float _z)
{
	x = _x, y = _y, z = _z;
	return *this;
}

//---------------------------------------------------------

void	Vector :: Clear ()
{
	x = 0, y = 0, z = 0;
}

//---------------------------------------------------------

Vector	Vector :: operator - () const
{
	float _x = -x; if (_x == -0) _x = 0;
	float _y = -y; if (_y == -0) _y = 0;
	float _z = -z; if (_z == -0) _z = 0;
	
	return Vector (_x, _y, _z);
}

//---------------------------------------------------------

Vector	Vector :: operator - (const Vector& v) const
{
	return Vector (x-v.x, y-v.y, z-v.z);// order is important
}

//---------------------------------------------------------

Vector	Vector :: operator + (const Vector& v) const
{
	return Vector (x+v.x, y+v.y, z+v.z);
}

//---------------------------------------------------------

Vector&		Vector :: operator =			(const Vector& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

//---------------------------------------------------------

Vector&		Vector :: operator += (const Vector& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

//---------------------------------------------------------

Vector&		Vector :: operator -= (const Vector& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

//---------------------------------------------------------

Vector	Vector :: operator += (float v)
{
	x += v;
	y += v;
	z += v;
	return *this;
}

//---------------------------------------------------------

Vector	Vector :: operator -= (float v)
{
	x -= v;
	y -= v;
	z -= v;
	return *this;
}

//---------------------------------------------------------

Vector	Vector :: operator *= (float v)
{
	x *= v;
	y *= v;
	z *= v;
	return *this;
}

//---------------------------------------------------------

Vector	Vector :: operator * (float mult) const
{
	return Vector (x*mult, y*mult, z*mult);
}

//---------------------------------------------------------

Vector	Vector :: operator /= (float v)
{
	float Reciprical = 1/v;
	x *= Reciprical;
	y *= Reciprical;
	z *= Reciprical;
	return *this;
}

//---------------------------------------------------------

Vector :: operator float*	() const
{
	return (float*) this;
}

//---------------------------------------------------------

float	Vector :: Dot () const// self only
{
	return x*x + y*y + z*z;
}

//---------------------------------------------------------

float	Vector :: Dot (const Vector& v) const
{
	return v.x*x + v.y*y + v.z*z;
}

//---------------------------------------------------------

Vector&	Vector :: Normalize ()
{
	float mag = 1/Magnitude ();
	if (mag == 0)
		return *this;
	x *= mag;
	y *= mag;
	z *= mag;
	return *this;
}

//---------------------------------------------------------

float	Vector :: NormalizedDot (const Vector& v) const
{
	return sqrt (v.x*x + v.y*y + v.z*z);
}

//---------------------------------------------------------

Vector  Vector :: Cross (const Vector& v) const
{
	float _x = (y * v.z) - (z * v.y);
	float _y = (z * v.x) - (x * v.z);
	float _z = (x * v.y) - (y * v.x);
	return Vector (_x, _y, _z);
}

//---------------------------------------------------------

Vector  Vector :: NormalizedCross (const Vector& v) const
{
	float _x = (y * v.z) - (z * v.y);
	float _y = (z * v.x) - (x * v.z);
	float _z = (x * v.y) - (y * v.x);
	return Vector (_x, _y, _z).AsNormal ();
}

//---------------------------------------------------------

Vector Vector :: AsNormal () const
{
	float d = Magnitude ();
	if (d == 0)
		return Vector ();
	float Reciprical = 1/d;
	
	return Vector (x*Reciprical, y*Reciprical, z*Reciprical);
}

//---------------------------------------------------------

float	 Vector :: Magnitude () const
{
	return sqrt (x*x + y*y + z*z);
}

void	Vector :: ConvertToDegrees ()
{
	float MultiplyFactor = 180/M_PI;
	x *= MultiplyFactor;
	y *= MultiplyFactor;
	z *= MultiplyFactor;
}

void	Vector :: ConvertToRadians ()
{
	float MultiplyFactor = M_PI/180;
	x *= MultiplyFactor;
	y *= MultiplyFactor;
	z *= MultiplyFactor;
}

//---------------------------------------------------------
//---------------------------------------------------------

void	Vector :: ConvertAngleInDegreesToXYComponents (float Angle)
{
	float CurrentAngle = M_PI/180 * Angle;
	x = sin (CurrentAngle);
	y = cos (CurrentAngle);
	z = 0;
}

//---------------------------------------------------------

Vector			Vector :: ProjectOn			(const Vector& rhs) const							
{
	const float t = rhs.Dot(rhs); 
	assert (t != 0);// "Divide by zero"
	Vector n = this->AsNormal();
	return (n) * (this->Dot(rhs) / t); 
}

//---------------------------------------------------------
/*
float	 Vector :: Magnitude () const 
{
	float val = ((x*x) + 
                 (y*y) + 
                 (z*z));
	// I've tried assembly here and it is slower
	// the code runs much faster written like this, about 5.5 times as fast
	return (float) sqrt(val);
}*/

//---------------------------------------------------------

float	 Vector :: SquareMagnitude () const 
{
	float val = ((x*x) + 
                 (y*y) + 
                 (z*z));
	
	return (val);
}

//---------------------------------------------------------

bool			Vector :: IsPerpendicular (const Vector& a) const
{
	if (DotProduct (a) == 0.0F) return true; 
	return false;
}

//---------------------------------------------------------

Vector operator* (const float x, const Vector& y)
{
	return y*x;
}

//---------------------------------------------------------

// convert vector to an angle value using x,y
float	Vector :: GetAngle () const
{
    return (float) atan2 (x, y);
}

//---------------------------------------------------------

float	Vector :: GetAngle (const Vector &V) const // this is the angle between these two vectors
{
	//
	// u . v = |u| |v| cos a     ->
	//
	//        -1  / u . v \
	// a = cos   ( ------- )
	//            \|u| |v|/
	//
	float ResultLength = Length () * V.Length ();
	
	if (Equal (ResultLength, 0.0F))
		return 0.0F;
	
	return (float) acos (DotProduct ( V ) / ResultLength );
}

//---------------------------------------------------------

void	Vector :: ClampMagnitude		(float MinDist, float MaxDist)
{
	if (MinDist > MaxDist)
		SWAP (MinDist, MaxDist);
	
	float dist = Magnitude ();
	
	if (dist <MinDist)
	{
		Normalize ();
		*this *= MinDist;
	}
	else if (dist>MaxDist)
	{
		Normalize ();
		*this *= MaxDist;
	}
}

//---------------------------------------------------------
//---------------------------------------------------------
/*
It's pretty simple to build your own look-at matrix.

First, calculate forward, right, and up vectors. The forward vector should be a unit vector pointing 
in the direction that your camera is looking. The right and up vectors must be unit vectors, and 
perpendicular to each other and the forward vector. How to come up with these vectors is a matter 
of how your camera works, so I won't go into more detail (unless you ask).

Now, make a 3x3 matrix that has as its first row, the right vector; as its second row, the up 
vector; and as its third row, the NEGATIVE of the forward vector (this is so the forward vector 
 maps onto the negative Z axis, which is the usual behavior for gluLookAt).

Finally, expand this matrix into a 4x4 (by adding a 1 in the lower-right corner and zeros
 elsewhere), and multiply it by a translation matrix that takes your camera location to the origin
 (i.e. the translation is by the negative of the camera location). And you've got a look-at matrix.
/*
// Offset pIn by pOffset into pOut
void myVectorOffset (GLpoint *pIn, GLpoint *pOffset, GLpoint *pOut)
{
	pOut->x = pIn->x - pOffset->x;
	pOut->y = pIn->y - pOffset->y;
	pOut->z = pIn->z - pOffset->z;
}

// Compute the cross product a X b into pOut
void VectorGetNormal (GLpoint *a, GLpoint *b, GLpoint *pOut)
{
	pOut->x = a->y * b->z - a->z * b->y;
	pOut->y = a->z * b->x - a->x * b->z;
	pOut->z = a->x * b->y - a->y * b->x;
}

// Normalize pIn vector into pOut
bool VectorNormalize (GLpoint *pIn, GLpoint *pOut)
{
	GLfloat len = (GLfloat)(sqrt(sqr(pIn->x) + sqr(pIn->y) + sqr(pIn->z)));
	if (len)
	{
		pOut->x = pIn->x / len;
		pOut->y = pIn->y / len;
		pOut->z = pIn->z / len;
		return true;
	}
	return false;
}

// Compute p1,p2,p3 face normal into pOut
bool ComputeFaceNormal (GLpoint *p1, GLpoint *p2, GLpoint *p3, GLpoint *pOut)
{
	// Uses p2 as a new origin for p1,p3
	GLpoint a;
	VectorOffset(p3, p2, &a);
	GLpoint b;
	VectorOffset(p1, p2, &b);
	// Compute the cross product a X b to get the face normal
	GLpoint pn;
	VectorGetNormal(&a, &b, &pn);
	// Return a normalized vector
	return VectorNormalize(&pn, pOut);
}
*/