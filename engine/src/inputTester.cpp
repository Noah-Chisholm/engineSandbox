#include "inputTester.h"
#include "InputHandler.h"
#include "delegates.h"
#include "logging.h"

inputTester::inputTester(std::string name, bool isActive) : tickableObject(name, isActive) {
	myDelegate = inputHandler::inputEventSig::Bind<inputTester, &inputTester::handleInput>(&(*this));
	inputHandler::getInstance().registerForAnyKeyEvent(myDelegate);
	inputHandler::getInstance().registerForKeyEvent('C', handleCDel);
}

void inputTester::handleInput(const FInputEvent& event) {
	std::string input = "User input - \n"\
		"VK: {}\n"\
		"ALPHA NUMERIC: {}\n"\
		"{}\n"\
		"MODS: {}\n\n\n";
	std::string pressedState = event.type == input::EInputEventType::KeyUp ? "Was Released" : "Was Pressed";
	std::string modsStr = (event.mods & input::EInputFlags::ALT) ? "ALT, " : "";
	if (event.mods & input::EInputFlags::CONTROL)
		modsStr += "CONTROL, ";
	if (event.mods & input::EInputFlags::SHIFT)
		modsStr += "CONTROL, ";

	logging::log(input, event.key.vk, static_cast<char>(event.key.vk), pressedState, modsStr);

	if (event.key.vk == 'T')
		setActive(!isActive);
}

void inputTester::tick(float dt) {
	//log("tick is active\n");
}