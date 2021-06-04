//
// Created by Emily Jia on 2021/6/2.
//

#include "RayTracer.h"
#include "group.hpp"
#include "light.hpp"
#define EPSILON 1e-3

inline double sq(double x) {
    return x * x;
}

// 计算反射方向
Vector3f RayTracer::get_reflect_direction(const Vector3f &normal, const Vector3f &incoming)
{
    return (incoming - 2 * Vector3f::dot(normal, incoming) * normal).normalized();
}

// 计算折射方向
/*
 * index_n, index_nt：折射率，根据折射定律 sin(入射) / sin(出射) = index_n / index_nt
 */
bool RayTracer::get_refract_direction(const Vector3f &normal, const Vector3f &incoming, double index_n, double index_nt, Vector3f &transmitted)
{
    if (index_nt < EPSILON)
        auto in = incoming.normalized(), norm = normal.normalized();
    double dot = Vector3f::dot(normal, incoming),
        n_relative = index_n / index_nt,
        cos_sq_angle_o = 1 - sq(n_relative) * (1 - sq(dot));
    if (cos_sq_angle_o < EPSILON)
        return false;
    Vector3f norm = normal.normalized(), in = incoming.normalized();
    transmitted = norm * sqrt(cos_sq_angle_o) + (in - dot * norm) * n_relative;
    return true;
}

// TODO: 透明度和weight?
Vector3f RayTracer::traceRay(Ray &ray, double tmin, int bounces, double refr_index, Hit &hit)
{
    Vector3f color = Vector3f::ZERO;
    hit = Hit(1e5, nullptr, Vector3f::ZERO);

    //计算光线与所有物体的交点中离start最近的点;
    bool interesect = group->intersect(ray, hit, tmin);
    if(!interesect)
        color = scene->getBackgroundColor();
    else {
        Material *material = hit.getMaterial();
        const Vector3f &incoming = ray.getDirection(),
                &normal = hit.getNormal();
        Vector3f point = ray.pointAtParameter(hit.getT());  // 交点坐标

        // 计算局部光强
        Vector3f local;
        for (int i = 0; i < scene->getNumLights(); i++) {
            Light *light = scene->getLight(i);
            Vector3f L, lightColor;
            light->getIllumination(point, L, lightColor);
            local += material->Shade(ray, hit, L, lightColor);
        }

        if(bounces == max_depth)
            return local;

        // 处理反射光
        Hit h_ = Hit(1e5, nullptr, Vector3f::ZERO);
        Vector3f &&reflect_dir = get_reflect_direction(normal, incoming);
        Ray reflect_light(point, reflect_dir);
        Vector3f reflect_color = traceRay(reflect_light, tmin, bounces + 1, refr_index, h_);

        // 处理折射光
        h_ = Hit(1e5, nullptr, Vector3f::ZERO);
        double n_out = material->getRefractRatio();
        Vector3f refract_dir, refract_color = Vector3f::ZERO;
        if (get_refract_direction(normal, incoming, refr_index, n_out, refract_dir)) {
            Ray refract_light(point, refract_dir);
            refract_color = traceRay(refract_light, tmin, bounces + 1, n_out, h_);
        }

        double r;
        auto r0 = sq((n_out - refr_index) / (n_out + refr_index));
        double c = refr_index <= n_out ? Vector3f::dot(incoming, normal) : Vector3f::dot(refract_dir, normal);
        r = r0 + (1 - r0) * pow(1 - fabs(c), 5);

        const Vector3f &transmit = material->getTransmitColor(),
                &spec = material->getSpecularColor();

        color = local + r * spec * reflect_color + (1 - r) * transmit * refract_color;
    }

    return color;
}