#pragma once



namespace CORE_AI
{
class SimpleDrawing;

class EnemyShip
{
public:
	EnemyShip (int x, int y, float radius, float speed, float angle);

	void	SetHeadingAt (int x, int y);

	bool	DoesHit (float r, float theta, float testradius);

	void	Update ();
	void	Draw (CORE_AI::SimpleDrawing* Drawing);

protected:

	int x, y;
	float headingAngle;
	float speed;
};
}