cmake_minimum_required(VERSION 3.10)  # also fixes the deprecation warning

project(engineSandbox LANGUAGES CXX)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(engineSandbox STATIC ${SOURCE_FILES})

target_compile_features(engineSandbox PUBLIC cxx_std_20)

target_include_directories(engineSandbox PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ---- app executable ----
add_executable(app entry.cpp)

target_link_libraries(engineSandbox PUBLIC d3d12 dxgi D3DCompiler)
target_link_libraries(app PRIVATE engineSandbox)