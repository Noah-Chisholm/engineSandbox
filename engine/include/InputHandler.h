#pragma once
#include <Windows.h>
#include <stdint.h>

namespace input {
	enum EInputEventTypes
	{
		MouseButtonUp,
		MouseButtonDown,
		MouseMove,
		MouseWheel,
		KeyUp,
		KeyDown
	};

	enum EModifierFlags {
		SHIFT = 0x0002,
		CONTROL = 0x0004,
		ALT = 0x0008,
	};

	struct FInputEvent
	{
		EInputEventTypes eventType = EInputEventTypes::KeyUp;
		uint32_t keyCode = 0;
		uint32_t mouseX = 0;
		uint32_t mouseY = 0;
		uint8_t flags = 0;
		FInputEvent();
	};

#define MAXIMUMEVENTS 128
static_assert((MAXIMUMEVENTS& (MAXIMUMEVENTS - 1)) == 0, "MAXIMUMEVENTS must be power of two");
};

using namespace input;
class inputHandler {
private:
	FInputEvent queuedEvents[MAXIMUMEVENTS];
	uint32_t eventsWrite = 0;
	uint32_t eventsRead = 0;
	inputHandler() = default;
	~inputHandler() = default;
public:
	bool enqueueEvent(FInputEvent event);
	bool dequeueEvent(FInputEvent& event);
	static inputHandler& getInstance();
};