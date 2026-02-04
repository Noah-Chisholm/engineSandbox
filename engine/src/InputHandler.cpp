#include "InputHandler.h"

using namespace input;

bool inputHandler::dequeueEvent(FInputEvent& event) {
	if (eventsRead != eventsWrite) {
		event = queuedEvents[eventsRead];
		eventsRead = (eventsRead + 1) & (MAXIMUMEVENTS - 1);
		return true;
	}
	return false;
}

bool inputHandler::enqueueEvent(FInputEvent event) {
	uint32_t nextWrite = (eventsWrite + 1) & (MAXIMUMEVENTS - 1);
	if (nextWrite == eventsRead) {
		eventsRead = (eventsRead + 1) & (MAXIMUMEVENTS - 1);
		queuedEvents[eventsWrite] = event;
		eventsWrite = nextWrite;
		return false;
	}
	else {
		queuedEvents[eventsWrite] = event;
		eventsWrite = nextWrite;
		return true;
	}
}

inputHandler& inputHandler::getInstance() {
	static inputHandler inst;
	return inst;
}