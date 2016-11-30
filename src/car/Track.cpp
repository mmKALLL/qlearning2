#include "Track.hpp"
#include <SFML/Graphics.hpp>

Track::Track() {
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "qlearning2");
	window.setVerticalSyncEnabled(true);

	sf::RectangleShape car(sf::Vector2f(50, 30));
	car.setOrigin(25, 15);
	car.setFillColor(sf::Color(255, 55, 55));
	float x = 0.f;

	sf::View camera;
	camera.setCenter(sf::Vector2f(0, 0));
	camera.setSize(sf::Vector2f(1000, 1000));
	window.setView(camera);

	while (window.isOpen()) { // Is it okay to leave this loop in the constructor?
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::Resized) {
				// Update the camera to the new window size.
				camera.setSize(sf::Vector2f(event.size.width, event.size.height));
				window.setView(camera);
			}
		}

		window.clear(sf::Color::Black);
		x += 0.05;
		car.move(x, 0);
		camera.move(7.5, 0);
		window.setView(camera);
		window.draw(car);
		window.display();
	}
}

Track::~Track() {

}
