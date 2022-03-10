#include <Arduino.h>
#include <CircuitOS.h>
#include <CMAudio.h>

#include <esp_log.h>

void initLog(){
	esp_log_level_set("*", ESP_LOG_NONE);

	const static auto tags = { "Synthia" };

	for(const char* tag : tags){
		esp_log_level_set(tag, ESP_LOG_VERBOSE);
	}
}

//#include <Synthia.h>
//#include <Pins.hpp>
#include <JayD.h>
#include <Input/InputJayD.h>

#include "src/AudioSystem/PlaybackSystem.h"
#include "src/Services/SlotPlayer.h"
#include <Loop/LoopManager.h>

EditSlot* editSlot;
uint8_t intensities[(uint8_t)EffectData::Type::COUNT] = {0};
uint8_t speed;

void setup(){
	Serial.begin(115200);
//	Synthia.begin();
	JayD.begin();
	initLog();

	playbackSystem.init();
	Player.begin();
}

void loop(){
	LoopManager::loop();
}