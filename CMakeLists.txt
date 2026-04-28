cmake_minimum_required(VERSION 3.10)  # also fixes the deprecation warning

project(engineSandbox LANGUAGES CXX)

add_library(engineSandbox STATIC
  "engine/src/tickHandler.cpp"
  "engine/src/input/inputHandler.cpp"
  "engine/src/GameWindowHandler.cpp"
  "engine/src/baseObject.cpp"
  "engine/src/tickableInterface.cpp"
  "engine/src/logging.cpp"
  "engine/src/inputTester.cpp"
  "engine/src/engine.cpp"
  "engine/src/world.cpp"
  "engine/src/rendering/renderHandler.cpp"
  "engine/src/renderble.cpp"
  "engine/src/sceneInterface.cpp" "engine/src/camera.cpp" "engine/src/name.cpp" "engine/src/cameraController.cpp" )

target_compile_features(engineSandbox PUBLIC cxx_std_20)

target_include_directories(engineSandbox PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/engine/include
)

# ---- app executable ----
add_executable(app app/src/main.cpp)

target_link_libraries(engineSandbox PUBLIC d3d12 dxgi D3DCompiler)
target_link_libraries(app PRIVATE engineSandbox)

# ---- compiler/sanitizer flags (AFTER targets exist) ----
if (MSVC)
  target_compile_options(engineSandbox PRIVATE
    $<$<CONFIG:Debug>:/fsanitize=address>
    $<$<CONFIG:Debug>:/Zi>
  )

  target_link_options(engineSandbox PRIVATE
    $<$<CONFIG:Debug>:/fsanitize=address>
    $<$<CONFIG:Debug>:/DEBUG>
  )

  target_compile_options(app PRIVATE
    $<$<CONFIG:Debug>:/fsanitize=address>
    $<$<CONFIG:Debug>:/Zi>
  )

  target_link_options(app PRIVATE
    $<$<CONFIG:Debug>:/fsanitize=address>
    $<$<CONFIG:Debug>:/DEBUG>
  )
endif()