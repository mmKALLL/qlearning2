#include "include/Box2D/Box2D.h"
#include <Vector>
#include "car.hpp"

class Physics
{

public:
	Physics();
	~Physics();

private:
	std::unique_ptr<b2World> world;
	std::vector<Car> cars;

	// Simulation runs at 60 fps
	float32 timeStep = 1 / 60.0;
	int32 velocityIterations = 8;   //how strongly to correct velocity
	int32 positionIterations = 3;   //how strongly to correct position
};

//myWorld->Step( timeStep, velocityIterations, positionIterations);
//Call the above function everytime you want the simulation to proceed, each call proceeds the simulation by 1/60 second.
