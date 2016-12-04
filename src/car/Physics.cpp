#include "Physics.hpp"

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




Physics::Physics(b2World* world)
{
	this->world = world;
}


std::vector<float> Physics::updateRays(b2Body& carBody) {

	std::vector<float> distances;
	// Rays for checking the distance
	// Initialized with starting and ending points
	b2Vec2 forwardRayStart = carBody.GetWorldPoint(b2Vec2(2, 0));
	b2Vec2 forwardRayEnd = carBody.GetWorldPoint(b2Vec2(10, 0));

	b2Vec2 leftRayStart = carBody.GetWorldPoint(b2Vec2(2, 0.75));
	b2Vec2 leftRayEnd = carBody.GetWorldPoint(b2Vec2(6, 6));

	b2Vec2 rightRayStart = carBody.GetWorldPoint(b2Vec2(2, -0.75));
	b2Vec2 rightRayEnd = carBody.GetWorldPoint(b2Vec2(6, -6));

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

void Physics::updateFriction(b2Body* carBody) {
	//lateral linear velocity
	b2Vec2 impulse = carBody->GetMass() * -getLateralVelocity(carBody);
	if (impulse.Length() > maxLateralImpulse)
		impulse *= maxLateralImpulse / impulse.Length();
	carBody->ApplyLinearImpulse(impulse, carBody->GetWorldCenter(), true);

	//angular velocity
	carBody->ApplyAngularImpulse(0.1f * carBody->GetInertia() * -carBody->GetAngularVelocity(), true);

	//forward linear velocity
	b2Vec2 currentForwardNormal = getForwardVelocity(carBody);
	float currentForwardSpeed = getForwardVelocity(carBody).Normalize();
	float dragForceMagnitude = -2 * currentForwardSpeed;
	carBody->ApplyForce(dragForceMagnitude * currentForwardNormal, carBody->GetWorldCenter(), true);
}

b2Vec2 Physics::getForwardVelocity(b2Body* carBody) const {
	b2Vec2 currentForwardNormal = carBody->GetWorldVector(b2Vec2(1, 0));
	return b2Dot(currentForwardNormal, carBody->GetLinearVelocity()) * currentForwardNormal;
}

b2Vec2 Physics::getLateralVelocity(b2Body* carBody) const {
	b2Vec2 currentRightNormal = carBody->GetWorldVector(b2Vec2(0, 1));
	return b2Dot(currentRightNormal, carBody->GetLinearVelocity()) * currentRightNormal;
}

int Physics::collisionCheck(b2Body* carBody) {
	for (b2ContactEdge* edge = carBody->GetContactList(); edge; edge = edge->next) {
		if (edge->contact->IsTouching() && ((bool)carBody->GetUserData() == false)) {
			b2Fixture* a = edge->contact->GetFixtureA();
			if (a->IsSensor()) {
				bool data = true;
				carBody->SetUserData((void*)data);
				return 1;
				
			}
			else {
				return -1;
			}
		}
		else if ((edge->contact->IsTouching() == false) && (carBody->GetUserData() == (void*)true)) {
			bool data = false;
			carBody->SetUserData((void*)data);
		}
	}

}