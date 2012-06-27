#include "main.h"

//the Material class
Material::Material(void) : diffuseColor(0.0f, 0.0f, 0.0f), specularColor(1.0f, 1.0f, 1.0f), diffuse(1.0f), specular(0.0f), reflectivity(0.0f), ior(0.0f)
{
}

Material::~Material(void)
{
}

void Material::SetColor(const Color &c)
{
	diffuseColor = c;
}

Color Material::GetColor(void) const
{
	return diffuseColor;
}

void Material::SetSpecularColor(const Color &c)
{
	specularColor = c;
}

Color Material::GetSpecularColor(void) const
{
	return specularColor;
}

void Material::SetEmissiveColor(const Color &c)
{
	emissiveColor = c;
}

Color Material::GetEmissiveColor(void) const
{
	return emissiveColor;
}

void Material::SetEmissive(const float emis)
{
	emissive = emis;
}

float Material::GetEmissive(void) const
{
	return emissive;
}

void Material::SetDiffuse(const float diff)
{
	diffuse = diff;
}

float Material::GetDiffuse(void) const
{
	return diffuse;
}

void Material::SetSpecular(const float spec)
{
	specular = spec;
}

float Material::GetSpecular(void) const
{
	return specular;
}

void Material::SetReflectivity(const float refl)
{
	reflectivity = refl;
}

float Material::GetReflectivity(void) const
{
	return reflectivity;
}

void Material::SetIOR(const float index)
{
	ior = index;
}

float Material::GetIOR(void) const
{
	return ior;
}

//the Geometry class
Geometry::Geometry(void) : mat(), name("Unknown"), isLight(false), lightIntensity(1.0f)
{
}

Geometry::~Geometry(void)
{
}

void Geometry::SetLight(bool l)
{
	isLight = l;
}

bool Geometry::IsLight(void) const
{
	return isLight;
}

void Geometry::SetLightIntensity(float i)
{
	lightIntensity = i;
}

float Geometry::GetLightIntensity(void)
{
	return lightIntensity;
}

void Geometry::SetMaterial(const Material &m)
{
	mat = m;
}

Material& Geometry::GetMaterial(void)
{
	return mat;
}

void Geometry::SetName(std::string nm)
{
	name = nm;
}

std::string Geometry::GetName(void) const
{
	return name;
}

//the Plane class
Plane::Plane(const Vector &norm, const float dist) : normal(norm), d(dist)
{
}

Plane::~Plane(void)
{
}

int Plane::GetType(void) const
{
	return PLANE;
}

Vector Plane::GetNormal(void) const
{
	return normal;
}

Vector Plane::GetNormal(Vector &pos) const
{
	return normal;
}

float Plane::GetD(void) const
{
	return d;
}

void Plane::SetNormal(const Vector &n)
{
	normal = n;
}

void Plane::SetD(const float dist)
{
	d = dist;
}

int Plane::Intersect(Ray &r, float &mindist) const
{
	float angle = normal.Dot(r.direction);
	if(angle != 0)
	{
		float dist = -(normal.Dot(r.origin) + d) / angle;
		if(dist > 0)
		{
			if(dist < mindist)
			{
				mindist = dist;
				return HIT;
			}
		}
	}
	return MISS;
}

Vector Plane::GeneratePoint() const
{
	float x = ((20.0f * lrflti(rand())) * MAX_RAND_DIVIDER) - 10.0f;
	float y = ((20.0f * lrflti(rand())) * MAX_RAND_DIVIDER) - 10.0f;
	float z = -((normal.xv() * x + normal.yv() * y + d) / normal.zv());
	return Vector(x, y, z);
}

//the Sphere class
Sphere::Sphere(const Vector &pos, const float r) : position(pos), radius(r), sqrRadius(r * r), invRadius(1.0f / r)
{
}

Sphere::~Sphere(void)
{
}

Vector Sphere::GetPosition(void) const
{
	return position;
}

void Sphere::SetPosition(const Vector &pos)
{
	position = pos;
}

float Sphere::GetRadius(void) const
{
	return radius;
}

float Sphere::GetSqrRadius(void) const
{
	return sqrRadius;
}

float Sphere::GetInvRadius(void) const
{
	return invRadius;
}

void Sphere::SetRadius(const float r)
{
	radius = r;
	sqrRadius = r * r;
	invRadius = 1.0f / r;
}

Vector Sphere::GetNormal(Vector &pos) const
{
	return (pos - position) * invRadius;
}

int Sphere::Intersect(Ray &r, float &mindist) const
{
#ifdef SSE2_ENABLE
   static const __m128 SIGNMASK = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));
   __m128 v = _mm_sub_ps(r.origin.v, position.v);
   #ifdef __SSE4_1__
      __m128 b = _mm_dp_ps(v, r.direction.v, 0xEF);
   #else
      __m128 b = _mm_mul_ps(v, r.direction.v);
      b = _mm_hadd_ps(b, b);
      b = _mm_hadd_ps(b, b);
   #endif
   __m128 b2 = _mm_mul_ps(b, b);
   #ifdef __SSE4_1__
      __m128 vmag2 = _mm_dp_ps(v, v, 0xEF);
   #else
      __m128 vmag2 = _mm_mul_ps(v, v);
      vmag2 = _mm_hadd_ps(vmag2, vmag2);
      vmag2 = _mm_hadd_ps(vmag2, vmag2);
   #endif
   __m128 r2 = _mm_set1_ps(sqrRadius);
   b = _mm_xor_ps(b, SIGNMASK); //wait until the last second to negate b

   __m128 det = _mm_sub_ps(b2, vmag2);
   det = _mm_add_ps(det, r2); //determinant is now in all slots of the vector
   int retval = MISS;
   if(_mm_comigt_ss(det, _mm_setzero_ps()))
   {
      det = _mm_sqrt_ps(det);
      __m128 i2v = _mm_add_ps(b, det); //addsub would just be stupid
      if(_mm_comigt_ss(i2v, _mm_setzero_ps()))
      {
         __m128 i1v = _mm_sub_ps(b, det);
         __m128 minv = _mm_set1_ps(mindist);
         if(_mm_comilt_ss(i1v, _mm_setzero_ps()))
         {
            if(_mm_comilt_ss(i2v, minv))
            {
               mindist = reinterpret_cast<vector_access&>(i2v).array[0];
               retval = INPRIM;
            }
         }
         else
         {
            if(_mm_comilt_ss(i1v, minv))
            {
               mindist = reinterpret_cast<vector_access&>(i1v).array[0];
               retval = HIT;
            }
         }
      }
   }
#else
	Vector v = r.origin - position;
	float b = -v.Dot(r.direction);
	float det = (b * b) - v.Dot(v) + sqrRadius;
	int retval = MISS;
	if(det > 0)
	{
		det = sqrtf(det);
		float i2 = b + det;
		if(i2 > 0)
		{
         float i1 = b - det;
			if(i1 < 0)
			{
				if(i2 < mindist)
				{
					mindist = i2;
					retval = INPRIM;
				}
			}
			else
			{
				if(i1 < mindist)
				{
					mindist = i1;
					retval = HIT;
				}
			}
		}
	}
#endif
	return retval;
}

int Sphere::GetType(void) const
{
	return SPHERE;
}

Vector Sphere::GeneratePoint(void) const
{
	float u = lrflti(rand()) * MAX_RAND_DIVIDER;
	float v = lrflti(rand()) * MAX_RAND_DIVIDER;
	float rsqrtv = sqrtf(v) * radius;
	float twopiu = TWOPI * u;
	return Vector(cosf(twopiu) * rsqrtv, sinf(twopiu) * rsqrtv, radius * sqrtf(1.0f - v)) + position;
}

int Sphere::GetIntersectionPoints(Ray &r, float &d1, float &d2) const
{
	//return both intersection points
	Vector v = r.origin - position;
	float b = -v.Dot(r.direction);
	float det = (b * b) - v.Dot(v) + sqrRadius;
	if(det > 0)
	{
		det = sqrtf(det);
		d1 = b - det;
		d2 = b + det;
		if(d2 > 0)
		{
			if(d1 < 0)
			{
				return INPRIM;
			}
			else
			{
				return HIT;
			}
		}
		return MISS;
	}
	return MISS;
}

//the Axis Aligned Box class
AABox::AABox(const Vector &min, const Vector &max)
{
   //ensure min and max before storing
   bounds[0] = Vector::Min(min, max);
   bounds[1] = Vector::Max(min, max);
}

AABox::~AABox(void)
{
}

Vector AABox::GetMin() const
{
   return bounds[0];
}

Vector AABox::GetMax() const
{
   return bounds[1];
}

int AABox::GetType() const
{
   return AABOX;
}

int AABox::Intersect(Ray &r, float &mindist) const
{
   //smits' method
   float tmin, tmax, tymin, tymax, tzmin, tzmax;
   Vector invdir = r.direction.Reciprocal();

   float dirx = r.direction.xv();
   float invdirx = invdir.xv();
   if(dirx >= 0)
   {
      tmin = (bounds[0].xv() - r.origin.xv()) * invdirx;
      tmax = (bounds[1].xv() - r.origin.xv()) * invdirx;
   }
   else
   {
      tmin = (bounds[1].xv() - r.origin.xv()) * invdirx;
      tmax = (bounds[0].xv() - r.origin.xv()) * invdirx;
   }
   float diry = r.direction.yv();
   float invdiry = invdir.yv();
   if(diry >= 0)
   {
      tymin = (bounds[0].yv() - r.origin.yv()) * invdiry;
      tymax = (bounds[1].yv() - r.origin.yv()) * invdiry;
   }
   else
   {
      tymin = (bounds[1].yv() - r.origin.yv()) * invdiry;
      tymax = (bounds[0].yv() - r.origin.yv()) * invdiry;
   }

   if((tmin > tymax) || (tymin > tmax))
      return MISS;

   if(tymin > tmin)
      tmin = tymin;
   if(tymax < tmax)
      tmax = tymax;

   float dirz = r.direction.zv();
   float invdirz = invdir.zv();
   if(dirz >= 0)
   {
      tzmin = (bounds[0].zv() - r.origin.zv()) * invdirz;
      tzmax = (bounds[1].zv() - r.origin.zv()) * invdirz;
   }
   else
   {
      tzmin = (bounds[1].zv() - r.origin.zv()) * invdirz;
      tzmax = (bounds[0].zv() - r.origin.zv()) * invdirz;
   }

   if((tmin > tzmax) || (tzmin > tmax))
      return MISS;

   if(tzmin > tmin)
      tmin = tzmin;
   if(tzmax < tmax)
      tmax = tzmax;

   if((tmin < mindist) && (tmax > 0))
   {
      mindist = tmin;
      return HIT;
   }

   return MISS;
}

Vector AABox::GetNormal(Vector &pos) const
{
   Vector mind = Vector::Abs(pos - bounds[0]);
   Vector maxd = Vector::Abs(pos - bounds[1]);

   float vals[6] = {mind.xv(), mind.yv(), mind.zv(), maxd.xv(), maxd.yv(), maxd.zv()};
   float min = vals[0];
   int mini = 0;
   for(int i = 1; i < 6; i++)
   {
      if(vals[i] < min)
      {
         min = vals[i];
         mini = i;
      }
   }

   switch(mini)
   {
      case 0:
         return Vector(-1, 0, 0);
      case 1:
         return Vector(0, -1, 0);
      case 2:
         return Vector(0, 0, -1);
      case 3:
         return Vector(1, 0, 0);
      case 4:
         return Vector(0, 1, 0);
      case 5:
         return Vector(0, 0, 1);
      default: break;
   }

   return Vector(1, 0, 0);
}

Vector AABox::GeneratePoint() const
{
   float xw = lrflti(rand()) * MAX_RAND_DIVIDER;
   float yw = lrflti(rand()) * MAX_RAND_DIVIDER;
   float zw = lrflti(rand()) * MAX_RAND_DIVIDER;
   Vector weights(xw, yw, zw);

   Vector res = (bounds[1] - bounds[0]) * weights;
   res += bounds[0];

   return res;
}

//the Metaball class
Metaball::Metaball(Sphere **ballList, int ballCount) : balls(ballList), numBalls(ballCount), threshold(1.0f), stepVal(0.01f)
{
}

Metaball::~Metaball(void)
{
	if(balls)
	{
		for(int i = 0; i < numBalls; i++)
		{
			delete balls[i];
		}
		delete[] balls;
		balls = NULL;
		numBalls = 0;
	}
}

Sphere Metaball::GetBounds(void) const
{
	//calculate bounding sphere
	float weight = 1.0f / sqrtf(threshold);
	if(numBalls <= 0)
	{
		//no balls
		return Sphere(Vector(), 1.0f);
	}
	else if(numBalls == 1)
	{
		//the bounds is the ball
		return Sphere(balls[0]->GetPosition(), weight);
	}
	else if(numBalls == 2)
	{
		//special case
		Vector line = balls[1]->GetPosition() - balls[0]->GetPosition();
		float dist = line.Length();
		return Sphere(balls[0]->GetPosition() + (line* 0.5f), (dist * 0.5f) + weight);
	}
	else
	{
		//general case
		Vector min = balls[0]->GetPosition(), max = balls[0]->GetPosition();
		for(int i = 1; i < numBalls; i++)
		{
			//mins
         min = Vector::Min(balls[i]->GetPosition(), min);
         /*
			if(balls[i]->GetPosition().x < min.x)
				min.x = balls[i]->GetPosition().x;
			if(balls[i]->GetPosition().y < min.y)
				min.y = balls[i]->GetPosition().y;
			if(balls[i]->GetPosition().z < min.z)
				min.z = balls[i]->GetPosition().z;
         */

			//max
         max = Vector::Max(balls[i]->GetPosition(), max);
         /*
			if(balls[i]->GetPosition().x > max.x)
				max.x = balls[i]->GetPosition().x;
			if(balls[i]->GetPosition().y > max.y)
				max.y = balls[i]->GetPosition().y;
			if(balls[i]->GetPosition().z > max.z)
				max.z = balls[i]->GetPosition().z;
         */
		}
		Vector line = max - min;
		float dist = line.Length();
		return Sphere(min + (line * 0.5f), (dist * 0.5f) + weight);
	}
}

float Metaball::GetThreshold(void) const
{
	return threshold;
}

void Metaball::SetThreshold(float val)
{
	threshold = val;
	if(threshold <= 0.0f)
		threshold = EPSILON;
}

Sphere* Metaball::GetBall(int num)
{
	if(num < numBalls)
	{
		return balls[num];
	}
	else
	{
		return NULL;
	}
}

void Metaball::SetBallList(Sphere **ballList, int ballCount)
{
	balls = ballList;
	numBalls = ballCount;
}

float Metaball::PotentialFunction(Vector p1, Vector p2) const
{
	// f = 1/(r^2)
	Vector d = p2 - p1;
	float r2 = d.SqrLength();
	if (r2 == 0.0f)
		r2 = 1.0f;
	return 1.0f / r2;
}

Vector Metaball::GetNormal(Vector &pos) const
{
	//distance weighted mean of all normals used to compose
	Vector accumulator, ray;
	float func;
	for(int i = 0; i < numBalls; i++)
	{
		func = PotentialFunction(pos, balls[i]->GetPosition());
		accumulator += (pos - balls[i]->GetPosition()) * func;
	}
	if(numBalls > 0)
	{
		accumulator.Normalize();
		return accumulator;
	}
	else
	{
		return Vector();
	}
	//return Vector(0, 0, 1);
}

int Metaball::Intersect(Ray &r, float &mindist) const
{
	//get bounds
	Sphere bounds = GetBounds();
	
	//get both intersection points
	float startVal = 0.0f, endVal = 0.0f;
	int intersect = bounds.GetIntersectionPoints(r, startVal, endVal);
	if(intersect == MISS)
		return MISS;
	if(intersect == INPRIM)
		startVal = 2.0f * stepVal;
	if(startVal > mindist)
		return MISS;
	if(endVal > mindist)
		endVal = mindist;

	//step from beginning to end of bounds
	Vector currPoint = r.origin + r.direction * startVal;
	Vector step = r.direction * stepVal;
	float accumulator;
	for(float counter = startVal; counter < endVal; counter += stepVal, currPoint += step)
	{
		//get intersection value
		accumulator = 0.0f;
		for(int i = 0; i < numBalls; i++)
		{
			accumulator += PotentialFunction(currPoint, balls[i]->GetPosition());
		}

		if(accumulator > threshold)
		{
			mindist = counter;
			return HIT;
		}
	}
	return MISS;
}

int Metaball::GetType(void) const
{
	return METABALL;
}

Vector Metaball::GeneratePoint(void) const
{
	if(numBalls > 0)
	{
		int num = rand() % numBalls;
		return balls[num]->GetPosition();
	}
	else
	{
		return Vector();
	}
}


//the Signed Distance Function class
SDF::SDF() : threshold(0.001f), iterations(1024)
{
}

SDF::~SDF()
{
}

void SDF::SetThreshold(float val)
{
   threshold = val;
}

float SDF::GetThreshold() const
{
   return threshold;
}

void SDF::SetIterations(int i)
{
   iterations = i;
}

int SDF::GetIterations() const
{
   return iterations;
}

Vector SDF::GetNormal(Vector &pos) const
{
   //Gradient method, use (v-e)-(v+e)
   Vector vxp = pos + Vector(EPSILON, 0, 0);
   Vector vyp = pos + Vector(0, EPSILON, 0);
   Vector vzp = pos + Vector(0, 0, EPSILON);
   Vector vxn = pos - Vector(EPSILON, 0, 0);
   Vector vyn = pos - Vector(0, EPSILON, 0);
   Vector vzn = pos - Vector(0, 0, EPSILON);
   float dxp = distance(vxp);
   float dyp = distance(vyp);
   float dzp = distance(vzp);
   float dxn = distance(vxn);
   float dyn = distance(vyn);
   float dzn = distance(vzn);
   Vector n(dxp - dxn, dyp - dyn, dzp - dzn);
   n.Normalize();
   return n;
}

int SDF::Intersect(Ray &r, float &mindist) const
{
   int i = 0;
   float t = 0;
   while((t < mindist) && (i < iterations))
   {
      Vector p = r.origin + (r.direction * t);
      float d = distance(p);
      if(d < EPSILON)
      {
         i++;
      }
      if(fabsf(d) < threshold)
      {
         mindist = t;
         return HIT;
      }
      t += d;
      i++;
   }

   return MISS;
}

int SDF::GetType() const
{
   return SDFUNC;
}

Vector SDF::GeneratePoint() const
{
   //choose random point and direction, then raymarch in that direction until intersect
   return Vector(); //FIX ME
}

float SDF::distance(Vector &pos) const
{
   return 0.0f; //always hit?
}
