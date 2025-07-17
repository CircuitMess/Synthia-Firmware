#include <Arduino.h>
#include <Synthia.h>
#include <CircuitOS.h>
#include <CMAudio.h>
#include <Util/HWRevision.h>
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
	char buf[7];
	int wp = 0;

	uint32_t start = millis();
	int c;
	while(millis() - start < 500){
		vTaskDelay(1);
		c = getchar();
		if(c == EOF) continue;
		buf[wp] = (char) c;
		wp = (wp + 1) % 7;

		for(int i = 0; i < 7; i++){
			int match = 0;
			static const char* target = "JIGTEST";

			for(int j = 0; j < 7; j++){
				match += buf[(i + j) % 7] == target[j];
			}

			if(match == 7) return true;
		}
	}

	return false;
}

void setup(){
	Serial.begin(115200);
	initLog();

	if(checkJig()){
		Synthia.initVer(1);
		Settings.reset();
		Synthia.begin();

		JigHWTest* test = new JigHWTest();
		test->start();

		for(;;);
	}else{
		printf("Hello\n");
	}

	Synthia.begin();

	RGBTrack.begin(&Synthia.TrackRGB);
	RGBSlot.begin(&Synthia.SlotRGB);

	if(!Settings.get().tested){
		if(HWRevision::get() > 0){
			Settings.get().tested = true;
			Settings.store();
		}else{
			auto test = new UserHWTest::Test();
			test->start();
			return;
		}
	}

	Playback.begin();
	Player.begin();
	//Player will be enabled after Intro finishes
	Player.disable();
	VMan.begin();
	saveManager.begin();

	State* state = new Intro();
	state->start();
}

void loop(){
	LoopManager::loop();
}

