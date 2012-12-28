#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include "Math/Matrix4.h"
#include "Math/Angle.h"
#include "Math/MathFunctions.h"
#include "Math/Vector3.h"
#include "Math/Quaternion.h"

// -----------------------------------------------------------------------------

inline Matrix4 :: Matrix4		()
{ 
	SetIdentity(); 
}

inline Matrix4 :: Matrix4		(const Matrix4& m) : 
m11(m.m11),m12(m.m12),m13(m.m13),m14(m.m14), 
m21(m.m21),m22(m.m22),m23(m.m23),m24(m.m24),
m31(m.m31),m32(m.m32),m33(m.m33),m34(m.m34),
m41(m.m41),m42(m.m42),m43(m.m43),m44(m.m44) 
{}

inline Matrix4 :: Matrix4	(	const float _11, const float _12, const float _13, const float _14,
							 const float _21, const float _22, const float _23, const float _24,
							 const float _31, const float _32, const float _33, const float _34,
							 const float _41, const float _42, const float _43, const float _44)	: 
m11(_11),m12(_12),m13(_13),m14(_14),
m21(_21),m22(_22),m23(_23),m24(_24),
m31(_31),m32(_32),m33(_33),m34(_34),
m41(_41),m42(_42),m43(_43),m44(_44) 
{}
// -----------------------------------------------------------------------------
inline Matrix4& Matrix4::operator = (const Matrix4& mtx)
{
	memcpy(this,&mtx,sizeof(Matrix4));
	return *this;
}

// -----------------------------------------------------------------------------
inline Matrix4& Matrix4::operator += (const Matrix4& mtx)
{
	m11 += mtx.m11; m12 += mtx.m12; m13 += mtx.m13; m14 += mtx.m14;
	m21 += mtx.m21; m22 += mtx.m22; m23 += mtx.m23; m24 += mtx.m24;
	m31 += mtx.m31; m32 += mtx.m32; m33 += mtx.m33; m34 += mtx.m34;
	m41 += mtx.m41; m42 += mtx.m42; m43 += mtx.m43; m44 += mtx.m44;
	return *this;
}

// -----------------------------------------------------------------------------
inline Matrix4& Matrix4::operator -= (const Matrix4& mtx)
{
	m11 -= mtx.m11; m12 -= mtx.m12; m13 -= mtx.m13; m14 -= mtx.m14;
	m21 -= mtx.m21; m22 -= mtx.m22; m23 -= mtx.m23; m24 -= mtx.m24;
	m31 -= mtx.m31; m32 -= mtx.m32; m33 -= mtx.m33; m34 -= mtx.m34;
	m41 -= mtx.m41; m42 -= mtx.m42; m43 -= mtx.m43; m44 -= mtx.m44;
	return *this;
}

// -----------------------------------------------------------------------------
inline Matrix4& Matrix4::operator *= (const Matrix4& m)
{
	float _11 = m11*m.m11 + m12*m.m21 + m13*m.m31 + m14*m.m41;
	float _12 = m11*m.m12 + m12*m.m22 + m13*m.m32 + m14*m.m42;
	float _13 = m11*m.m13 + m12*m.m23 + m13*m.m33 + m14*m.m43;
	float _14 = m11*m.m14 + m12*m.m24 + m13*m.m34 + m14*m.m44;
	float _21 = m21*m.m11 + m22*m.m21 + m23*m.m31 + m24*m.m41;
	float _22 = m21*m.m12 + m22*m.m22 + m23*m.m32 + m24*m.m42;
	float _23 = m21*m.m13 + m22*m.m23 + m23*m.m33 + m24*m.m43;
	float _24 = m21*m.m14 + m22*m.m24 + m23*m.m34 + m24*m.m44;
	float _31 = m31*m.m11 + m32*m.m21 + m33*m.m31 + m34*m.m41;
	float _32 = m31*m.m12 + m32*m.m22 + m33*m.m32 + m34*m.m42;
	float _33 = m31*m.m13 + m32*m.m23 + m33*m.m33 + m34*m.m43;
	float _34 = m31*m.m14 + m32*m.m24 + m33*m.m34 + m34*m.m44;
	float _41 = m41*m.m11 + m42*m.m21 + m43*m.m31 + m44*m.m41;
	float _42 = m41*m.m12 + m42*m.m22 + m43*m.m32 + m44*m.m42;
	float _43 = m41*m.m13 + m42*m.m23 + m43*m.m33 + m44*m.m43;
	float _44 = m41*m.m14 + m42*m.m24 + m43*m.m34 + m44*m.m44;
	
	m11 = _11; m12 = _12; m13 = _13; m14 = _14;
	m21 = _21; m22 = _22; m23 = _23; m24 = _24;
	m31 = _31; m32 = _32; m33 = _33; m34 = _34;
	m41 = _41; m42 = _42; m43 = _43; m44 = _44;
	
	return *this;
}

// -----------------------------------------------------------------------------
inline Matrix4& Matrix4::operator *= ( const float value )
{
	// #if CORE_PS3
	// 			*this *= value;
	// #else
	m11 *= value; m12 *= value; m13 *= value; m14 *= value;
	m21 *= value; m22 *= value; m23 *= value; m24 *= value;
	m31 *= value; m32 *= value; m33 *= value; m34 *= value;
	m41 *= value; m42 *= value; m43 *= value; m44 *= value;
	// #endif
	return *this;
}

// -----------------------------------------------------------------------------
inline Matrix4& Matrix4::operator /= ( const float value )
{
	assert (value != 0.0f);//, "division by zero");
	// #if CORE_PS3
	// 			*this /= value;
	// #else
	m11 /= value; m12 /= value; m13 /= value; m14 /= value;
	m21 /= value; m22 /= value; m23 /= value; m24 /= value;
	m31 /= value; m32 /= value; m33 /= value; m34 /= value;
	m41 /= value; m42 /= value; m43 /= value; m44 /= value;
	
	// #endif
	return *this;
}

// unary operators 
inline Matrix4 Matrix4::operator - () const
{
	return Matrix4(*this) *= -1.f;
}

// -----------------------------------------------------------------------------
inline Matrix4 Matrix4::operator + (const Matrix4& mtx) const
{
	return Matrix4(*this) += mtx;
}

// -----------------------------------------------------------------------------
inline Matrix4 Matrix4::operator - (const Matrix4& mtx) const
{
	return Matrix4(*this) -= mtx;
}

// -----------------------------------------------------------------------------
inline Matrix4 Matrix4::operator * (const Matrix4& mtx) const
{
	return Matrix4(*this) *= mtx;
}

//---------------------------------------------------------------------------------------------------------------------------------
inline const bool Matrix4::operator == (const Matrix4& m) const
{
	return 
	EqualRelative(m11,m.m11) &&
	EqualRelative(m12,m.m12) &&
	EqualRelative(m13,m.m13) &&
	EqualRelative(m14,m.m14) &&
	EqualRelative(m21,m.m21) &&
	EqualRelative(m22,m.m22) &&
	EqualRelative(m23,m.m23) &&
	EqualRelative(m24,m.m24) &&
	EqualRelative(m31,m.m31) &&
	EqualRelative(m32,m.m32) &&
	EqualRelative(m33,m.m33) &&
	EqualRelative(m34,m.m34) &&
	EqualRelative(m41,m.m41) &&
	EqualRelative(m42,m.m42) &&
	EqualRelative(m43,m.m43) &&
	EqualRelative(m44,m.m44);
}

inline const bool Matrix4::operator != (const Matrix4& m) const
{
	return !(*this == m);
}

// -----------------------------------------------------------------------------

inline float Matrix4::GetE(int row, int col) const
{
	return static_cast<const float*>(&m11)[row*4+col];
	//return (*this)[row][col];
}

inline void Matrix4::SetE(int row, int col, float val)
{
	static_cast<float*>(&m11)[row*4+col] = val;
	//(*this)[row][col] = val;
}

// -----------------------------------------------------------------------------
Matrix4& Matrix4::SetIdentity()
{
	m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = m41 = m42 = m43 = 0.f;
	m11 = m22 = m33 = m44 = 1.f;
	return *this;
}

Matrix4& Matrix4::SetTranslation (float x, float y, float z)
{
	SetRow(3,Vector3(x,y,z));
	return *this;
}

Matrix4& Matrix4::SetTranslation (const Vector3& v)
{
	SetRow(3,v);
	return *this;
}

Matrix4& Matrix4::SetYawPitchRoll (Angle yaw, Angle pitch, Angle roll)
{
	float s0 = sin(pitch.AsRadians());
	float s1 = sin(roll.AsRadians());
	float s2 = sin(yaw.AsRadians());
	float c0 = cos(pitch.AsRadians());
	float c1 = cos(roll.AsRadians());
	float c2 = cos(yaw.AsRadians());
	
	m11 = s0*s1*s2 + c1*c2;
	m12 = c0*s2;
	m13 = s0*c1*s2 - s1*c2;
	
	m21 = s0*s1*c2 - c1*s2;
	m22 = c0*c2;
	m23 = s1*s2 + s0*c1*c2;
	
	m31 = c0*s1;
	m32 = -s0;
	m33 = c0*c1;
	
	return *this;
}

Matrix4& Matrix4::SetEulerXYZ (Angle rx, Angle ry, Angle rz)
{
	float s0 = sin(rx.AsRadians());
	float s1 = sin(ry.AsRadians());
	float s2 = sin(rz.AsRadians());
	float c0 = cos(rx.AsRadians());
	float c1 = cos(ry.AsRadians());
	float c2 = cos(rz.AsRadians());
	
	m11 = c1*c2;
	m12 = c1*s2;
	m13 = -s1;
	
	m21 = s0*s1*c2 - c0*s2;
	m22 = s0*s1*s2 + c0*c2;
	m23 = s0*c1;
	
	m31 = s0*s2 + c0*s1*c2;
	m32 = c0*s1*s2 - s0*c2;
	m33 = c0*c1;
	
	return *this;
}

// -----------------------------------------------------------------------------

inline Vector3 Matrix4::GetRow (const int row) const
{
	return Vector3(GetE(row,0), GetE(row,1), GetE(row,2));
}

inline void Matrix4::SetRow (int row, const Vector3& newVec)
{
	SetE(row, 0, newVec.X());
	SetE(row, 1, newVec.Y());
	SetE(row, 2, newVec.Z());
}

inline Vector3 Matrix4::GetCol (const int col) const
{
	return Vector3(GetE(0, col), GetE(1, col), GetE(2, col));
}

inline void Matrix4::SetCol(int col, const Vector3& newVec)
{
	SetE(0, col, newVec.X());
	SetE(1, col, newVec.Y());
	SetE(2, col, newVec.Z());
}


inline Vector3 Matrix4::GetXbasis() const
{
	return GetCol(0);
}

inline Vector3 Matrix4::GetYbasis() const
{
	return GetCol(1);
}

inline Vector3 Matrix4::GetZbasis() const
{
	return GetCol(2);
}

inline const Matrix4& Matrix4::SetXbasis(const Vector3& newVec)
{
	SetCol(0, newVec);
	return *this;
}

inline const Matrix4& Matrix4::SetYbasis(const Vector3& newVec)
{
	SetCol(1, newVec);
	return *this;
}

inline const Matrix4& Matrix4::SetZbasis(const Vector3& newVec)
{
	SetCol(2, newVec);
	return *this;
}

inline float Matrix4::Determinant () const
{
	return
	(m11 * m22 - m12 * m21) * (m33 * m44 - m34 * m43)
	-(m11 * m23 - m13 * m21) * (m32 * m44 - m34 * m42)
	+(m11 * m24 - m14 * m21) * (m32 * m43 - m33 * m42)
	+(m12 * m23 - m13 * m22) * (m31 * m44 - m34 * m41)
	-(m12 * m24 - m14 * m22) * (m31 * m43 - m33 * m41)
	+(m13 * m24 - m14 * m23) * (m31 * m42 - m32 * m41);
}

// -----------------------------------------------------------------------------
inline Matrix4 Matrix4::Inverse() const
{
	float det = Determinant();
	
	assert (det != 0.0f);// "division by zero");
	det = 1.0f/det;
	float _11 = det * (m22*(m33*m44 - m34*m43) + m23*(m34*m42 - m32*m44) + m24*(m32*m43 - m33*m42));
	float _12 = det * (m32*(m13*m44 - m14*m43) + m33*(m14*m42 - m12*m44) + m34*(m12*m43 - m13*m42));
	float _13 = det * (m42*(m13*m24 - m14*m23) + m43*(m14*m22 - m12*m24) + m44*(m12*m23 - m13*m22));
	float _14 = det * (m12*(m24*m33 - m23*m34) + m13*(m22*m34 - m24*m32) + m14*(m23*m32 - m22*m33));
	float _21 = det * (m23*(m31*m44 - m34*m41) + m24*(m33*m41 - m31*m43) + m21*(m34*m43 - m33*m44));
	float _22 = det * (m33*(m11*m44 - m14*m41) + m34*(m13*m41 - m11*m43) + m31*(m14*m43 - m13*m44));
	float _23 = det * (m43*(m11*m24 - m14*m21) + m44*(m13*m21 - m11*m23) + m41*(m14*m23 - m13*m24));
	float _24 = det * (m13*(m24*m31 - m21*m34) + m14*(m21*m33 - m23*m31) + m11*(m23*m34 - m24*m33));
	float _31 = det * (m24*(m31*m42 - m32*m41) + m21*(m32*m44 - m34*m42) + m22*(m34*m41 - m31*m44));
	float _32 = det * (m34*(m11*m42 - m12*m41) + m31*(m12*m44 - m14*m42) + m32*(m14*m41 - m11*m44));
	float _33 = det * (m44*(m11*m22 - m12*m21) + m41*(m12*m24 - m14*m22) + m42*(m14*m21 - m11*m24));
	float _34 = det * (m14*(m22*m31 - m21*m32) + m11*(m24*m32 - m22*m34) + m12*(m21*m34 - m24*m31));
	float _41 = det * (m21*(m33*m42 - m32*m43) + m22*(m31*m43 - m33*m41) + m23*(m32*m41 - m31*m42));
	float _42 = det * (m31*(m13*m42 - m12*m43) + m32*(m11*m43 - m13*m41) + m33*(m12*m41 - m11*m42));
	float _43 = det * (m41*(m13*m22 - m12*m23) + m42*(m11*m23 - m13*m21) + m43*(m12*m21 - m11*m22));
	float _44 = det * (m11*(m22*m33 - m23*m32) + m12*(m23*m31 - m21*m33) + m13*(m21*m32 - m22*m31));
	return Matrix4(_11,_12,_13,_14,
				   _21,_22,_23,_24,
				   _31,_32,_33,_34,
				   _41,_42,_43,_44);
}

inline Matrix4 Matrix4::Transpose() const
{
	return Matrix4(		
				   m11,	m21,	m31,	m41,
				   m12,	m22,	m32,	m42,
				   m13,	m23,	m33,	m43,
				   m14,	m24,	m34,	m44	);
}

inline Vector3	Matrix4::GetTranslation () const
{
	return GetRow(3);
}

inline Vector3	Matrix4::GetScale () const // retrieve the scale vector only
{
	return Vector3 (m11, m22, m33);
}
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

inline Matrix4	Matrix4::RotationX (Angle angle)
{
	const float c = cos(angle.AsRadians());
	const float s = sin(angle.AsRadians());
	
	return Matrix4(		
				   1,	0,	0,	0,
				   0,	c,	s,	0,
				   0,	-s,	c,	0,
				   0,	0,	0,	1	);
}

inline Matrix4	Matrix4::RotationY (Angle angle)
{
	const float c = cos(angle.AsRadians());
	const float s = sin(angle.AsRadians());
	
	return Matrix4(		
				   c,	0,	-s,	0,
				   0,	1,	0,	0,
				   s,	0,	c,	0,
				   0,	0,	0,	1	);
}

inline Matrix4	Matrix4::RotationZ (Angle angle)
{
	const float c = cos(angle.AsRadians());
	const float s = sin(angle.AsRadians());
	
	return Matrix4(		c,	s,	0,	0,
				   -s,	c,	0,	0,
				   0,	0,	1,	0,
				   0,	0,	0,	1	);
}

inline Matrix4	Matrix4::Rotation	(Angle x, Angle y, Angle z)
{
	Matrix4 mat;
	mat.SetEulerXYZ (x, y, z);
	return Matrix4 (mat);
}

inline Matrix4	Matrix4::Rotation	(const Vector3& rotation)
{
	Matrix4 mat;
	mat.SetEulerXYZ (rotation.X(), rotation.Y(), rotation.Z());
	return Matrix4 (mat);
}


inline	Matrix4			Matrix4 :: Rotation	(const Quaternion& q)
{
	// formulas copied entirely from "Quaternions and Rotation Sequences" by Kuipers
	float q0 = q.W();
	float q1 = q.X();
	float q2 = q.Y();
	float q3 = q.Z();
	
	float m11 = 2.0f * (q0 * q0) + 2.0f * (q1 * q1) - 1.0f;// note the squared
	float m12 = 2.0f * (q1 * q2) + 2.0f * (q0 * q3);
	float m13 = 2.0f * (q1 * q3) - 2.0f * (q0 * q2); // note the minus here
	
	float m21 = 2.0f * (q1 * q2) - 2.0f * (q0 * q3);
	float m22 = 2.0f * (q0 * q0) + 2.0f * (q2 * q2) - 1.0f;// note the squared
	float m23 = 2.0f * (q2 * q3) + 2.0f * (q0 * q1);
	
	float m31 = 2.0f * (q1 * q3) + 2.0f * (q0 * q2);
	float m32 = 2.0f * (q2 * q3) - 2.0f * (q0 * q1);
	float m33 = 2.0f * (q0 * q0) + 2.0f * (q3 * q3) - 1.0f;// note the squared
	
	return Matrix4(m11, m12, m13, 0.0f,
				   m21, m22, m23, 0.0f,
				   m31, m32, m33, 0.0f,
				   0.0f, 0.0f, 0.0f, 1.0f);
	// 	RigidMatrix rm (q,Vector3(0.0f,0.0f,0.0f));
	// 	return rm.AsMatrix4();
}

Matrix4	Matrix4::TranslationX (float dist)
{
	return Matrix4(		
				   1,		0,		0,		0,
				   0,		1,		0,		0,
				   0,		0,		1,		0,
				   dist,	0.0,	0.0,	1	);
}

Matrix4	Matrix4::TranslationY (float dist)
{
	return Matrix4(		
				   1,		0,		0,		0,
				   0,		1,		0,		0,
				   0,		0,		1,		0,
				   0.0,	dist,	0.0,	1	);
}

Matrix4	Matrix4::TranslationZ (float dist)
{
	return Matrix4(		
				   1,		0,		0,		0,
				   0,		1,		0,		0,
				   0,		0,		1,		0,
				   0.0,	0.0,	dist,	1	);
}

Matrix4	Matrix4::Translation (float x, float y, float z)
{
	return Matrix4(		
				   1,	0,	0,	0,
				   0,	1,	0,	0,
				   0,	0,	1,	0,
				   x,	y,	z,	1	);
}

Matrix4	Matrix4::Translation (const Vector3& v)
{
	return Matrix4(		
				   1,		0,		0,		0,
				   0,		1,		0,		0,
				   0,		0,		1,		0,
				   v.X(),	v.Y(),	v.Z(),	1	);
}


Matrix4	Matrix4::ScaleX   (float x)
{
	return Matrix4(		
				   x,		0,		0,		0,
				   0,		1,		0,		0,
				   0,		0,		1,		0,
				   0,		0,		0,		1	);
}
inline Matrix4	Matrix4::ScaleY   (float y)
{
	return Matrix4(		
				   1,		0,		0,		0,
				   0,		y,		0,		0,
				   0,		0,		1,		0,
				   0,		0,		0,		1	);
}
inline Matrix4	Matrix4::ScaleZ   (float z)
{
	return Matrix4(		
				   1,		0,		0,		0,
				   0,		1,		0,		0,
				   0,		0,		z,		0,
				   0,		0,		0,		1	);
}
inline Matrix4	Matrix4::Scale	(float x, float y, float z)// each of these is a scale factor
{
	return Matrix4(		
				   x,		0,		0,		0,
				   0,		y,		0,		0,
				   0,		0,		z,		0,
				   0,		0,		0,		1	);
}
inline Matrix4	Matrix4::Scale	(const Vector3& s)
{
	return Matrix4(	
				   s.X(),		0,		0,		0,
				   0,		s.Y(),		0,		0,
				   0,			0,	s.Z(),		0,
				   0,			0,		0,		1	);
}
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
// friends ---------------------------------------------------------------------
inline Matrix4 operator * ( const float value, const Matrix4& m )
{
	return Matrix4(m) *= value;
}

inline Matrix4 operator * (const Matrix4& m, const float value)
{
	return Matrix4(m) *= value;
}

inline Matrix4 operator / (const Matrix4& m, const float value)
{
	return Matrix4(m) /= value;
}

// -----------------------------------------------------------------------------

inline Vector3 operator * ( const Vector3& vec, const Matrix4& mtx)
{
	float x = vec.X();
	float y = vec.Y();
	float z = vec.Z();
	
	return Vector3(		mtx.m11 * x + mtx.m21 * y + mtx.m31 * z + mtx.m41, 
				   mtx.m12 * x + mtx.m22 * y + mtx.m32 * z + mtx.m42, 
				   mtx.m13 * x + mtx.m23 * y + mtx.m33 * z + mtx.m43	);
}


// -----------------------------------------------------------------------------


Matrix4		FromQuaternion( const Quaternion& quat )
{
	Matrix4 m;
	m.Rotation( quat );
	return m;
}

// -----------------------------------------------------------------------------

