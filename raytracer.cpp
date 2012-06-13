#include "main.h"

Raytracer::Raytracer(void)
{
	rs = NULL;
	cam = NULL;
	sc = NULL;
	shadowQuality = 1;
	reflectionBlur = 1;
	multisampling = 1;
	occlusion = 1;
}

Raytracer::~Raytracer(void)
{
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
	occlusion = (numSamples > 1) ? numSamples : 1;
}

int Raytracer::GetOcclusion(void)
{
	return occlusion;
}

Geometry* Raytracer::Raytrace(Ray &r, Color &col, int depth, float &dist)
{
	if(depth > TRACEDEPTH)
		return NULL;
	dist = 1000000.0f;
	Vector intersection, L, N, V, R, sampleDir, samplePos;
	Geometry *geom = NULL, *pg = NULL, *light = NULL, *shadowObj = NULL, *ambientObj = NULL;
	int result = 0;
	int res, l, s;
	int numItems = sc->GetNumObjects();
	float dot, diff, spec, shade, ldist, rdist, tempdist, ambient;
	float step = 1.0f / lrflti(shadowQuality);
	Color diffuse, specular, reflection, accumulator;
	Ray shadow, ambientRay;
	bool inShade;

	//find the nearest intersection
	for(int i = 0; i < numItems; i++)
	{
		pg = sc->GetObjectAt(i);
      res = pg->Intersect(r, dist);
		if(res)
		{
			geom = pg;
			result = res;
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
		for(l = 0; l < numItems; l++)
		{
			light = sc->GetObjectAt(l);
			if((light->IsLight()) && (light->GetType() == Geometry::SPHERE))
			{
				L = ((Sphere *)light)->GetPosition() - intersection;
				ldist = L.Length();
				L /= ldist;

				//shadow
				if(shadowQuality == 1)
				{
					shade = 1.0f;
					shadow.origin = intersection + L * EPSILON;
					shadow.direction = L;
					for(s = 0; s < numItems; s++)
					{
						shadowObj = sc->GetObjectAt(s);
						if((shadowObj != light) && (shadowObj->Intersect(shadow, ldist)))
						{
							shade = 0.0f;
							break;
						}
					}
				}
				else
				{
					shade = 0.0f;
					shadow.origin = intersection + L * EPSILON;
					int i;
					//apparently parallel shadows slows it down
					//#pragma omp parallel for default(none) shared(light, intersection, ldist, numItems, step) private(i, inShade, tempdist, shadowObj, s) firstprivate(shadow) reduction(+:shade) schedule(dynamic, 1)
					for(i = 0; i < shadowQuality; i++)
					{
						shadow.direction = light->GeneratePoint() - intersection;
						shadow.direction.Normalize();
						inShade = false;
						tempdist = ldist;
						for(s = 0; s < numItems; s++)
						{
							shadowObj = sc->GetObjectAt(s);
							if((shadowObj != light) && (shadowObj->Intersect(shadow, tempdist)))
							{
								inShade = true;
								break;
							}
						}
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
							diffuse = geom->GetMaterial().GetColor() * light->GetMaterial().GetColor() * diff;
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
							specular = geom->GetMaterial().GetSpecularColor() * light->GetMaterial().GetColor() * spec;
						}
					}

					col += (diffuse + specular) * shade;
				}
			}
		} //end light loop

		//calculate ambient lighting
		if(occlusion > 1)
		{
			//initialize the ambient light
			ambient = 0.0f;
			float invsamples = 1.0f / lrflti(occlusion);
			float u, v, sqrtv, angle, ambdist;
			bool intersected;
			Vector sampleDir;

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
				ambientRay.origin = intersection + sampleDir * EPSILON;
				ambientRay.direction = sampleDir;
				ambdist = MAXDEPTH;
				for(s = 0; s < numItems; s++)
				{
					ambientObj = sc->GetObjectAt(s);
					if((ambientObj != light) && (ambientObj->Intersect(ambientRay, ambdist)))
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
	Geometry *geom = NULL;
	float dist = 0.0f;

	//render each line in parallel
	int y, x, s;
	float u, v;
#ifdef OMP_ENABLE
   #pragma omp parallel for default(none) shared(height, width, dy, dx, o, surface, topleft, offset, numSamples, divider) private(x, y, screenpos, pixel, dir, geom, s, accumulator, u, v) firstprivate(r, dist) schedule(dynamic, 1)
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
				u = lrflti(rand()) * MAX_RAND_DIVIDER;
				v = lrflti(rand()) * MAX_RAND_DIVIDER;
				dir = screenpos - offset + (dx * u) + (dy * v);
				dir = dir / dir.Length();
				r.direction = dir;
				//trace the actual ray
				geom = Raytrace(r, pixel, 0, dist);
				accumulator += pixel;
			}

			//update the pixel color
			surface[y * width + x] = accumulator * divider;
		}
	}
}
