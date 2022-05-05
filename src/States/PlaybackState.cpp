#include <Synthia.h>
#include <Loop/LoopManager.h>
#include "PlaybackState.h"
#include "../Services/SlotPlayer.h"
#include "../Visualization/RGBController.h"
#include "../AudioSystem/PlaybackSystem.h"
#include "../Visualization/LEDStrip.h"

PlaybackState::PlaybackState(const TrackTimeline& timeline, uint8_t& tempo) : timeline(timeline), tempo(tempo){
	setButtonHoldTime(BTN_ENC_L, 500);
	for(int i = 0; i < 5; i++){
		setButtonHoldTime(Synthia.slotToBtn(i), 500);
	}
}

void PlaybackState::onStart(){
	vis.setMain();
	pushTrackVis();

	LoopManager::addListener(this);
	Input::getInstance()->addListener(this);
	Sliders.addListener(this);
	Encoders.addListener(this);

	lastStepTime = micros();
	stepDuration = 60000000 / (tempo * 4);
	cursor = 0;

	LEDStrip.setLeft(map(tempo, 60, 220, 0, 255));
	LEDStrip.setRight(Playback.getVolume());

	playStep(0);
	pushTrackVis();
}

void PlaybackState::onStop(){
	VMan.clearMain();
	LoopManager::removeListener(this);
	Input::getInstance()->removeListener(this);
	Sliders.removeListener(this);
	Encoders.removeListener(this);
}

void PlaybackState::pushTrackVis(){
	vis.push({ timeline, cursor });
}

void PlaybackState::loop(uint t){
	if(micros() - lastStepTime < stepDuration) return;

	cursor = (cursor + 1) % 16;
	lastStepTime = micros();

	playStep(cursor);
	pushTrackVis();
}

void PlaybackState::playStep(uint8_t step){
	for(int i = 0; i < 5; i++){
		if(timeline.get(step, i) && !disabledSlots[i]){
			Player.play(i);
		}
	}
}

void PlaybackState::buttonHeld(uint i){
	if(i == BTN_ENC_L){
		pop();
		return;
	}else if(i != BTN_ENC_R){
		int slot = Synthia.btnToSlot(i);
		if(slot == -1) return;
		disabledSlots[slot] = !disabledSlots[slot];

		if(disabledSlots[slot]){
			RGBSlot.setColor(slot, MatrixPixel::Red);
			RGBTrack.setColor(slot, MatrixPixel::Red);
		}else{
			RGBSlot.setColor(slot, MatrixPixel::Off);
			RGBTrack.setColor(slot, MatrixPixel::Off);
		}
	}
}

void PlaybackState::leftPotMove(uint8_t value){
	tempo = map(value, 0, 255, 60, 220);
	stepDuration = 60000000 / (tempo * 4);
	LEDStrip.setLeft(map(tempo, 60, 220, 0, 255));
}

void PlaybackState::rightPotMove(uint8_t value){
	Playback.setVolume(value);
	LEDStrip.setRight(value);
}

void PlaybackState::rightEncMove(int8_t amount){
	if(cursor == 0 && amount < 0){
		cursor = 16;
	}

	cursor = (cursor + amount) % 16;

	lastStepTime = micros();
	playStep(cursor);
	pushTrackVis();
}
