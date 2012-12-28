#pragma once

class Angle;
class Vector3;

//---------------------------------------------------------------------------------------------------------------------------------

class Quaternion
{
public:
	Quaternion	();
	Quaternion	(float x, float y, float z, float w);
	Quaternion	(const Vector3& v, float w);
	Quaternion	(Angle yaw, Angle pitch, Angle roll);
	Quaternion (float yaw, float pitch, float roll);

	Quaternion	(const Quaternion&);
	
	// Assignment operators
	Quaternion&			operator	=		(const Quaternion& restrict);
	Quaternion&			operator	+=		(const Quaternion& restrict);
	Quaternion&			operator	-=		(const Quaternion& restrict);
	Quaternion&			operator	*=		(const Quaternion& restrict);
	Quaternion&			operator	*=		(float);
	Quaternion&			operator	/=		(float);
	
	// Unary operators
	Quaternion			operator	-		() const;
	
	// Binary operators
	Quaternion			operator	+		(const Quaternion& restrict) const;
	Quaternion			operator	-		(const Quaternion& restrict) const;
	Quaternion			operator	*		(const Quaternion& restrict) const;
	Quaternion			operator	*		(float) const;
	Quaternion			operator	/		(float) const;
	
	// Extern functions
	friend 
	Quaternion			operator	*		(float f, const Quaternion& restrict );
	friend 
	Vector3				operator	*		(const Vector3&  vect, const Quaternion& quat);
	friend 
	Quaternion			Slerp				(Quaternion quat1, Quaternion quat2, float fraction);
	friend 
	Quaternion			SlerpPrecise		(const Quaternion& quat1, const Quaternion& quat2, float fraction);
	friend 
	Quaternion			CreateQuaternionFromVectors (const Vector3& vect1, const Vector3& vect2);
	
	// Comparison
	bool				operator	==		(const Quaternion& quat) const;
	bool				operator	!=		(const Quaternion& quat) const;
	
	// Other operations
	float				Dot					(const Quaternion& quat) const;
	Quaternion&			Set					(float _x, float _y, float _z, float _w);
	Quaternion&			SetYawPitchRoll		(Angle yaw, Angle pitch, Angle roll);
	Quaternion&			SetYawPitchRoll		(float yaw, float pitch, float roll);
	Quaternion&			AxisAngle			(const Vector3& vect, Angle angle); // axis MUST be normalized!
	Quaternion&			SetIdentity			(); 
	Quaternion&			Normalize			();
	Quaternion&			Invert				();
	Quaternion			AsNormal			() const;
	Quaternion			AsConjugate			() const; 
	Quaternion			AsInverse			() const; 
	float				Magnitude			() const; 
	float				SquareMagnitude		() const;
	bool				IsNormal			() const;
	
	float				X					() const {return x;}
	float				Y					() const {return y;}
	float				Z					() const {return z;}
	float				W					() const {return w;}
	
	void				X					(float valX);
	void				Y					(float valY);
	void				Z					(float valZ);
	void				W					(float valW);
	
	void				Validate			() const;
	
private:
	Quaternion&			ReflectYawPitchRoll		(float yaw, float pitch, float roll);
public:
	float				x, y, z, w;
	
public:
	static const Quaternion IDENTITY;

};

//---------------------------------------------------------------------------------------------------------------------------------
// utility functions
//---------------------------------------------------------------------------------------------------------------------------------

Quaternion			operator * (float f, const Quaternion& restrict);
Vector3				operator * (const Vector3& v, const Quaternion& restrict);
Quaternion			Slerp (Quaternion q1, Quaternion q2, float fraction);
Quaternion			SlerpPrecise (const Quaternion& quat1, const Quaternion& quat2, float fraction);
Quaternion			CreateQuaternionFromVectors(const Vector3& vect1, const Vector3& vect2);	
//---------------------------------------------------------------------------------------------------------------------------------

