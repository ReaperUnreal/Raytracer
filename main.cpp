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
      static const Vector normal(0, 1, 0);
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

class SDFScene2 : public SDF
{
public:
   virtual float distance(Vector &pos) const
   {
      static const Vector spherePos(-1.5f, 1.5f, 0);
      float sd = sphere(translate(pos, spherePos), 1.23f);

      static const Vector normal(0, 1, 0);
      float fd = plane(pos, normal, -2.0f);

      static const Vector boxSize(1, 1, 1);
      static const Vector boxPos(-1.5f, 1.5f, 0);
      float bd = box(translate(pos, boxPos), boxSize);

      static const Vector torusPos(1.0f, 0, 0);
      float t82d = torus82(translate(rotateX(pos, PIBYTWO), torusPos), 1.0f, 0.5f);

      static const Vector t88Pos(1.6f, 1.0f, -1.3f);
      float t88d = torus88(rotateX(translate(pos, t88Pos), PIBYTWO), 0.6f, 0.3f);

      float d = opSubtract(bd, sd);
      d = opUnion(fd, d);
      d = opUnion(t82d, d);
      d = opUnion(t88d, d);


      return d;
   }
};

class SDFMetaball : public SDF
{
public:
   float c(const float r2) const
   {
      //if r < R
      //2r^3/R^3 - 3r^2/R^2 + 1
      //R = 2
      // => 1/4r^3 - 3/4r^2 + 1

      if(r2 >= 4.0f)
         return 0.0f;

      float r = sqrtf(r2);
      float r3 = r2 * r;
      return (0.25f * r3) - (0.75f * r2) + 1;
   }

   float f(const Vector &x) const
   {
      // the 3 points
      static const Vector v1(-1.0f, 0.0f, 0.0f);
      static const Vector v2(1.0f, 0.0f, 0.0f);
      static const Vector v3(0.0f, sqrtf(3.0f), 0.0f);

      //f = T - sum(c(||x - pi||))
      float r1 = (x - v1).SqrLength();
      float r2 = (x - v2).SqrLength();
      float r3 = (x - v3).SqrLength();

      float sum = c(r1) + c(r2) + c(r3);

      return 0.005f - sum;
   }

   virtual float distance(Vector &pos) const
   {
      //d(x, B) >= 2/3 * f(x) * sum(Ri)
      //Ri = 2
      //d(x, B) >= 2/3 * 6 * f(x)
      //d(x, B) >= 4 * f(x)

      float bd =  4.0f * f(pos);
      float d = bd;

      //cylinder
      Vector cp(pos.xv(), pos.yv(), 0.0f);
      float cd = cp.Length() - 1.0f;

      //d = fmax(-cd, bd);

      //the floor plane
      static const Vector normal(0, 1, 0);
      float fd = pos.Dot(normal) + 2.0f;

      d = fmin(d, fd);

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

   //SDFScene *sdf = new SDFScene();
   SDFScene2 *sdf = new SDFScene2();
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
	raytracer->SetShadowQuality(512);
	raytracer->SetMultisampling(64);
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
   Vector v1(2.5f, 4.5f, -1.5f);
   //v1.array[0] = 4;
   Vector v2(2.0f, 2.0f, 2.0f);

   printf("[%0.1f, %0.1f, %0.1f, %0.1f]\n", v1.array[0], v1.array[1], v1.array[2], v1.array[3]);
   printf("[%0.1f, %0.1f, %0.1f, %0.1f]\n", v2.array[0], v2.array[1], v2.array[2], v2.array[3]);

   Vector v = Vector::Mod(v1, v2);

   printf("[%0.1f, %0.1f, %0.1f, %0.1f]\n", v.array[0], v.array[1], v.array[2], v.array[3]);
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
