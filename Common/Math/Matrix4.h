/*
 *  Matrix4.h
 *  Station05
 */

#pragma once

class Angle;
class Vector;
class Quaternion;
//----------------------------------------------------------

class Matrix4
{
public:
	Matrix4		();
	Matrix4		(const Matrix4& restrict);
	Matrix4		(const float _11, const float _12, const float _13, const float _14,
				 const float _21, const float _22, const float _23, const float _24,
				 const float _31, const float _32, const float _33, const float _34,
				 const float _41, const float _42, const float _43, const float _44);
	
	// Assignment operators
	Matrix4&				operator =			(const Matrix4& restrict);
	Matrix4&				operator +=			(const Matrix4& restrict);
	Matrix4&				operator -=			(const Matrix4& restrict);
	Matrix4&				operator *=			(const Matrix4& restrict);
	Matrix4&				operator *=			(const float f);
	Matrix4&				operator /=			(const float f);
	
	// Unary arithmetic operators
	//Matrix4					operator +			() const;// completely unnecessary
	Matrix4					operator -			() const;
	
	// Binary arithmetic operators
	Matrix4					operator +			(const Matrix4& mat) const;
	Matrix4					operator -			(const Matrix4& mat) const;
	Matrix4					operator *			(const Matrix4& mat) const;
	
	// Comparison operators
	const bool				operator ==			(const Matrix4& mat) const;
	const bool				operator !=			(const Matrix4& mat) const;
	
	// Friend operators
	friend Matrix4			operator *			(const Matrix4& mat, const float f);
	friend Matrix4			operator *			(const float, const Matrix4& mat);
	friend Vector			operator *			(const Vector& vect, const Matrix4& mat);
	friend Matrix4			operator /			(const Matrix4& mat, const float f);
	
	// access operators	
	float					GetE				(int row, int col) const;
	void					SetE				(int row, int col, float val);
	
	Vector					GetRow				(const int row) const;
	void					SetRow				(int row, const Vector& vect);
	
	Vector					GetCol				(const int col) const;
	void					SetCol				(int col, const Vector& vect);
	
	Vector					GetXbasis			() const;
	Vector					GetYbasis			() const;
	Vector					GetZbasis			() const;
	const Matrix4&			SetXbasis			(const Vector& vect);
	const Matrix4&			SetYbasis			(const Vector& vect);
	const Matrix4&			SetZbasis			(const Vector& vect);
	
	Matrix4&				SetIdentity			();
	Matrix4&				SetTranslation		(float x, float y, float z);
	Matrix4&				SetTranslation		(const Vector& vect);
	Matrix4&				SetYawPitchRoll		(Angle yaw, Angle pitch, Angle roll);
	Matrix4&				SetEulerXYZ			(Angle rx, Angle ry, Angle rz);
	
	float					Determinant			() const;
	Matrix4					Inverse				() const;
	Matrix4					Transpose			() const;
	
	Vector					GetTranslation		() const; // retrieve the translation vector only
	Vector					GetScale			() const; // retrieve the scale vector only
	
public:
	// ------------------------- Helper functions for Matrix4 ---------------------------------------
	static	Matrix4			RotationX   (Angle angle);
	static	Matrix4			RotationY   (Angle angle);
	static	Matrix4			RotationZ   (Angle angle);
	static	Matrix4			Rotation	(Angle x, Angle y, Angle z);
	static	Matrix4			Rotation	(const Vector& vect); //rotation angle
	static	Matrix4			Rotation	(const Quaternion& quat); // rotation angle
	
	static	Matrix4			TranslationX (float dist);
	static	Matrix4			TranslationY (float dist);
	static	Matrix4			TranslationZ (float dist);
	static	Matrix4			Translation	 (float x, float y, float z);
	static	Matrix4			Translation  (const Vector& vect);// considered to be a distance vector (normalized will probably not work well)
	
	static	Matrix4			ScaleX   (float factor);
	static	Matrix4			ScaleY   (float factor);
	static	Matrix4			ScaleZ   (float factor);
	static	Matrix4			Scale	 (float x, float y, float z);// each of these is a scale factor
	static	Matrix4			Scale	 (const Vector& vect); // scale_factor
	
	static const int sSize = 4;
	
protected:
	// the matrix itself.
	float					m11, m12, m13, m14;
	float					m21, m22, m23, m24;
	float					m31, m32, m33, m34;
	float					m41, m42, m43, m44;
};

//----------------------------------------------------------

