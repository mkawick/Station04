/*
 *  MathFunctions.cpp
 *  SDL04_Station
 */

#include "stdafx.h"
#include <windows.h>
#include <math.h>
#include "MathFunctions.h"

const float PI = 			3.1415927f;
const float TWO_PI =		6.2831855f;
const float PI_DIV_TWO =	1.5707964f;
const float PI_DIV_FOUR =	0.78539819f;
const float TO_RADIANS =	0.017453292f; // (PI/180.f)
const float TO_DEGREES =	57.295779513f; // (180.f/PI)
const float EPSILON =		1e-6f;
const float FLOAT_MAX =		3.402823466e+38F; // Largest positive float
const float FLOAT_MIN =		1.175494351e-38F; // Smallest positive float


//---------------------------------------------------------

unsigned int BitCount (unsigned int v)
{
    unsigned int tmp = v - ((v >> 1) & 0x55555555);
	
    tmp = (tmp & 0x33333333) + ((tmp >> 2) & 0x33333333);
	
    unsigned int CountOfBits = ((tmp + (tmp >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
	
    return CountOfBits;
}

//---------------------------------------------------------

float	ConvertToDegrees (float inAngle)
{
	return inAngle * 180/M_PI;
}

float	ConvertToRadians (float inAngle)
{
	return M_PI/180 * inAngle;
}

//---------------------------------------------------------

Vector	GetPointClosestToLine (const Vector& v1, // origin point
							   const Vector& v2,
							   const Vector& pt) //tested point
{
	Vector connect1 = v2 - v1;
	Vector connect2 = pt - v1;
	
	float normalizedDist = connect1.Dot (connect2) / 
					connect1.Dot ();// 
	// Clamp
	if (normalizedDist < 0.0F) normalizedDist = 0.0F;
	if (normalizedDist > 1.0F) normalizedDist = 1.0F;
	
	Vector ClosestPoint = v1 + normalizedDist * connect1.AsNormal ();// origin + n*v
	return ClosestPoint;
}

//---------------------------------------------------------

float	DistanceFromPointToLine (const Vector& v1, // origin point
							   const Vector& v2,
							   const Vector& pt) //tested point
{
	Vector connect1 = v2 - v1;
	Vector connect2 = pt - v1;
	
	// projection equation
	float normalizedDist = connect1.Dot (connect2) / 
			connect1.Dot ();// |L||L|
	// Clamp
	if (normalizedDist < 0.0F) normalizedDist = 0.0F;
	if (normalizedDist > 1.0F) normalizedDist = 1.0F;
	
	// shadowed point
	Vector ClosestPoint = v1 + normalizedDist * connect1.AsNormal ();// origin + n*v
	float dist = (pt-ClosestPoint).Magnitude ();
	return dist;
}

//---------------------------------------------------------

Vector	GetPointInTriangleClosestToPoint (const Vector& point, 
										  const Vector& pointInTriangle,
										  const Vector& normalOfTriangle)
{
	// Calculate D. This is defined in most calculus books.
	float	D = pointInTriangle.Dot (normalOfTriangle);
	
	// this is your standard projection equation defined in pre-calculus and is the distance
	// normally needed.
	float	projectionLength = (normalOfTriangle.Dot (point) - D) / (normalOfTriangle.Dot ());
	
	return	point - projectionLength*normalOfTriangle;
}

//---------------------------------------------------------

bool	IsPointInTriangle (Vector a, Vector b, Vector c, Vector point)
{
	// translate all points to the origin which greatl simplifies the math
	a -= point, b -= point, c -= point;
	
	// computing the winding of the triangles formed with 
	// the point and the vertices of the triangle.
	Vector n1 = b.Cross (c);
	Vector n2 = c.Cross (a);
	
	// now we have two normals. If we compute the angle between them, then we can tell if they
	// face in the same relative direction (a opposed to being opposites)
	if (n1.Dot (n2) < 0) // is the cos of the angle is negative, then the angle>90∘ 
	{					 // and the point cannot be in the triangle
		return false;
	}
	
	Vector n3 = a.Cross (b);// compute the third normal
	// ensure it's winding/facing
	if (n1.Dot (n3) < 0) // is the cos of the angle is negative, then the angle>90∘ 
	{					 // and the point cannot be in the triangle
		return false;
	}
	
	return true;
}

//---------------------------------------------------------

// this is highly optimal and very fast.
float	GetSquareDistanceFromPointToLine (Vector v1, Vector v2, Vector pt)// shortest
{
	Vector line = v2 - v1, edge1 = pt - v1, edge2 = pt - v2;
	
	float MagnitudeOfProjection = edge1.Dot (line);
	// the projection may put out point beyond the ends of the line.
	if (MagnitudeOfProjection <= 0.0f) return edge1.SquareMagnitude ();// distance to the point
	
	float SquareMagnitudeOfLine = line.SquareMagnitude ();
	// try the other edge, note how we use the already computed length
	if (MagnitudeOfProjection >= SquareMagnitudeOfLine) return edge2.SquareMagnitude();// distance to the point
	
	// project perp vector onto the 'height' of the point
	return edge1.SquareMagnitude() - MagnitudeOfProjection*MagnitudeOfProjection / SquareMagnitudeOfLine;
}

//---------------------------------------------------------

Vector	GetInPlanePortionOfVector (const Vector& vect, const Vector& PlaneNormal)
{
	float height = (vect.Dot (PlaneNormal));
	Vector shadow = vect - height * PlaneNormal;
	return shadow;
}

//---------------------------------------------------------

float	GetDistanceFromLineToLine (Vector v1, Vector v2, Vector pt)// shortest
{
	return 0.0;
}

//---------------------------------------------------------

float	GetDistanceFromPointToLine2D (Vector v1, Vector v2, Vector pt)// shortest
{
	// find 'normal' to line
	Vector v = v2 - v1;
	
	Vector Perpendicular (-v.y, v.x, 0);// negative inverse slope
	Perpendicular.Normalize ();// now we have a 'normal' to the line
	
	Vector Hypotenuse = pt - v1;
	Vector VectorDistance =  Hypotenuse.ProjectOn (Perpendicular);
	
	return VectorDistance.Magnitude();
}

//---------------------------------------------------------

Vector	CreateVectorUpInHex (Vector startposition, float distance)
{
	return Vector (startposition.x, startposition.y+distance, startposition.z);
}

//---------------------------------------------------------

Vector	CreateVectorUpLeftInHex (Vector startposition, float distance)
{
	float ThirtyDegrees = M_PI / 6;// 60∘
	float HeightBetweenCenters = sin (ThirtyDegrees) * distance;
	float WidthBetweenCenters = cos (ThirtyDegrees) * distance;
	
	return Vector (startposition.x-WidthBetweenCenters, startposition.y+HeightBetweenCenters, startposition.z);
}

//---------------------------------------------------------

Vector	CreateVectorUpRightInHex (Vector startposition, float distance)
{
	float ThirtyDegrees = M_PI / 6;// 60∘
	float HeightBetweenCenters = sin (ThirtyDegrees) * distance;
	float WidthBetweenCenters = cos (ThirtyDegrees) * distance;
	
	return Vector (startposition.x+WidthBetweenCenters, startposition.y+HeightBetweenCenters, startposition.z);
}

//---------------------------------------------------------

Vector	CreateVectorDownInHex (Vector startposition, float distance)
{
	return Vector (startposition.x, startposition.y-distance, startposition.z);
}

//---------------------------------------------------------

Vector	CreateVectorDownLeftInHex (Vector startposition, float distance)
{
	float ThirtyDegrees = M_PI / 6;// 60∘
	float HeightBetweenCenters = sin (ThirtyDegrees) * distance;
	float WidthBetweenCenters = cos (ThirtyDegrees) * distance;
	
	return Vector (startposition.x-WidthBetweenCenters, startposition.y-HeightBetweenCenters, startposition.z);	
}

//---------------------------------------------------------

Vector	CreateVectorDownRightInHex (Vector startposition, float distance)
{
	float ThirtyDegrees = M_PI / 6;// 60∘
	float HeightBetweenCenters = sin (ThirtyDegrees) * distance;
	float WidthBetweenCenters = cos (ThirtyDegrees) * distance;
	
	return Vector (startposition.x+WidthBetweenCenters, startposition.y-HeightBetweenCenters, startposition.z);
}

//---------------------------------------------------------
//---------------------------------------------------------
