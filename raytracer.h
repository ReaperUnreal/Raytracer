#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "main.h"

enum
{
	INPRIM = -1,
	MISS = 0,
	HIT = 1
};

const int TRACEDEPTH = 6;
const float MAXDEPTH = 100000.0f;

class Raytracer
{
public:
	Raytracer(void);
	~Raytracer(void);

	void SetTarget(RenderSurface *surface);
	RenderSurface* GetTarget(void);
	void SetCamera(Camera *camera);
	Camera* GetCamera(void);
	void SetScene(Scene *scene);
	Scene* GetScene(void);
   void SetProgressUpdater(ProgressUpdater *progress);
   void SetProgress(float p);
   void InitProgress();
   void FinishProgress();
	void SetShadowQuality(int quality);
	int GetShadowQuality(void);
	void SetReflectionBlur(int blur);
	int GetReflectionBlur(void);
	void SetMultisampling(int numSamples);
	int GetMultisampling(void);
	void SetOcclusion(int numSamples);
	int GetOcclusion(void);

	void Render(void);
	Geometry* Raytrace(Ray &r, Color &col, int depth, float &dist);
private:
	RenderSurface *rs;
	Camera *cam;
	Scene *sc;
   ProgressUpdater *pu;
	int shadowQuality;
	int reflectionBlur;
	int multisampling;
	int occlusion;
};

#endif
