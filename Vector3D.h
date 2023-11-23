#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>

class Vector3D {
public:
    float x, y, z;

    Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}

    // Vector addition
    Vector3D operator+(const Vector3D& other) const {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }

    // Vector subtraction
    Vector3D operator-(const Vector3D& other) const {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }

    // Dot product
    float dot(const Vector3D& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Cross product
    Vector3D cross(const Vector3D& other) const {
        return Vector3D(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    // Magnitude (length) of the Vector3D
    float magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }
};
#endif // VECTOR3D_H