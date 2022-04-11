#include <cstdlib>
#include "TrackEditState.h"
#include "../Visualization/LEDStrip.h"
#include "../AudioSystem/PlaybackSystem.h"
#include "../SaveManager.h"
#include "SaveState.h"

uint8_t TrackEditState::cursor = 0;

TrackEditState::TrackEditState(){

}

void TrackEditState::onStart(){
	for(uint8_t i = 0; i < 5; i++){
		setButtonHoldTime(i, 800);
	}
	setButtonHoldTime(BTN_ENC_R, 800);
	setButtonHoldTime(BTN_ENC_L, 800);

	LEDStrip.setLeft(track.tempo);
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
	if(i < 5){
		//TODO - open SampleEditState
	}else if(i == BTN_ENC_R){
		track.timeline.clear(cursor);
		pushTrackVis();
	}else if(i == BTN_ENC_L){
		//TODO - open PlaybackState
	}
}

void TrackEditState::click(uint8_t i){
	if(i < 5){
		track.timeline.set(cursor, i);
		pushTrackVis();
	}else if(i == BTN_ENC_R){
		for(int j = 0; j < 5; ++j){
			if(track.timeline.get(cursor, j)){
				Playback.play(j);
			}
		}
		pushTrackVis();
	}else if(i == BTN_ENC_L){
		auto save = new SaveState(this);
		save->push(this);
	}
}

void TrackEditState::doubleClick(uint8_t i){
	if(i >= 5) return;

	track.timeline.clear(cursor, i);
	pushTrackVis();
}

void TrackEditState::buttonPressed(uint i){
	if(i >= 5){
		click(i);
		return;
	}

	if(!clickTimes[i]){
		click(i);
	}else{
		if(clickTimes[i] + doubleClickInterval >= millis()){
			doubleClick(i);
		}else{
			click(i);
		}
	}
	clickTimes[i] = millis();

}

void TrackEditState::pushTrackVis(){
	TrackVisData data;
	data.cursor = cursor;
	data.timeline = track.timeline;

	trackVis.push(data);
}
