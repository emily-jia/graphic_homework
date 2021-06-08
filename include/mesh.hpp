#ifndef MESH_H
#define MESH_H

#include <vector>
#include <map>
#include "object3d.hpp"
#include "triangle.hpp"
#include "Octree.h"
#include "Vector2f.h"
#include "Vector3f.h"

std::vector<std::string> split(const string& str, char c);

class Mesh : public Object3D {
public:
    Mesh(const char *filename, Material *m);

    struct TriangleIndex {
        TriangleIndex() {
            x[0] = 0; x[1] = 0; x[2] = 0;
        }
        int &operator[](const int i) { return x[i]; }
        // By Computer Graphics convention, counterclockwise winding is front face
        int x[3]{};   // 顶点id
        int norm[3];  // 平面法向id （三个顶点）
        int f[3];     // 贴图映射的顶点id
        Material* material;
    };

    std::vector<Vector3f> v;            //顶点集
    std::vector<TriangleIndex> t;       //三角面片集
    std::vector<Vector3f> n;            //顶点法线集
    std::vector<Vector2f> vt;           //贴图坐标集
    bool intersect(const Ray &r, Hit &h, float tmin) override;
    int size() const { return t.size(); }

private:

    // Normal can be used for light estimation
    void computeNormal();
    void readObj(std::ifstream& f);
    static std::map<std::string, Material *> readMtl(const string& filename);

    Octree octree;
};

#endif
