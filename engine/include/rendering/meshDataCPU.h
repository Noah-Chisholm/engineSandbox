#pragma once

#include "core/name.h"
#include "rendering/vertex.h"
#include <cstdint>

namespace rendering {
	struct meshDataCPU {
		core::Name key;
		std::vector<Vertex> verts;
		std::vector<std::uint32_t> indicies;
	};
}