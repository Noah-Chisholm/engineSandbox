#pragma once
#include <array>

namespace core {
    namespace colors {
        struct SColorRGBA
        {
            float r = 1.f;
            float g = 1.f;
            float b = 1.f;
            float a = 1.f;

            constexpr SColorRGBA() = default;

            constexpr SColorRGBA(float in) : r(in), g(in), b(in), a(1.f) {}
            constexpr SColorRGBA(float in, float alpha) : r(in), g(in), b(in), a(alpha) {}

            constexpr SColorRGBA(float r_, float g_, float b_) : r(r_), g(g_), b(b_), a(1.f) {}
            constexpr SColorRGBA(float r_, float g_, float b_, float alpha) : r(r_), g(g_), b(b_), a(alpha) {}
        };

        inline constexpr SColorRGBA WHITE{ 1.f, 1.f, 1.f, 1.f };
        inline constexpr SColorRGBA BLACK{ 0.f, 0.f, 0.f, 1.f };
        inline constexpr SColorRGBA GRAY{ 0.5f, 0.5f, 0.5f, 1.f };
        inline constexpr SColorRGBA DARK_GRAY{ 0.25f, 0.25f, 0.25f, 1.f };
        inline constexpr SColorRGBA LIGHT_GRAY{ 0.75f, 0.75f, 0.75f, 1.f };

        inline constexpr SColorRGBA RED{ 1.f, 0.f, 0.f, 1.f };
        inline constexpr SColorRGBA GREEN{ 0.f, 1.f, 0.f, 1.f };
        inline constexpr SColorRGBA BLUE{ 0.f, 0.f, 1.f, 1.f };

        inline constexpr SColorRGBA CYAN{ 0.f, 1.f, 1.f, 1.f };
        inline constexpr SColorRGBA MAGENTA{ 1.f, 0.f, 1.f, 1.f };
        inline constexpr SColorRGBA YELLOW{ 1.f, 1.f, 0.f, 1.f };

        inline constexpr SColorRGBA ORANGE{ 1.f, 0.5f, 0.f, 1.f };
        inline constexpr SColorRGBA PURPLE{ 0.5f, 0.f, 0.5f, 1.f };
        inline constexpr SColorRGBA BROWN{ 0.4f, 0.25f, 0.1f, 1.f };

        inline constexpr SColorRGBA CTRANSPARENT{ 0.f, 0.f, 0.f, 0.f };

        inline constexpr SColorRGBA RED_50{ 1.f, 0.f, 0.f, 0.5f };
        inline constexpr SColorRGBA GREEN_50{ 0.f, 1.f, 0.f, 0.5f };
        inline constexpr SColorRGBA BLUE_50{ 0.f, 0.f, 1.f, 0.5f };

        inline constexpr SColorRGBA ENGINE_DARK_BLUE{ 0.05f, 0.05f, 0.10f, 1.f };
        inline constexpr SColorRGBA ENGINE_DARK_GRAY{ 0.10f, 0.10f, 0.10f, 1.f };
        inline constexpr SColorRGBA SKY_BLUE{ 0.53f, 0.81f, 0.92f, 1.f };

        inline constexpr std::array<SColorRGBA, 21> colorArray = {
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
}