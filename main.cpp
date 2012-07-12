#include "main.h"

Raytracer *raytracer;
Scene *scene;
Camera *cam;
RenderSurface *target;

class SDFScene : public SDF
{
public:
   virtual float distance(Vector &pos) const
   {
      //just a sphere
      float sd = pos.Length() - 1.5f;

      //the floor plane
      Vector normal(0, 1, 0);
      float fd = pos.Dot(normal) + 2.0f;

      //cylinder
      Vector cp(pos.xv(), 0.0f, pos.zv());
      float cd = cp.Length() - 1.0f;

      //torus
      Vector txz(pos.xv(), 0.0f, pos.zv());
      float xz = txz.Length();
      Vector ty(xz - 1.0f, pos.yv() + 1.0f, 0.0f);
      float td = ty.Length() - 0.5;

      //subtraction
      float d = max(-sd, cd);

      //union
      d = fmin(fd, d);
      d = fmin(td, d);

      return d;
   }
};

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

   SDFScene *sdf = new SDFScene();
   sdf->GetMaterial().SetColor(Color::white);
   sdf->GetMaterial().SetDiffuse(1.0f);
	sdf->GetMaterial().SetSpecular(0.0f);
   scene->AddObject(sdf);

	Sphere *sphere = new Sphere(Vector(0, 0, 0), 2.0f);
	sphere->GetMaterial().SetColor(Color::white);
	sphere->GetMaterial().SetDiffuse(1.0f);
	sphere->GetMaterial().SetSpecular(0.0f);
	sphere->GetMaterial().SetReflectivity(0.0f);
   //scene->AddObject(sphere);

	Sphere *smaller = new Sphere(Vector(2.0f, -1.5f, 1.3f), 0.5f);
	smaller->GetMaterial().SetColor(Color::white);
	smaller->GetMaterial().SetDiffuse(0.0f);
	smaller->GetMaterial().SetSpecular(0.0f);
   smaller->GetMaterial().SetReflectivity(1.0f);
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
   //scene->AddObject(floor);

	Plane *wall = new Plane(Vector(0.0f, 0.0f, 1.0f), 6.0f);
	wall->GetMaterial().SetColor(Color::white);
	wall->GetMaterial().SetDiffuse(1.0f);
   //scene->AddObject(wall);

	Plane *wall2 = new Plane(Vector(1.0f, 0.0f, 0.0f), 6.0f);
	wall2->GetMaterial().SetColor(Color::red);
	wall2->GetMaterial().SetDiffuse(1.0f);
   //scene->AddObject(wall2);

	Plane *wall3 = new Plane(Vector(-1.0f, 0.0f, 0.0f), 6.0f);
	wall3->GetMaterial().SetColor(Color::blue);
	wall3->GetMaterial().SetDiffuse(1.0f);
   //scene->AddObject(wall3);

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

   AABox *floorbox = new AABox(Vector(-6.0f, -2.5f, -6.0f), Vector(6.0f, -2.0f, 6.0f));
   floorbox->GetMaterial().SetColor(Color::white);
   floorbox->GetMaterial().SetDiffuse(1.0f);
   //scene->AddObject(floorbox);

   AABox *wallbox1 = new AABox(Vector(-6.0f, -2.0f, -6.5f), Vector(6.0f, 6.0f, -6.0f));
   wallbox1->GetMaterial().SetColor(Color::white);
   wallbox1->GetMaterial().SetDiffuse(1.0f);
   //scene->AddObject(wallbox1);

   AABox *wallbox2 = new AABox(Vector(-6.5f, -2.0f, -6.0f), Vector(-6.0f, 6.0f, 6.0f));
   wallbox2->GetMaterial().SetColor(Color::red);
   wallbox2->GetMaterial().SetDiffuse(1.0f);
   //scene->AddObject(wallbox2);

   AABox *wallbox3 = new AABox(Vector(6.0f, -2.0f, -6.0f), Vector(6.5f, 6.0f, 6.0f));
   wallbox3->GetMaterial().SetColor(Color::blue);
   wallbox3->GetMaterial().SetDiffuse(1.0f);
   //scene->AddObject(wallbox3);

	raytracer->SetScene(scene);

	//unthinkable without multithreading
	raytracer->SetShadowQuality(256);
	raytracer->SetMultisampling(32);
	raytracer->SetReflectionBlur(1);
   raytracer->SetOcclusion(10);
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
   Vector v1(1, 2, 3);
   v1.array[0] = 4;
   Vector v2(5, 6, 7);
   v2.array[0] = 8;

   printf("[%0.1f, %0.1f, %0.1f, %0.1f]\n", v1.array[0], v1.array[1], v1.array[2], v1.array[3]);
   printf("[%0.1f, %0.1f, %0.1f, %0.1f]\n", v2.array[0], v2.array[1], v2.array[2], v2.array[3]);

   __m128 mv = _mm_addsub_ps(v1.v, v2.v);
   mv = _mm_shuffle_ps(mv, mv, _MM_SHUFFLE(0, 0, 0, 0));
   Vector m(mv);
   printf("[%0.1f, %0.1f, %0.1f, %0.1f]\n", m.array[0], m.array[1], m.array[2], m.array[3]);
}

void test2()
{
   Vector o(0, 0, 5);
   Vector d(0, 0, -1);
   Ray r(o, d);
   Vector dx(1.0f / 6.0f, 0, 0);
   Vector dy(1.0f / 6.0f, 0, 0);

	raytracer = new Raytracer();

	cam = new Camera();
	cam->MoveTo(Vector(0.0f, 0.0f, 5.0f));
	cam->LookAt(Vector(0.0f, 0.0f, 0.0f));
	raytracer->SetCamera(cam);

	target = new RenderSurface(SCREEN_WIDTH, SCREEN_HEIGHT);
	raytracer->SetTarget(target);

	scene = new Scene(8);

	//Sphere *sphere = new Sphere(Vector(0, 0, 0), 2.0f);
   SDFScene *sdf = new SDFScene();
   sdf->GetMaterial().SetColor(Color::white);
   sdf->GetMaterial().SetDiffuse(1.0f);
	sdf->GetMaterial().SetSpecular(0.0f);
   scene->AddObject(sdf);

	Sphere *light = new Sphere(Vector(0.0f, 0.0f, 7.0f), 1.0f);
	light->SetLight(true);
	light->SetLightIntensity(1.0f);
	light->GetMaterial().SetColor(Color::white);
	scene->AddObject(light);

	raytracer->SetScene(scene);

	//unthinkable without multithreading
	raytracer->SetShadowQuality(1);
	raytracer->SetMultisampling(1);
	raytracer->SetReflectionBlur(1);
   raytracer->SetOcclusion(0);

   for(int i = 0; i < 10000000; ++i)
   {
      Color pixel = Color::black;
      float mindist = MAX_DIST;
      float u = (lrflti(rand()) * MAX_RAND_DIVIDER) - 0.5f;
      float v = (lrflti(rand()) * MAX_RAND_DIVIDER) - 0.5f;
      Vector dir = d + (dx * u) + (dy * v);
      dir.Normalize();
      r.direction = dir;
      Geometry *geom = raytracer->Raytrace(r, pixel, 0, mindist);
      if(geom != sdf)
      {
         printf("No hit! d = <%0.1f, %0.1f, %0.1f> mindist = %0.1f\n", r.direction.xv(), r.direction.yv(), r.direction.zv(), mindist);
         break;
      }
      if((pixel.rv() < 0.05f) && (pixel.gv() < 0.05f) && (pixel.bv() < 0.05f))
      {
         printf("Black! c = <%0.1f, %0.1f, %0.1f> d = <%0.1f, %0.1f, %0.1f> mindist = %0.1f\n", pixel.rv(), pixel.gv(), pixel.bv(), r.direction.xv(), r.direction.yv(), r.direction.zv(), mindist);
         break;
      }
   }
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
