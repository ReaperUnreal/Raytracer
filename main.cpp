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
      static const Vector sphereMove(0.0f, 1.5f, 0.0f);
      Vector spherePos = translate(pos, sphereMove);
      float sd = sphere(repeatZ(repeatX(spherePos, 2.0f), 2.0f), 0.5f);

      //the floor plane
      static const Vector normal(0, 1, 0);
      float fd = pos.Dot(normal) + 2.0f;

      // the hidden squared donut
      static const Vector donutPos(0.0f, -1.5f, -5.5f);
      float td88 = torus88(rotateZ(rotateX(translate(pos, donutPos), PIBYTWO), PI * -0.125f), 1.0f, 0.3f);

      //union
      float d = fmin(fd, sd);
      d = fmin(d, td88);

      return d;
   }
};

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

	scene = new Scene(8);

   SDFScene *sdf = new SDFScene();
   sdf->GetMaterial().SetColor(Color::white);
   sdf->GetMaterial().SetDiffuse(1.0f);
	sdf->GetMaterial().SetSpecular(0.0f);
   scene->AddObject(sdf);

	Sphere *light = new Sphere(Vector(-4.0f, 4.0f, 3.0f), 1.0f);
	light->SetLight(true);
	light->SetLightIntensity(1.0f);
	light->GetMaterial().SetColor(Color::white);
	scene->AddObject(light);

   Sphere *ball = new Sphere(Vector(0.0f, 0.5f, 0.0f), 1.5f);
   ball->GetMaterial().SetColor(Color::white);
   ball->GetMaterial().SetDiffuse(0.0f);
   ball->GetMaterial().SetReflectivity(1.0f);
   scene->AddObject(ball);

	Plane *floor = new Plane(Vector(0.0f, 1.0f, 0.0f), 2.0f);
	floor->GetMaterial().SetColor(Color::white);
	floor->GetMaterial().SetDiffuse(1.0f);
   //scene->AddObject(floor);

	raytracer->SetScene(scene);

	//unthinkable without multithreading
	raytracer->SetShadowQuality(128);
	raytracer->SetMultisampling(32);
	raytracer->SetReflectionBlur(1);
   raytracer->SetOcclusion(0);
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
