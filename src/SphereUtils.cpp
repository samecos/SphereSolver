#include "SphereUtils.h"
#include <cmath>
#include <random>
#include <numeric>
#include <iostream>

// Helper to calculate distance between two points
double distance(const Point3D &p1, const Point3D &p2)
{
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2) + std::pow(p1.z - p2.z, 2));
}

Point3D SphereUtils::calculateSphereCenter(double radius, const std::vector<Point3D> &points, double inputVariance, double *outVariance)
{
    if (points.empty())
        return {0, 0, 0};

    // Initial guess: Centroid
    Point3D currentCenter = {0, 0, 0};
    for (const auto &p : points)
    {
        currentCenter.x += p.x;
        currentCenter.y += p.y;
        currentCenter.z += p.z;
    }
    currentCenter.x /= points.size();
    currentCenter.y /= points.size();
    currentCenter.z /= points.size();

    // Determine initial noise variance (mean squared residual on distances)
    double noiseVar = 0.0;
    if (inputVariance > 0.0)
    {
        noiseVar = inputVariance;
    }
    else
    {
        for (const auto &p : points)
        {
            double d = distance(currentCenter, p);
            double res = d - radius;
            noiseVar += res * res;
        }
        noiseVar /= points.size();
    }
    if (noiseVar < 1e-12)
        noiseVar = 1e-12; // avoid division by zero later

    // Gradient Descent parameters
    double learningRate = 0.1; // Reasonable starting learning rate
    int iterations = 20000;
    double tolerance = 1e-7;

    for (int iter = 0; iter < iterations; ++iter)
    {
        Point3D gradient = {0, 0, 0};
        double totalError = 0;

        for (const auto &p : points)
        {
            double dist = distance(currentCenter, p);
            if (dist < 1e-9)
                dist = 1e-9; // Avoid division by zero

            // Objective: Minimize Sum (dist - R)^2
            // Derivative w.r.t C: 2 * (dist - R) * d(dist)/dC
            // d(dist)/dC = (C - P) / dist
            // Gradient += 2 * (dist - R) * (C - P) / dist
            //            = 2 * (1 - R/dist) * (C - P)

            double factor = 2 * (1.0 - radius / dist);

            // Residual and per-point weight using estimated noise variance.
            double residual = dist - radius;
            double weight = 1.0 / (1.0 + (residual * residual) / noiseVar);

            gradient.x += weight * factor * (currentCenter.x - p.x);
            gradient.y += weight * factor * (currentCenter.y - p.y);
            gradient.z += weight * factor * (currentCenter.z - p.z);

            totalError += std::pow(dist - radius, 2);
        }

        // Adaptive learning rate: reduce step when estimated noise is large
        double adjustedLR = learningRate / (1.0 + 5.0 * noiseVar);

        // Update center
        // We divide gradient by points.size() to average it, preventing explosion with many points
        currentCenter.x -= adjustedLR * gradient.x / points.size();
        currentCenter.y -= adjustedLR * gradient.y / points.size();
        currentCenter.z -= adjustedLR * gradient.z / points.size();

        if (totalError / points.size() < tolerance || iter == iterations - 1)
        {
            // Output convergence info
            std::cout << "Converged after " << iter + 1 << " iterations with error " << totalError / points.size() << "\n";
            break;
        }
    }

    // Compute final mean squared residual (variance) using the optimized center
    double finalVar = 0.0;
    for (const auto &p : points)
    {
        double d = distance(currentCenter, p);
        double res = d - radius;
        finalVar += res * res;
    }
    finalVar /= points.size();
    if (outVariance)
        *outVariance = finalVar;

    return currentCenter;
}

std::vector<Point3D> SphereUtils::generateSpherePoints(Point3D center, double radius, int count, double variance)
{
    std::vector<Point3D> points;
    // Standard deviation is sqrt(variance)
    double stdDev = (variance > 0) ? std::sqrt(variance) : 0;

    std::mt19937 gen(std::random_device{}());
    std::normal_distribution<double> noiseDist(0.0, stdDev);
    std::uniform_real_distribution<double> thetaDist(0.0, 2 * 3.14159265359);
    std::uniform_real_distribution<double> uDist(-1.0, 1.0); // For cos(phi)

    for (int i = 0; i < count; ++i)
    {
        double theta = thetaDist(gen);
        double cosPhi = uDist(gen);
        double sinPhi = std::sqrt(1.0 - cosPhi * cosPhi);

        // Point on unit sphere
        double ux = sinPhi * std::cos(theta);
        double uy = sinPhi * std::sin(theta);
        double uz = cosPhi;

        // Scale by radius and add to center
        double x = center.x + radius * ux;
        double y = center.y + radius * uy;
        double z = center.z + radius * uz;

        // Add noise
        x += noiseDist(gen);
        y += noiseDist(gen);
        z += noiseDist(gen);

        points.push_back({x, y, z});
    }
    return points;
}

std::vector<Point3D> SphereUtils::generateSpherePoints(Point3D center, double radius, int count, double variance, double minTheta, double maxTheta, double minPhi, double maxPhi)
{
    std::vector<Point3D> points;
    // Standard deviation is sqrt(variance)
    double stdDev = (variance > 0) ? std::sqrt(variance) : 0;

    std::mt19937 gen(std::random_device{}());
    std::normal_distribution<double> noiseDist(0.0, stdDev);
    std::uniform_real_distribution<double> thetaDist(minTheta, maxTheta);
    std::uniform_real_distribution<double> phiDist(minPhi, maxPhi);

    for (int i = 0; i < count; ++i)
    {
        double theta = thetaDist(gen);
        double phi = phiDist(gen);

        double sinPhi = std::sin(phi);
        double cosPhi = std::cos(phi);

        // Point on unit sphere
        double ux = sinPhi * std::cos(theta);
        double uy = sinPhi * std::sin(theta);
        double uz = cosPhi;

        // Scale by radius and add to center
        double x = center.x + radius * ux;
        double y = center.y + radius * uy;
        double z = center.z + radius * uz;

        // Add noise
        x += noiseDist(gen);
        y += noiseDist(gen);
        z += noiseDist(gen);

        points.push_back({x, y, z});
    }
    return points;
}
