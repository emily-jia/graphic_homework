#include "mesh.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <utility>
#include <sstream>

enum MtlToken {
    NEWMTL, NS, NI, KA, KD, KS, MAP_KD, COMMENT1, D, TR, TF, KE
};

enum ObjToken {
    V, F, VT, VN, USEMTL, MTLLIB, COMMENT2
};

map<string, int> MtlTokenDict = {
        {"newmtl", NEWMTL},
        {"Ns", NS},
        {"Ni", NI},
        {"Ka", KA},
        {"Kd", KD},
        {"Ks", KS},
        {"map_Kd",  MAP_KD},
        {"d", D},
        {"Tr", TR},
        {"TF", TF},
        {"KE", KE},
        {"#", COMMENT1}
};

map<string, int> ObjTokenDict = {
        {"v", V},
        {"f", F},
        {"vt", VT},
        {"vn", VN},
        {"usemtl", USEMTL},
        {"mtllib", MTLLIB},
        {"#", COMMENT2}
};

const string mlt_dir = "mesh/";

bool Mesh::intersect(const Ray &r, Hit &h, float tmin) {

    // Optional: Change this brute force method into a faster one.
    bool result = false;
    for (int triId = 0; triId < (int) t.size(); ++triId) {
        TriangleIndex& triIndex = t[triId];
        Triangle triangle(v[triIndex[0]],
                          v[triIndex[1]], v[triIndex[2]], triIndex.material? triIndex.material:material, &n[triId]);
        result |= triangle.intersect(r, h, tmin);
    }
    return result;
}

Mesh::Mesh(const char *filename, Material *material) : Object3D(material) {

    // Optional: Use tiny obj loader to replace this simple one.
    std::ifstream f;
    f.open(filename);
    if (!f.is_open()) {
        std::cerr << "Cannot open OBJ file: " << filename << "\n";
        assert(f.is_open());
    }

    readObj(f);
    f.close();
}

void Mesh::computeNormal() {
    n.resize(t.size());
    for (int triId = 0; triId < (int) t.size(); ++triId) {
        TriangleIndex& triIndex = t[triId];
        Vector3f a = v[triIndex[1]] - v[triIndex[0]];
        Vector3f b = v[triIndex[2]] - v[triIndex[0]];
        b = Vector3f::cross(a, b);
        n[triId] = b / b.length();
    }
}

void Mesh::readObj(std::ifstream &f) {
    std::string line;
    std::string vTok("v");
    std::string fTok("f");
    std::string texTok("vt");
    std::string normTok("vn");
    std::string mtlTok("usemtl");
    std::string loadMtlTok("mtllib");

    std::string tok;

    Material* cur_mtl = nullptr;
    std::map<std::string, Material *> mtl_map;

    while (true) {
        std::getline(f, line);
        if (f.eof()) {
            break;
        }
        if (line.size() < 3) {
            continue;
        }
        if (line.at(0) == '#') {   // 忽略注释
            continue;
        }

        std::stringstream ss(line);
        ss >> tok;              // 读入开头第一个word到 tok中
        if(tok == loadMtlTok) {
            string filename;
            ss >> filename;
            mtl_map = readMtl(filename);
        }
        if (tok == vTok) {
            Vector3f vec;
            ss >> vec[0] >> vec[1] >> vec[2];
            v.push_back(vec);  // 顶点坐标
        } else if (tok == fTok) { // face
            TriangleIndex trig;
            trig.material = cur_mtl;
            std::string info;
            for (int ii = 0; ii < 3; ii++)
            {
                ss >> info;
                std::vector<string> fields = split(info, '/');
                trig[ii] = stoi(fields[0]) - 1;

                if (fields.size() >= 2)
                    trig.f[ii] = stoi(fields[1]) - 1;

                if (fields.size() == 3)
                    trig.norm[ii] = stoi(fields[2]) - 1;
            }
            t.push_back(trig);
        } else if (tok == texTok) { // 贴图坐标点
            Vector2f texcoord;
            ss >> texcoord[0];
            ss >> texcoord[1];
            vt.push_back(texcoord);
        } else if (tok == normTok) { // 顶点法线
            Vector3f norm;
            ss >> norm[0] >> norm[1] >> norm[2];
            n.push_back(norm);
        } else if (tok == mtlTok){  // 更改material状态
            std::string mtl_name;
            ss >> mtl_name;
            cur_mtl = mtl_map[mtl_name];
        }
    }
    computeNormal();
}

std::map<std::string, Material *> Mesh::readMtl(const string &filename) {
    std::ifstream f;
    f.open(mlt_dir + filename);
    if (!f.is_open()) {
        std::cerr << "Cannot open MTL file: " << filename << "\n";
        assert(f.is_open());
    }
    std::map<std::string, Material*> materials;

    double ns, ni, d, tr;
    Vector3f ka, kd, ks, ke, tf;
    string mlt_name, texture_name;

    while(true)
    {
        string line, tok;
        getline(f, line);
        stringstream ss(line);
        ss >> tok;
        if(f.eof())
            break;

        switch (MtlTokenDict[tok]) {
            case NEWMTL: {
                auto* m = new Material(kd, ks, ns, tr, ni, tf);
                if(!texture_name.empty())
                {
                    //TODO: load texture from file
                }
                materials[mlt_name] = m;
                ss >> mlt_name;
                texture_name.clear();
                break;
            }
            case NS:{
                ss >> ns; break;
            }
            case NI: {
                ss >> ni; break;
            }
            case KA: {
                ss >> ka[0] >> ka[1] >> ka[2]; break;
            }
            case KD: {
                ss >> kd[0] >> kd[1] >> kd[2]; break;
            }
            case KS: {
                ss >> ks[0] >> ks[1] >> ks[2]; break;
            }
            case MAP_KD: {
                ss >> texture_name; break;
            }
            case KE: {
                ss >> ke[0] >> ke[1] >> ke[2]; break;
            }
            case TF: {
                ss >> tf[0] >> tf[1] >> tf[2]; break;
            }
            case TR: {
                ss >> tr; break;
            }
            case D: {
                ss >> d; break;
            }
            default:
                break;
        }
    }

    return materials;
}

std::vector<std::string> split(const string& str, char c)
{
    std::vector<std::string> ret;
    int len = str.length();
    for(int p=0;p<len;)
    {
        for(;p<len && str[p]==c;p++) {}
        int st = p;
        for(;p<len && str[p]!=c; p++) {}
        ret.push_back(str.substr(st, p-st));
    }
    return ret;
}