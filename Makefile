CC=g++
CFLAGS=-c -Wall -fopenmp -DOMP_ENABLE -DSSE2_ENABLE -march=core2 -mssse3 -mfpmath=sse
LFLAGS=-fopenmp
EXECUTABLE=rt
EASY_BMP_CFLAGS=-c -fopenmp -O3 -march=core2 -mssse3 -mfpmath=sse -mrecip

all: CFLAGS+=-O3 -ffast-math -fprefetch-loop-arrays -funsafe-loop-optimizations -fno-signed-zeros -fno-trapping-math -fassociative-math -freciprocal-math -mrecip
all: $(EXECUTABLE)

debug: CFLAGS+=-g
debug: $(EXECUTABLE)

$(EXECUTABLE): camera.o color.o EasyBMP.o geometry.o main.o ray.o raytracer.o rendersurface.o scene.o vector_sse.o
	$(CC) $(LFLAGS) camera.o color.o EasyBMP.o geometry.o main.o ray.o raytracer.o rendersurface.o scene.o vector_sse.o -o $(EXECUTABLE)

camera.o: camera.cpp
	$(CC) $(CFLAGS) camera.cpp

color.o: color.cpp
	$(CC) $(CFLAGS) color.cpp

EasyBMP.o: EasyBMP.cpp
	$(CC) $(EASY_BMP_CFLAGS) EasyBMP.cpp

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

vector_sse.o: vector_sse.cpp
	$(CC) $(CFLAGS) vector_sse.cpp

clean:
	\rm -rf *.o rt
