#include "inputTester.h"
#include "input/InputHandler.h"
#include "delegates.h"
#include "logging.h"

inputTester::inputTester(name _str) : baseObject(_str) {
	input = input::InputHandler::InputEventSig::Bind<inputTester, &inputTester::handleInput>(&(*this));
	input::InputHandler::getInstance().registerForAnyKeyEvent(input);
}

void inputTester::handleInput(const input::SInputEvent& event) {
	std::string input = "User input - \n"\
		"VK: {}\n"\
		"ALPHA NUMERIC: {}\n"\
		"{}\n"\
		"MODS: {}\n\n\n";
	std::string pressedState = event.type == input::EInputEventType::KeyUp ? "Was Released" : "Was Pressed";
	std::string modsStr = (event.mods.hasFlag(input::EInputFlag::ALT)) ? "ALT, " : "";
	if (event.mods.hasFlag(input::EInputFlag::CONTROL))
		modsStr += "CONTROL, ";
	if (event.mods.hasFlag(input::EInputFlag::SHIFT))
		modsStr += "SHIFT, ";

	logging::log(input, event.key.vk, static_cast<char>(event.key.vk), pressedState, modsStr);
}