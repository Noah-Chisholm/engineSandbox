#pragma once

#include "InputHandler.h"
#include "baseObject.h"
#include "tickableInterface.h"
#include "name.h"

class inputTester : public baseObject, public tickableInterface {
public:
	inputTester(name _str, bool isActive = true);

	void handleInput(const FInputEvent& event);

	virtual void tick(float dt) override;
	virtual void init() override;

private:
	inputHandler::inputEventSig myDelegate;
	inputHandler::inputEventSig handleCDel;

	int curColor = 0;
};