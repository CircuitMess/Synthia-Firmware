#include <cstdlib>
#include "SaveState.h"
#include "../SaveManager.h"
#include "../AudioSystem/Baker.h"
#include "../AudioSystem/PlaybackSystem.h"
#include <Loop/LoopManager.h>

uint8_t SaveState::currentSaveSlot = 0;

SaveState::SaveState(TrackEditState* trackEdit) : State(trackEdit), trackEdit(trackEdit){

}

void SaveState::loop(uint micros){
	if(step != SlotSelect) return;

	inactiveTimer += micros;
	if(inactiveTimer >= inactiveTimeout * 1000){
		ESP_LOGI("SaveState", "inactivity timeout");
		pop();
	}
}

void SaveState::onStart(){
	step = SlotSelect;
	visualizer.push({step, currentSaveSlot});
	selection = currentSaveSlot;
	Encoders.addListener(this);
	Sliders.addListener(this);
	Input::getInstance()->addListener(this);
	LoopManager::addListener(this);
	inactiveTimer = 0;
}

void SaveState::onStop(){
	Synthia.TrackMatrix.stopAnimation();
	Synthia.TrackMatrix.clear();
	Synthia.TrackMatrix.push();
	Synthia.CursorMatrix.clear();
	Synthia.CursorMatrix.push();

	Encoders.removeListener(this);
	Sliders.removeListener(this);
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);

}

void SaveState::save(){
	ESP_LOGI("SaveState", "saving...");

	currentSaveSlot = selectedSlot;
	SaveData data;
	data.track = trackEdit->getTrack();
	for(uint8_t i = 0; i < 5; ++i){
		data.slots[i] = Playback.getConfig(i);
	}
	saveManager.store(selectedSlot, data);
	pop();
}

void SaveState::load(){
	ESP_LOGI("SaveState", "loading...");

	currentSaveSlot = selectedSlot;
	SaveData data = saveManager.load(selectedSlot);
	std::array<SlotConfig, 5> slotconfigs;
	std::copy(std::begin(data.slots), std::end(data.slots), slotconfigs.begin());

	Baker baker(slotconfigs);
	baker.start();
	while(!baker.isDone());


	trackEdit->setTrack(data.track);
	pop();
}

void SaveState::rightEncMove(int8_t amount){
	inactiveTimer = 0;
	if(step == SlotSelect){
		if((amount < 0 && selection == 0) || (amount > 0 && selection == 9)) return;

		selection += amount;
	}else if(step == ActionSelect || step == Confirmation){
		if((amount < 0 && !selection) || (amount > 0 && selection)) return;
		selection = !selection;
	}
	visualizer.push({step, selection});
}

void SaveState::buttonPressed(uint i){
	if(i == BTN_ENC_R){
		inactiveTimer = 0;
		switch(step){
			case SlotSelect:
				step = ActionSelect;
				selectedSlot = selection;
				selection = 0;
				Synthia.TrackMatrix.clear();
				visualizer.push({step, selection});
				break;

			case ActionSelect:
				step = Confirmation;
				selectedAction = (selection ? Save : Load);
				selection = 0;
				visualizer.push({step, selection});
				break;

			case Confirmation:
				if(selection){
					selectedAction ? save() : load();
				}else{
					pop();
				}
				break;
		}
	}else{
		pop(); //exits on any other inputs
	}
}

void SaveState::leftEncMove(int8_t amount){
	ESP_LOGI("SaveState", "pop L enc : %d", amount);

	pop();
}

void SaveState::leftPotMove(uint8_t value){
	ESP_LOGI("SaveState", "pop L pot");

	pop();
}

void SaveState::rightPotMove(uint8_t value){
	ESP_LOGI("SaveState", "pop R pot");

	pop();
}



