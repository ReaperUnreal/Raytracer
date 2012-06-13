#include "main.h"

SDL_Surface *screen;
Raytracer *raytracer;
Scene *scene;
Camera *cam;
RenderSurface *target;

inline void PutPixel(int x, int y, unsigned int c)
{
	((unsigned int *)screen->pixels)[x + y * (screen->pitch / 4)] = c;
}


void setupScene()
{
	raytracer = new Raytracer();

	cam = new Camera();
	cam->MoveTo(Vector(0.0f, 0.0f, 5.0f));
	cam->LookAt(Vector(0.0f, 0.0f, 0.0f));
	raytracer->SetCamera(cam);

	target = new RenderSurface(SCREEN_WIDTH, SCREEN_HEIGHT);
	raytracer->SetTarget(target);

	scene = new Scene(8);

	Sphere *sphere = new Sphere(Vector(0, 0, 0), 2.0f);
	sphere->GetMaterial().SetColor(Color::white);
	sphere->GetMaterial().SetDiffuse(1.0f);
	sphere->GetMaterial().SetSpecular(0.0f);
	sphere->GetMaterial().SetReflectivity(0.0f);
	scene->AddObject(sphere);

	Sphere *smaller = new Sphere(Vector(2.0f, -1.5f, 1.3f), 0.5f);
	smaller->GetMaterial().SetColor(Color::blue);
	smaller->GetMaterial().SetDiffuse(1.0f);
	smaller->GetMaterial().SetSpecular(0.0f);
	//scene->AddObject(smaller);

	Sphere *another = new Sphere(Vector(6.0f, -1.0f, -4.0f), 1.0f);
	another->GetMaterial().SetColor(Color::red);
	another->GetMaterial().SetDiffuse(1.0f);
	another->GetMaterial().SetSpecular(0.0f);
	//scene->AddObject(another);

	Sphere *light = new Sphere(Vector(-4.0f, 4.0f, 3.0f), 1.0f);
	light->SetLight(true);
	light->SetLightIntensity(1.0f);
	light->GetMaterial().SetColor(Color::white);
	scene->AddObject(light);

	Sphere *light2 = new Sphere(Vector(-2.0f, 3.0f, 4.0f), 1.0f);
	light2->SetLight(true);
	light2->SetLightIntensity(1.0f);
	light2->GetMaterial().SetColor(Color::white);
	//scene->AddObject(light2);

	Plane *floor = new Plane(Vector(0.0f, 1.0f, 0.0f), 2.0f);
	floor->GetMaterial().SetColor(Color::white);
	floor->GetMaterial().SetDiffuse(1.0f);
	scene->AddObject(floor);

	Plane *wall = new Plane(Vector(0.0f, 0.0f, 1.0f), 6.0f);
	wall->GetMaterial().SetColor(Color::white);
	wall->GetMaterial().SetDiffuse(1.0f);
	scene->AddObject(wall);

	Plane *wall2 = new Plane(Vector(1.0f, 0.0f, 0.0f), 6.0f);
	wall2->GetMaterial().SetColor(Color::red);
	wall2->GetMaterial().SetDiffuse(1.0f);
	scene->AddObject(wall2);

	Plane *wall3 = new Plane(Vector(-1.0f, 0.0f, 0.0f), 6.0f);
	wall3->GetMaterial().SetColor(Color::blue);
	wall3->GetMaterial().SetDiffuse(1.0f);
	scene->AddObject(wall3);

	Sphere **sphereList = new Sphere*[3];
	sphereList[0] = new Sphere(Vector(-1.0f, 0.0f, 0.0f), 0.5f);
	sphereList[1] = new Sphere(Vector(1.0f, 0.0f, 0.0f), 0.5f);
	sphereList[2] = new Sphere(Vector(0.0f, sqrtf(3.0f), 0.0f), 0.5f);
	Metaball *metaball = new Metaball(sphereList, 3);
	metaball->SetThreshold(2.3f);
	metaball->GetMaterial().SetColor(Color::green);
	metaball->GetMaterial().SetDiffuse(1.0f);
	metaball->GetMaterial().SetSpecular(1.0f);
	metaball->GetMaterial().SetReflectivity(0.0f);
	//scene->AddObject(metaball);

	raytracer->SetScene(scene);

	//unthinkable without multithreading
	raytracer->SetShadowQuality(4);
	raytracer->SetMultisampling(4);
	raytracer->SetReflectionBlur(1);
	raytracer->SetOcclusion(4);
}

void cleanupScene()
{
	scene->DeleteAll();

	delete scene;
	delete target;
	delete cam;
	delete raytracer;
}

void render()
{
   //seed random
   struct timeval start;
   gettimeofday(&start, NULL);
	srand(start.tv_usec);

	//raytrace the scene
	raytracer->Render();
}

int main(int argc, char* argv[])
{
   //setup the scene
   printf("Setup...\n");
	setupScene();

   //render stuff
   printf("Begin Render\n");
#ifdef __linux__
   struct timeval start, end;
   gettimeofday(&start, NULL);
#endif
   render();
#ifdef __linux__
   gettimeofday(&end, NULL);
   long seconds = end.tv_sec - start.tv_sec;
   long useconds = end.tv_usec - start.tv_usec;
   long mtime = static_cast<long>((seconds * 1000 + useconds / 1000.0f) + 0.5f);
   printf("Render Complete, Time Taken: %ld ms\n", mtime);
#else
   printf("Render Complete\n");
#endif

	//cleanup everything
   printf("Cleanup\n");
	cleanupScene();
}
