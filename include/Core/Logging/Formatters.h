#pragma once

#include <format>

#include "Core/Math/MathCommon.h"

template<>
struct std::formatter<Core::Math::SVector, char> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const Core::Math::SVector& vec, std::format_context& ctx) const {
        return std::format_to(
            ctx.out(),
            "(X: {}, Y: {}, Z: {})",
            vec.x,
            vec.y,
            vec.z
        );
    }
};

template<>
struct std::formatter<Core::Math::SRotator, char> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const Core::Math::SRotator& rot, std::format_context& ctx) const {
        return std::format_to(
            ctx.out(),
            "(Pitch: {}, Yaw: {}, Roll: {})",
            rot.pitch,
            rot.yaw,
            rot.roll
        );
    }
};