#include "Physics.hpp"

Physics::Physics(b2World* world, Car* car) : world(world), car(car){
	// Set the world to use our physics class as collision checker
	this->world->SetContactListener(this);
}

std::vector<float> Physics::updateRays(b2Body& carBody, int size, int degrees, std::vector<float> rayDistances){
	std::vector<float> distances;
	
	for (int i = -(size - 1) / 2; i <= (size - 1) / 2; i++) {

		m_hit = false;
		b2Vec2 rayStart = carBody.GetWorldPoint(b2Vec2(0, 0));

		float y = rayLenght*cos((90 + i*degrees / (size - 1))*DEGTORAD);
		float x = rayLenght*sin((90  + i*degrees / (size - 1))*DEGTORAD);

		b2Vec2 rayEnd = carBody.GetWorldPoint(b2Vec2(x, y));

		world->RayCast(this, rayStart, rayEnd);
		if (this->m_hit) {
			for (auto it = rayDistances.begin(); it != rayDistances.end(); it++) {
				if (m_fraction <= *it) {
					distances.push_back(*it);
					break;
				}
			}
		}
		else {
			distances.push_back(1);
		}


	}

	return distances;
}

void Physics::updateFriction(b2Body* carBody) {
	//lateral linear velocity
	b2Vec2 impulse = carBody->GetMass() * -getLateralVelocity(carBody);
	if (impulse.Length() > maxLateralImpulse){
		impulse *= maxLateralImpulse / impulse.Length();
	}
	carBody->ApplyLinearImpulse(impulse, carBody->GetWorldCenter(), true);

	//angular velocity
	carBody->ApplyAngularImpulse(1.0f * carBody->GetInertia() * -carBody->GetAngularVelocity(), true);

	//forward linear velocity
	b2Vec2 currentForwardNormal = getForwardVelocity(carBody);
	float dragForceMagnitude = -0.1 * 120;
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


void Physics::BeginContact(b2Contact* contact) {

	// Car has it's userdata set and it's used to check collision status of walls
	// Checkpoints are sensors and walls are not and approriate action is selected
	void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	if (!userDataA) {
		if (contact->GetFixtureA()->IsSensor() == false) {
			car->setCollisionStatus(true);

		}
	}

	if (!userDataB) {
		if (contact->GetFixtureB()->IsSensor() == false) {
			car->setCollisionStatus(true);
		}
	}

}

void Physics::EndContact(b2Contact* contact){
	void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
	void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();
	if (!userDataA) {
		if (contact->GetFixtureA()->IsSensor() == true) {
			car->addCheckpoint();
		}
		else {
			car->setCollisionStatus(false);
		}
	}

	if (!userDataB) {
		if (contact->GetFixtureB()->IsSensor() == true) {
			car->addCheckpoint();
		}
		else {
			car->setCollisionStatus(false);
		}
	}
}
float32 Physics::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction){
	if (!fixture->IsSensor()) {
		m_hit = true;
		m_point = point;
		m_fraction = fraction;
		m_normal = normal;

	}
	return fraction;
}