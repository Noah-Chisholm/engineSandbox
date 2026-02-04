# CMakeList.txt : CMake project for engineSandbox, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(engineSandbox LANGUAGES CXX)

# ---- engine library ----
add_library(engineSandbox STATIC
  engine/src/tickHandler.cpp
  engine/src/InputHandler.cpp
  engine/src/GameWindowHandler.cpp
  engine/src/baseObject.cpp
  engine/src/tickableObject.cpp
 "engine/src/globalFunctions.cpp")

target_compile_features(engineSandbox PUBLIC cxx_std_20)

# Public include path: consumers can #include <engine/TickHandler.h>
target_include_directories(engineSandbox PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/engine/include
)

if (MSVC)
  target_compile_options(engineSandbox PRIVATE /W3 /permissive-)
else()
  target_compile_options(engineSandbox PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ---- app executable ----
add_executable(app
  app/src/main.cpp
 "engine/src/globalFunctions.cpp")

target_link_libraries(app PRIVATE engineSandbox)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET engineSandbox PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
