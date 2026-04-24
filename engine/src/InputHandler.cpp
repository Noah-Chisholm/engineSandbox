#include "InputHandler.h"

using namespace input;

bool inputHandler::dequeueEvent(FInputEvent& event) {
	return queuedEvents.dequeue(event);
}

bool inputHandler::enqueueEvent(const FInputEvent& event) {
	return queuedEvents.enqueue(event);
}

inputHandler& inputHandler::getInstance() {
	static inputHandler inst;
	return inst;
}

void inputHandler::handleAllEvents() {
	FInputEvent eventToHandle{};
	while (dequeueEvent(eventToHandle)) {
		switch (eventToHandle.type)
		{
		case input::EInputEventType::Beg:
			break;
		case input::EInputEventType::KeyDown: {
			auto oldState = keyStates[eventToHandle.key.vk];
			if (oldState == true) {
				break;
			}
			else {
				keyStates[eventToHandle.key.vk] = true;
				uint8_t vk = static_cast<uint8_t>(eventToHandle.key.vk);
				auto& eventsToCall = keyRegistrants[vk];

				for (auto it = eventsToCall.begin(); it != eventsToCall.end(); it++) {
					(*it)(eventToHandle);
				}
				for (auto it = anyKeyRegistrants.begin(); it != anyKeyRegistrants.end(); it++) {
					(*it)(eventToHandle);
				}
				break;
			}
		}
		case input::EInputEventType::KeyUp: {
			auto oldState = keyStates[eventToHandle.key.vk];
			if (oldState == false) {
				break;
			} else {
				keyStates[eventToHandle.key.vk] = false;
				uint8_t vk = static_cast<uint8_t>(eventToHandle.key.vk);
				auto& eventsToCall = keyRegistrants[vk];

				for (auto it = eventsToCall.begin(); it != eventsToCall.end(); it++) {
					(*it)(eventToHandle);
				}
				for (auto it = anyKeyRegistrants.begin(); it != anyKeyRegistrants.end(); it++) {
					(*it)(eventToHandle);
				}
				break;
			}
		}
		case input::EInputEventType::MouseDown: {
			auto oldState = mouseStates[static_cast<uint8_t>(eventToHandle.mouseBtn.button)];
			if (oldState == true) {
				break;
			}
			else {
				mouseStates[static_cast<uint8_t>(eventToHandle.mouseBtn.button)] = true;
				uint8_t mouseEventIndex = static_cast<uint8_t>(eventToHandle.mouseBtn.button);
				auto& eventsToCall = mouseButtonRegistrants[mouseEventIndex];

				for (auto it = eventsToCall.begin(); it != eventsToCall.end(); it++) {
					(*it)(eventToHandle);
				}
				break;
			}
		}
		case input::EInputEventType::MouseUp: {
			auto oldState = mouseStates[static_cast<uint8_t>(eventToHandle.mouseBtn.button)];
			if (oldState == false) {
				break;
			}
			else {
				mouseStates[static_cast<uint8_t>(eventToHandle.mouseBtn.button)] = false;
				uint8_t mouseEventIndex = static_cast<uint8_t>(eventToHandle.mouseBtn.button);
				auto& eventsToCall = mouseButtonRegistrants[mouseEventIndex];

				for (auto it = eventsToCall.begin(); it != eventsToCall.end(); it++) {
					(*it)(eventToHandle);
				}
				break;
			}
		}
		case input::EInputEventType::MouseMove: {
			for (auto it = mouseMoveRegistrants.begin(); it != mouseMoveRegistrants.end(); it++) {
				(*it)(eventToHandle);
			}
			break;
		}
		case input::EInputEventType::MouseWheel: {
			for (auto it = mouseWheelRegistrants.begin(); it != mouseWheelRegistrants.end(); it++) {
				(*it)(eventToHandle);
			}
			break;
		}
		case input::EInputEventType::End:
			break;
		default:
			break;
		}
	}
}

bool inputHandler::registerForKeyEvent(uint32_t key, inputEventSig callBack) {
	keyRegistrants[key].push_back(callBack);
	return true;
}

bool inputHandler::registerForAnyKeyEvent(inputEventSig callBack) {
	anyKeyRegistrants.push_back(callBack);
	return true;
}

bool inputHandler::registerForMouseEvent(EMouseInputTypes event, inputEventSig callBack) {
	mouseButtonRegistrants[static_cast<uint8_t>(event)].push_back(callBack);
	return true;
}

bool inputHandler::registerForMouseMove(inputEventSig callBack) {
	mouseMoveRegistrants.push_back(callBack);
	return true;
}

bool inputHandler::registerForMouseWheel(inputEventSig callBack) {
	mouseWheelRegistrants.push_back(callBack);
	return true;
}