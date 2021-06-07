//
// Created by Emily Jia on 2021/6/8.
//

#include "Texture.h"
#include "bitmap_image.hpp"
#include <cassert>

void Texture::load(const char *filename)
{
    bimg = new bitmap_image(filename);
    height = bimg->height();
    width = bimg->width();
}

void Texture::operator()(int x, int y, unsigned char *color)
{
    x = clamp(x, 0, width - 1);
    y = clamp(y, 0, height - 1);
    bimg->get_pixel(x, y, color[0], color[1], color[2]);
}
bool Texture::valid()
{
    return bimg != 0;
}

inline static void check_range(double &x)
{
    while (x < 0) x += 1;
    while (x > 1) x -= 1;
}

///@param x assumed to be between 0 and 1
Vector3f Texture::operator()(double x, double y)
{
    Vector3f color;
    int ix, iy;
    check_range(x);
    check_range(y);
    x = x * width;
    y = (1 - y) * height;
    ix = (int)x;
    iy = (int)y;
    unsigned char pixels[4][3];
    double alpha = x - ix;
    double beta = y - iy;
    operator()(ix, iy, pixels[0]);
    operator()(ix + 1, iy, pixels[1]);
    operator()(ix, iy + 1, pixels[2]);
    operator()(ix + 1, iy + 1, pixels[3]);
    for (int ii = 0; ii < 3; ii++)
    {
        color[ii] = (1 - alpha) * (1 - beta) * pixels[0][ii] + alpha * (1 - beta) * pixels[1][ii] + (1 - alpha) * beta * pixels[2][ii] + alpha * beta * pixels[3][ii];
    }
    if (std::isnan(color.length()))
    {
        puts("nan texture");
    }
    // printf("texture @ %.2lf %.2lf\n", x / width, y / height);
    // color.print();
    // fflush(stdout);
    assert(!std::isnan(color.length()));
    return color / 255;
}

Texture::~Texture()
{
    if (bimg != 0)
    {
        delete bimg;
    }
}

Texture::Texture() : bimg(0), width(0), height(0)
{
}
