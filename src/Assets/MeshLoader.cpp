#include <filesystem>
#include <fstream>

#include "Assets/MeshLoader.h"
#include "Core/Logging/Logging.h"

using namespace Assets;

bool MeshLoader::loadFile(std::string filePath, Assets::SMeshDataCPU& data) {
    std::filesystem::path meshPath = filePath;

    if (!std::filesystem::exists(meshPath)) {
        Core::Logging::log("Cannot find mesh file: \'{}\'", meshPath.string());
        return false;
    }

    std::ifstream meshFile(meshPath, std::ios_base::binary);

    if (!meshFile.is_open()) {
        Core::Logging::log("Cannot find open mesh file: \'{}\'", meshPath.string());
        return false;
    }

    std::uint32_t nameLength = 0;

    meshFile.read(
        reinterpret_cast<char*>(&nameLength),
        sizeof(nameLength)
    );

    std::string name;
    name.resize(nameLength);

    meshFile.read(
        name.data(),
        nameLength
    );

    data.key = Core::Name(name);

    std::uint64_t vertexCount = 0;

    meshFile.read(
        reinterpret_cast<char*>(&vertexCount),
        sizeof(vertexCount)
    );

    data.verts.resize(vertexCount);

    meshFile.read(
        reinterpret_cast<char*>(data.verts.data()),
        data.verts.size() * sizeof(data.verts[0])
    );

    std::uint64_t indexCount = 0;

    meshFile.read(
        reinterpret_cast<char*>(&indexCount),
        sizeof(indexCount)
    );

    data.indices.resize(indexCount);

    meshFile.read(
        reinterpret_cast<char*>(data.indices.data()),
        data.indices.size() * sizeof(data.indices[0])
    );

    return true;
}