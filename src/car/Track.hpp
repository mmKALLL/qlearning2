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
	/*
	void straight(&float, &b2Vec2, &sf::Vector2f, &sf::Vector2f);
	void leftTurn(&float, &b2Vec2, &sf::Vector2f, &sf::Vector2f);
	void rightTurn(&float, &b2Vec2, &sf::Vector2f, &sf::Vector2f);
	*/
	void newSector(float width, float height, float angle, b2Vec2 middlePoint);
	void GUI(std::vector<sf::VertexArray>);

private:
	std::vector<b2Body*> circuit;
	b2World* world;
	Controller* controller;
};

#endif
