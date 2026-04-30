#include "Assets/ObjectImporter.h"
#include "Core/Logging/Logging.h"

using namespace Core;
using namespace Assets::ObjectImporter;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::string arg = argv[1];
        if (arg == "-h" || arg == "--help") {
            Logging::log(usage());
            return 0;
        }

        Logging::log("Too few args!");
        Logging::log(usage());
        return 1;
    }

    if (argc > 4) {
        Logging::log("Too many args!");
        Logging::log(usage());
        return 1;
    }

    std::filesystem::path inputPath = argv[1];
    std::filesystem::path outputPath = argv[2];

    if (argc == 4) {
        outputPath /= argv[3];
    }

    if (!std::filesystem::exists(inputPath)) {
        Logging::log("Cannot find input file: \'{}\'", inputPath.string());
        return 1;
    }

    if (!(inputPath.has_extension() && inputPath.extension().string() == ".obj")) {
        Logging::log("Input File is not a .obj file!", inputPath.string());
        return 1;
    }

    if (std::filesystem::is_directory(outputPath)) {
        std::filesystem::create_directories(outputPath);
        outputPath /= inputPath.filename().replace_extension(".bin");
    }
    else
    if (outputPath.has_parent_path() && !std::filesystem::exists(outputPath.parent_path())) {
        std::filesystem::create_directories(outputPath.parent_path());
    }

    Assets::SMeshDataCPU data{
        Core::Name(inputPath.stem().string())
    };

    parseInputFile(inputPath, data);
    writeFile(outputPath, data);

    return 0;
}