#include <numbers>

#include "Engine/EngineHandler.h"
#include "GameObjects/CameraController.h"
#include "GameObjects/Renderable.h"

using namespace Engine;

void EngineHandler::gameplayLoop() {
    while (!mainGameWindow.quitRequested) {
        mainGameWindow.readMessages();
        mainTickHandler.tick();
        mainInputHandler.handleAllEvents();
        mainRenderHandler.render();
        handleEngineEvents();
    }
    activeWorld.reset();
}
    
void EngineHandler::startEngine() {
    activeWorld = std::make_unique<GameObjects::CoreObjects::World>();
    Core::Math::STransform defaultCameraTrans;
    float defaultFovYRadians = std::numbers::pi * 60.0f / 180.0f; // 60 degrees
    float defaultAspectRatio = static_cast<float>(defaultWindowW) / static_cast<float>(defaultWindowH);
    float defaultNearPlane = 0.1f;
    float defaultFarPlane = 1000.0f;

    GameObjects::Camera::SCameraData defaultCameraData = GameObjects::Camera::SCameraData(
        defaultFovYRadians,
        defaultAspectRatio,
        defaultNearPlane,
        defaultFarPlane,
        defaultWindowW,
        defaultWindowH
    );
    auto defaultCamera = static_pointer_cast<GameObjects::Camera>(activeWorld->spawnObject<GameObjects::CameraController>(nullptr, Core::Name("mainCamera"), defaultCameraTrans, defaultCameraData));
    mainRenderHandler.init(defaultWindowW, defaultWindowH, mainGameWindow.initWindow(defaultWindowW, defaultWindowH), defaultCamera);
    
    Core::Math::STransform testTriTrans = { Core::Math::SVector(0.0f,0.0f,10.0f), Core::Math::SRotator((45 * std::numbers::pi / 180),0.0f,0.0f), Core::Math::SVector(1.0f, 1.0f, 1.0f) };
    Assets::SMeshDataCPU testCube = {
        Core::Name("cubeModel"),
        {
            // Front face, z = -0.5
            { { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // 0 top-left
            { {  0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // 1 top-right
            { {  0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // 2 bottom-right
            { { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // 3 bottom-left

            // Back face, z = 0.5
            { { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f } }, // 4 top-left
            { {  0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f, 1.0f } }, // 5 top-right
            { {  0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // 6 bottom-right
            { { -0.5f, -0.5f,  0.5f }, { 0.3f, 0.3f, 0.3f, 1.0f } }  // 7 bottom-left
        },
        {
            // Front face
            0, 1, 2,
            0, 2, 3,

            // Right face
            1, 5, 6,
            1, 6, 2,

            // Back face
            5, 4, 7,
            5, 7, 6,

            // Left face
            4, 0, 3,
            4, 3, 7,

            // Top face
            4, 5, 1,
            4, 1, 0,

            // Bottom face
            3, 2, 6,
            3, 6, 7
        }
    };

    activeWorld->spawnObject<GameObjects::Renderable>(nullptr, Core::Name("testTri"), mainRenderHandler.createGpuMesh(testCube), testTriTrans);
    gameplayLoop();
}

void EngineHandler::handleResize(int w, int h, bool minimized) {
    isMinimized = minimized;
}

EngineHandler& EngineHandler::getInstance() {
    static EngineHandler inst;
    return inst;
}

void EngineHandler::handleEngineEvents() {
    SEngineEvent curEvent{};
    while (mainGameWindow.dequeueEngineEvent(curEvent) ) {
        switch (curEvent.type)
        {
        case EEngineEventType::RESIZE:
            handleResize(curEvent.resize.newW, curEvent.resize.newH, curEvent.resize.newMin);
            break;
        default:
            break;
        }
    }
}
