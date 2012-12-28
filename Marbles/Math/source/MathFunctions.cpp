#include <cmath>
#include <cstdio>
#include <cstdlib>
#include "Math\MathFunctions.h"

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

Vector3	GetPointClosestToLine (const Vector3& v1, // origin point
							   const Vector3& v2,
							   const Vector3& pt) //tested point
{
	Vector3 connect1 = v2 - v1;
	Vector3 connect2 = pt - v1;
	
	float normalizedDist = connect1.Dot (connect2) / 
					connect1.Dot ();// 
	// Clamp
	if (normalizedDist < 0.0F) normalizedDist = 0.0F;
	if (normalizedDist > 1.0F) normalizedDist = 1.0F;
	
	Vector3 ClosestPoint = v1 + normalizedDist * connect1.AsNormal ();// origin + n*v
	return ClosestPoint;
}

//---------------------------------------------------------

float	DistanceFromPointToLine (const Vector3& v1, // origin point
							   const Vector3& v2,
							   const Vector3& pt) //tested point
{
	Vector3 connect1 = v2 - v1;
	Vector3 connect2 = pt - v1;
	
	// projection equation
	float normalizedDist = connect1.Dot (connect2) / 
			connect1.Dot ();// |L||L|
	// Clamp
	if (normalizedDist < 0.0F) normalizedDist = 0.0F;
	if (normalizedDist > 1.0F) normalizedDist = 1.0F;
	
	// shadowed point
	Vector3 ClosestPoint = v1 + normalizedDist * connect1.AsNormal ();// origin + n*v
	float dist = (pt-ClosestPoint).Magnitude ();
	return dist;
}

//---------------------------------------------------------

Vector3	GetPointInTriangleClosestToPoint (const Vector3& point, 
										  const Vector3& pointInTriangle,
										  const Vector3& normalOfTriangle)
{
	// Calculate D. This is defined in most calculus books.
	float	D = pointInTriangle.Dot (normalOfTriangle);
	
	// this is your standard projection equation defined in pre-calculus and is the distance
	// normally needed.
	float	projectionLength = (normalOfTriangle.Dot (point) - D) / (normalOfTriangle.Dot ());
	
	return	point - projectionLength*normalOfTriangle;
}

//---------------------------------------------------------

bool	IsPointInTriangle (Vector3 a, Vector3 b, Vector3 c, Vector3 point)
{
	// translate all points to the origin which greatl simplifies the math
	a -= point, b -= point, c -= point;
	
	// computing the winding of the triangles formed with 
	// the point and the vertices of the triangle.
	Vector3 n1 = b.Cross (c);
	Vector3 n2 = c.Cross (a);
	
	// now we have two normals. If we compute the angle between them, then we can tell if they
	// face in the same relative direction (a opposed to being opposites)
	if (n1.Dot (n2) < 0) // is the cos of the angle is negative, then the angle>90∘ 
	{					 // and the point cannot be in the triangle
		return false;
	}
	
	Vector3 n3 = a.Cross (b);// compute the third normal
	// ensure it's winding/facing
	if (n1.Dot (n3) < 0) // is the cos of the angle is negative, then the angle>90∘ 
	{					 // and the point cannot be in the triangle
		return false;
	}
	
	return true;
}

//---------------------------------------------------------

// this is highly optimal and very fast.
float	GetSquareDistanceFromPointToLine (Vector3 v1, Vector3 v2, Vector3 pt)// shortest
{
	Vector3 line = v2 - v1, edge1 = pt - v1, edge2 = pt - v2;
	
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

Vector3	GetInPlanePortionOfVector (const Vector3& vect, const Vector3& PlaneNormal)
{
	float height = (vect.Dot (PlaneNormal));
	Vector3 shadow = vect - height * PlaneNormal;
	return shadow;
}

//---------------------------------------------------------

float	GetDistanceFromLineToLine (Vector3 v1, Vector3 v2, Vector3 pt)// shortest
{
	(void)v1; (void)v2; (void) pt;
	return 0.0;
}

//---------------------------------------------------------

float	GetDistanceFromPointToLine2D (Vector3 v1, Vector3 v2, Vector3 pt)// shortest
{
	// find 'normal' to line
	Vector3 v = v2 - v1;
	
	Vector3 Perpendicular (-v.y, v.x, 0);// negative inverse slope
	Perpendicular.Normalize ();// now we have a 'normal' to the line
	
	Vector3 Hypotenuse = pt - v1;
	Vector3 VectorDistance =  Hypotenuse.ProjectOn (Perpendicular);
	
	return VectorDistance.Magnitude();
}

//---------------------------------------------------------

Vector3	CreateVectorUpInHex (Vector3 startposition, float distance)
{
	return Vector3 (startposition.x, startposition.y+distance, startposition.z);
}

//---------------------------------------------------------

Vector3	CreateVectorUpLeftInHex (Vector3 startposition, float distance)
{
	float ThirtyDegrees = M_PI / 6;// 60∘
	float HeightBetweenCenters = sin (ThirtyDegrees) * distance;
	float WidthBetweenCenters = cos (ThirtyDegrees) * distance;
	
	return Vector3 (startposition.x-WidthBetweenCenters, startposition.y+HeightBetweenCenters, startposition.z);
}

//---------------------------------------------------------

Vector3	CreateVectorUpRightInHex (Vector3 startposition, float distance)
{
	float ThirtyDegrees = M_PI / 6;// 60∘
	float HeightBetweenCenters = sin (ThirtyDegrees) * distance;
	float WidthBetweenCenters = cos (ThirtyDegrees) * distance;
	
	return Vector3 (startposition.x+WidthBetweenCenters, startposition.y+HeightBetweenCenters, startposition.z);
}

//---------------------------------------------------------

Vector3	CreateVectorDownInHex (Vector3 startposition, float distance)
{
	return Vector3 (startposition.x, startposition.y-distance, startposition.z);
}

//---------------------------------------------------------

Vector3	CreateVectorDownLeftInHex (Vector3 startposition, float distance)
{
	float ThirtyDegrees = M_PI / 6;// 60∘
	float HeightBetweenCenters = sin (ThirtyDegrees) * distance;
	float WidthBetweenCenters = cos (ThirtyDegrees) * distance;
	
	return Vector3 (startposition.x-WidthBetweenCenters, startposition.y-HeightBetweenCenters, startposition.z);	
}

//---------------------------------------------------------

Vector3	CreateVectorDownRightInHex (Vector3 startposition, float distance)
{
	float ThirtyDegrees = M_PI / 6;// 60∘
	float HeightBetweenCenters = sin (ThirtyDegrees) * distance;
	float WidthBetweenCenters = cos (ThirtyDegrees) * distance;
	
	return Vector3 (startposition.x+WidthBetweenCenters, startposition.y-HeightBetweenCenters, startposition.z);
}

//---------------------------------------------------------

void	Random2dVector( Vector3& v, float magnitude )
{
	float angle = static_cast<float>( rand() % 720 ) *0.5f;
	float angleInRads = ConvertToRadians( angle );
	float x = cos( angleInRads ) * magnitude;
	float y = sin( angleInRads ) * magnitude;
	v.Set( x, y, 0 );
}
//---------------------------------------------------------
//---------------------------------------------------------
