#pragma once

#include <cstdint>

#include "Core/Name.h"
#include "Core/Colors.h"

namespace Assets {

    enum class EMaterialKind : std::uint8_t {
        SolidColor,
        VertexColor
    };

    struct SMaterialConstants {
        Core::Colors::SColorRGBA baseColor = Core::Colors::SColorRGBA(1.0f);
    };

    struct SMaterialDataCPU {
        Core::Name materialName;
        EMaterialKind kind = EMaterialKind::SolidColor;
        SMaterialConstants constants;
        LPCWSTR PSPath = L"D:\\personal one drive\\OneDrive\\Documents\\engineSandbox\\data\\shaders\\pixel\\solidPixelShader.hlsl";
        LPCWSTR VSPath = L"D:\\personal one drive\\OneDrive\\Documents\\engineSandbox\\data\\shaders\\vertex\\vertexShader.hlsl";
    };

}