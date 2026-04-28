#pragma once

#include "Engine/EngineHandler.h"

int main(int argc, char* argv[]) {
	Engine::EngineHandler::getInstance().startEngine();
	return 0;
}