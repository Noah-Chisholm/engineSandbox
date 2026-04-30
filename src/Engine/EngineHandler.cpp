#include <numbers>

#include "Engine/EngineHandler.h"
#include "GameObjects/CameraController.h"
#include "GameObjects/Renderable.h"
#include "Assets/MeshLoader.h"
#include "Assets/SMaterialDataCPU.h"

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
    
    Core::Math::STransform testTriTrans = { Core::Math::SVector(0.0f,0.0f,10.0f), Core::Math::SRotator(0.0f,0.0f,0.0f), Core::Math::SVector(1.0f, 1.0f, 1.0f) };
    Assets::SMeshDataCPU testCube{};
    Assets::SMeshDataCPU testDresser{};
    Assets::SMeshDataCPU testCouch{};
    Assets::SMaterialDataCPU green{
        Core::Name("solidGreen")
    };
    green.constants.baseColor = Core::Colors::GREEN;
    Assets::SMaterialDataCPU blue{
        Core::Name("solidBlue")
    };
    blue.constants.baseColor = Core::Colors::BLUE;
    Assets::SMaterialDataCPU red{
        Core::Name("solidRed")
    };
    red.constants.baseColor = Core::Colors::RED;
    Assets::SMaterialDataCPU white{
        Core::Name("solidWhite")
    };
    Assets::SMaterialDataCPU noMat{
        Core::Name("noMat")
    };
    noMat.PSPath = L"D:\\personal one drive\\OneDrive\\Documents\\engineSandbox\\data\\shaders\\pixel\\pixelShader.hlsl";
    if (Assets::MeshLoader::loadFile(R"(D:\personal one drive\OneDrive\Documents\engineSandbox\data\sampleModels\couch.bin)", testCouch)) {
        Assets::MeshLoader::loadFile(R"(D:\personal one drive\OneDrive\Documents\engineSandbox\data\sampleModels\unitCube.bin)", testCube);
        Assets::MeshLoader::loadFile(R"(D:\personal one drive\OneDrive\Documents\engineSandbox\data\sampleModels\dresser.bin)", testDresser);
        activeWorld->spawnObject<GameObjects::Renderable>(nullptr, Core::Name("testTri"), mainRenderHandler.createGpuMesh(testCouch), testTriTrans, mainRenderHandler.createGpuMaterial(blue));
        testTriTrans.location += Core::Math::SVector(0.0f, 0.0f, 25.0f);
        activeWorld->spawnObject<GameObjects::Renderable>(nullptr, Core::Name("testTri"), mainRenderHandler.createGpuMesh(testDresser), testTriTrans, mainRenderHandler.createGpuMaterial(noMat));
        testTriTrans.location += Core::Math::SVector(0.0f, 0.0f, 25.0f);
        activeWorld->spawnObject<GameObjects::Renderable>(nullptr, Core::Name("testTri"), mainRenderHandler.createGpuMesh(testDresser), testTriTrans, mainRenderHandler.createGpuMaterial(green));
        testTriTrans.location += Core::Math::SVector(0.0f, 0.0f, 25.0f);
        testTriTrans.scale += Core::Math::SVector(5.0f);
        activeWorld->spawnObject<GameObjects::Renderable>(nullptr, Core::Name("testTri"), mainRenderHandler.createGpuMesh(testCube), testTriTrans, mainRenderHandler.createGpuMaterial(white));
        testTriTrans.location += Core::Math::SVector(0.0f, 0.0f, 25.0f);
        activeWorld->spawnObject<GameObjects::Renderable>(nullptr, Core::Name("testTri"), mainRenderHandler.createGpuMesh(testCube), testTriTrans, mainRenderHandler.createGpuMaterial(red));
    }
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
