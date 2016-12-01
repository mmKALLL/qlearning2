OBJECTS = Launch.o Network.o Interface.o
CC = g++
COMPILE = -std=c++11 -Wall -Wextra -c
LINK = -Wall -Wextra

qlearn: $(OBJECTS)
	$(CC) $(LINK) $(OBJECTS) -o qlearn -lsfml-graphics -lsfml-window -lsfml-system

Launch.o: src/Launcher.hpp src/Launcher.cpp src/Controller.hpp src/Controller.cpp src/Tests.hpp src/Tests.cpp
	$(CC) $(COMPILE) src/Launcher.cpp src/Controller.cpp src/Tests.cpp

Network.o: src/network/Learning.hpp src/network/Learning.cpp src/network/NeuralNetwork.hpp src/network/NeuralNetwork.cpp src/network/Node.hpp src/network/Node.cpp
	$(CC) $(COMPILE) src/network/Learning.cpp src/network/NeuralNetwork.cpp src/network/Node.cpp

Interface.o: src/car/Car.hpp src/car/Car.cpp src/car/CarType.hpp src/car/CarType.cpp src/car/Physics.hpp src/car/Physics.cpp src/car/Track.hpp src/car/Track.cpp
	$(CC) $(COMPILE) src/car/Car.cpp src/car/CarType.cpp src/car/Physics.cpp src/car/Track.cpp

.PHONY: clean
clean:
	rm -f *.o qlearn *~
