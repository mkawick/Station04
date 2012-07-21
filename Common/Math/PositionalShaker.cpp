#include "PositionalShaker.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "MathFunctions.h"

const float defaultShakeMagnitude = 0.4f;
const float defaultShakeMagnitudeDecrease = 0.005f;
PositionalShaker::PositionalShaker() : isActive( false ), shakeMagnitude( defaultShakeMagnitude ), countDown( 0 ), lifeSpan( 0 )
{
}

PositionalShaker::~PositionalShaker(void)
{
}

void	PositionalShaker::Update()
{
	if( isActive )
	{
		countDown --;
		lifeSpan --;
	}
	if( lifeSpan <=0 )
	{
		isActive = false;
	}
	else
	{
		if( countDown <= 0 )
		{
			SetupCountDown();
			SetupOffset();
			shakeMagnitude -= defaultShakeMagnitudeDecrease;
			if( shakeMagnitude <= 0 )
			{
				shakeMagnitude = 0;
				lifeSpan = 0;
			}
		}
	}
}
void	PositionalShaker::Init( int LifeSpan )
{
	SetupOffset();
	lifeSpan = LifeSpan;
	SetupCountDown();
	isActive = true;
	shakeMagnitude = defaultShakeMagnitude;
}
void	PositionalShaker::SetupCountDown()
{
	int countDown = rand() %8 + 24;
	if( countDown > lifeSpan )
		countDown = lifeSpan;
}
void	PositionalShaker::SetupOffset()
{
	float angle = static_cast<float>( rand() % 720 ) *0.5f;
	float angleInRads = ConvertToRadians( angle );
	float x = cos( angleInRads ) * shakeMagnitude;
	float y = sin( angleInRads ) * shakeMagnitude;
	positionOffset.Set( x, y, 0 );
}