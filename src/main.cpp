#include <iostream>
#include <vector>
#include <iomanip>
#include "SphereUtils.h"

void clearInput()
{
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}

int main()
{
    while (true)
    {
        std::cout << "\n--- Sphere Solver ---\n";
        std::cout << "1. Calculate Sphere Center (from Radius + Points)\n";
        std::cout << "2. Generate Noisy Sphere Points\n";
        std::cout << "3. Exit\n";
        std::cout << "Select option: ";

        int choice;
        if (!(std::cin >> choice))
        {
            if (std::cin.eof())
                break;
            clearInput();
            continue;
        }

        if (choice == 3)
            break;

        if (choice == 1)
        {
            double r;
            int n;
            std::cout << "Enter known Radius: ";
            std::cin >> r;
            std::cout << "Enter number of points: ";
            std::cin >> n;

            std::vector<Point3D> points;
            std::cout << "Enter points (x y z):\n";
            for (int i = 0; i < n; ++i)
            {
                Point3D p;
                std::cout << "Point " << i + 1 << ": ";
                std::cin >> p.x >> p.y >> p.z;
                points.push_back(p);
            }

            double inputVar = -1.0;
            std::cout << "Enter known noise variance (enter negative to auto-estimate): ";
            std::cin >> inputVar;

            double variance = 0.0;
            Point3D center = SphereUtils::calculateSphereCenter(r, points, inputVar, &variance);
            std::cout << std::fixed << std::setprecision(4);
            std::cout << "Calculated Center: (" << center.x << ", " << center.y << ", " << center.z << ")\n";
            std::cout << "Estimated mean squared residual (variance): " << variance << "\n";
        }
        else if (choice == 2)
        {
            Point3D c;
            double r, v;
            int n;
            std::cout << "Enter Center (x y z): ";
            std::cin >> c.x >> c.y >> c.z;
            std::cout << "Enter Radius: ";
            std::cin >> r;
            std::cout << "Enter Variance (Noise Power): ";
            std::cin >> v;
            std::cout << "Enter number of points to generate: ";
            std::cin >> n;

            std::vector<Point3D> points = SphereUtils::generateSpherePoints(c, r, n, v);
            std::cout << "\nGenerated Points (x y z):\n";
            std::cout << std::fixed << std::setprecision(4);
            for (const auto &p : points)
            {
                std::cout << p.x << " " << p.y << " " << p.z << "\n";
            }
        }
    }
    return 0;
}
