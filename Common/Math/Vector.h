/*
 *  Vector.h
 *  Station05
 */

#pragma once

// -------------------------------------------------

struct Vector2D
{
	Vector2D () : x(0), y(0) {}
	Vector2D ( float _x, float _y ) : x( _x ), y( _y ) {}
	void	Set (float _x, float _y) {x = _x, y = _y;}
	float x, y;
};

// -------------------------------------------------

class Vector 
{
	//-------------------------------------------
public:
	Vector ();
	Vector (float _x, float _y, float _z);
	Vector (const Vector& v);
	
	// construct a vector from two points in space
	Vector (const Vector& v1, const Vector& v2);
	
	void	Clear ();
	
	Vector&				Set					(const Vector& v);
	Vector&				Set					(float _x, float _y, float _z);
	
	
	//-------------------------------------------
	
	// Unary operator
	Vector				operator -			() const;
	
	// Assignment arithmetic operators
	Vector&				operator =			(const Vector& v);
	Vector&				operator =			(const float v );
	Vector&				operator -=			(const Vector& v);
	Vector&				operator +=			(const Vector& v);
	Vector				operator *=			(float v);
	Vector				operator /=			(float v);
	
	// add to all members an offset
	Vector				operator +=			(float v);	
	Vector				operator -=			(float v);	
	Vector				operator *			(float mult) const;
	
						operator float*		() const;
	
	// Binary arithmetic operators
	Vector				operator +			(const Vector& rhs) const;
	Vector				operator -			(const Vector& rhs) const;
	Vector				operator *			(const Vector& rhs) const;
	Vector				operator /			(const Vector& rhs) const;
	
	// Comparative operators
	bool 				operator ==			(const Vector& rhs) const;
	bool 				operator !=			(const Vector& rhs) const;
	static 
	bool 				IsEqual				(const Vector& lhs, const Vector& rhs, float epsilon);
	
	float&	operator[]	( int i );
	float&	operator[]	( unsigned int i );
	
	//-------------------------------------------
	
	float				X					() const {return x;}
	float				Y					() const {return y;}
	float				Z					() const {return z;}
	float				X					(float newX);
	float				Y					(float newY);
	float				Z					(float newZ);
	
	//-------------------------------------------
	Vector&				SetZero				();
	Vector				AsNormal			() const;
	Vector				AsReciprocal		() const;
	
	Vector&				Normalize			();
	
	float				Dot					() const;// self only
	float				Dot					(const Vector& v) const;
	float				DotProduct			(const Vector& v) const {return Dot (v);}
	float				NormalizedDot		(const Vector& v) const;
	Vector				ProjectOn			(const Vector& v) const;
	
	float				SquareMagnitude		() const;
	float				Magnitude			() const;
	float				Distance			() const {return Magnitude ();}
	float				Length				() const {return Magnitude ();}
	
	Vector				Cross				(const Vector& v) const;
	Vector				NormalizedCross		(const Vector& v) const;
	
	//------------------------------------------- 
	// misc utils
	// should be normalized, but will work for other vector types
	bool				IsPerpendicular		(const Vector& a) const;
	
	// convert vector to an angle value using x,y
	float				GetAngle			() const;
	float				GetAngle			(const Vector &V) const;
	void				ClampMagnitude		(float MinDist, float MaxDist);
	
	//-------------------------------------------
	
	void	ConvertToDegrees ();
	void	ConvertToRadians ();
	
	void	ConvertAngleInDegreesToXYComponents (float Angle);
	
public:
	union
	{
		float x;
		float r;
		float theta;
		float pitch;
	};
	union
	{
		float y;
		float g;
		float phi;
		float yaw;
	};
	union
	{
		float z;
		float b;
		float psi;
		float roll;
	};
};

Vector operator* (const float x, const Vector& y);

template <typename T>
T	sign (const T& t)
{
	if (t >= 0)
		return static_cast <T> (1);
	return static_cast <T> (-1);
}

class ColorVector :  public Vector
{
public: 
	ColorVector() : Vector(), alpha( 0 ), isAlphaSet( false ) {}

	float&	operator[]	( int i );
	float&	operator[]	( unsigned int i );

	union
	{
		float alpha;
		float a;
	};
	bool	isAlphaSet;
};

//Vector	CreateVector (Vector center, 
// -------------------------------------------------
// -------------------------------------------------
