#pragma once

#include "InputHandler.h"
#include "tickableObject.h"

class inputTester : public tickableObject {
public:
	inputTester(std::string name, bool isActive = true);

	void handleInput(const FInputEvent& event);

	void swapRenderColor(const FInputEvent& event);

	virtual void tick(float dt) override;

private:
	inputHandler::inputEventSig myDelegate;
	inputHandler::inputEventSig handleCDel;

	int curColor = 0;
};