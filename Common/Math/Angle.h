/*
 *  Angle.h
 *  SDL04_Station
 */

#pragma once

class Angle
{
public:
	
	Angle			( const float radian = 0.0f );
	Angle			( const Angle& angle );
	
	Angle&			operator =		( const Angle& angle );
	Angle			operator -		() const;
	Angle			operator +		( const Angle& angle) const;
	Angle			operator -		( const Angle& angle) const;
	Angle			operator *		( const float scaler) const;
	Angle			operator /		( const float scaler) const;
	float			operator /		( const Angle& angle) const;
	
	inline          Angle&			operator +=		( const Angle& angle);
	inline          Angle&			operator -=		( const Angle& angle);
	inline          Angle&			operator *=		( const float scaler);
	inline          Angle&			operator /=		( const float scaler);
	
	bool			operator ==		( const Angle& angle) const;
	bool			operator !=		( const Angle& angle) const;
	bool			operator <=		( const Angle& angle) const;
	bool			operator >=		( const Angle& angle) const;
	bool			operator <		( const Angle& angle) const;
	bool			operator >		( const Angle& angle) const;
	
	float			AsRadians() const;
	float			AsDegrees() const;
	float			AsPositiveRadians() const;
	float			AsPositiveDegrees() const;
	inline  void	Normalize();
	
	static Angle	FromDegrees(const float degrees);
	static Angle	FromRadians(const float radians);
	
private:
	void			SetFromDegrees(const float degrees);
	
	float			mRadian;
public:
	static Angle	Deg0;
	static Angle	DegHalf;
	static Angle	Deg1;
	static Angle	Deg5;
	static Angle	Deg10;
	static Angle	Deg15;
	static Angle	Deg30;
	static Angle	Deg45;
	static Angle	Deg60;
	static Angle	Deg90;
	static Angle	Deg120;
	static Angle	Deg135;
	static Angle	Deg180;
};

typedef Angle RadianRotation;
