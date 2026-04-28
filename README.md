# engineSandbox

`engineSandbox` is a learning-focused C++20 game engine built from low-level systems upward.

The goal is not to ship a commercial engine or hide complexity behind large frameworks. The goal is to understand engine architecture by building explicit, inspectable systems using raw platform and graphics APIs where possible.

## Project Goals

- Build a small game engine in C++20.
- Use raw Win32 APIs for windowing and OS interaction.
- Use Direct3D 12 for rendering.
- Keep systems explicit, deterministic, and debuggable.
- Prefer learning clarity over convenience.
- Avoid black-box engine/framework abstractions.

## Current Status

The engine currently supports:

- Win32 window creation.
- Engine-controlled main loop.
- Input event capture and dispatch.
- Basic object/world structure.
- Per-frame ticking.
- Direct3D 12 rendering.
- CPU/GPU mesh data separation.
- Basic camera and camera controller behavior.

Current rendering milestone:

- Render basic geometry through Direct3D 12.
- Support camera movement and rotation.
- Prepare code structure for future material support.

## High-Level Architecture

The engine is organized around a few major subsystem boundaries:

```txt
Engine/
    Owns startup, shutdown, main loop, ticking, and high-level coordination.

Platforms/
    Owns direct OS interaction. Win32 code belongs here.

Input/
    Owns engine-level input events, input flags, keyboard/mouse data, and input handling.

Rendering/
    Owns GPU-side rendering logic, render handlers, vertices, GPU mesh data, and D3D12-facing types.

Assets/
    Owns file loading and CPU-side asset representations.

GameObjects/
    Owns base object types, world structure, scene interfaces, tickable interfaces, cameras, and renderable objects.

Core/
    Owns reusable foundation utilities such as names, logging, delegates, flag sets, queues, colors, and math types.
```

The main architectural rule is:

> Capture OS facts early, translate them into engine-shaped data, and avoid letting platform-specific details leak into the rest of the engine.

Example flow:

```txt
Win32 message
    ↓
Platforms/Win32 translates it
    ↓
Engine-level event/input data is queued
    ↓
Engine tick processes queued work
    ↓
Objects respond through engine-level systems
```

## Folder Layout

```txt
engine/
├── include/
│   ├── Assets/
│   ├── Core/
│   │   ├── Logging/
│   │   └── Math/
│   ├── Engine/
│   ├── GameObjects/
│   │   └── CoreObjects/
│   ├── Input/
│   ├── Platforms/
│   │   └── Win32/
│   ├── Rendering/
│   └── ThirdParty/
│
└── src/
    ├── Assets/
    ├── Core/
    ├── Engine/
    ├── GameObjects/
    ├── Input/
    ├── Platforms/
    └── Rendering/
```

## Coding Standards

This project uses a consistent style to keep the code readable and predictable.

### C++ Version

The project targets:

```txt
C++20
```

### Header Layout

Headers should generally follow this structure:

```cpp
#pragma once

#include <standard_library_header>

#include "Project/Header.h"

namespace NamespaceName {
    // Declarations go here.
}
```

Standard library includes using `<...>` go first.

Project/local includes using `"..."` go after standard includes.

There should be a blank line between the two include groups.

Example:

```cpp
#pragma once

#include <cstdint>
#include <vector>

#include "Core/Name.h"
#include "Rendering/Vertex.h"

namespace Assets {
    struct SMeshDataCPU {
        Core::Name name;
        std::vector<Rendering::SVertex> vertices;
        std::vector<std::uint32_t> indices;
    };
}
```

### Include Rules

Headers should include what they directly use.

Do not rely on transitive includes.

For example, if a header uses `std::vector`, that header should include:

```cpp
#include <vector>
```

even if another included file already includes `<vector>`.

### Include Path Casing

Include paths should match the actual file and folder casing exactly.

Good:

```cpp
#include "Core/Math/SVector.h"
#include "GameObjects/CameraController.h"
#include "Input/InputCommon.h"
```

Avoid:

```cpp
#include "core/math/SVector.h"
#include "GameObjects/cameraController.h"
#include "input/InputCommon.h"
```

Windows may allow case mismatches, but the codebase should not depend on that.

## Naming Standards

### Files

Files should generally match the primary type, namespace, or module they represent.

Examples:

```txt
SVector.h
SRotator.h
STransform.h
Camera.h
CameraController.h
RenderHandler.h
InputCommon.h
MathCommon.h
```

### Namespaces

Namespaces use UpperCamelCase.

Examples:

```cpp
namespace Core { }
namespace Core::Math { }
namespace Input { }
namespace Rendering { }
namespace Platforms::Win32 { }
```

### Classes

Classes use UpperCamelCase.

```cpp
class RenderHandler;
class GameWindowHandler;
class CameraController;
```

### Structs

Structs are prefixed with `S`.

```cpp
struct SVector;
struct SRotator;
struct STransform;
struct SInputEvent;
struct SMeshDataCPU;
```

### Enums

Enums are prefixed with `E`.

Scoped enums should be preferred.

```cpp
enum class EInputEventType : std::uint8_t;
enum class EInputFlag : std::uint8_t;
enum class EEngineEventType : std::uint8_t;
```

### Constants

Constants use ALL_UPPERCASE.

```cpp
static constexpr std::size_t VK_COUNT = 256;
static constexpr std::size_t MOUSE_BUTTON_COUNT = 5;
```

### Variables

Variables use camelCase.

```cpp
std::uint32_t renderWidth;
std::uint32_t renderHeight;
bool isRunning;
float deltaTime;
```

### Functions

Functions use camelCase.

```cpp
startEngine();
processEvents();
hasFlag();
getForwardVector();
```

## Type Standards

Prefer standard fixed-width integer types when the size matters.

Use:

```cpp
std::uint8_t
std::uint16_t
std::uint32_t
std::uint64_t

std::int8_t
std::int16_t
std::int32_t
std::int64_t
```

Include them with:

```cpp
#include <cstdint>
```

Use `std::size_t` for sizes, counts, and container indexes.

Include it with:

```cpp
#include <cstddef>
```

Example:

```cpp
std::size_t vertexCount = vertices.size();
```

## Platform/API Type Rule

Use standard C++ types for engine-owned data.

Use API-specific types when filling API-owned structs.

Example engine-owned values:

```cpp
std::uint32_t renderWidth;
std::uint32_t renderHeight;
```

Example API-owned struct:

```cpp
D3D12_RECT scissorRect{
    0,
    0,
    static_cast<LONG>(renderWidth),
    static_cast<LONG>(renderHeight)
};
```

Do not replace Win32/D3D types such as `LONG`, `UINT`, `HWND`, or `HANDLE` with standard integer types inside API-owned structures.

## Input System Standards

Input should be event-driven.

The platform layer captures OS facts and translates them into engine-level input events.

Input events are queued and processed during the engine tick.

The intended flow is:

```txt
Win32 input message
    ↓
GameWindowHandler translates message
    ↓
Input::SInputEvent is created
    ↓
InputHandler queues event
    ↓
Engine tick processes event
```

## Enum Standards

Use scoped enums:

```cpp
enum class EInputEventType : std::uint8_t {
    KeyDown,
    KeyUp,
    MouseDown,
    MouseUp,
    MouseMove,
    MouseWheel,

    Count
};
```

For regular enums that represent one choice out of many, values should usually start at zero and end with `Count`.

For flag enums, values should be powers of two:

```cpp
enum class EInputFlag : std::uint8_t {
    SHIFT   = 1 << 0,
    CONTROL = 1 << 1,
    ALT     = 1 << 2
};
```

## Math Standards

Math types live under:

```txt
Core/Math/
```

Current math types include:

```txt
SVector
SRotator
STransform
```

Use `SVector` for:

```txt
positions
directions
offsets
scale
velocity
```

Use `SRotator` for:

```txt
pitch
yaw
roll
```

Direction:

```txt
SVector      = vector math
SRotator     = Euler rotation storage and conversion
SMatrix4     = transform/rotation composition
SQuaternion  = future orientation/interpolation support
```

## Rendering Standards

Rendering code belongs in:

```txt
Rendering/
```

Rendering should remain separate from platform/window/input code where possible.

Rendering data should distinguish between CPU-side and GPU-side representations.

Example:

```txt
Assets/SMeshDataCPU.h
Rendering/SMeshDataGPU.h
```

The intended flow is:

```txt
CPU mesh data
    ↓
GPU upload/resource creation
    ↓
Rendering submits draw commands
```

Rendering should not directly own high-level game object logic.

## Logging and Formatting

Logging lives under:

```txt
Core/Logging/
```

Custom `std::format` formatters should be placed in a dedicated formatting header, such as:

```txt
Core/Logging/Formatters.h
```

Custom types passed directly into `std::format` require a formatter specialization.

Example:

```cpp
template<>
struct std::formatter<Core::Math::SVector, char> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const Core::Math::SVector& vec, std::format_context& ctx) const {
        return std::format_to(
            ctx.out(),
            "(X: {}, Y: {}, Z: {})",
            vec.x,
            vec.y,
            vec.z
        );
    }
};
```

A file that logs formatted custom types must include the formatter header before the log call is compiled.

## Third-Party Code

Third-party headers live under:

```txt
ThirdParty/
```

Third-party files do not need to follow this project’s formatting or naming standards.

Do not edit third-party files unless necessary.

Example:

```txt
ThirdParty/Rendering/d3dx12.h
```

## Build

This project uses CMake.

Typical build flow:

```txt
Configure with CMake
Build with Visual Studio or MSBuild
Run the app executable
```

More detailed build instructions should be added as the project stabilizes.

## Current Development Notes

Near-term refactor goals:

- Standardize include path casing.
- Standardize file names to match types/modules.
- Keep headers self-contained.
- Remove `using namespace` from headers.
- Keep platform-specific logic isolated.
- Continue separating CPU asset data from GPU rendering data.

Near-term engine goals:

- Keep the current rendering path working.
- Add clearer material representation.
- Add better math support for transforms.
- Improve camera and input control flow.
- Prepare asset-loading boundaries for custom mesh/material formats.

## Design Principles

1. Prefer explicit systems over hidden behavior.
2. Prefer deterministic data flow.
3. Keep platform code behind platform boundaries.
4. Keep rendering isolated from input and Win32 where possible.
5. Let abstractions emerge from repeated concrete code.
6. Make debugging and inspection easy.
7. Favor small, understandable systems over large clever systems.
8. Capture OS facts early, interpret them later, and never let the OS leak past the engine boundary.