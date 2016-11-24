#include "include\Box2D\Box2D.h"

class Car
{

public:
	Car(b2World * world, const b2Vec2& position, const b2Vec2& dimensions);
	~Car();

	void accelerate();
	void brake();
	

private:
	b2Body* carBody = nullptr;

};

class Tire
{
public:
	Tire();
	~Tire();
};
