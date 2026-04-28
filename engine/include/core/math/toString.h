#pragma once

#include <format>
#include "core/math/mathCommon.h"

template<>
struct std::formatter<core::math::SVector, char> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const core::math::SVector& vec, std::format_context& ctx) const {
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
struct std::formatter<core::math::SRotator, char> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const core::math::SRotator& rot, std::format_context& ctx) const {
        return std::format_to(
            ctx.out(),
            "(Pitch: {}, Yaw: {}, Roll: {})",
            rot.pitch,
            rot.yaw,
            rot.roll
        );
    }
};