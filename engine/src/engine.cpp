#include "engine.h"
#include "camera.h"
#include "renderable.h"
#include <numbers>

void engine::gameplayLoop() {
    while (!mainGameWindow.quitRequested) {
        mainGameWindow.readMessages();
        mainTickHandler.tick();
        mainInputHandler.handleAllEvents();
        mainRenderHandler.render();
        handleEngineEvents();
    }
    activeWorld.reset();
}
    
void engine::startEngine() {
    activeWorld = std::make_unique<world>();
    transform defaultCameraTrans = { fVector(), fVector(), fVector()};
    float defaultFovYRadians = std::numbers::pi * 60.0f / 180.0f; // 60 degrees
    float defaultAspectRatio = static_cast<float>(defaultWindowW) / static_cast<float>(defaultWindowH);
    float defaultNearPlane = 0.1f;
    float defaultFarPlane = 1000.0f;

    camera::cameraData defaultCameraData = camera::cameraData(
        defaultFovYRadians,
        defaultAspectRatio,
        defaultNearPlane,
        defaultFarPlane,
        defaultWindowW,
        defaultWindowH
    );
    auto defaultCamera = static_pointer_cast<camera>(activeWorld->spawnObject<camera>(nullptr, name("mainCamera"), defaultCameraTrans, defaultCameraData));
    mainRenderHandler.init(defaultWindowW, defaultWindowH, mainGameWindow.initWindow(defaultWindowW, defaultWindowH), defaultCamera);
    transform testTriTrans = { fVector(0.0f,0.0f,10.0f), fVector((45 * std::numbers::pi / 180),0.0f,0.0f), fVector(1.0f, 1.0f, 1.0f) };
    transform testTri2Trans = { fVector(10.0f,0.0f,50.0f), fVector((45 * std::numbers::pi / 180),0.0f,0.0f), fVector(1.0f, 1.0f, 1.0f)};
    meshDataCPU testCube = {
        name("cubeModel"),
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
    activeWorld->spawnObject<renderable>(nullptr, name("testTri"), mainRenderHandler.createGpuMesh(testCube), testTriTrans);
    activeWorld->spawnObject<renderable>(nullptr, name("testTri2"), mainRenderHandler.createGpuMesh(testCube), testTri2Trans);
    activeWorld->spawnObject<inputTester>(nullptr, name("Tester"));
    gameplayLoop();
}

void engine::handleResize(int w, int h, bool minimized) {
    isMinimized = minimized;
}

engine& engine::getInstance() {
    static engine inst;
    return inst;
}

void engine::handleEngineEvents() {
    FengineEvent curEvent{};
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
