#include "BakeSystem.h"
#include <Audio/OutputWAV.h>

BakeSystem SampleBaker;

File BakeSystem::bake(EditSlot* slot){
	File outFile = RamFile::open(nullptr, 0, false);
	OutputWAV output(outFile);
	output.setSource(&slot->getGenerator());

	ESP_LOGI("Synthia", "start bake");
	uint32_t temp = millis();
	slot->play();
	output.start();
	while(output.isRunning()){
		output.loop(0);
	}
	temp = millis() - temp;
	ESP_LOGV("Synthia", "bake done in %d ms", temp);

	return outFile;
}
