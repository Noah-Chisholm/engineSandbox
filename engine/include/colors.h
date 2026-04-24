#pragma once
#include <array>

struct FcolorRGBA
{
    float r = 1.f;
    float g = 1.f;
    float b = 1.f;
    float a = 1.f;

    constexpr FcolorRGBA() = default;

    constexpr FcolorRGBA(float in) : r(in), g(in), b(in), a(1.f) {}
    constexpr FcolorRGBA(float in, float alpha) : r(in), g(in), b(in), a(alpha) {}

    constexpr FcolorRGBA(float r_, float g_, float b_) : r(r_), g(g_), b(b_), a(1.f) {}
    constexpr FcolorRGBA(float r_, float g_, float b_, float alpha) : r(r_), g(g_), b(b_), a(alpha) {}
};

namespace colors {
    inline constexpr FcolorRGBA WHITE{ 1.f, 1.f, 1.f, 1.f };
    inline constexpr FcolorRGBA BLACK{ 0.f, 0.f, 0.f, 1.f };
    inline constexpr FcolorRGBA GRAY{ 0.5f, 0.5f, 0.5f, 1.f };
    inline constexpr FcolorRGBA DARK_GRAY{ 0.25f, 0.25f, 0.25f, 1.f };
    inline constexpr FcolorRGBA LIGHT_GRAY{ 0.75f, 0.75f, 0.75f, 1.f };

    inline constexpr FcolorRGBA RED{ 1.f, 0.f, 0.f, 1.f };
    inline constexpr FcolorRGBA GREEN{ 0.f, 1.f, 0.f, 1.f };
    inline constexpr FcolorRGBA BLUE{ 0.f, 0.f, 1.f, 1.f };

    inline constexpr FcolorRGBA CYAN{ 0.f, 1.f, 1.f, 1.f };
    inline constexpr FcolorRGBA MAGENTA{ 1.f, 0.f, 1.f, 1.f };
    inline constexpr FcolorRGBA YELLOW{ 1.f, 1.f, 0.f, 1.f };

    inline constexpr FcolorRGBA ORANGE{ 1.f, 0.5f, 0.f, 1.f };
    inline constexpr FcolorRGBA PURPLE{ 0.5f, 0.f, 0.5f, 1.f };
    inline constexpr FcolorRGBA BROWN{ 0.4f, 0.25f, 0.1f, 1.f };

    inline constexpr FcolorRGBA CTRANSPARENT{ 0.f, 0.f, 0.f, 0.f };

    inline constexpr FcolorRGBA RED_50{ 1.f, 0.f, 0.f, 0.5f };
    inline constexpr FcolorRGBA GREEN_50{ 0.f, 1.f, 0.f, 0.5f };
    inline constexpr FcolorRGBA BLUE_50{ 0.f, 0.f, 1.f, 0.5f };

    inline constexpr FcolorRGBA ENGINE_DARK_BLUE{ 0.05f, 0.05f, 0.10f, 1.f };
    inline constexpr FcolorRGBA ENGINE_DARK_GRAY{ 0.10f, 0.10f, 0.10f, 1.f };
    inline constexpr FcolorRGBA SKY_BLUE{ 0.53f, 0.81f, 0.92f, 1.f };

    inline constexpr std::array<FcolorRGBA, 21> colorArray = {
        colors::WHITE,
        colors::BLACK,
        colors::GRAY,
        colors::DARK_GRAY,
        colors::LIGHT_GRAY,
        colors::RED,
        colors::GREEN,
        colors::BLUE,
        colors::CYAN,
        colors::MAGENTA,
        colors::YELLOW,
        colors::ORANGE,
        colors::PURPLE,
        colors::BROWN,
        colors::CTRANSPARENT,
        colors::RED_50,
        colors::GREEN_50,
        colors::BLUE_50,
        colors::ENGINE_DARK_BLUE,
        colors::ENGINE_DARK_GRAY,
        colors::SKY_BLUE
    };
}