#include <Arduino.h>
#include <Synthia.h>
#include <CircuitOS.h>
#include <CMAudio.h>
#include <Loop/LoopManager.h>
#include <esp_log.h>

#include "src/JigHWTest/JigHWTest.h"
#include "src/AudioSystem/PlaybackSystem.h"
#include "src/Services/SlotPlayer.h"
#include "src/Visualization/Visualizer.h"
#include "src/Visualization/RGBController.h"
#include "src/States/State.h"
#include "src/States/Intro.h"
#include "src/UserHWTest/Test.h"
#include "src/SaveManager.h"

void initLog(){
	esp_log_level_set("*", ESP_LOG_NONE);

	const static auto tags = { "Synthia" };

	for(const char* tag : tags){
		esp_log_level_set(tag, ESP_LOG_VERBOSE);
	}
}

bool checkJig(){
	Wire.beginTransmission(0x51);
	return Wire.endTransmission() == 0;
}

void setup(){
	Serial.begin(115200);
	initLog();
	Synthia.begin();

	if(checkJig()){
		JigHWTest* test = new JigHWTest();
		test->start();

		for(;;);
	}

	RGBTrack.begin(&Synthia.TrackRGB);
	RGBSlot.begin(&Synthia.SlotRGB);

	if(!Settings.get().tested){
		auto test = new UserHWTest::Test();
		test->start();
		return;
	}

	Playback.begin();
	Player.begin();
	VMan.begin();
	saveManager.begin();

	LoopManager::removeListener(Synthia.getInput());

	State* state = new Intro();
	state->start();
}

void loop(){
	LoopManager::loop();
}