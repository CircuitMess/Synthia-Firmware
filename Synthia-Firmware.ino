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
void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
	Serial.printf("Listing directory: %s\n", dirname);

	File root = fs.open(dirname);
	if(!root){
		Serial.println("Failed to open directory");
		return;
	}
	if(!root.isDirectory()){
		Serial.println("Not a directory");
		return;
	}

	File file = root.openNextFile();
	while(file){
		if(file.isDirectory()){
			Serial.print("  DIR : ");
			Serial.println(file.name());
			if(levels){
				listDir(fs, file.name(), levels -1);
			}
		} else {
			Serial.print("  FILE: ");
			Serial.print(file.name());
			Serial.print("  SIZE: ");
			Serial.println(file.size());
		}
		file = root.openNextFile();
	}
}

#include "src/AudioSystem/PlaybackSystem.h"
#include "src/Services/SlotPlayer.h"
#include <Loop/LoopManager.h>
PlaybackSystem* ps;
EditSlot* editSlot;
uint8_t intensities[(uint8_t)EffectData::Type::COUNT] = {0};
uint8_t speed;

void setup(){
	Serial.begin(115200);
//	Synthia.begin();
	JayD.begin();
	initLog();

	Playback.begin();
	Player.begin();
}

void loop(){
	LoopManager::loop();
}