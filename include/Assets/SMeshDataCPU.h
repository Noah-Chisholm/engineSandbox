#pragma once

#include <cstdint>
#include <vector>

#include "Core/Name.h"
#include "Rendering/Vertex.h"

namespace Assets {
	struct SMeshDataCPU {
		Core::Name key{};
		std::vector<Rendering::SVertex> verts;
		std::vector<std::uint32_t> indices;
	};
}