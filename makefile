OBJECTS = Launcher.o Controller.o Tests.o Learning.o NeuralNetwork.o Node.o Car.o CarType.o Physics.o Track.o
CC = g++
COMPILE = -std=c++11 -Wall -Wextra -c
INCLUDE = -I./src/include
LINK = -Wall -Wextra

qlearn: $(OBJECTS) # Is it good practice to only use the first missing .o file to create multiple of them?
	$(CC) $(LINK) $(INCLUDE) $(OBJECTS) -o qlearn -L./src/lib -lsfml-graphics -lsfml-window -lsfml-system  -l Box2D

Launcher.o: src/Launcher.hpp src/Launcher.cpp src/Controller.hpp src/Controller.cpp src/Tests.hpp src/Tests.cpp
	$(CC) $(COMPILE) $(INCLUDE) src/Launcher.cpp src/Controller.cpp src/Tests.cpp

Learning.o: src/network/Learning.hpp src/network/Learning.cpp src/network/NeuralNetwork.hpp src/network/NeuralNetwork.cpp src/network/Node.hpp src/network/Node.cpp
	$(CC) $(COMPILE) $(INCLUDE) src/network/Learning.cpp src/network/NeuralNetwork.cpp src/network/Node.cpp

Car.o: src/car/Car.hpp src/car/Car.cpp src/car/CarType.hpp src/car/CarType.cpp src/car/Physics.hpp src/car/Physics.cpp src/car/Track.hpp src/car/Track.cpp
	$(CC) $(COMPILE) $(INCLUDE) src/car/Car.cpp src/car/CarType.cpp src/car/Physics.cpp src/car/Track.cpp

.PHONY: clean
clean:
	rm -f *.o qlearn *~
