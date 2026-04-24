#pragma once

enum class EEngineEventType : UINT8 {
    RESIZE
};

struct FengineEvent {
    EEngineEventType type{};

    union {
        struct { uint32_t newW; uint32_t newH; bool newMin; } resize;
    };
};