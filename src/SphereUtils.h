#ifndef SPHERE_UTILS_H
#define SPHERE_UTILS_H

#include <vector>

struct Point3D
{
    double x, y, z;
};

class SphereUtils
{
public:
    // Calculate center given radius and surface points using Gradient Descent
    // outVariance: optional output parameter to receive the mean squared residuals
    // (i.e. variance of (distance - radius)) for the provided surface points.
    // If provided, it will be set to the computed variance after optimization.
    // inputVariance: optional input hint for the variance (mean squared noise of distances).
    //   If > 0, the optimizer will use this value to weight points during optimization.
    //   If <= 0, the function will estimate variance from the provided points.
    // outVariance: optional output parameter to receive the mean squared residuals
    // (i.e. variance of (distance - radius)) for the provided surface points after optimization.
    // If provided, it will be set to the computed variance after optimization.
    static Point3D calculateSphereCenter(double radius, const std::vector<Point3D> &points, double inputVariance = -1.0, double *outVariance = nullptr);

    // Generate points on sphere surface with noise
    static std::vector<Point3D> generateSpherePoints(Point3D center, double radius, int count, double variance);
};

#endif // SPHERE_UTILS_H
