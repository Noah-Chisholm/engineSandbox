#pragma once
#include "tickableObject.h"
#include "TickHandler.h"
#include "GameWindowHandler.h"

int main(int argc, char* argv[]) {
    tickableObject* objs[10];
    for (int i = 0; i < 10; i++)
        objs[i] = new tickableObject("Object #" + std::to_string(i));

    tickHandler& mainTickHandler = tickHandler::getHandler();
    GameWindowHandler& mainGameWindow = GameWindowHandler::getInstance();

    while (!mainGameWindow.quitRequested) {
        mainTickHandler.tick();
        mainGameWindow.readMessages();
    }
}