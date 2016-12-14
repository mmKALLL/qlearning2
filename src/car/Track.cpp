#include "Track.hpp"
// Rad to Deg -> * 57.2957795f

Track::Track(b2World* world, Controller* controller) : world(world), controller(controller) {

	// Set the width and height of a single sector.
	float width = 50.0f;
	float height = 200.0f;
	float length = sqrt(((width / 2) * (width / 2)) + ((height / 2) * (height / 2)));
	float heightLengthAngle = acos((height / 2) / length);
	
	// TODO: Possibly read track composition from a file?
	// TODO: The track must always begin with a "straight", add a check for this
	std::vector<std::string> track = {
		"straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "left", "straight", "straight", "straight", "straight", "straight", "straight", "right", "straight", "straight", "straight", "straight", "straight", "straight", "left", "straight", "straight", "straight", "straight", "straight", "straight", "left", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "left", "left", "straight", "straight", "straight", "right", "straight", "straight", "straight", "right", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "right", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "left", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "left", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "left", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight"
	};

	// Initialize an empty vector to hold the graphical representation of the circuit.
	std::vector<sf::VertexArray> sectors;
	float angle = 0.0f;
	b2Vec2 midPoint = b2Vec2(50, 0);
	sf::Vector2f lastLeftCorner;
	sf::Vector2f lastRightCorner;
	
	for (auto & element : track) {
		if (element == "straight") {
			newSector(width, height, angle, midPoint, element);
			
			sf::VertexArray sector = drawSector(length, heightLengthAngle, angle, midPoint);
			lastLeftCorner = sector[1].position;
			lastRightCorner = sector[0].position;
			sectors.push_back(sector);
			
			midPoint.x += cos(angle * DEGTORAD) * width;
			midPoint.y += cos((angle + 90.0f) * DEGTORAD) * width;
		} else if (element == "left") {
			for (unsigned i = 0; i < 10; i++) {
				newSector(width, height, angle, midPoint, element);
				
				sf::VertexArray sector = drawSector(length, heightLengthAngle, angle, midPoint);
				lastLeftCorner = sector[1].position;
				lastRightCorner = sector[0].position;
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
				newSector(width, height, angle, midPoint, element);
				
				sf::VertexArray sector = drawSector(length, heightLengthAngle, angle, midPoint);
				lastLeftCorner = sector[1].position;
				lastRightCorner = sector[0].position;
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
	
	GUI(sectors); // TODO: Does the constructor need to return anything?
	
}

Track::~Track() {

}

// Creates a track part that the physics engine can utilize. The method takes the width, height, angle and middle point of the track part as parameters.
void Track::newSector(float width, float height, float angle, b2Vec2 middlePoint, std::string direction) {
	
	b2BodyDef bd;
	bd.position.Set(middlePoint.x, middlePoint.y);
	
	b2Body* trackPart = world->CreateBody(&bd);
	
	b2EdgeShape shape;
	b2FixtureDef checkpoints;
	checkpoints.shape = &shape;
	checkpoints.isSensor = true;
	
	// End of sector (right vertical edge)
	shape.Set(b2Vec2(width / 2, -height / 2), b2Vec2(width / 2, height / 2));
	trackPart->CreateFixture(&checkpoints);
	
	b2FixtureDef walls;
	walls.shape = &shape;
	
	// Left barrier (top horizontal edge)
	if (direction != "left") {
		shape.Set(b2Vec2(-width / 2, height / 2), b2Vec2(width / 2, height / 2));
		trackPart->CreateFixture(&walls);
	}
	
	// Right barrier (bottom horizontal edge)
	if (direction != "right") {
		shape.Set(b2Vec2(-width / 2, -height / 2), b2Vec2(width / 2, -height / 2));
		trackPart->CreateFixture(&walls);
	}

	trackPart->SetTransform(middlePoint, -angle * DEGTORAD);
	
}

// Creates a VertexArray that SFML uses to draw the track part in GUI
sf::VertexArray Track::drawSector(float length, float heightLengthAngle, float angle, b2Vec2 midPoint) {
	
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
	sector[0].color = sf::Color::Blue;
	sector[1].color = sf::Color::Blue;
	sector[2].color = sf::Color::Blue;
	sector[3].color = sf::Color::Blue;
	sector[4].color = sf::Color::Blue;
	return sector;
	
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
	float carRotation;
	
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
		carPosition = controller->getCarPosition();
		car.setPosition(carPosition[0], carPosition[1]);
		carRotation = controller->getCarRotation();
		car.setRotation(carRotation);
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
