#ifndef CURVE_HPP
#define CURVE_HPP

#include "object3d.hpp"
#include <vecmath.h>
#include <vector>
#include <utility>

#include <algorithm>

// TODO (PA3): Implement Bernstein class to compute spline basis function.
//       You may refer to the python-script for implementation.

// The CurvePoint object stores information about a point on a curve
// after it has been tesselated: the vertex (V) and the tangent (T)
// It is the responsiblility of functions that create these objects to fill in all the data.
struct CurvePoint {
    Vector3f V; // Vertex
    Vector3f T; // Tangent  (unit)
};

class Curve : public Object3D {
protected:
    std::vector<Vector3f> controls;
public:
    explicit Curve(std::vector<Vector3f> points) : controls(std::move(points)) {}

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        return false;
    }

    std::vector<Vector3f> &getControls() {
        return controls;
    }

    virtual void discretize(int resolution, std::vector<CurvePoint>& data) = 0;
};

class BezierCurve : public Curve {
public:
    explicit BezierCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4 || points.size() % 3 != 1) {
            printf("Number of control points of BezierCurve must be 3n+1!\n");
            exit(0);
        }
    }

    void discretize(int resolution, std::vector<CurvePoint>& data) override {
        data.clear();
        // DONE (PA3): fill in data vector
        int n = controls.size();
        for(int i=0;i<=resolution;i++)
            data.emplace_back(get_point(i/float(resolution), n-1));
    }

protected:
    CurvePoint get_point(float t, int n)
    {
        std::vector<Vector3f> points = controls;
        for(int i=1;i<n;i++)
            for(int j=0;j<=n-i;j++)
                points[j] = (1-t) * points[j] + t * points[j+1];

        CurvePoint point;
        point.T  = n * (points[1] - points[0]);
        point.V = (1-t) * points[0] + t * points[1];
        return point;
    }
};

const int K = 3;
class BsplineCurve : public Curve {
   int knot_num;
public:
    BsplineCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4) {
            printf("Number of control points of BspineCurve must be more than 4!\n");
            exit(0);
        }
        knot_num = points.size() +  K;  // 约定k=3, knot_num = n+k+1
    }

    void discretize(int resolution, std::vector<CurvePoint>& data) override {
        data.clear();
        // TODO (PA3): fill in data vector
        int n = controls.size(); // n=n+1
        for(int region=K;region<n;region++)
            for(int i=0;i<=resolution;i++)
                data.emplace_back(get_point(region + i/float(resolution)));
    }

protected:
    CurvePoint get_point(float t_times_knot_num)  // n=n+1
    {
        int range = floor(t_times_knot_num);
        std::vector<Vector3f> ps;

        for(int i=range-K;i<=range;i++)
            ps.push_back(controls[i]);

        CurvePoint point;
        for(int r=1; r <= K; r++)
        {
            if(r == K)
                point.T = (ps[K] - ps[K-1]) * knot_num;

            for(int i=K; i >=r; i--)
                ps[i] = ((t_times_knot_num - (range-K+i)) * ps[i] + (i+range-r+1 - t_times_knot_num) * ps[i-1]) /
                        (K - r + 1);
        }

        point.V = ps[K];
        return point;
    }
};

#endif // CURVE_HPP
