#include <format>

struct fVector {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    fVector() = default;

    explicit fVector(float i)
        : x(i), y(i), z(i)
    {
    }

    fVector(float inX, float inY, float inZ)
        : x(inX), y(inY), z(inZ)
    {
    }

    fVector operator*(float val) const {
        return fVector(x * val, y * val, z * val);
    }

    fVector operator*(const fVector& val) const {
        return fVector(x * val.x, y * val.y, z * val.z);
    }

    fVector operator+(float val) const {
        return fVector(x + val, y + val, z + val);
    }

    fVector operator+(const fVector& val) const {
        return fVector(x + val.x, y + val.y, z + val.z);
    }

    fVector& operator*=(float val) {
        x *= val;
        y *= val;
        z *= val;
        return *this;
    }

    fVector& operator*=(const fVector& val) {
        x *= val.x;
        y *= val.y;
        z *= val.z;
        return *this;
    }

    fVector& operator+=(float val) {
        x += val;
        y += val;
        z += val;
        return *this;
    }

    fVector& operator+=(const fVector& val) {
        x += val.x;
        y += val.y;
        z += val.z;
        return *this;
    }

    fVector& operator-=(float val) {
        x -= val;
        y -= val;
        z -= val;
        return *this;
    }

    fVector& operator-=(const fVector& val) {
        x -= val.x;
        y -= val.y;
        z -= val.z;
        return *this;
    }
};

inline fVector operator*(float val, const fVector& vec) {
    return vec * val;
}

inline fVector operator+(float val, const fVector& vec) {
    return vec + val;
}

template<>
struct std::formatter<fVector, char> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const fVector& vec, std::format_context& ctx) const {
        return std::format_to(
            ctx.out(),
            "({}, {}, {})",
            vec.x,
            vec.y,
            vec.z
        );
    }
};