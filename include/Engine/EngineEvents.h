#pragma once

#include <cstdint>

namespace Engine {
    enum class EEngineEventType : std::uint8_t {
        RESIZE
    };

    struct SEngineEvent {
        EEngineEventType type{};

        union {
            struct { std::uint32_t newW; std::uint32_t newH; bool newMin; } resize;
        };
    };
}