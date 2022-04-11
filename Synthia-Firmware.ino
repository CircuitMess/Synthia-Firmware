#include <Arduino.h>
#include <Synthia.h>
#include <CircuitOS.h>
#include <CMAudio.h>
#include <Loop/LoopManager.h>
#include <esp_log.h>

#include "src/AudioSystem/PlaybackSystem.h"
#include "src/Services/SlotPlayer.h"
#include "src/Visualization/Visualizer.h"
#include "src/Visualization/RGBController.h"
#include "src/States/State.h"
#include "src/States/Intro.h"

void initLog(){
	esp_log_level_set("*", ESP_LOG_NONE);

	const static auto tags = { "Synthia" };

	for(const char* tag : tags){
		esp_log_level_set(tag, ESP_LOG_VERBOSE);
	}
}

void setup(){
	Serial.begin(115200);
	initLog();
	Synthia.begin();

	Playback.begin();
	Player.begin();
	VMan.begin();

	RGBTrack.begin(&Synthia.TrackRGB);
	RGBSlot.begin(&Synthia.SlotRGB);

	State* state = new Intro();
	state->start();
}

void loop(){
	LoopManager::loop();
}