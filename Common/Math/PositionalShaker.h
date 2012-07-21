#pragma once
#include "vector.h"

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

	const Vector& pos() const { return positionOffset; }

protected:
	bool	isActive;
	int		lifeSpan;
	int		countDown;
	float	shakeMagnitude;
	Vector	positionOffset;
};
