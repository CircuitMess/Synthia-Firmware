#include <cstdlib>
#include "TrackEditState.h"
#include "../Visualization/LEDStrip.h"
#include "../AudioSystem/PlaybackSystem.h"
#include "../SaveManager.h"
#include "SaveState.h"
#include "PlaybackState.h"

uint8_t TrackEditState::cursor = 0;

TrackEditState::TrackEditState(){
	for(uint8_t i = 0; i < 5; i++){
		setButtonHoldTime(i, 500);
	}

	setButtonHoldTime(BTN_ENC_R, 500);
	setButtonHoldTime(BTN_ENC_L, 500);
}

void TrackEditState::onStart(){
	LEDStrip.setLeft(track.tempo);
	Playback.setVolume(Sliders.getRightPotValue());
	LEDStrip.setRight(Playback.getVolume());
	pushTrackVis();
	trackVis.setMain();

	//TODO - start rgb slotAnim for track rgb leds
	//Synthia.TrackRGB.startAnimation()

	Input::getInstance()->addListener(this);
	Encoders.addListener(this);
	Sliders.addListener(this);
}

void TrackEditState::onStop(){
	Input::getInstance()->removeListener(this);
	Encoders.removeListener(this);
	Sliders.removeListener(this);
	VMan.clearMain();
}

void TrackEditState::setTrack(Track track){
	this->track = track;
	cursor = 0;
}

const Track& TrackEditState::getTrack(){
	return track;
}

void TrackEditState::rightEncMove(int8_t amount){
	for(uint& clickTime : clickTimes){
		clickTime = 0;
	}

	if(cursor == 0 && amount < 0){
		cursor = 15;
	}else{
		cursor = (cursor + amount) % 16;
	}


	for(auto btn : { BTN_1, BTN_2, BTN_3, BTN_4, BTN_5 }){
		uint8_t slot = btnToSlot(btn);

		if(slotEraser[slot]){
			track.timeline.clear(cursor, slot);
		}else if(Input::getInstance()->getButtonState(btn)){
			track.timeline.set(cursor, slot);
		}
	}

	pushTrackVis();
}

void TrackEditState::leftEncMove(int8_t amount){
	auto save = new SaveState(this);
	save->push(this);
}

void TrackEditState::leftPotMove(uint8_t value){
	track.tempo = map(value, 0, 255, 60, 220);
	tempoVis.push(track.tempo);
}

void TrackEditState::rightPotMove(uint8_t value){
	Playback.setVolume(value);
	volumeVis.push(value);
}


void TrackEditState::buttonHeld(uint i){
	int slot = btnToSlot(i);
	if(slot != -1){
		//TODO - open SampleEditState
	}else if(i == BTN_ENC_R){
		track.timeline.clear(cursor);
		pushTrackVis();
	}else if(i == BTN_ENC_L){
		auto state = new PlaybackState(track.timeline, track.tempo);
		state->push(this);
	}
}

void TrackEditState::buttonReleased(uint i){
	int slot = btnToSlot(i);
	if(i == -1) return;
	slotEraser[slot] = false;
}

void TrackEditState::click(uint8_t i){
	int slot = btnToSlot(i);

	if(slot != -1){
		track.timeline.set(cursor, slot);
		pushTrackVis();
	}else if(i == BTN_ENC_R){
		for(int j = 0; j < 5; ++j){
			if(track.timeline.get(cursor, j)){
				Playback.play(j);
			}
		}
		pushTrackVis();
	}
}

void TrackEditState::doubleClick(uint8_t i){
	int slot = btnToSlot(i);

	if(slot == -1) return;

	track.timeline.clear(cursor, slot);
	slotEraser[slot] = true;
	pushTrackVis();
}

void TrackEditState::buttonPressed(uint i){
	int slot = btnToSlot(i);

	if(slot == -1){
		click(i);
		return;
	}

	if(!clickTimes[slot]){
		click(i);
	}else{
		if(clickTimes[slot] + doubleClickInterval >= millis()){
			doubleClick(i);
		}else{
			click(i);
		}
	}
	clickTimes[slot] = millis();

}

void TrackEditState::pushTrackVis(){
	trackVis.push({ track.timeline, cursor });
}

int TrackEditState::btnToSlot(uint8_t i){
	static const std::unordered_map<uint8_t, uint8_t> map = {
			{ BTN_1, 0 },
			{ BTN_2, 1 },
			{ BTN_3, 2 },
			{ BTN_4, 3 },
			{ BTN_5, 4 },
	};

	auto pair = map.find(i);
	if(pair == map.end()) return -1;
	return pair->second;
}
