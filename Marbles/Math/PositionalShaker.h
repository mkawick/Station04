#pragma once
#include "math/vector3.h"

class PositionalShaker
{
public:
	PositionalShaker(void);
	~PositionalShaker(void);

	bool	isDoneShaking() { if( isActive ) return false; return true; }
	void	Update();
	void	Init( int LifeSpan );
	void	SetupCountDown();
	void	SetupOffset();

	const Vector3& pos() const { return positionOffset; }

protected:
	bool	isActive;
	int		lifeSpan;
	int		countDown;
	float	shakeMagnitude;
	Vector3	positionOffset;
};
