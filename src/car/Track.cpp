#include "Track.hpp"

Track::Track() {
	std::vector<sf::VertexArray> sectors;
	sectors.push_back(createSector(0, 0));
	sectors.push_back(createSector(50, 0));
	sectors.push_back(createSector(100, 0));
	sectors.push_back(createSector(150, 0));
	sectors.push_back(createSector(200, 0));
	
	GUI(sectors);
}

Track::~Track() {

}

sf::VertexArray Track::createSector(int x, int y) {
	sf::VertexArray sector(sf::LinesStrip, 5);
	sector[0].position = sf::Vector2f(x, y - 100);
	sector[1].position = sf::Vector2f(x + 50, y - 100);
	sector[2].position = sf::Vector2f(x + 50, y + 100);
	sector[3].position = sf::Vector2f(x, y + 100);
	sector[4].position = sf::Vector2f(x, y - 100);
	sector[0].color = sf::Color::White;
	sector[1].color = sf::Color::Blue;
	sector[2].color = sf::Color::Blue;
	sector[3].color = sf::Color::Blue;
	sector[4].color = sf::Color::Blue;
	return sector;
}

void Track::GUI(std::vector<sf::VertexArray> sectors) {
	
	// Experiment area
	
	sf::Texture sector;
	unsigned int x = 50;
	unsigned int y = 200;
	sector.create(x, y);
	
	sf::Uint8* pixels = new sf::Uint8[x * y * 4];
	int j = 0;
	for (unsigned i = 0; i < x * y * 4; i += 4) {
		if (j < 255) {
			j++;
		} else {
			j = 0;
		}
    	pixels[i] = 0;
    	pixels[i + 1] = 0;
    	pixels[i + 2] = j;
    	pixels[i + 3] = 255;
	}
	sector.update(pixels);
	
	sf::Sprite sprite;
	sprite.setTexture(sector);
	
	// Actual code
	
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "qlearning2");
	window.setVerticalSyncEnabled(true);

	sf::RectangleShape car(sf::Vector2f(40, 30));
	car.setOrigin(20, 15);
	car.setFillColor(sf::Color(255, 55, 55));

	sf::View camera;
	camera.setSize(sf::Vector2f(1000, 1000));

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::Resized) {
				// Update the camera to the new window size.
				camera.setSize(sf::Vector2f(event.size.width, event.size.height));
			}
		}

		window.clear(sf::Color::Black);
		car.move(1, 0);
		camera.setCenter(car.getPosition());
		window.setView(camera);
		for (auto x : sectors) {
			window.draw(x);
		}
		window.draw(car);
		window.draw(sprite);
		window.display();
	}
}
