#pragma once

namespace Core {
    namespace Math {
        struct SVector {
            float x = 0.0f;
            float y = 0.0f;
            float z = 0.0f;

            SVector() = default;

            explicit SVector(float i)
                : x(i), y(i), z(i)
            {
            }

            SVector(float inX, float inY, float inZ)
                : x(inX), y(inY), z(inZ)
            {
            }

            SVector operator*(float val) const {
                return SVector(x * val, y * val, z * val);
            }

            SVector operator*(const SVector& val) const {
                return SVector(x * val.x, y * val.y, z * val.z);
            }

            SVector operator+(float val) const {
                return SVector(x + val, y + val, z + val);
            }

            SVector operator+(const SVector& val) const {
                return SVector(x + val.x, y + val.y, z + val.z);
            }

            SVector& operator*=(float val) {
                x *= val;
                y *= val;
                z *= val;
                return *this;
            }

            SVector& operator*=(const SVector& val) {
                x *= val.x;
                y *= val.y;
                z *= val.z;
                return *this;
            }

            SVector& operator+=(float val) {
                x += val;
                y += val;
                z += val;
                return *this;
            }

            SVector& operator+=(const SVector& val) {
                x += val.x;
                y += val.y;
                z += val.z;
                return *this;
            }

            SVector& operator-=(float val) {
                x -= val;
                y -= val;
                z -= val;
                return *this;
            }

            SVector& operator-=(const SVector& val) {
                x -= val.x;
                y -= val.y;
                z -= val.z;
                return *this;
            }
        };
    }
}

inline Core::Math::SVector operator*(float val, const Core::Math::SVector& vec) {
    return vec * val;
}

inline Core::Math::SVector operator+(float val, const Core::Math::SVector& vec) {
    return vec + val;
}