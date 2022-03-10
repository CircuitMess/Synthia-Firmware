#include <unordered_map>
#include "SlotPlayer.h"
#include "../AudioSystem/PlaybackSystem.h"
#include <Synthia.h>

SlotPlayer Player;

void SlotPlayer::begin(){
	enable();
}

void SlotPlayer::buttonPressed(uint id){
	if(!enabled){
		Synthia.getInput()->removeListener(this);
		return;
	}

	static const std::unordered_map<uint8_t, uint8_t> KeyMap = {
			{ BTN_1, 0 },
			{ BTN_2, 1 },
			{ BTN_3, 2 },
			{ BTN_4, 3 },
			{ BTN_5, 4 },
	};

	auto pair = KeyMap.find(id);
	if(pair == KeyMap.end()) return;

	playOnInput(pair->second);
}

void SlotPlayer::enable(){
	enabled = true;
	Synthia.getInput()->addListener(this);
}

void SlotPlayer::disable(){
	enabled = false;
	Synthia.getInput()->removeListener(this);
}

void SlotPlayer::play(uint8_t slot){
	Playback.play(slot);
	//TODO - add RGBSlot blue blink
}

void SlotPlayer::playOnInput(uint8_t slot){
	Playback.play(slot);
	//TODO - add RGBSlot green blink
}
