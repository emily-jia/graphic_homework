#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

typedef Vector3f Pos;

// TODO: Implement functions and add more fields as necessary

class Sphere : public Object3D {
    Pos _center;
    float _r;
public:
    Sphere() {
        // unit ball at the center
        _center = Pos(0, 0, 0);
        _r = 1;
    }

    Sphere(const Vector3f &center, float radius, Material *material) : Object3D(material) {
        //
        _center = center;
        _r = radius;
    }

    ~Sphere() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        Vector3f source_to_center_vec = _center - r.getOrigin();   // 光源到球心的向量
        float l_sq = source_to_center_vec.squaredLength(), r_sq = _r * _r;

        // 计算光源到球心的向量在r方向上的投影长度
        float projected_len = Vector3f::dot(source_to_center_vec, r.getDirection()) / r.getDirection().length();
        float distance_sq = l_sq - projected_len * projected_len;   // 计算球心到光源方向的投影距离的平方

        // 光源在球外且球的位置与r的传播方向相背 || 到圆心的距离大于球的半径，则无交点
        if ((l_sq > r_sq && projected_len < 0 )|| distance_sq > r_sq)
            return false;

        float intersect_point_to_foot = sqrt(r_sq - distance_sq);    // 计算与球的交点到垂足的距离
        float t = l_sq==r_sq ? 0 :  // 计算光源到交点的距离，根据光源位置进行分类，在球面上则距离为0
                (l_sq>r_sq? projected_len-intersect_point_to_foot: projected_len+intersect_point_to_foot);

        if(t < tmin || t > h.getT())  // 本次相交并非离相机最近的，应当舍弃
            return false;

        Vector3f instersect_point = r.getOrigin() + t * r.getDirection();
        Vector3f normal = l_sq >= r_sq ? instersect_point-_center : _center-instersect_point; // 计算法向量
        normal.normalize();
        h.set(t, this->material, normal);
        return true;
    }

protected:

};


#endif
