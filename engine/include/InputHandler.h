#pragma once
#include <Windows.h>
#include "delegates.h"
#include <vector>
#include <array>
#include "baseObject.h"
#include "input.h"
#include "circularQueue.h"
#include <bitset>

using namespace input;
class inputHandler {
public:
	using inputEventSig = Delegate<const FInputEvent&>;
private:
	queue<FInputEvent, 128, EOverflowResolutions::OVERWRITE_OLDEST> queuedEvents;
	inputHandler() = default;
	~inputHandler() = default;

	static constexpr size_t VK_COUNT = 256;
	static constexpr size_t TYPE_COUNT = static_cast<size_t>(static_cast<uint8_t>(EInputEventType::End) - 1);
	static constexpr size_t MOUSEBTN_COUNT = static_cast<size_t>(static_cast<uint8_t>(EMouseInputTypes::End) - 1);
	
	std::vector<inputEventSig> anyKeyRegistrants;
	std::array<std::vector<inputEventSig>, VK_COUNT> keyRegistrants;
	std::array<std::vector<inputEventSig>, MOUSEBTN_COUNT> mouseButtonRegistrants;
	std::vector<inputEventSig> mouseMoveRegistrants;
	std::vector<inputEventSig> mouseWheelRegistrants;

	std::bitset<VK_COUNT> keyStates; // 1 is key down
	std::bitset<MOUSEBTN_COUNT> mouseStates; // 1 is button down
public:
	bool enqueueEvent(const FInputEvent& event);
	bool dequeueEvent(FInputEvent& event);
	static inputHandler& getInstance();
	void handleAllEvents();
	bool registerForKeyEvent(uint32_t key, inputEventSig callBack);
	bool registerForAnyKeyEvent(inputEventSig callBack);
	bool registerForMouseEvent(EMouseInputTypes event, inputEventSig callBack);
	bool registerForMouseMove(inputEventSig callBack);
	bool registerForMouseWheel(inputEventSig callBack);
};