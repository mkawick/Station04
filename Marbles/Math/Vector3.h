#pragma once

// -------------------------------------------------

struct Vector2
{
	Vector2 () : x(0), y(0) {}
	Vector2 ( float _x, float _y ) : x( _x ), y( _y ) {}
	void	Set (float _x, float _y) {x = _x, y = _y;}
	float x, y;
};

// -------------------------------------------------

class Vector3 
{
	//-------------------------------------------
public:
	Vector3 ();
	Vector3 (float _x, float _y, float _z);
	Vector3 (const Vector3& v);
	
	// construct a vector from two points in space
	Vector3 (const Vector3& v1, const Vector3& v2);
	
	void	Clear ();
	
	Vector3&				Set					(const Vector3& v);
	Vector3&				Set					(float _x, float _y, float _z);
	
	
	//-------------------------------------------
	
	// Unary operator
	Vector3				operator -			() const;
	
	// Assignment arithmetic operators
	Vector3&				operator =			(const Vector3& v);
	Vector3&				operator =			(const float v );
	Vector3&				operator -=			(const Vector3& v);
	Vector3&				operator +=			(const Vector3& v);
	Vector3				operator *=			(float v);
	Vector3				operator /=			(float v);
	
	// add to all members an offset
	Vector3				operator +=			(float v);	
	Vector3				operator -=			(float v);	
	Vector3				operator *			(float mult) const;
	
						operator float*		() const;
	
	// Binary arithmetic operators
	Vector3				operator +			(const Vector3& rhs) const;
	Vector3				operator -			(const Vector3& rhs) const;
	Vector3				operator *			(const Vector3& rhs) const;
	Vector3				operator /			(const Vector3& rhs) const;
	
	// Comparative operators
	bool 				operator ==			(const Vector3& rhs) const;
	bool 				operator !=			(const Vector3& rhs) const;
	static 
	bool 				IsEqual				(const Vector3& lhs, const Vector3& rhs, float epsilon);
	
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
	Vector3&				SetZero				();
	Vector3				AsNormal			() const;
	Vector3				AsReciprocal		() const;
	
	Vector3&				Normalize			();
	
	float				Dot					() const;// self only
	float				Dot					(const Vector3& v) const;
	float				DotProduct			(const Vector3& v) const {return Dot (v);}
	float				NormalizedDot		(const Vector3& v) const;
	Vector3				ProjectOn			(const Vector3& v) const;
	
	float				SquareMagnitude		() const;
	float				Magnitude			() const;
	float				Distance			() const {return Magnitude ();}
	float				Length				() const {return Magnitude ();}
	
	Vector3				Cross				(const Vector3& v) const;
	Vector3				NormalizedCross		(const Vector3& v) const;
	
	//------------------------------------------- 
	// misc utils
	// should be normalized, but will work for other vector types
	bool				IsPerpendicular		(const Vector3& a) const;
	
	// convert vector to an angle value using x,y
	float				GetAngle			() const;
	float				GetAngle			(const Vector3 &V) const;
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

Vector3 operator* (const float x, const Vector3& y);

template <typename T>
T	sign (const T& t)
{
	if (t >= 0)
		return static_cast <T> (1);
	return static_cast <T> (-1);
}

class Color4 :  public Vector3
{
public: 
	Color4() : Vector3(), alpha( 0 ), isAlphaSet( false ) {}

	float&	operator[]	( int i );
	float&	operator[]	( unsigned int i );

	union
	{
		float alpha;
		float a;
	};
	bool	isAlphaSet;
};

//Vector3	CreateVector3 (Vector3 center, 
// -------------------------------------------------
// -------------------------------------------------
