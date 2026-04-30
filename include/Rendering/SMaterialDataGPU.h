#pragma once

#include <d3d12.h>
#include <wrl.h>

#include "Assets/SMaterialDataCPU.h"

namespace Rendering {

    struct SMaterialGpuData {
        Assets::SMaterialDataCPU description;

        Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
    };

}