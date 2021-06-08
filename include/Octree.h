//
// Created by Emily Jia on 2021/5/24.
//

#ifndef RAYTRACING_OCTREE_H
#define RAYTRACING_OCTREE_H
#include <vecmath.h>
#include <vector>

class Mesh;
class Ray;
class Hit;

struct BoundingBox
{
    Vector3f minium, maxium;
    BoundingBox() = default;
    BoundingBox(const Vector3f& min, const Vector3f& max): minium(min), maxium(max) {}
};

struct IntersectInfo
{
    Mesh* m;
    const Ray* ray;
    Hit& hit;
    double tm;
    bool result;
};

struct OctTreeNode
{
    OctTreeNode* child[8] {nullptr};
    std::vector<int> ids;
    bool isLeaf() {
        return !child[0];
    }
};

const int LEVEL_MAX = 8;
const int MESH_MAX = 7;

class Octree {
    BoundingBox _box;
    OctTreeNode* _root;

public:
    void build(const Mesh& mesh);
    void intersect(const Ray& ray);

private:
    OctTreeNode * build_node(std::vector<int>& ids, OctTreeNode* parent, const BoundingBox& box, const Mesh & mesh, int level);
    static bool point_in_box(const BoundingBox& box, const Vector3f& point);
};


#endif //RAYTRACING_OCTREE_H
