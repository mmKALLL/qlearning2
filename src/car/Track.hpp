#ifndef QLEARNING_TRACK_H
#define QLEARNING_TRACK_H
#define DEGTORAD 0.0174532925199432957f
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>
#include <vector>
#include <tuple>

class Controller;
#include "../Controller.hpp"

class Track {
public:
	Track(b2World*, Controller*);
	~Track();
	void setControllerReference(Controller* cntrl) { controller = cntrl; }
	
	void newSector(float, float, float, b2Vec2, std::string);
	sf::VertexArray drawSector(float, float, float, b2Vec2);
	
	void GUI(std::vector<sf::VertexArray>);

private:
	b2World* world;
	Controller* controller;
	
};

#endif
