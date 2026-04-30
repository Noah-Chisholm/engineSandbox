#pragma once

#include "Assets/SMeshDataCPU.h"

namespace Assets {
	namespace MeshLoader {
		bool loadFile(std::string filePath, Assets::SMeshDataCPU& data);
	}
}