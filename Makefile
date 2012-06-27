CC=g++
CFLAGS=-c -Wall -fopenmp -march=core2
LFLAGS=-fopenmp
EXECUTABLE=rt
EASY_BMP_CFLAGS=-c -fopenmp -O3 -march=core2 -mrecip
SSE_FLAGS=
SSE_FILES=color.o vector.o

ifndef DISABLE_SSE
	SSE_FLAGS += -mssse3 -mfpmath=sse -DSSE2_ENABLE
	SSE_FILES = color_sse.o vector_sse.o
endif
CFLAGS += $(SSE_FLAGS)

all: CFLAGS+=-O3 -DOMP_ENABLE -ffast-math -fprefetch-loop-arrays -funsafe-loop-optimizations -fno-signed-zeros -fno-trapping-math -fassociative-math -freciprocal-math -mrecip
all: $(EXECUTABLE)

debug: CFLAGS+=-g
debug: $(EXECUTABLE)

$(EXECUTABLE): camera.o EasyBMP.o geometry.o main.o progress.o ray.o raytracer.o rendersurface.o scene.o $(SSE_FILES)
	$(CC) $(LFLAGS) camera.o EasyBMP.o geometry.o main.o progress.o ray.o raytracer.o rendersurface.o scene.o $(SSE_FILES) -o $(EXECUTABLE)

camera.o: camera.cpp
	$(CC) $(CFLAGS) camera.cpp

color.o: color.cpp
	$(CC) $(CFLAGS) color.cpp

color_sse.o: color_sse.cpp
	$(CC) $(CFLAGS) color_sse.cpp

EasyBMP.o: EasyBMP.cpp
	$(CC) $(EASY_BMP_CFLAGS) EasyBMP.cpp

geometry.o: geometry.cpp
	$(CC) $(CFLAGS) geometry.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

progress.o: progress.cpp
	$(CC) $(CFLAGS) progress.cpp

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

vector_sse.o: vector_sse.cpp
	$(CC) $(CFLAGS) vector_sse.cpp

clean:
	\rm -rf *.o rt
