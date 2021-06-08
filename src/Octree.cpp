//
// Created by Emily Jia on 2021/5/24.
//

#include "Octree.h"
#include "mesh.hpp"

#define INFINITY 1e15
void Octree::build(const Mesh &mesh) {
    _root = new OctTreeNode;

    //计算整个模型的bounding box
    _box.maxium = {-INFINITY};
    _box.minium = {INFINITY};
    for(const Vector3f& p: mesh.v)
    {
        for(int i=0;i<3;i++)
        {
            _box.minium[i] = _box.minium[i]>p[i]? p[i]: _box.minium[i];
            _box.maxium[i] = _box.maxium[i]<p[i]? p[i]: _box.maxium[i];
        }
    }

    std::vector<int> ids;
    int size = mesh.size();
    ids.reserve(size);
    for(int i=0;i<size;i++)
        ids.push_back(i);

    build_node(ids, _root, _box, mesh, 0);
}

OctTreeNode * Octree::build_node(std::vector<int> &ids, OctTreeNode *cur,
                                 const BoundingBox &box, const Mesh &mesh, int level) {
    cur->ids = ids;

    if(level < LEVEL_MAX && ids.size() > MESH_MAX)
    {
        BoundingBox bs[8];
        float ix = box.minium[0], iy = box.minium[1], iz = box.minium[2], //box xyz的最小值
            ax = box.maxium[0], ay = box.maxium[1], az = box.maxium[2],  //box xyz的最大值
            cx = (ax+ix)/2, cy = (ay+iy)/2, cz = (az+iz)/2;   //box中心点xyz值

        // 计算子节点对应的空间区域
        bs[0] = BoundingBox({ix, iy, cz}, {cx, cy, az});
        bs[1] = BoundingBox({cx, iy, cz}, {ax, cy, az});
        bs[2] = BoundingBox({cx, cy, cz}, {ax, ay, az});
        bs[3] = BoundingBox({ix, cy, cz}, {cx, ay, az});
        bs[4] = BoundingBox({ix, iy, iz}, {cx, cy, cz});
        bs[5] = BoundingBox({cx, iy, iz}, {ax, cy, cz});
        bs[6] = BoundingBox({cx, cy, iz}, {ax, ay, cz});
        bs[7] = BoundingBox({ix, cy, iz}, {cx, ay, cz});

        std::vector<int> child_ids[8];
        for(int& id: ids)
        {
            Mesh::TriangleIndex trig = mesh.t[id];
            Vector3f p = mesh.v[trig[0]];
            for(int i=0;i<8;i++)
            {
                if(point_in_box(bs[i], p)){
                    child_ids[i].push_back(id); break;
                }
            }
        }

        for(int i=0, j=0;i<8;i++)
            if(!child_ids[i].empty())
                cur->child[j++]=build_node(ids, new OctTreeNode, bs[i], mesh, level+1);
    }
    return cur;
}

inline bool operator <= (const Vector3f& v1, const Vector3f& v2) {
    return v1.x()<=v2.x() && v1.y()<=v2.y() && v1.z()<=v2.z();
}

bool Octree::point_in_box(const BoundingBox &box, const Vector3f &point) {
    return box.minium<=point && point<=box.maxium;
}

void Octree::intersect(const Ray &ray) {

}