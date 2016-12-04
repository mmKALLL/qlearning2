#ifndef QLEARNING_TRACK_H
#define QLEARNING_TRACK_H
#define DEGTORAD 0.0174532925199432957f
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>
#include <vector>

class Track {
public:
	Track(b2World* world);
	~Track();

	sf::VertexArray createSector(int, int);

	void GUI(std::vector<sf::VertexArray>);
	void newPhysicsCircuitPart(float width, float height, float angle, b2Vec2 middlepoint);

private:
    std::vector<std::vector<float>> circuit;
	std::vector<b2Body*> circuit_physics;
	b2World* world;
};

#endif
