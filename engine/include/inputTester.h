#pragma once

#include "input/inputHandler.h"
#include "baseObject.h"
#include "tickableInterface.h"
#include "name.h"

class inputTester : public baseObject {
public:
	inputTester(name _str);

	void handleInput(const input::SInputEvent& event);

	virtual void init() override;

private:
	input::InputHandler::InputEventSig input;
};