#ifndef QLEARNING_TRACK_H
#define QLEARNING_TRACK_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Track {
public:
	Track();
	~Track();

	sf::VertexArray createSector(int, int);

	void GUI(std::vector<sf::VertexArray>);
private:
    std::vector<std::vector<float>> circuit;
};

#endif
