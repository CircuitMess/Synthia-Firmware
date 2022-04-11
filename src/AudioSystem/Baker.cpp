#include "Baker.h"
#include "PlaybackSystem.h"
#include "SlotBaker.h"

Baker::Baker(std::array<SlotConfig, 5>& configs) : configs(configs),
												   task("SlotBaker", [](Task* task){static_cast<Baker*>(task->arg)->bake();}, 8192, this){


}

void Baker::start(){
	if(state != WAITING) return;
	state = WORKING;

	//open ramfile and start bake process for all five slots
	for(uint8_t i = 0; i < 5; i++){
		auto config = configs[i];
		auto editSlot = new EditSlot(config);
		auto file = RamFile::create();
		auto slotbaker = new SlotBaker(editSlot, file);

		slotFiles[i] = file;
		editSlots[i] = editSlot;
		slotBakers[i] = slotbaker;

		slotbaker->start();
	}
	ESP_LOGI("Baker", "Start baking all five slots");
	task.start(0, 0);
}

void Baker::bake(){
	uint32_t time = millis();
	bool running = true;
	bool bakersRunning[5] = {true, true, true, true, true};

	while(running){
		running = false;

		for(uint8_t i = 0; i < 5; i++){
			auto slotBaker = slotBakers[i];
			if(slotBaker->isDone() && bakersRunning[i]){
				Playback.set(i, slotFiles[i], configs[i]);
				bakersRunning[i] = false;
			}
			running |= bakersRunning[i];
		}
	}
	ESP_LOGI("Baker", "All baked in %ld ms", millis() - time);
	for(uint8_t i = 0; i < 5; i++){
		delete editSlots[i];
		delete slotBakers[i];
	}


	state = DONE;
}

bool Baker::isDone(){
	return state == DONE;
}
