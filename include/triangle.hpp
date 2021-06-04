#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>
using namespace std;

// TODO: implement this class and add more fields as necessary,
class Triangle: public Object3D {
public:
	Triangle() = delete;

    // a b c are three vertex positions of the triangle
	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m, Vector3f* norm= nullptr) : Object3D(m) {
	    vertices[0] = a;
	    vertices[1] = b;
	    vertices[2] = c;

	    if(!norm)
        {
            Vector3f vec1 = a - b;
            Vector3f vec2 = b - c;
            normal = Vector3f::cross(vec1, vec2);
            normal.normalize();
        }else{
	        normal = *norm;
	    }
	}

	bool intersect( const Ray& ray,  Hit& hit , float tmin) override {
	    Vector3f e1 = vertices[0] - vertices[1],
	             e2 = vertices[0] - vertices[2],
	             s  = vertices[0] - ray.getOrigin();
	    Vector3f solution = Vector3f(Matrix3f(s, e1, e2).determinant(),
	                                Matrix3f(ray.getDirection(), s, e2).determinant(),
	                                Matrix3f(ray.getDirection(), e1, s).determinant())
	                        / Matrix3f(ray.getDirection(), e1, e2).determinant();
	    float t = solution.x(), beta = solution.y(), gamma = solution.z();
	    if(!(t>=0 && beta>=0 && gamma >=0 && beta+gamma<=1) || t < tmin || t > hit.getT())
            return false;
	    hit.set(t, this->material, Vector3f::dot(normal, ray.getDirection())>=0? -normal:normal);
        return true;
	}
	Vector3f normal;
	Vector3f vertices[3];
protected:

};

#endif //TRIANGLE_H
