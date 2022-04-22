#include <Loop/LoopManager.h>
#include "Baker.h"
#include "PlaybackSystem.h"
#include "SlotBaker.h"

Baker::Baker(const std::array<SlotConfig, 5>& configs) : configs(configs),
												   task("SlotBaker", [](Task* task){ static_cast<Baker*>(task->arg)->prepare();}, 8192, this){


}

void Baker::start(){
	if(state != WAITING) return;
	state = PREPARING;


	ESP_LOGI("Baker", "Start baking all five slots");
	task.start(1, 0);

	// Preparing is done on core 0
	// Baking is done using SlotBakers which each start a task on core 0 => checking is done on main thread
	// We register with LoopManager before checking starts to avoid registering on another core (LoopManager probably isnt thread safe)
	LoopManager::addListener(this);
}

void Baker::loop(uint micros){
	if(state == DONE){
		LoopManager::removeListener(this);
		cleanup();
		return;
	}else if(state == PREPARING){
		// see comment above
		return;
	}else if(state != BAKING){
		LoopManager::removeListener(this);
		return;
	}

	if(baked == 5){
		state = DONE;
		cleanup();
		return;
	}

	if(slotBakers[baked]->isDone()){
		baked++;
		if(baked == 5){
			state = DONE;
			LoopManager::removeListener(this);
			cleanup();
			return;
		}
		slotBakers[baked]->start();
	}
}

void Baker::prepare(){

	prepareSamples();

	slotBakers[0]->start();
	state = BAKING;
}

bool Baker::isBaking(){
	return state == BAKING || state == PREPARING;
}

bool Baker::isDone(){
	return state == DONE;
}

void Baker::prepareSamples(){
	//open ramfile and start bake process for all five slots
	for(uint8_t i = 0; i < 5; i++){
		auto config = configs[i];
		auto file = RamFile::create();

		File sampleFile = openSample(config);
		auto editSlot = new EditSlot(config, RamFile::open(sampleFile));
		sampleFile.close();

		auto slotbaker = new SlotBaker(editSlot, file);

		slotFiles[i] = file;
		editSlots[i] = editSlot;
		slotBakers[i] = slotbaker;
	}
}

void Baker::cleanup(){
	for(uint8_t i = 0; i < 5; i++){
		delete editSlots[i];
		delete slotBakers[i];
		editSlots[i] = nullptr;
		slotBakers[i] = nullptr;
	}
}

std::array<File, 5> Baker::getFiles(){
	return slotFiles;
}

std::array<SlotConfig, 5> Baker::getConfigs(){
	return configs;
}
