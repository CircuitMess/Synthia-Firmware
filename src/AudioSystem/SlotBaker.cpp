#include "SlotBaker.h"
#include <Audio/OutputWAV.h>
#include <utility>

SlotBaker::SlotBaker(EditSlot* slot, File outFile) : slot(slot), outFile(std::move(outFile)), output(outFile)
													  task("SlotBaker", [](Task* task){ static_cast<SlotBaker*>(task->arg)->bake(); }, 8192, this){
	output.setSource(&slot->getGenerator());
}

void SlotBaker::start(){
	if(state != WAITING) return;
	state = WORKING;

	task.start(1, 0);
}

bool SlotBaker::isDone(){
	return state == DONE;
}

void SlotBaker::bake(){
	ESP_LOGI("SlotBaker", "Start bake");
	uint32_t time = millis();

	slot->play();
	output.start();
	while(output.isRunning()){
		output.loop(0);
	}

	ESP_LOGI("SlotBaker", "Bake done in %ld ms", millis() - time);

	state = DONE;
}
