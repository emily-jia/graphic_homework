//
// Created by Emily Jia on 2021/6/8.
//

#ifndef RAYTRACING_CUBEMAP_H
#define RAYTRACING_CUBEMAP_H

#include "Texture.h"
#include "Vector3f.h"
class CubeMap{
public:
///@brief assumes a directory containing
///left right up down front back.bmp
    CubeMap(const char * dir);
    enum FACE
    {
        LEFT,RIGHT,UP,DOWN,FRONT,BACK
    };
    Vector3f operator()(const Vector3f&);
    Texture t[6];

};

#endif //RAYTRACING_CUBEMAP_H
