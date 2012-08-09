#include "main.h"

Raytracer *raytracer;
Scene *scene;
Camera *cam;
RenderSurface *target;

void setupScene()
{
	raytracer = new Raytracer();

	cam = new Camera();
	cam->MoveTo(Vector(0.0f, 0.5f, 5.0f));
	cam->LookAt(Vector(0.0f, -1.0f, 0.5f));
	raytracer->SetCamera(cam);

	target = new RenderSurface(SCREEN_WIDTH, SCREEN_HEIGHT);
	raytracer->SetTarget(target);

   raytracer->SetProgressUpdater(new PercentMonitor());

	scene = new Scene(100);

   //all the reflective spheres
   for(int y = 0; y < 11; y++)
   {
      for(int x = 0; x < 15; x++)
      {
         Vector pos = Vector(lrflti(2 * (x - 7)), -1.5f, lrflti(2 - (2 * y)));
         float offset = Perlin(pos);
         pos += Vector((offset - 0.5f) * 1.0f, (offset - 0.5f) * 0.2f, 0.0f);
         Sphere *sphere = new Sphere(pos, 0.5f);
         sphere->GetMaterial().SetColor(Color::white);
         sphere->GetMaterial().SetDiffuse(0.0f);
         sphere->GetMaterial().SetSpecular(0.0f);
         sphere->GetMaterial().SetReflectivity(1.0f);
         scene->AddObject(sphere);
      }
   }


   //the one red sphere
   Sphere *red = new Sphere(Vector(3.8f, -1.5f, -3.3f), 0.5f);
   red->GetMaterial().SetColor(Color::red);
   red->GetMaterial().SetDiffuse(1.0f);
   red->GetMaterial().SetSpecular(0.0f);
   scene->AddObject(red);

	Sphere *light = new Sphere(Vector(-4.0f, 4.0f, 3.0f), 1.0f);
	light->SetLight(true);
	light->SetLightIntensity(1.0f);
	light->GetMaterial().SetColor(Color::white);
	scene->AddObject(light);

	Plane *floor = new Plane(Vector(0.0f, 1.0f, 0.0f), 2.0f);
	floor->GetMaterial().SetColor(Color::white);
	floor->GetMaterial().SetDiffuse(1.0f);
   scene->AddObject(floor);

	raytracer->SetScene(scene);

	//unthinkable without multithreading
	raytracer->SetShadowQuality(128);
	raytracer->SetMultisampling(32);
	raytracer->SetReflectionBlur(1);
   raytracer->SetOcclusion(256);
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

void test()
{
   //Vector v1(2.5f, 4.5f, -1.5f);
   ////v1.array[0] = 4;
   //Vector v2(2.0f, 2.0f, 2.0f);

   //printf("[%0.1f, %0.1f, %0.1f, %0.1f]\n", v1.array[0], v1.array[1], v1.array[2], v1.array[3]);
   //printf("[%0.1f, %0.1f, %0.1f, %0.1f]\n", v2.array[0], v2.array[1], v2.array[2], v2.array[3]);

   //Vector v = Vector::Mod(v1, v2);

   //printf("[%0.1f, %0.1f, %0.1f, %0.1f]\n", v.array[0], v.array[1], v.array[2], v.array[3]);
   Vector pos(0.0f, 0.0f, 5.0f);
   Vector p(1.0f, 0.0f, 0.0f);

   //SDFMetaball *sdf = new SDFMetaball();
   //float f = sdf->distance(p);
   //printf("%f\n", f);
}

int main(int argc, char* argv[])
{
   //test();
   //test2();
   //return 0;

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
