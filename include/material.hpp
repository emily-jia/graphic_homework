#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "ray.hpp"
#include "hit.hpp"
#include "Texture.h"
#include <iostream>

// TODO: add Texture.
class Material {
public:

    explicit Material(const Vector3f &d_color, const Vector3f &s_color = Vector3f::ZERO,
            double s = 0, double tr=0, double refr_n=1, const Vector3f& tf = Vector3f::ZERO) :
            diffuseColor(d_color), specularColor(s_color), shininess(s), transparency(tr), n_out(refr_n),
            transmitColor(tf){

    }

    virtual ~Material() = default;

    virtual Vector3f getDiffuseColor() const {
        return diffuseColor;
    }

    Vector3f getTransmitColor() const {
        return transmitColor;
    }

    Vector3f getSpecularColor() const {
        return specularColor;
    }

    double getRefractRatio() const {
        return n_out;
    }

    double getTransparency() const {  // 透明度
        return transparency;
    }

    Vector3f Shade(const Ray &ray, const Hit &hit,
                   const Vector3f &dirToLight, const Vector3f &lightColor) {
        float dot_ln = Vector3f::dot(dirToLight, hit.getNormal());
        Vector3f reflect_dir = 2 * dot_ln * hit.getNormal() - dirToLight;
        float specular = Vector3f::dot(-ray.getDirection(), reflect_dir),
              diffuse = dot_ln > 0? dot_ln : 0;
        specular = specular > 0? specular: 0;
        specular = pow(specular, shininess);

        Vector3f kd;
        if(t.valid() && hit.hasTex){
            Vector2f texCoord = hit.texCoord;
            Vector3f texColor = t(texCoord[0],texCoord[1]);
            kd = texColor;
        }else{
            kd = this->diffuseColor;
        }

        Vector3f shaded = lightColor * (diffuse*diffuseColor + specular*specularColor);
        return shaded;
    }

protected:
    Vector3f diffuseColor;
    Vector3f specularColor;
    double shininess;

    double n_out;  // 折射率
    Vector3f transmitColor;
    double transparency;

    Texture t;
};


#endif // MATERIAL_H
