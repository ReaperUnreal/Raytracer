#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "main.h"

//the Material class
class Material
{
	public:
		Material(void);
		virtual ~Material(void);

		void SetColor(const Color &c);
		Color GetColor(void) const;

		void SetSpecularColor(const Color &c);
		Color GetSpecularColor(void) const;

		void SetEmissiveColor(const Color &c);
		Color GetEmissiveColor(void) const;

		void SetDiffuse(const float diff);
		float GetDiffuse(void) const;

		void SetSpecular(const float spec);
		float GetSpecular(void) const;

		void SetEmissive(const float emis);
		float GetEmissive(void) const;

		void SetReflectivity(const float refl);
		float GetReflectivity(void) const;

		void SetIOR(const float index);
		float GetIOR(void) const;
	private:
		Color emissiveColor, diffuseColor, specularColor;
		float emissive, diffuse, specular, reflectivity, ior;
};

//the Geometry class
class Geometry
{
	public:
		enum
		{
			SPHERE = 1,
			PLANE,
			TRIANGLE,
			AABOX,
			CYLINDER,
			ELLIPSE,
			QUADRIC,
			METABALL,
			SDFUNC,
			BVHACCEL
		};
		Geometry(void);
		virtual ~Geometry(void);

		void SetMaterial(const Material &m);
		Material& GetMaterial(void);

		void SetName(std::string nm);
		std::string GetName(void) const;

		void SetLight(bool l);
		bool IsLight(void) const;

		void SetLightIntensity(float i);
		float GetLightIntensity(void);

		virtual int GetType(void) const = 0;
		virtual int Intersect(Ray &r, float &mindist) const = 0;
		virtual Vector GetNormal(Vector &pos) const = 0;
		virtual Vector GeneratePoint() const = 0;
	private:
		Material mat;
		std::string name;
		bool isLight;
		float lightIntensity;
};

//the Plane class
class Plane : public Geometry
{
	public:
		Plane(const Vector &norm, const float dist);
		virtual ~Plane();

		int GetType(void) const;

		Vector GetNormal(Vector &pos) const;
		Vector GetNormal(void) const;
		float GetD(void) const;
		void SetNormal(const Vector &n);
		void SetD(const float dist);

		int Intersect(Ray &r, float &mindist) const;
		Vector GeneratePoint() const;
	private:
		Vector normal;
		float d;
};

//the Sphere class
class Sphere : public Geometry
{
	public:
		Sphere(const Vector &pos, const float r);
		virtual ~Sphere(void);

		Vector GetPosition(void) const;
		void SetPosition(const Vector &pos);

		float GetRadius(void) const;
		float GetSqrRadius(void) const;
		float GetInvRadius(void) const;
		void SetRadius(const float r);

		Vector GetNormal(Vector &pos) const;
		int Intersect(Ray &r, float &mindist) const;
		int GetType(void) const;
		Vector GeneratePoint() const;
		int GetIntersectionPoints(Ray &r, float &d1, float &d2) const;
	private:
		Vector position;
		float radius, sqrRadius, invRadius;
};

//the Axis Aligned Box class
class AABox : public Geometry
{
	public:
		AABox(const Vector &min, const Vector &max);
		virtual ~AABox(void);

		Vector GetMin() const;
		Vector GetMax() const;

		Vector GetNormal(Vector &pos) const;
		int Intersect(Ray &r, float &mindist) const;
		int GetType() const;
		Vector GeneratePoint() const;
	private:
		Vector bounds[2];
};

//the Metalball class
class Metaball : public Geometry
{
	public:
		Metaball(Sphere **ballList, int ballCount);
		virtual ~Metaball(void);

		Sphere GetBounds(void) const;
		void SetThreshold(float val);
		float GetThreshold(void) const;

		Sphere *GetBall(int num);
		void SetBallList(Sphere **ballList, int ballCount);

		Vector GetNormal(Vector &pos) const;
		int Intersect(Ray &r, float &mindist) const;
		int GetType(void) const;
		Vector GeneratePoint() const;
	private:
		float PotentialFunction(Vector p1, Vector p2) const;

		Sphere **balls;
		int numBalls;
		float threshold;
		const float stepVal;
};

//try out some signed distance fields
class SDF : public Geometry
{
	public:
		SDF();
		virtual ~SDF();

		Vector GetNormal(Vector &pos) const;
		int Intersect(Ray &r, float &mindist) const;
		int GetType(void) const;
		Vector GeneratePoint() const;

		void SetThreshold(float val);
		float GetThreshold() const;
		void SetIterations(int i);
		int GetIterations() const;

		virtual float distance(Vector &p) const;

		Vector translate(const Vector &p, const Vector &t) const;
		Vector rotateX(const Vector &p, float theta) const;
		Vector rotateY(const Vector &p, float theta) const;
		Vector rotateZ(const Vector &p, float theta) const;
		Vector repeat(const Vector &p, const Vector &c) const;

		float sphere(const Vector &p, float r) const;
		float box(const Vector &p, const Vector &b) const;
		float cylinderxy(const Vector &p, float r) const; //xy cylinder
		float cylinderxz(const Vector &p, float r) const; //xz cylinder
		float cylinderyz(const Vector &p, float r) const; //yz cylinder
		float cylinder(const Vector &p, const Vector &c) const; //general cylinder from IQ
		float plane(const Vector &p, const Vector &n, float d) const;
		float torus(const Vector &p, float major, float minor) const;
		float torus82(const Vector &p, float major, float minor) const;
		float torus88(const Vector &p, float major, float minor) const;

		float opUnion(float d1, float d2) const;
		float opSubtract(float d1, float d2) const;
		float opIntersect(float d1, float d2) const;

	private:
		float threshold;
		int iterations;
};

//BVH for speed
class BVH : public Geometry
{
	public:
		BVH(int childrenPerLevel = 4);
		virtual ~BVH();

		Vector GetNormal(Vector &pos) const;
		int Intersect(Ray &r, float &mindist) const;
		int GetType(void) const;
		Vector GeneratePoint() const;

		int IntersectRecursive(Ray &r, float &mindist, Geometry **obj) const;

		void AddObject(Geometry *obj, Sphere *bounds);
		void CalculateTree();

	private:
		class TreeNode
		{
			public:
				void CalcBounds();
				bool HasChildren();

				vector<TreeNode *> children;
				vector<Geometry *> members;
				vector<Sphere *> memberBounds;
				Sphere *bounds;
		};

		void SplitNode(TreeNode *node);
		int Recurse(TreeNode *node, Ray &r, float &mindist, Geometry **obj) const;

		vector<Geometry *> objects;
		vector<Sphere *> objectBounds;
		int branchiness;
		int numObjects;
		TreeNode *root;
};

#endif
