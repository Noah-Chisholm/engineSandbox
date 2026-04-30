#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Assets/ObjectImporter.h"
#include "Rendering/Vertex.h"
#include "Core/Colors.h"
#include "Core/Logging/Logging.h"
#include "Core/Name.h"

using namespace Assets;

std::uint32_t ObjectImporter::parseObjPositionIndex(const std::string& faceToken) {
	const std::size_t slashPos = faceToken.find('/');

	std::string indexText;

	if (slashPos == std::string::npos) {
		indexText = faceToken;
	}
	else {
		indexText = faceToken.substr(0, slashPos);
	}

	const std::uint32_t objIndex = static_cast<std::uint32_t>(std::stoul(indexText));

	return objIndex - 1;
}

void ObjectImporter::windTriangles(std::string input, Assets::SMeshDataCPU& data) {
	std::istringstream inputStream(input);
	std::string line;

	while (std::getline(inputStream, line)) {
		std::istringstream lineStream(line);

		std::string prefix;
		lineStream >> prefix;

		if (prefix != "f") {
			continue;
		}

		std::vector<std::uint32_t> faceIndices;

		std::string token;

		while (lineStream >> token) {
			faceIndices.push_back(parseObjPositionIndex(token));
		}

		if (faceIndices.size() < 3) {
			Core::Logging::log("Skipping malformed face line: '{}'", line);
			continue;
		}

		for (std::size_t i = 1; i + 1 < faceIndices.size(); ++i) {
			data.indices.push_back(faceIndices[0]);
			data.indices.push_back(faceIndices[i]);
			data.indices.push_back(faceIndices[i + 1]);
		}
	}
}

uint8_t vertIndex = 0;
Core::Colors::SColorRGBA vertColors[8] = {Core::Colors::BLUE, Core::Colors::RED, Core::Colors::CYAN, Core::Colors::DARK_GRAY, Core::Colors::ENGINE_DARK_BLUE, Core::Colors::GREEN, Core::Colors::MAGENTA, Core::Colors::ORANGE};

void ObjectImporter::parseVertices(std::string input, Assets::SMeshDataCPU& data) {
	std::istringstream inputStream(input);
	std::string line;
	std::string dummy;
	

	while (std::getline(inputStream, line)) {
		std::istringstream lineStream(line);
		lineStream >> dummy;

		Rendering::SVertex vert{};
		vert.color = vertColors[vertIndex++];
		if (vertIndex == 7)
			vertIndex = 0;
		lineStream >> vert.position.x;
		lineStream >> vert.position.y;
		lineStream >> vert.position.z;

		data.verts.push_back(vert);
	}
}

std::string ObjectImporter::usage() {
	return R"(Usage: ObjectImporter.exe <Input.obj file> <Output Directory> [OutFileName]

Ex: ObjectImporter.exe C:/Models/SampleCube.obj C:/Models/
This will create a binary file: C:/Models/SampleCube.bin

Ex: ObjectImporter.exe C:/Models/SampleCube.obj C:/Models/ untitled.bin
This will create a binary file: C:/Models/untitled.bin

Ex: ObjectImporter.exe C:/Models/SampleCube.obj C:/Models/ untitled
This will create a binary file: C:/Models/untitled.bin
)";
}

bool ObjectImporter::writeFile(std::filesystem::path outputPath, const Assets::SMeshDataCPU& data) {
	std::ofstream outputFile(outputPath, std::ios_base::binary);
	if (!outputFile.is_open()) {
		Core::Logging::log("Failed to open output file: '{}'", outputPath.string());
		return false;
	}

	std::string name = data.key.getString();

	std::uint32_t nameLength = static_cast<std::uint32_t>(name.size());
	std::uint64_t vertexCount = static_cast<std::uint64_t>(data.verts.size());
	std::uint64_t indexCount = static_cast<std::uint64_t>(data.indices.size());

	// Write name length.
	outputFile.write(
		reinterpret_cast<const char*>(&nameLength),
		sizeof(nameLength)
	);

	// Write name bytes. No null terminator needed because length was written.
	outputFile.write(
		name.data(),
		nameLength
	);

	// Write vertex count.
	outputFile.write(
		reinterpret_cast<const char*>(&vertexCount),
		sizeof(vertexCount)
	);

	// Write vertex array.
	outputFile.write(
		reinterpret_cast<const char*>(data.verts.data()),
		data.verts.size() * sizeof(data.verts[0])
	);

	// Write index count.
	outputFile.write(
		reinterpret_cast<const char*>(&indexCount),
		sizeof(indexCount)
	);

	// Write index array.
	outputFile.write(
		reinterpret_cast<const char*>(data.indices.data()),
		data.indices.size() * sizeof(data.indices[0])
	);

	outputFile.close();

	return true;
}

bool ObjectImporter::parseInputFile(const std::filesystem::path inputPath, Assets::SMeshDataCPU& data) {
	std::ifstream inputFile(inputPath);

	if (!inputFile.is_open()) {
		Core::Logging::log("Failed to open input file: '{}'", inputPath.string());
		return false;
	}

	std::string line;
	uint32_t counter = 1;
	std::string batch = "";

	while (std::getline(inputFile, line)) {
		std::istringstream lineStream(line);

		std::string prefix;
		lineStream >> prefix;

	top:
		if (prefix == "#") {
			Core::Logging::log("Line {} - Skipping Comment: {}!\n", counter++, line);
		}
		else if (prefix == "v") {
			Core::Logging::log("Found the vertex block!\n");
			Core::Logging::log("Line {}: Processing vertex {}!\n", counter++, line);
			batch += line + "\n";
			while (std::getline(inputFile, line)) {
				lineStream = std::istringstream(line);

				lineStream >> prefix;
				if (prefix == "v") {
					Core::Logging::log("Line {}: Processing vertex {}!\n", counter++, line);
					batch += line + "\n";
				}
				else {
					parseVertices(batch, data);
					goto top;
				}
			}
			parseVertices(batch, data);
		}
		else if (prefix == "vn") {
			Core::Logging::log("Line {} - Skipping vertex normal!: {}\n", counter++, line);
		}
		else if (prefix == "vt") {
			Core::Logging::log("Line {} - Skipping UV!: {}\n", counter++, line);
		}
		else if (prefix == "s") {
			Core::Logging::log("Line {} - SKipping shading group!: {}\n", counter++, line);
		}
		else if (prefix == "f") {
			Core::Logging::log("Found the face block!\n");
			Core::Logging::log("Line {}: Processing face {}!\n", counter++, line);
			batch = line + "\n";
			while (std::getline(inputFile, line)) {
				lineStream = std::istringstream(line);

				lineStream >> prefix;
				if (prefix == "f") {
					Core::Logging::log("Line {}: Processing face {}!\n", counter++, line);
					batch += line + "\n";
				}
				else {
					windTriangles(batch, data);
					goto top;
				}
			}
			windTriangles(batch, data);
		}
		else {
			Core::Logging::log("Line {} - Unhandled Prefix!: {}\n", counter++, line);
		}


	}
	Core::Logging::log("Finished!");

	inputFile.close();

	return true;
}