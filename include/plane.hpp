#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// TODO: Implement Plane representing an infinite plane
// function: ax+by+cz=d
// choose your representation , add more fields and fill in the functions

class Plane : public Object3D {
    Vector3f _n;  // 法向量
    float _d;     // 偏移
public:
    Plane() {
        _n = Vector3f(0, 0, 1);
        _d = 0;
    }

    Plane(const Vector3f &normal, float d, Material *m) : Object3D(m) {
        _n = normal;
        _d = -d;
    }

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        float dis_origin = _d + Vector3f::dot(_n, r.getOrigin());  // 计算光源到平面的距离
        float t = - dis_origin / Vector3f::dot(_n, r.getDirection());
        if(t < tmin || t < 0 || t > h.getT())
            return false;
        h.set(t, this->material, dis_origin >= 0 ? _n : -_n);   // 平面到光源的方向与法向一致，则为平面法向，否则需反向
        return true;
    }

protected:


};

#endif //PLANE_H
		

