CC=g++
CFLAGS=-c -Wall -fopenmp -DOMP_ENABLE
LFLAGS=-fopenmp
EXECUTABLE=rt

all: CFLAGS+=-O3 -ffast-math
all: $(EXECUTABLE)

debug: CFLAGS+=-g
debug: $(EXECUTABLE)

$(EXECUTABLE): camera.o color.o geometry.o main.o ray.o raytracer.o rendersurface.o scene.o vector.o
	$(CC) $(LFLAGS) camera.o color.o geometry.o main.o ray.o raytracer.o rendersurface.o scene.o vector.o -o $(EXECUTABLE)

camera.o: camera.cpp
	$(CC) $(CFLAGS) camera.cpp

color.o: color.cpp
	$(CC) $(CFLAGS) color.cpp

geometry.o: geometry.cpp
	$(CC) $(CFLAGS) geometry.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

ray.o: ray.cpp
	$(CC) $(CFLAGS) ray.cpp

raytracer.o: raytracer.cpp
	$(CC) $(CFLAGS) raytracer.cpp

rendersurface.o: rendersurface.cpp
	$(CC) $(CFLAGS) rendersurface.cpp

scene.o: scene.cpp
	$(CC) $(CFLAGS) scene.cpp

vector.o: vector.cpp
	$(CC) $(CFLAGS) vector.cpp

clean:
	\rm -rf *.o rt
