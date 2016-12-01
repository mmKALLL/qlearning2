OBJECTS = Launcher.o Controller.o Track.o
CC = g++
COMPILE = -std=c++11 -Wall -Wextra -c
LINK = -Wall -Wextra

learn: $(OBJECTS)
	$(CC) $(LINK) $(OBJECTS) -o learn -lsfml-graphics -lsfml-window -lsfml-system

Launcher.o: Launcher.hpp Launcher.cpp
	$(CC) $(COMPILE) Launcher.cpp

Controller.o: Controller.hpp Controller.cpp
	$(CC) $(COMPILE) Controller.cpp

Track.o: car/Track.hpp car/Track.cpp
	$(CC) $(COMPILE) car/Track.cpp

.PHONY: clean
clean:
	rm -f *.o *~ learn
