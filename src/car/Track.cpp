#include "Track.hpp"
// Rad to Deg -> * 57.2957795f

Track::Track(b2World* world, Controller* controller) : world(world), controller(controller) {
	if (this->world) {
		std::cout << "Track connected to world" << std::endl;
	}
	// Set the width and height of a single sector.
	float width = 50.0f;
	float height = 200.0f;
	float length = sqrt(((width / 2) * (width / 2)) + ((height / 2) * (height / 2)));
	float heightLengthAngle = acos((height / 2) / length);
	
	// TODO: Possibly read track composition from a file?
	// TODO: The track must always begin with a "straight"
	std::vector<std::string> track = {
		"straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "left", "straight", "straight", "straight", "straight", "straight", "straight", "right", "straight", "straight", "straight", "straight", "straight", "straight", "left", "straight", "straight", "straight", "straight", "straight", "straight", "left", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "left", "left", "straight", "straight", "straight", "right", "straight", "straight", "straight", "right", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "right", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "left", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "left", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "left", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight"
	};

	// Initialize an empty vector to hold the graphical representation of the circuit.
	std::vector<sf::VertexArray> sectors;
	float angle = 0.f;
	b2Vec2 midPoint = b2Vec2(50, 0);
	sf::Vector2f lastLeftCorner;
	sf::Vector2f lastRightCorner;
	
	for (auto & element : track) {
		if (element == "straight") {
			newSector(width, height, angle, midPoint);
			
			float bottomLeftYOffset = cos(angle * DEGTORAD - heightLengthAngle) * length;
			float bottomLeftXOffset = sin(angle * DEGTORAD - heightLengthAngle) * length;
			float topRightYOffset = -bottomLeftYOffset;
			float topRightXOffset = -bottomLeftXOffset;
			float topLeftYOffset = sin((90.0f - angle) * DEGTORAD - heightLengthAngle) * length;
			float topLeftXOffset = cos((90.0f - angle) * DEGTORAD - heightLengthAngle) * length;
			float bottomRightYOffset = -topLeftYOffset;
			float bottomRightXOffset = -topLeftXOffset;
			
			sf::VertexArray sector(sf::LinesStrip, 5);
			sector[0].position = sf::Vector2f(midPoint.x + topLeftXOffset, midPoint.y + topLeftYOffset);
			sector[1].position = sf::Vector2f(midPoint.x + topRightXOffset, midPoint.y + topRightYOffset);
			sector[2].position = sf::Vector2f(midPoint.x + bottomRightXOffset, midPoint.y + bottomRightYOffset);
			sector[3].position = sf::Vector2f(midPoint.x + bottomLeftXOffset, midPoint.y + bottomLeftYOffset);
			sector[4].position = sf::Vector2f(midPoint.x + topLeftXOffset, midPoint.y + topLeftYOffset);
			lastLeftCorner = sector[1].position;
			lastRightCorner = sector[0].position;
			sector[0].color = sf::Color::Blue;
			sector[1].color = sf::Color::Blue;
			sector[2].color = sf::Color::Blue;
			sector[3].color = sf::Color::Blue;
			sector[4].color = sf::Color::Blue;
			
			sectors.push_back(sector);
			
			midPoint.x += cos(angle * DEGTORAD) * width;
			midPoint.y += cos((angle + 90.0f) * DEGTORAD) * width;
		} else if (element == "left") {
			for (unsigned i = 0; i < 10; i++) {
				newSector(width, height, angle, midPoint);
				
				float bottomLeftYOffset = cos(angle * DEGTORAD - heightLengthAngle) * length;
				float bottomLeftXOffset = sin(angle * DEGTORAD - heightLengthAngle) * length;
				float topRightYOffset = -bottomLeftYOffset;
				float topRightXOffset = -bottomLeftXOffset;
				float topLeftYOffset = sin((90.0f - angle) * DEGTORAD - heightLengthAngle) * length;
				float topLeftXOffset = cos((90.0f - angle) * DEGTORAD - heightLengthAngle) * length;
				float bottomRightYOffset = -topLeftYOffset;
				float bottomRightXOffset = -topLeftXOffset;
				
				sf::VertexArray sector(sf::LinesStrip, 5);
				sector[0].position = sf::Vector2f(midPoint.x + topLeftXOffset, midPoint.y + topLeftYOffset);
				sector[1].position = sf::Vector2f(midPoint.x + topRightXOffset, midPoint.y + topRightYOffset);
				sector[2].position = sf::Vector2f(midPoint.x + bottomRightXOffset, midPoint.y + bottomRightYOffset);
				sector[3].position = sf::Vector2f(midPoint.x + bottomLeftXOffset, midPoint.y + bottomLeftYOffset);
				sector[4].position = sf::Vector2f(midPoint.x + topLeftXOffset, midPoint.y + topLeftYOffset);
				lastLeftCorner = sector[1].position;
				lastRightCorner = sector[0].position;
				sector[0].color = sf::Color::Blue;
				sector[1].color = sf::Color::Blue;
				sector[2].color = sf::Color::Blue;
				sector[3].color = sf::Color::Blue;
				sector[4].color = sf::Color::Blue;
				
				sectors.push_back(sector);

				if (i < 9) {
					angle += 10.0f;
				}
				float xChange = cos(angle * DEGTORAD) * (width / 2);
				float yChange = sin(angle * DEGTORAD) * (width / 2);
				midPoint.x = lastRightCorner.x;
				midPoint.y = lastRightCorner.y;
				midPoint.x += xChange;
				midPoint.y -= yChange;
				midPoint.x -= sin(angle * DEGTORAD) * (height / 2);
				midPoint.y -= cos(angle * DEGTORAD) * (height / 2);
			}
		} else if (element == "right") {
			for (unsigned i = 0; i < 10; i++) {
				newSector(width, height, angle, midPoint);
				
				float bottomLeftYOffset = cos(angle * DEGTORAD - heightLengthAngle) * length;
				float bottomLeftXOffset = sin(angle * DEGTORAD - heightLengthAngle) * length;
				float topRightYOffset = -bottomLeftYOffset;
				float topRightXOffset = -bottomLeftXOffset;
				float topLeftYOffset = sin((90.0f - angle) * DEGTORAD - heightLengthAngle) * length;
				float topLeftXOffset = cos((90.0f - angle) * DEGTORAD - heightLengthAngle) * length;
				float bottomRightYOffset = -topLeftYOffset;
				float bottomRightXOffset = -topLeftXOffset;
				
				sf::VertexArray sector(sf::LinesStrip, 5);
				sector[0].position = sf::Vector2f(midPoint.x + topLeftXOffset, midPoint.y + topLeftYOffset);
				sector[1].position = sf::Vector2f(midPoint.x + topRightXOffset, midPoint.y + topRightYOffset);
				sector[2].position = sf::Vector2f(midPoint.x + bottomRightXOffset, midPoint.y + bottomRightYOffset);
				sector[3].position = sf::Vector2f(midPoint.x + bottomLeftXOffset, midPoint.y + bottomLeftYOffset);
				sector[4].position = sf::Vector2f(midPoint.x + topLeftXOffset, midPoint.y + topLeftYOffset);
				lastLeftCorner = sector[1].position;
				lastRightCorner = sector[0].position;
				sector[0].color = sf::Color::Blue;
				sector[1].color = sf::Color::Blue;
				sector[2].color = sf::Color::Blue;
				sector[3].color = sf::Color::Blue;
				sector[4].color = sf::Color::Blue;
				
				sectors.push_back(sector);

				if (i < 9) {
					angle -= 10.0f;
				}
				float xChange = cos(angle * DEGTORAD) * (width / 2);
				float yChange = sin(angle * DEGTORAD) * (width / 2);
				midPoint.x = lastLeftCorner.x;
				midPoint.y = lastLeftCorner.y;
				midPoint.x += xChange;
				midPoint.y -= yChange;
				midPoint.x += sin(angle * DEGTORAD) * (height / 2);
				midPoint.y += cos(angle * DEGTORAD) * (height / 2);
			}
		} else {
			throw "Corrupted track file.";
		}
	}
	
	GUI(sectors); // Return?
}

Track::~Track() {

}
/*
void Track::straight(&float angle, &b2Vec2 midPoint, &sf::Vector2f rightCorner, &sf::Vector2f leftCorner) {
	
}

void Track::leftTurn(&float angle, &b2Vec2 midPoint, &sf::Vector2f rightCorner, &sf::Vector2f leftCorner) {
	
}

void Track::rightTurn(&float angle, &b2Vec2 midPoint, &sf::Vector2f rightCorner, &sf::Vector2f leftCorner) {
	
}
*/
// This method creates a track part that the physics engine can utilize. The method takes the width, height, angle and middle point of the track part as parameters.
void Track::newSector(float width, float height, float angle, b2Vec2 middlePoint) {
	b2BodyDef bd;
	bd.position.Set(middlePoint.x, middlePoint.y);
	
	b2Body* trackPart = world->CreateBody(&bd);

	b2EdgeShape shape;
	b2FixtureDef checkpoints;
	checkpoints.shape = &shape;
	checkpoints.isSensor = true;

	// Left vertical
	shape.Set(b2Vec2(-width, -height), b2Vec2(-width, height));
	trackPart->CreateFixture(&checkpoints);

	// Right vertical
	shape.Set(b2Vec2(width, -height), b2Vec2(width, height));
	trackPart->CreateFixture(&checkpoints);

	b2FixtureDef walls;
	walls.shape = &shape;

	walls.isSensor = false;

	// Top horizontal
	shape.Set(b2Vec2(-width, height), b2Vec2(width, height));
	trackPart->CreateFixture(&walls);

	// Bottom horizontal
	shape.Set(b2Vec2(-width, -height), b2Vec2(width, -height));

	trackPart->CreateFixture(&walls);
	trackPart->SetTransform(middlePoint, angle * DEGTORAD);
	circuit.push_back(trackPart);
}

void Track::GUI(std::vector<sf::VertexArray> sectors) {
	
	sf::Sprite sprite;
	sprite.setOrigin(25, 100);
	
	// Create the finish line texture
	sf::Texture sector;
	unsigned int x = 50;
	unsigned int y = 200;
	sector.create(x, y);
	sf::Uint8* pixels = new sf::Uint8[x * y * 4];
	int column = 0;
	int row = 0;
	int color = 255;
	for (unsigned i = 0; i < x * y * 4; i += 4) {
		pixels[i] = color;
		pixels[i + 1] = color;
		pixels[i + 2] = color;
		pixels[i + 3] = 255;
		
		column++;
		if (column >= 10) {
			column = 0;
			row++;
			if (!(row % 5 != 0) != (row % 50 != 0)) {
				if (color == 255) {
					color = 0;
				} else {
					color = 255;
				}
			}
		}
	}
	sector.update(pixels);
	sprite.setTexture(sector);
	
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "qlearning2");
	window.setVerticalSyncEnabled(true);
	
	sf::RectangleShape car(sf::Vector2f(40, 30));
	car.setOrigin(20, 15);
	car.setFillColor(sf::Color(255, 55, 55));
	std::vector<float> carPosition;
	
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
		std::cout << "Trying to get the position of the current car from Track.cpp" << std::endl;
		carPosition = controller->getCarPosition();
		car.setPosition(carPosition[0], carPosition[1]);
		camera.setCenter(car.getPosition());
		window.setView(camera);
		for (auto x : sectors) {
			window.draw(x);
		}
		window.draw(sprite);
		window.draw(car);
		window.display();
		controller->stepForward();
		
	}
}
