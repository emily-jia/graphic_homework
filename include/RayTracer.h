//
// Created by Emily Jia on 2021/6/2.
//

#ifndef RAYTRACING_RAYTRACER_H
#define RAYTRACING_RAYTRACER_H

#include <cassert>
#include <vector>
#include "ray.hpp"
#include "hit.hpp"
#include "scene_parser.hpp"


class RayTracer
{
public:
    RayTracer() = delete;
    RayTracer(SceneParser* scene, int max_depth):
        scene(scene), group(scene->getGroup()), max_depth(max_depth) {}

    static Vector3f get_reflect_direction(const Vector3f& normal, const Vector3f& incoming);

    // index_n, index_nt: 入射介质，出射介质的折射率
    static bool get_refract_direction(const Vector3f& normal, const Vector3f& incoming, double index_n, double index_nt,
            Vector3f& transmitted);

    Vector3f traceRay(Ray &ray, double tmin, int bounces,
                      double refr_index, Hit &hit);

protected:
    SceneParser* scene;
    Group* group;
    int max_depth;
};

#endif //RAYTRACING_RAYTRACER_H
