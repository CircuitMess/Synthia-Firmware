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
		auto file = RamFile::open(nullptr, 0, false);
		auto slotbaker = new SlotBaker(editSlot, file);

		slotBakers[i] = slotbaker;
		slotFiles[i] = file;

		slotbaker->start();
	}
	task.start(2, 0);
}

void Baker::bake(){
	ESP_LOGI("Baker", "Start baking all five slots");
	uint32_t time = millis();
	bool running = true;

	while(running){
		running = false;

		for(uint8_t i = 0; i < 5; i++){
			auto baker = slotBakers[i];
			if(baker->isDone()){
				Playback.set(i, slotFiles[i]);
			}
			running |= baker->isDone();
		}
	}

	ESP_LOGI("Baker", "All baked in %ld ms", millis() - time);

	state = DONE;
}

bool Baker::isDone(){
	return state == DONE;
}
