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
	if(state == PREPARING){
		// see comment above
		return;
	}else if(state != BAKING){
		LoopManager::removeListener(this);
		return;
	}

	bool running = false;
	bool bakersRunning[5] = {true, true, true, true, true};

	for(uint8_t i = 0; i < 5; i++){
		auto slotBaker = slotBakers[i];
		if(slotBaker->isDone() && bakersRunning[i]){
			Playback.set(i, slotFiles[i], configs[i]);
			bakersRunning[i] = false;
		}
		running |= bakersRunning[i];
	}

	if(!running){
		for(uint8_t i = 0; i < 5; i++){
			delete editSlots[i];
			delete slotBakers[i];
		}

		state = DONE;
		LoopManager::removeListener(this);
	}
}

void Baker::prepare(){

	prepareSamples();

	for(int i = 0; i < 5; i++){
		slotBakers[i]->start();
	}

	state = BAKING;
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
