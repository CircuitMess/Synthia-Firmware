#include <cstdlib>
#include "SaveState.h"
#include "../AudioSystem/Baker.h"
#include "../AudioSystem/PlaybackSystem.h"
#include <Loop/LoopManager.h>
#include "../Visualization/LEDStrip.h"
#include "../Services/SlotPlayer.h"

uint8_t SaveState::currentSaveSlot = 0;
static const char* TAG = "SaveState";
SaveData SaveState::saveData;

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

		if(selectedAction == SaveAction::Save && myTask){
			if(myTask->isStopped()){
				pop();
			}
		}else if(selectedAction == SaveAction::Load && myTask){
			if(myTask->isStopped() && baker){
				if(!baker->isDone() && !baker->isBaking()){
					baker->start();
				}else if(baker->isDone()){
					auto files = baker->getFiles();
					auto configs = baker->getConfigs();
					for(int i = 0; i < 5; i++){
						Playback.set(i, files[i], configs[i]);
					}
					delete baker;
					trackEdit->setTrack(saveData.track);
					pop();
				}
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
	Player.enable();
}

void SaveState::save(){
	step = Wait;
	ESP_LOGI(TAG, "saving...");

	currentSaveSlot = selectedSlot;

	visualizer.push({ step, 0 });

	saveData.track = trackEdit->getTrack();
	for(uint8_t i = 0; i < 5; ++i){
		saveData.slots[i] = Playback.getConfig(i);
	}

	Encoders.removeListener(this);
	Sliders.removeListener(this);
	Input::getInstance()->removeListener(this);
	Player.disable();

	myTask = std::unique_ptr<Task>(new Task("saveTask", [](Task* t){
		saveManager.store(SaveState::currentSaveSlot, SaveState::saveData);
		ESP_LOGI(TAG, "stored");
	}, 8192));
	myTask->start(1, 0);
}

void SaveState::load(){
	step = Wait;
	ESP_LOGI(TAG, "loading...");

	currentSaveSlot = selectedSlot;
	visualizer.push({ step, 1 });


	Encoders.removeListener(this);
	Sliders.removeListener(this);
	Input::getInstance()->removeListener(this);
	Player.disable();

	myTask = std::unique_ptr<Task>(new Task("loadTask", [](Task* t){
		auto &data = SaveState::saveData;
		data = saveManager.load(SaveState::currentSaveSlot);
		std::array<SlotConfig, 5> slotconfigs;
		std::copy(std::begin(data.slots), std::end(data.slots), slotconfigs.begin());

		auto baker = new Baker(slotconfigs);
		*((Baker**)t->arg) = baker;
	}, 8192, (void*)&baker));
	myTask->start(1, 0);
}

void SaveState::leftEncMove(int8_t amount){
	inactiveTimer = 0;
	if(step == SlotSelect){
		if((amount < 0 && selection == 0) || (amount > 0 && selection == 3)) return;

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
