#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>


// TODO: Implement Group - add data structure to store a list of Object*
class Group : public Object3D {
    std::vector<Object3D*> _arr;
public:

    Group() = default;

    explicit Group (int num_objects): _arr(num_objects) {}

    ~Group() override {}

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        bool result = false;
        for (Object3D* obj: _arr)
            result |= (obj? obj->intersect(r, h, tmin):false);
        return result;
    }

    void addObject(int index, Object3D *obj) {
        if(index > _arr.size())
            throw std::out_of_range("In Group::addObject: index out of range!");
        _arr.insert(_arr.begin()+index, obj);
    }

    int getGroupSize() {
        return _arr.size();
    }

private:

};

#endif
	
