#include "Math\Angle.h"
#include "Math\MathFunctions.h"

#include <cmath>

Angle Angle::Deg0		= Angle::FromDegrees(0.f);
Angle Angle::DegHalf	= Angle::FromDegrees(0.5f);
Angle Angle::Deg1		= Angle::FromDegrees(1.f);
Angle Angle::Deg5		= Angle::FromDegrees(5.f);
Angle Angle::Deg10		= Angle::FromDegrees(10.f);
Angle Angle::Deg15		= Angle::FromDegrees(15.f);
Angle Angle::Deg30		= Angle::FromDegrees(30.f);
Angle Angle::Deg45		= Angle::FromDegrees(45.f);
Angle Angle::Deg60		= Angle::FromDegrees(60.f);
Angle Angle::Deg90		= Angle::FromDegrees(90.f);
Angle Angle::Deg120		= Angle::FromDegrees(120.f);
Angle Angle::Deg135		= Angle::FromDegrees(135.f);
Angle Angle::Deg180		= Angle::FromDegrees(180.f);

// -----------------------------------------------------------------------------

extern const float TO_DEGREES;
extern const float TO_RADIANS;
extern const float PI;
extern const float TWO_PI;

// -----------------------------------------------------------------------------
Angle::Angle( const float radian )
: mRadian( radian )
{
	Normalize();
}

// -----------------------------------------------------------------------------
Angle::Angle( const Angle& angle )
: mRadian(angle.mRadian)
{
}

// -----------------------------------------------------------------------------
Angle& Angle::operator = ( const Angle& angle)
{ 
	mRadian = angle.mRadian;
	return *this;
}

// -----------------------------------------------------------------------------
Angle Angle::operator -() const
{
	return Angle(-this->mRadian);
}

// -----------------------------------------------------------------------------
Angle Angle::operator + ( const Angle& angle) const
{
	return Angle(*this) += angle;
}

// -----------------------------------------------------------------------------
Angle Angle::operator - ( const Angle& angle) const
{
	return Angle(*this) -= angle;
}

// -----------------------------------------------------------------------------
float Angle::operator / ( const Angle& angle) const
{
	return mRadian / angle.mRadian;
}

// -----------------------------------------------------------------------------
Angle Angle::operator * ( const float scaler ) const
{
	return Angle(*this) *= scaler;
}

// -----------------------------------------------------------------------------
Angle Angle::operator / ( const float scaler ) const
{
	return Angle(*this) /= scaler;
}


// -----------------------------------------------------------------------------
Angle& Angle::operator += ( const Angle& angle)
{
	mRadian += angle.mRadian;
	Normalize();
	return *this;
}

// -----------------------------------------------------------------------------
Angle& Angle::operator -= ( const Angle& angle)
{
	mRadian -= angle.mRadian;
	Normalize();
	return *this;
}

// -----------------------------------------------------------------------------
Angle& Angle::operator *= ( const float scaler)
{
	mRadian *= scaler;
	return *this;
}

// -----------------------------------------------------------------------------
Angle& Angle::operator /= ( const float scaler)
{
	mRadian /= scaler;
	return *this;
}

// -----------------------------------------------------------------------------
bool Angle::operator ==	( const Angle& angle) const
{
	return mRadian == angle.mRadian;
}

// -----------------------------------------------------------------------------
bool Angle::operator !=	( const Angle& angle) const
{
	return mRadian != angle.mRadian;
}

// -----------------------------------------------------------------------------
bool Angle::operator <=	( const Angle& angle) const
{
	return mRadian <= angle.mRadian;
}

// -----------------------------------------------------------------------------
bool Angle::operator >=	( const Angle& angle) const
{
	return mRadian >= angle.mRadian;
}

// -----------------------------------------------------------------------------
bool Angle::operator < ( const Angle& angle) const
{
	return mRadian < angle.mRadian;
}

// -----------------------------------------------------------------------------
bool Angle::operator > ( const Angle& angle) const
{
	return mRadian > angle.mRadian;
}

// -----------------------------------------------------------------------------
float Angle::AsRadians() const
{
	return mRadian;
}

// -----------------------------------------------------------------------------
float Angle::AsDegrees() const
{
	return ConvertToDegrees (mRadian);
}

// -----------------------------------------------------------------------------
float Angle::AsPositiveRadians() const
{
	return Select (mRadian, mRadian, static_cast <float> (mRadian + M_PI*2));
}

// -----------------------------------------------------------------------------
float Angle::AsPositiveDegrees() const
{
	return ConvertToDegrees (AsPositiveRadians());
}

// -----------------------------------------------------------------------------
Angle Angle::FromDegrees(const float degrees)
{
	return Angle(ConvertToRadians (degrees));
}

// -----------------------------------------------------------------------------
Angle Angle::FromRadians(const float radians)
{
	return Angle(radians);
}

// -----------------------------------------------------------------------------
void Angle::SetFromDegrees(const float degrees)
{
	mRadian = ConvertToRadians (degrees);
}

// -----------------------------------------------------------------------------
void Angle::Normalize()
{
	// Review(danc): We normalize to [PI -PI] inclusive (ie both PI and -PI are acceptable values)
	while( mRadian > M_PI )
	{
		mRadian -= M_PI*2;
	}
	while( mRadian < -M_PI )
	{
		mRadian += M_PI*2;
	}
}

// -----------------------------------------------------------------------------
Angle operator * ( const float scaler, const Angle& angle)
{
	return Angle(angle) *= scaler;
}


// -----------------------------------------------------------------------------

