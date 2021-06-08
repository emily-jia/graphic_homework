#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "scene_parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"
#include "RayTracer.h"

#include <string>

using namespace std;
#define EPSILON 1e-3

int main(int argc, char *argv[]) {
    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

    if (argc != 3) {
        cout << "Usage: ./bin/PA1 <input scene file> <output bmp file>" << endl;
        return 1;
    }
    string inputFile = argv[1];
    string outputFile = argv[2];  // only bmp is allowed.

    SceneParser sceneParser(inputFile.c_str());
    Camera* camera = sceneParser.getCamera();
    Image output(camera->getWidth(), camera->getHeight());
    RayTracer rayTracer(&sceneParser, 5);

    // 循环屏幕空间的像素
    for (int x = 0; x < camera->getWidth(); ++x) {
        for (int y = 0; y < camera->getHeight();++y) {
            // 计算当前像素(x,y)处相机出射光线camRay
            Ray camRay = sceneParser.getCamera()->generateRay(Vector2f(x, y));

            Hit hit;
            // Ray Tracing
            Vector3f color = rayTracer.traceRay(camRay, EPSILON, 0, 1, hit);
            output.SetPixel(x, y, color);
        }
    }

    output.SaveBMP(outputFile.c_str());
    return 0;
}

