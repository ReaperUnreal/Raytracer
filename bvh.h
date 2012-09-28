#ifndef BVH_H
#define BVH_H
#include "main.h"

//BVH for speed
class BVH : public Geometry
{
	public:
		BVH(int childrenPerLevel = 2);
		virtual ~BVH();

		Vector GetNormal(Vector &pos) const;
		int Intersect(Ray &r, float &mindist) const;
		int GetType(void) const;
		Vector GeneratePoint() const;
      void Preprocess();

      void AddObject(Geometry *obj);

		int IntersectRecursive(Ray &r, float &mindist, Geometry **obj) const;
	private:
		class TreeNode
		{
			public:
				vector<TreeNode *> children;
            Geometry *object;
            Geometry *bounds;
		};

      //intersection routines
		int Recurse(TreeNode *node, Ray &r, float &mindist, Geometry **obj) const;

      //consturction routines
      TreeNode* RecursiveSplit(vector<Geometry *> list, int start, int end);
      AABox* ComputeBoundingBox(vector<Geometry *> list, int start, int end) const;
      float Cost(AABox *b) const;

		int branchiness;
		TreeNode *root;
      vector<Geometry *> objectList;
};

#endif
