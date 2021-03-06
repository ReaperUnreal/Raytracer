#include "main.h"

Raytracer::Raytracer(void)
{
	rs = NULL;
	cam = NULL;
	sc = NULL;
	pu = NULL;
	shadowQuality = 1;
	reflectionBlur = 1;
	multisampling = 1;
	occlusion = 0;
}

Raytracer::~Raytracer(void)
{
	if(pu)
	{
		delete pu;
		pu = NULL;
	}
}


void Raytracer::SetTarget(RenderSurface *surface)
{
	rs = surface;
}

RenderSurface* Raytracer::GetTarget(void)
{
	return rs;
}

void Raytracer::SetCamera(Camera *camera)
{
	cam = camera;
}

Camera* Raytracer::GetCamera(void)
{
	return cam;
}

void Raytracer::SetScene(Scene *scene)
{
	sc = scene;
}

Scene* Raytracer::GetScene(void)
{
	return sc;
}

void Raytracer::SetProgressUpdater(ProgressUpdater *progress)
{
	pu = progress;
}

void Raytracer::SetProgress(float p)
{
	if(pu)
	{
		pu->Update(p);
	}
}

void Raytracer::InitProgress()
{
	if(pu)
	{
		pu->Init();
	}
}

void Raytracer::FinishProgress()
{
	if(pu)
	{
		pu->Finish();
	}
}

void Raytracer::SetShadowQuality(int quality)
{
	if(quality < 1)
		quality = 1;
	shadowQuality = quality;
}

int Raytracer::GetShadowQuality(void)
{
	return shadowQuality;
}

void Raytracer::SetReflectionBlur(int blur)
{
	if(blur < 1)
		blur = 1;
	reflectionBlur = blur;
}

int Raytracer::GetReflectionBlur(void)
{
	return reflectionBlur;
}

void Raytracer::SetMultisampling(int numSamples)
{
	multisampling = (numSamples > 1) ? numSamples : 1;
}

int Raytracer::GetMultisampling(void)
{
	return multisampling;
}

void Raytracer::SetOcclusion(int numSamples)
{
	occlusion = (numSamples > 0) ? numSamples : 0;
}

int Raytracer::GetOcclusion(void)
{
	return occlusion;
}

Geometry* Raytracer::Raytrace(Ray &r, Color &col, int depth, float &dist)
{
	if(depth > TRACEDEPTH)
		return NULL;
	dist = MAX_DIST;
	Vector intersection, L, N, V, R, sampleDir, samplePos;
	Geometry *geom = NULL;
	//int result = 0;
	int res;
	float dot, diff, spec, shade = 0.0f, ldist, rdist, tempdist, ambient;
	float step = 1.0f / lrflti(shadowQuality);
	Color diffuse, specular, reflection, accumulator;
	Ray shadow, ambientRay;
	bool inShade;

	//find the nearest intersection
	vector<Geometry *> &gv = sc->GetObjects();
	for(vector<Geometry *>::iterator git = gv.begin(); git != gv.end(); git++)
	{
		if((*git)->GetType() == Geometry::BVHACCEL)
		{
			BVH *bvh = static_cast<BVH *>(*git);
			Geometry *ng = NULL;
			res = bvh->IntersectRecursive(r, dist, &ng);
			if(res)
			{
				geom = ng;
			}
		}
		else
		{
			res = (*git)->Intersect(r, dist);
			if(res)
			{
				geom = *git;
				//result = res;
			}
		}
	}

	//if there's no hit, terminate the ray
	if(!geom)
		return NULL;

	if(geom->IsLight())
	{
		col = geom->GetMaterial().GetColor() * geom->GetLightIntensity();
	}
	else
	{
		//determine the point of intersection
		intersection = r.origin + (r.direction * dist);

		//accumulate the lighting
		vector<Geometry *> &lov = sc->GetObjects();
		for(vector<Geometry *>::iterator lit = lov.begin(); lit != lov.end(); lit++)
		{
			if(((*lit)->IsLight()) && ((*lit)->GetType() == Geometry::SPHERE))
			{
				L = ((Sphere *)(*lit))->GetPosition() - intersection;
				ldist = L.Length();
				L /= ldist;

				if(geom->GetType() == Geometry::SDFUNC)
				{
					N = geom->GetNormal(intersection);
					shadow.origin = intersection + N * (10.0f * EPSILON);
				}
				else
				{
					shadow.origin = intersection + L * EPSILON;
				}

				//shadow
				if(shadowQuality == 1)
				{
               Geometry *lcache = (*lit)->GetLightCacheItem(omp_get_thread_num());
					shade = 1.0f;

					shadow.direction = L;
               
               //try shadow cache first
               bool lightCacheHit = false;
               if(lcache && (lcache->Intersect(shadow, ldist)))
               {
                  lightCacheHit = true;
                  shade = 0.0f;
               }

               //now iterate through the geometry
               if(!lightCacheHit)
               {
                  vector<Geometry *> &sov = sc->GetObjects();
                  for(vector<Geometry *>::iterator sit = sov.begin(); sit != sov.end(); sit++)
                  {
                     if((*sit != *lit) && (*sit != lcache) && ((*sit)->Intersect(shadow, ldist)))
                     {
                        (*lit)->SetLightCacheItem(*sit, omp_get_thread_num());
                        shade = 0.0f;
                        break;
                     }
                  }
               }
				}
				else
				{
					shade = 0.0f;
					int i;
					//apparently parallel shadows slows it down
					//#pragma omp parallel for default(none) shared(light, intersection, ldist, numItems, step) private(i, inShade, tempdist, shadowObj, s) firstprivate(shadow) reduction(+:shade) schedule(dynamic, 1)
					for(i = 0; i < shadowQuality; i++)
					{
                  Geometry *lcache = (*lit)->GetLightCacheItem(omp_get_thread_num());
						shadow.direction = (*lit)->GeneratePoint() - intersection;
						shadow.direction.Normalize();
						inShade = false;
						tempdist = ldist;

                  //try shadow cache first
                  bool lightCacheHit = false;
                  if(lcache && (lcache->Intersect(shadow, tempdist)))
                  {
                     lightCacheHit = true;
                     shade = 0.0f;
                  }

                  //now iterate through the geometry
                  if(!lightCacheHit)
                  {
                     vector<Geometry *> &sov = sc->GetObjects();
                     for(vector<Geometry *>::iterator sit = sov.begin(); sit != sov.end(); sit++)
                     {
                        if((*sit != *lit) && (*sit != lcache) && ((*sit)->Intersect(shadow, tempdist)))
                        {
                           (*lit)->SetLightCacheItem(*sit, omp_get_thread_num());
                           inShade = true;
                           break;
                        }
                     }
                  }

                  //update the shading
						if(!inShade)
						{
							shade += step;
						}
					}
				}

				//make sure the point isn't in a shadow
				if(shade != 0.0f)
				{
					N = geom->GetNormal(intersection);

					//diffuse
					if(geom->GetMaterial().GetDiffuse() > 0)
					{
						dot = N.Dot(L);
						if(dot > 0)
						{
							diff = dot * geom->GetMaterial().GetDiffuse();
							diffuse = geom->GetMaterial().GetColor() * (*lit)->GetMaterial().GetColor() * diff;
						}
					}

					//specular
					if(geom->GetMaterial().GetSpecular() > 0)
					{
						V = r.direction;
						R = L - N * L.Dot(N) * 2.0f;
						dot = V.Dot(R);
						if(dot > 0)
						{
							spec = powf(dot, 20.0f) * geom->GetMaterial().GetSpecular();
							specular = geom->GetMaterial().GetSpecularColor() * (*lit)->GetMaterial().GetColor() * spec;
						}
					}

					col += (diffuse + specular) * shade;
					//col += geom->GetMaterial().GetColor();
				}
			}
		} //end light loop

		//calculate ambient lighting
		if(occlusion > 0)
		{
			//setup needed for both types of AO
			ambient = 0.0f;

			//we can use really fake and fast AO for SDFs
			if(geom->GetType() == Geometry::SDFUNC)
			{
				//this type of AO calculation needs far less "rays"
				occlusion /= 12;
				if(occlusion == 0)
					occlusion = 1;

				//from Rendering Worlds With Two Triangles paper
				//ao = 1 - k * sum(1, 5, 1/(2^i) * (pink(i) - yellow(i)));
				// pink(i) = distance from intersection point to sample point along the normal
				//         = delta * i
				// yellow(i) = distance from sample point to surface
				//           = distfield(intersection + delta * i * n)
				N = geom->GetNormal(intersection);
				SDF *sdf = reinterpret_cast<SDF *>(geom);
				float sum = 0.0f;
				float k = 0.3f; // magic number for choosing overall strength of AO
				float scale = 1.0f;
				float delta = 0.1f; // magic number for sample distances
				for(int i = 1; i <= occlusion; i++)
				{
					scale *= 0.5f;
					float pink = delta * static_cast<float>(i);

					Vector sample = intersection + (N * pink);
					float yellow = sdf->distance(sample);

					sum += scale * fmax(pink - yellow, 0.0f); //note that pink >= yellow since the intersection point is the furthest possible you could go
				}
				ambient = k * (1.0f - sum);
			}
			else
			{
				//initialize the ambient light
				float u, v, sqrtv, angle, ambdist;
				bool intersected;
				Vector sampleDir;
				float invsamples = 1.0f / lrflti(occlusion);

				//shoot out cosine weighted rays
				for(int i = 0; i < occlusion; i++)
				{
					u = lrflti(rand()) * MAX_RAND_DIVIDER * TWOPI;
					v = lrflti(rand()) * MAX_RAND_DIVIDER;
					sqrtv = sqrtf(v);
					sampleDir =  Vector(cosf(u) * sqrtv, sinf(u) * sqrtv, sqrtf(1.0f - v));

					N = geom->GetNormal(intersection);
					angle = N.Dot(sampleDir);
					if(angle < 0)
					{
						sampleDir *= -1.0f;
						angle = -angle;
					}

					intersected = false;
					if(geom->GetType() == Geometry::SDFUNC)
					{
						ambientRay.origin = intersection + N * (10.0f * EPSILON);
					}
					else
					{
						ambientRay.origin = intersection + sampleDir * EPSILON;
					}
					ambientRay.direction = sampleDir;
					ambdist = MAXDEPTH;
					vector<Geometry *> &aov = sc->GetObjects();
					for(vector<Geometry *>::iterator aoit = aov.begin(); aoit != aov.end(); aoit++)
					{
						if((!(*aoit)->IsLight()) && ((*aoit)->Intersect(ambientRay, ambdist)))
						{
							intersected = true;
							break;
						}
					}

					if(!intersected)
					{
						ambient += invsamples * angle;
					}
				}
			}

			col += geom->GetMaterial().GetColor() * geom->GetMaterial().GetDiffuse() * ambient;
		}

		//get reflection
		if(geom->GetMaterial().GetReflectivity() > 0)
		{	
			if(depth < TRACEDEPTH)
			{
				/*N = geom->GetNormal(intersection);
				  R = r.direction - N * (2.0f * N.Dot(r.direction));
				  Raytrace(Ray(intersection + R * EPSILON, R), reflection, depth + 1, rdist);
				  col += reflection * geom->GetMaterial().GetColor() * geom->GetMaterial().GetReflectivity();*/

				N = geom->GetNormal(intersection);
				R = r.direction - N * (2.0f * N.Dot(r.direction));
				samplePos = intersection + R * EPSILON;
				Ray reflectedRay(samplePos, R);
				Raytrace(reflectedRay, accumulator, depth + 1, rdist);
				for(int i = 1; i < reflectionBlur; i++)
				{
					float x = ((0.2f * lrflti(rand())) / RAND_MAX) - 0.1f;
					float y = ((0.2f * lrflti(rand())) / RAND_MAX) - 0.1f;
					float z = ((0.2f * lrflti(rand())) / RAND_MAX) - 0.1f;
					sampleDir = R + Vector(x, y, z);
					sampleDir.Normalize();
					Ray sampleRay(samplePos, sampleDir);
					Raytrace(sampleRay, reflection, depth + 1, rdist);
					accumulator += reflection;
				}
				if(reflectionBlur > 1)
					accumulator /= lrflti(reflectionBlur);
				col += accumulator * geom->GetMaterial().GetColor() * geom->GetMaterial().GetReflectivity();
			}
		}
	}
	return geom;
}

//random supersampling
void Raytracer::Render(void)
{
	//need to have a camera, a render surface, and a scene
	if(!cam)
		return;
	if(!rs)
		return;
	if(!sc)
		return;


	//get the data we need
	Vector o = cam->position;
	Vector updir = cam->up;
	Vector rightdir = cam->right;

	int width = rs->GetWidth();
	int height = rs->GetHeight();
	Color *surface = rs->GetSurface();

	float invaspect = lrflti(height) / lrflti(width);
	float xmag = tanf(cam->fov);
	float ymag = xmag * invaspect;

	Vector topleft = o + cam->normal - rightdir * xmag + updir * ymag;
	Vector dx = rightdir * (2.0f * xmag / lrflti(width));
	Vector dy = updir * (-2.0f * ymag / lrflti(height));
	Vector screenpos, currline = topleft, dir;

	int numSamples = multisampling;
	Vector d = dx + dy;
	Vector offset = o + d * 0.5;
	Color accumulator;
	float divider = 1.0f / lrflti(numSamples);

	Ray r;
	r.origin = o;

	Color pixel;
	float dist = 0.0f;
	InitProgress();

	//render each line in parallel
	int y, x, s;
	float u = 0.0f, v = 0.0f;
#ifdef OMP_ENABLE
#pragma omp parallel for default(none) shared(height, width, dy, dx, o, surface, topleft, offset, numSamples, divider) private(x, y, screenpos, pixel, dir, s, accumulator, u, v) firstprivate(r, dist) schedule(dynamic, 2)
#endif
	for(y = 0; y < height; y++)
	{
		screenpos = topleft + (dy * lrflti(y));
		//now each pixel of that line
		for(x = 0; x < width; x++, screenpos += dx)
		{
			//for each sample of that pixel
			accumulator = Color::black;
			for(s = 0; s < numSamples; s++)
			{
				//clear out the pixel
				pixel = Color::black;
				//create the ray
#ifdef DISABLE_JITTER
				dir = screenpos - offset;
#else
				u = lrflti(rand()) * MAX_RAND_DIVIDER;
				v = lrflti(rand()) * MAX_RAND_DIVIDER;
				dir = screenpos - offset + (dx * u) + (dy * v);
#endif
				dir = dir / dir.Length();
				r.direction = dir;
				//trace the actual ray
				Raytrace(r, pixel, 0, dist);
				accumulator += pixel;
			}

			//update the pixel color
			surface[y * width + x] = accumulator * divider;

			//update the progression
			SetProgress(static_cast<float>(y * width + x) / static_cast<float>(width * height));
		}
	}

	FinishProgress();
}
