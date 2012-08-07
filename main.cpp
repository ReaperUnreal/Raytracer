#include "main.h"

Raytracer *raytracer;
Scene *scene;
Camera *cam;
RenderSurface *target;

void setupScene()
{
	raytracer = new Raytracer();

	cam = new Camera();
	cam->MoveTo(Vector(0.0f, 0.0f, 5.0f));
	cam->LookAt(Vector(0.0f, 0.0f, 0.0f));
	raytracer->SetCamera(cam);

	target = new RenderSurface(SCREEN_WIDTH, SCREEN_HEIGHT);
	raytracer->SetTarget(target);

	raytracer->SetProgressUpdater(new PercentMonitor());

	scene = new Scene(8);

	Sphere *sphere = new Sphere(Vector(0, 0, 0), 2.0f);
	sphere->GetMaterial().SetColor(Color::white);
	sphere->GetMaterial().SetDiffuse(1.0f);
	sphere->GetMaterial().SetSpecular(0.0f);
	sphere->GetMaterial().SetReflectivity(0.0f);
	//scene->AddObject(sphere);

	Sphere *light = new Sphere(Vector(-4.0f, 4.0f, 3.0f), 1.0f);
	light->SetLight(true);
	light->SetLightIntensity(1.0f);
	light->GetMaterial().SetColor(Color::white);
	scene->AddObject(light);

	//set the scene
	raytracer->SetScene(scene);

	//unthinkable without multithreading
	raytracer->SetShadowQuality(1);
	raytracer->SetMultisampling(1);
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

void outputImage(const char *filename = "out.bmp")
{
	BMP out;
	target->ToBMP(out);
	out.WriteToFile(filename);
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

	//output the render result
	printf("Writing Image\n");
	outputImage();

	//cleanup everything
	printf("Cleanup\n");
	cleanupScene();


	return 0;
}
