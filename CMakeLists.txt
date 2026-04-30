cmake_minimum_required(VERSION 3.10)  # also fixes the deprecation warning

project(engineSandbox LANGUAGES CXX)

# ---- Engine Setup ----

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(engineSandbox STATIC ${SOURCE_FILES})

target_compile_features(engineSandbox PUBLIC cxx_std_20)

target_include_directories(engineSandbox PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ---- Engine Executable ----
add_executable(app "EntryPoints/EngineEntry.cpp")

target_link_libraries(engineSandbox PUBLIC d3d12 dxgi D3DCompiler)
target_link_libraries(app PRIVATE engineSandbox)

# ---- Object Importer Setup ----

add_library(ObjectImporterLib STATIC
    "src/Assets/ObjectImporter.cpp" "src/Core/Name.cpp"
)

target_compile_features(ObjectImporterLib PUBLIC cxx_std_20)

target_include_directories(ObjectImporterLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ---- Object Importer Executable ----

add_executable(ObjectImporter
    "EntryPoints/ObjectImporterEntry.cpp"
)

target_link_libraries(ObjectImporter PRIVATE
    ObjectImporterLib
)