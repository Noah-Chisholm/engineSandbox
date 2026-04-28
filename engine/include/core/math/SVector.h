#pragma once

namespace core {
    namespace math {
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

    inline math::SVector operator*(float val, const math::SVector& vec) {
        return vec * val;
    }

    inline math::SVector operator+(float val, const math::SVector& vec) {
        return vec + val;
    }
}