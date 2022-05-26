#include <Loop/LoopManager.h>
#include "Baker.h"
#include "PlaybackSystem.h"
#include "SlotBaker.h"

Baker::Baker(const std::array<SlotConfig, 5>& configs) : configs(configs){

}

void Baker::start(){
	if(state != WAITING) return;
	state = BAKING;

	for(int i = 0; i < 5; i++){
		slotFiles[i] = RamFile::create();
	}

	editSlot = new EditSlot(configs[0], openSample(configs[0]));
	baker = new SlotBaker(editSlot, slotFiles[0]);

	baker->start();

	ESP_LOGI("Baker", "Start baking all five slots");

	// Preparing is done on core 0
	// Baking is done using SlotBakers which each start a task on core 0 => checking is done on main thread
	// We register with LoopManager before checking starts to avoid registering on another core (LoopManager probably isnt thread safe)
	LoopManager::addListener(this);
}

void Baker::loop(uint micros){
	if(state != BAKING || baker == nullptr){
		LoopManager::removeListener(this);
		return;
	}

	if(baker->isDone()){
		delete baker;
		delete editSlot;

		if(oneDoneCallback){
			oneDoneCallback(baked);
		}

		baked++;
		if(baked == 5){
			baker = nullptr;
			editSlot = nullptr;

			LoopManager::removeListener(this);

			state = DONE;
			return;
		}

		editSlot = new EditSlot(configs[baked], openSample(configs[baked]));
		baker = new SlotBaker(editSlot, slotFiles[baked]);

		baker->start();
	}
}

bool Baker::isBaking(){
	return state == BAKING;
}

bool Baker::isDone(){
	return state == DONE;
}

std::array<File, 5> Baker::getFiles(){
	return slotFiles;
}

std::array<SlotConfig, 5> Baker::getConfigs(){
	return configs;
}

void Baker::setOneDoneCallback(const std::function<void(uint8_t)>& oneDoneCallback){
	Baker::oneDoneCallback = oneDoneCallback;
}
