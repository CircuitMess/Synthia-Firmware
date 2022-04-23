#include <cstdlib>
#include "SaveState.h"
#include "../AudioSystem/Baker.h"
#include "../AudioSystem/PlaybackSystem.h"
#include <Loop/LoopManager.h>
#include "../Visualization/LEDStrip.h"

uint8_t SaveState::currentSaveSlot = 0;
static const char* TAG = "SaveState";

SaveState::SaveState(TrackEditState* trackEdit) : State(trackEdit), trackEdit(trackEdit){

}

void SaveState::loop(uint micros){
	if(step == Wait){
		waitFillTime += micros;
		if(waitFillTime >= waitFillInterval){
			waitFillTime = 0;
			waitFill++;
			LEDStrip.setMidFill(waitFill);
		}
		if(selectedAction == SaveAction::Save && saveTask){
			if(saveTask->isStopped()){
				pop();
			}
		}else if(selectedAction == SaveAction::Load && baker){
			if(baker->isDone()){
				auto files = baker->getFiles();
				for(int i = 0; i < 5; i++){
					Playback.set(i, files[i], saveData->slots[i]);
				}
				trackEdit->setTrack(saveData->track);
				pop();
			}
		}
	}

	if(step != SlotSelect) return;

	inactiveTimer += micros;
	if(inactiveTimer >= inactiveTimeout * 1000){
		ESP_LOGI(TAG, "inactivity timeout");
		pop();
	}
}

void SaveState::onStart(){
	step = SlotSelect;
	selection = currentSaveSlot;
	inactiveTimer = 0;

	visualizer.push({ step, currentSaveSlot });

	Encoders.addListener(this);
	Sliders.addListener(this);
	Input::getInstance()->addListener(this);
	LoopManager::addListener(this);
}

void SaveState::onStop(){
	Encoders.removeListener(this);
	Sliders.removeListener(this);
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);
}

void SaveState::save(){
	step = Wait;
	ESP_LOGI(TAG, "saving...");

	currentSaveSlot = selectedSlot;

	visualizer.push({ step, 0 });

	auto data = new SaveData;
	data->track = trackEdit->getTrack();
	for(uint8_t i = 0; i < 5; ++i){
		data->slots[i] = Playback.getConfig(i);
	}

	Encoders.removeListener(this);
	Sliders.removeListener(this);
	Input::getInstance()->removeListener(this);

	saveTask = std::unique_ptr<Task>(new Task("saveTask", [](Task* t){
		auto data = (SaveData*)t->arg;
		saveManager.store(SaveState::currentSaveSlot, *data);
		ESP_LOGI(TAG, "stored");
		delete data;
		ESP_LOGI(TAG, "temp data deleted");
	}, 8192, (void*)data));
	saveTask->start(1, 0);
}

void SaveState::load(){
	uint32_t m = millis();
	step = Wait;
	ESP_LOGI(TAG, "loading...");

	currentSaveSlot = selectedSlot;
	visualizer.push({ step, 1 });

	saveData = std::unique_ptr<SaveData>(new SaveData(saveManager.load(selectedSlot)));

	Encoders.removeListener(this);
	Sliders.removeListener(this);
	Input::getInstance()->removeListener(this);

	std::array<SlotConfig, 5> slotconfigs;
	std::copy(std::begin(saveData->slots), std::end(saveData->slots), slotconfigs.begin());

	m = millis() - m;
	baker = std::unique_ptr<Baker>(new Baker(slotconfigs));
	baker->start();
	Serial.printf("load time : %d\n", m);
}

void SaveState::leftEncMove(int8_t amount){
	inactiveTimer = 0;
	if(step == SlotSelect){
		if((amount < 0 && selection == 0) || (amount > 0 && selection == 9)) return;

		selection += amount;
	}else if(step == ActionSelect || step == Confirmation){
		if((amount < 0 && !selection) || (amount > 0 && selection)) return;
		selection = !selection;
	}
	visualizer.push({ step, selection });
}

void SaveState::buttonPressed(uint i){
	if(i != BTN_ENC_L){
		pop(); //exits on any other inputs
		return;
	}

	inactiveTimer = 0;
	switch(step){
		case SlotSelect:
			step = ActionSelect;
			selectedSlot = selection;
			selection = 0;
			Synthia.TrackMatrix.clear();
			visualizer.push({ step, selection });
			break;

		case ActionSelect:
			step = Confirmation;
			selectedAction = (selection ? SaveAction::Load : SaveAction::Save);
			selection = 0;
			visualizer.push({ step, selection });
			break;

		case Confirmation:
			if(selection){
				switch(selectedAction){
					case SaveAction::Save:
						save();
						break;
					case SaveAction::Load:
						load();
						break;
				}
			}else{
				pop();
			}
			break;

		default:
			break;
	}
}

void SaveState::rightEncMove(int8_t amount){
	ESP_LOGI(TAG, "pop R enc : %d", amount);

	pop();
}

void SaveState::leftPotMove(uint8_t value){
	ESP_LOGI(TAG, "pop L pot");

	pop();
}

void SaveState::rightPotMove(uint8_t value){
	ESP_LOGI(TAG, "pop R pot");

	pop();
}



