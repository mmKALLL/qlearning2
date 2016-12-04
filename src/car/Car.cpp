#include "Car.hpp"

class CarRayCallback : public b2RayCastCallback
{
public:
	CarRayCallback()
	{
		m_hit = false;
	}

	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
	{
		m_hit = true;
		m_point = point;
		return fraction;
	}

	bool m_hit;
	b2Vec2 m_point;
};


Car::Car(b2World * world, const b2Vec2& position, const b2Vec2& dimensions)
{
	this->world = world;
	//Create definition for car body
	b2BodyDef carBodyDef;
	carBodyDef.type = b2_dynamicBody;

	//Set the initial position and angle and then create the body
	carBodyDef.position.Set(position.x, position.y);
	carBodyDef.angle = 0; //set the starting angle
	carBody = world->CreateBody(&carBodyDef);
	carBody->SetAngularDamping(3);

	b2PolygonShape carShape;
	// Car is 4 meters long and 1.5 meter wide
	carShape.SetAsBox(2.0f, 0.75f);

	b2FixtureDef carFixtureDef;
	carFixtureDef.shape = &carShape;
	// Density controls the mass of the car
	carFixtureDef.density = 1;
	// Restitution is how bouncy it is, eg. 0 no bounce and 1 all kinetic energy is conserved in collisions
	carFixtureDef.restitution = 0.25;
	// Well, friction
	carFixtureDef.friction = 0.5;
	carBody = world->CreateBody(&carBodyDef);
	carBody->CreateFixture(&carFixtureDef);
}

Car::Car(b2World * world, const b2Vec2& position, const b2Vec2& dimensions, NeuralNetwork createdNetwork)
{
	Car result = Car(b2World * world, const b2Vec2& position, const b2Vec2& dimensions);
	
}


Car::~Car()
{
	carBody->GetWorld()->DestroyBody(carBody);
	
}

void Car::accelerate(int direction)
{
	// Get current forward speed and set force
	b2Vec2 currentForwardNormal = getForwardVelocity();
	force = 0;

	// Set desired speed in relation to if we are reversing or going forward
	if (direction > 0) {
		desiredSpeed = maxSpeed;
	}
	else {
		desiredSpeed = maxReverse;
	}


	currentForwardNormal = carBody->GetWorldVector(b2Vec2(1, 0));

	currentSpeed = b2Dot(getForwardVelocity(), currentForwardNormal);

	//Depending on current speed the amount of force is determined
	if (desiredSpeed > currentSpeed) {
		force = maxDriveForce;
	}
	else if (desiredSpeed < currentSpeed) {
		force = -maxReverseForce;
	}
	else {
		return;
	}

	// Apply the force
	if (direction > 0) {
		carBody->ApplyForce(force * currentForwardNormal, carBody->GetWorldCenter(), false);
	}
	else {
		carBody->ApplyForce(-force * currentForwardNormal, carBody->GetWorldCenter(), false);
	}

}


void Car::turn(int direction)
{
	if (direction > 0) {
		carBody->ApplyTorque(MaxTurningForce, true);
	}
	else {
		carBody->ApplyTorque(-MaxTurningForce, true);
	}
}

b2Vec2 Car::getForwardVelocity() {
	b2Vec2 currentForwardNormal = carBody->GetWorldVector(b2Vec2(1, 0));
	return b2Dot(currentForwardNormal, carBody->GetLinearVelocity()) * currentForwardNormal;
}

b2Vec2 Car::getLateralVelocity() {
	b2Vec2 currentRightNormal = carBody->GetWorldVector(b2Vec2(0, 1));
	return b2Dot(currentRightNormal, carBody->GetLinearVelocity()) * currentRightNormal;
}

void Car::updateFriction() {
	//Remove the lateral velocity by applying impulse thats the opposite for the lateral velocity
	b2Vec2 impulse = carBody->GetMass() * -getLateralVelocity();
	if (impulse.Length() > maxLateralImpulse)
		impulse *= maxLateralImpulse / impulse.Length();
	carBody->ApplyLinearImpulse(impulse, carBody->GetWorldCenter(), true);

	//Stop the car from rotating infinitely
	carBody->ApplyAngularImpulse(0.1f * carBody->GetInertia() * -carBody->GetAngularVelocity(), true);

	//Add drag, so the car will stop after a while
	b2Vec2 currentForwardNormal = getForwardVelocity();
	float currentForwardSpeed = currentForwardNormal.Normalize();
	float dragForceMagnitude = -2 * currentForwardSpeed;
	carBody->ApplyForce(dragForceMagnitude * currentForwardNormal, carBody->GetWorldCenter(), true);
}

std::vector<float> Car::updateRays() {

	distances.clear();
	// Rays for checking the distance
	// Initialized with starting and ending points
	b2Vec2 forwardRayStart = carBody->GetWorldPoint(b2Vec2(2, 0));
	b2Vec2 forwardRayEnd = carBody->GetWorldPoint(b2Vec2(10, 0));

	b2Vec2 leftRayStart = carBody->GetWorldPoint(b2Vec2(2, 0.75));
	b2Vec2 leftRayEnd = carBody->GetWorldPoint(b2Vec2(6, 6));

	b2Vec2 rightRayStart = carBody->GetWorldPoint(b2Vec2(2, -0.75));
	b2Vec2 rightRayEnd = carBody->GetWorldPoint(b2Vec2(6, -6));

	// Callbacks for checking if ray collides with something
	CarRayCallback callback;
	CarRayCallback callback1;
	CarRayCallback callback2;

	// Add rays to the world and if collision happens check for distance
	world->RayCast(&callback, forwardRayStart, forwardRayEnd);
	if (callback.m_hit) {
		distances.push_back((forwardRayStart - callback.m_point).Length());
	}
	else {
		distances.push_back(10);
	}

	world->RayCast(&callback1, leftRayStart, leftRayEnd);
	if (callback1.m_hit) {
		distances.push_back((leftRayStart - callback1.m_point).Length());
	}
	else {
		distances.push_back(10);
	}

	world->RayCast(&callback2, rightRayStart, rightRayEnd);
	if (callback2.m_hit) {
		distances.push_back((rightRayStart - callback2.m_point).Length());
	}
	else {
		distances.push_back(10);
	}


	return distances;
}

// function for returning just the distances to walls
std::vector<float> Car::getDistances() {
	return distances;
}

void setNetwork(NeuralNetwork newNetwork) {
	this.network = newNetwork;
}