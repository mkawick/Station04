#include <cmath>
#include <cassert>
#include "Math/Vector3.h"
#include "Math/MathFunctions.h"


//---------------------------------------------------------

Vector3 :: Vector3 () :
x(0), y(0), z(0) 
{}

//---------------------------------------------------------

Vector3 :: Vector3 (float _x, float _y, float _z) :
x(_x), y(_y), z(_z)
{
}

//---------------------------------------------------------

Vector3 :: Vector3 (const Vector3& v) : x(v.x), y(v.y), z(v.z)
{
}

//---------------------------------------------------------

// construct a vector from two points in space
Vector3 :: Vector3 (const Vector3& v1, const Vector3& v2) : x (v2.x-v1.x), y (v2.y-v1.y), z (v2.z-v1.z)
{
}

//---------------------------------------------------------

Vector3&	Vector3 :: Set (float _x, float _y, float _z)
{
	x = _x, y = _y, z = _z;
	return *this;
}

//---------------------------------------------------------

void	Vector3 :: Clear ()
{
	x = 0, y = 0, z = 0;
}

//---------------------------------------------------------

Vector3	Vector3 :: operator - () const
{
	float _x = -x; if (_x == -0) _x = 0;
	float _y = -y; if (_y == -0) _y = 0;
	float _z = -z; if (_z == -0) _z = 0;
	
	return Vector3 (_x, _y, _z);
}

//---------------------------------------------------------

Vector3	Vector3 :: operator - (const Vector3& v) const
{
	return Vector3 (x-v.x, y-v.y, z-v.z);// order is important
}

//---------------------------------------------------------

Vector3	Vector3 :: operator + (const Vector3& v) const
{
	return Vector3 (x+v.x, y+v.y, z+v.z);
}

//---------------------------------------------------------

Vector3&		Vector3 :: operator =			(const Vector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

Vector3&		Vector3 :: operator =			(const float v )
{
	x = v;
	y = v;
	z = v;
	return *this;
}

//---------------------------------------------------------

Vector3&		Vector3 :: operator += (const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

//---------------------------------------------------------

Vector3&		Vector3 :: operator -= (const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

//---------------------------------------------------------

Vector3	Vector3 :: operator += (float v)
{
	x += v;
	y += v;
	z += v;
	return *this;
}

//---------------------------------------------------------

Vector3	Vector3 :: operator -= (float v)
{
	x -= v;
	y -= v;
	z -= v;
	return *this;
}

//---------------------------------------------------------

Vector3	Vector3 :: operator *= (float v)
{
	x *= v;
	y *= v;
	z *= v;
	return *this;
}

//---------------------------------------------------------

Vector3	Vector3 :: operator * (float mult) const
{
	return Vector3 (x*mult, y*mult, z*mult);
}

//---------------------------------------------------------

Vector3	Vector3 :: operator /= (float v)
{
	float Reciprical = 1/v;
	x *= Reciprical;
	y *= Reciprical;
	z *= Reciprical;
	return *this;
}

//---------------------------------------------------------

Vector3 :: operator float*	() const
{
	return (float*) this;
}

//---------------------------------------------------------

float&	Vector3::operator[]	( int i )
{
	assert( i>=0 && i<3 );
	switch( i )
	{
	case 0: return x;
	case 1: return y;
	case 2: return z;
	}

	return x;
}

float&	Vector3::operator[]	(unsigned int i )
{
	return operator[]((int) i);
}

//---------------------------------------------------------

float	Vector3 :: Dot () const// self only
{
	return x*x + y*y + z*z;
}

//---------------------------------------------------------

float	Vector3 :: Dot (const Vector3& v) const
{
	return v.x*x + v.y*y + v.z*z;
}

//---------------------------------------------------------

Vector3&	Vector3 :: Normalize ()
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

float	Vector3 :: NormalizedDot (const Vector3& v) const
{
	return sqrt (v.x*x + v.y*y + v.z*z);
}

//---------------------------------------------------------

Vector3  Vector3 :: Cross (const Vector3& v) const
{
	float _x = (y * v.z) - (z * v.y);
	float _y = (z * v.x) - (x * v.z);
	float _z = (x * v.y) - (y * v.x);
	return Vector3 (_x, _y, _z);
}

//---------------------------------------------------------

Vector3  Vector3 :: NormalizedCross (const Vector3& v) const
{
	float _x = (y * v.z) - (z * v.y);
	float _y = (z * v.x) - (x * v.z);
	float _z = (x * v.y) - (y * v.x);
	return Vector3 (_x, _y, _z).AsNormal ();
}

//---------------------------------------------------------

Vector3 Vector3 :: AsNormal () const
{
	float d = Magnitude ();
	if (d == 0)
		return Vector3 ();
	float Reciprical = 1/d;
	
	return Vector3 (x*Reciprical, y*Reciprical, z*Reciprical);
}

//---------------------------------------------------------

float	 Vector3 :: Magnitude () const
{
	return sqrt (x*x + y*y + z*z);
}

void	Vector3 :: ConvertToDegrees ()
{
	float MultiplyFactor = 180/M_PI;
	x *= MultiplyFactor;
	y *= MultiplyFactor;
	z *= MultiplyFactor;
}

void	Vector3 :: ConvertToRadians ()
{
	float MultiplyFactor = M_PI/180;
	x *= MultiplyFactor;
	y *= MultiplyFactor;
	z *= MultiplyFactor;
}

//---------------------------------------------------------
//---------------------------------------------------------

void	Vector3 :: ConvertAngleInDegreesToXYComponents (float Angle)
{
	float CurrentAngle = M_PI/180 * Angle;
	x = sin (CurrentAngle);
	y = cos (CurrentAngle);
	z = 0;
}

//---------------------------------------------------------

Vector3			Vector3 :: ProjectOn			(const Vector3& rhs) const							
{
	const float t = rhs.Dot(rhs); 
	assert (t != 0);// "Divide by zero"
	Vector3 n = this->AsNormal();
	return (n) * (this->Dot(rhs) / t); 
}

//---------------------------------------------------------
/*
float	 Vector3 :: Magnitude () const 
{
	float val = ((x*x) + 
                 (y*y) + 
                 (z*z));
	// I've tried assembly here and it is slower
	// the code runs much faster written like this, about 5.5 times as fast
	return (float) sqrt(val);
}*/

//---------------------------------------------------------

float	 Vector3 :: SquareMagnitude () const 
{
	float val = ((x*x) + 
                 (y*y) + 
                 (z*z));
	
	return (val);
}

//---------------------------------------------------------

bool			Vector3 :: IsPerpendicular (const Vector3& a) const
{
	if (DotProduct (a) == 0.0F) return true; 
	return false;
}

//---------------------------------------------------------

Vector3 operator* (const float x, const Vector3& y)
{
	return y*x;
}

//---------------------------------------------------------

// convert vector to an angle value using x,y
float	Vector3 :: GetAngle () const
{
    return (float) atan2 (x, y);
}

//---------------------------------------------------------

float	Vector3 :: GetAngle (const Vector3 &V) const // this is the angle between these two vectors
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

void	Vector3 :: ClampMagnitude		(float MinDist, float MaxDist)
{
	if (MinDist > MaxDist)
		std::swap (MinDist, MaxDist);
	
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

float&	Color4::operator[]	( int i )
{
	assert( i>=0 && i<4 );
	switch( i )
	{
	case 0: return r;
	case 1: return g;
	case 2: return b;
	case 3: return a;
	}

	return r;
}

float&	Color4::operator[]	(unsigned int i )
{
	return operator[]((int) i);
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
void myVector3Offset (GLpoint *pIn, GLpoint *pOffset, GLpoint *pOut)
{
	pOut->x = pIn->x - pOffset->x;
	pOut->y = pIn->y - pOffset->y;
	pOut->z = pIn->z - pOffset->z;
}

// Compute the cross product a X b into pOut
void Vector3GetNormal (GLpoint *a, GLpoint *b, GLpoint *pOut)
{
	pOut->x = a->y * b->z - a->z * b->y;
	pOut->y = a->z * b->x - a->x * b->z;
	pOut->z = a->x * b->y - a->y * b->x;
}

// Normalize pIn vector into pOut
bool Vector3Normalize (GLpoint *pIn, GLpoint *pOut)
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
	Vector3Offset(p3, p2, &a);
	GLpoint b;
	Vector3Offset(p1, p2, &b);
	// Compute the cross product a X b to get the face normal
	GLpoint pn;
	Vector3GetNormal(&a, &b, &pn);
	// Return a normalized vector
	return Vector3Normalize(&pn, pOut);
}
*/