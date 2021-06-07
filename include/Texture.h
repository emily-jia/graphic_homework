//
// Created by Emily Jia on 2021/6/8.
//

#ifndef RAYTRACING_TEXTURE_H
#define RAYTRACING_TEXTURE_H

#include "bitmap_image.hpp"
#include "Vector3f.h"
///@brief helper class that stores a texture and faciliates lookup
///assume 4byte RGBA image data
class Texture{
public:
    Texture();
    bool valid();
    void load(const char * filename);
    void operator()(int x, int y,  unsigned char * color);
    ///@param x assumed to be between 0 and 1
    Vector3f operator()(double x, double y);
    ~Texture();
private:
    bitmap_image * bimg;
    int width , height;
};

#endif //RAYTRACING_TEXTURE_H
