#include "main.h"

//the Bounding Volume Heirarchy class
BVH::BVH(int childrenPerLevel) : branchiness(2), root(NULL) //force binary tree for now
{
}

BVH::~BVH()
{
	//tear down tree most likely
}

int BVH::GetType() const
{
	return BVHACCEL;
}

Vector BVH::GetNormal(Vector &pos) const
{
	// should never use this
	return Vector();
}

void BVH::AddObject(Geometry *obj)
{
   objectList.push_back(obj);
}

int BVH::Intersect(Ray &r, float &mindist) const
{
	// similar to IntersectRecursive but I only care IF we hit, not WHAT we hit
	Geometry *geom = NULL;
	return IntersectRecursive(r, mindist, &geom);
}

Vector BVH::GeneratePoint() const
{
	// should never use this
	return MISS;
}

int BVH::Recurse(TreeNode *node, Ray &r, float &mindist, Geometry **obj) const
{
   //base case
   float newdist = mindist;
   if(node->bounds->Intersect(r, newdist) == MISS)
      return MISS;

   //recursive case
   int numChildren = node->children.size();
   for(int i = 0; i < numChildren; i++)
   {
      newdist = mindist;
      int res = Recurse(node->children[i], r, newdist, obj);
      if(res != MISS)
         return res;
   }

   //if we didn't hit any of the children, that means we're a leaf node, or we're done
   //so check the object at this node
   if(node->object)
   {
      int res = node->object->Intersect(r, mindist);
      if(res != MISS)
      {
         //hit some actual geometry, so record it
         *obj = node->object;
      }
      return res;
   }

   //done, nothing hit
   return MISS;
}

int BVH::IntersectRecursive(Ray &r, float &mindist, Geometry **obj) const
{
	//the main BVH method
	//traverse the heirarchy and find the right object
   int rc = Recurse(root, r, mindist, obj);

   return rc;
}

void BVH::Preprocess()
{
   //start the recursion
   root = RecursiveSplit(objectList, 0, objectList.size());
}

BVH::TreeNode* BVH::RecursiveSplit(vector<Geometry *> list, int start, int end)
{
   return NULL;
}

AABox* BVH::ComputeBoundingBox(vector<Geometry *> list, int start, int end) const
{
   Geometry *initialBounds = list[start]->GetBounds();
   AABox *box = new AABox(*static_cast<AABox *>(initialBounds));
   for(int i = start + 1; i < end; ++i)
   {
      Geometry *bounds = list[i]->GetBounds();
      box->Union(*static_cast<AABox *>(bounds));
   }
   return box;
}

float BVH::Cost(AABox *b) const
{
   //SAH heuristic
   Vector size = b->GetMax() - b->GetMin();

   //2wh + 2wd + 2dh = 2(w(h + d) + dh)
   //only needs to be proportional, so drop 2

   float cost = (size.xv() * (size.yv() + size.zv())) + (size.yv() + size.zv());
   return cost;
}

