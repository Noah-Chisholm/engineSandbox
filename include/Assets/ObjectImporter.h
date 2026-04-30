#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <filesystem>

#include "Rendering/Vertex.h"
#include "Assets/SMeshDataCPU.h"

namespace Assets {
	namespace ObjectImporter {
		std::string usage();

		std::uint32_t parseObjPositionIndex(const std::string& faceToken);
		
		void windTriangles(std::string input, Assets::SMeshDataCPU& data);

		void parseVertices(std::string input, Assets::SMeshDataCPU& data);

		Assets::SMeshDataCPU parseFile(std::string file);

		bool writeFile(std::filesystem::path outputPath, const Assets::SMeshDataCPU& data);

		bool parseInputFile(const std::filesystem::path inputPath, Assets::SMeshDataCPU& data);
	};
}