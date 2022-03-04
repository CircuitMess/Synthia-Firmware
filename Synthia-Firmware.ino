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

	ps = new PlaybackSystem();
	ps->init();

	SlotConfig config;
	config.speed = 127;
	config.sample.sample = Sample::SampleType::KICK;
	config.slotIndex = 0;
	config.effects[4].intensity = 255;
	editSlot = ps->edit(0, config);

/*	Input::getInstance()->setBtnPressCallback(BTN_1, [](){ps->play(0);});
	Input::getInstance()->setBtnPressCallback(BTN_2, [](){ps->play(1);});
	Input::getInstance()->setBtnPressCallback(BTN_3, [](){ps->play(2);});
	Input::getInstance()->setBtnPressCallback(BTN_4, [](){ps->play(3);});
	Input::getInstance()->setBtnPressCallback(BTN_5, [](){ps->play(4);});*/
	auto input = InputJayD::getInstance();


	input->setBtnPressCallback(BTN_L1, [](){ps->play(0);});
	input->setBtnPressCallback(BTN_L2, [](){ps->play(1);});
	input->setBtnPressCallback(BTN_L3, [](){ps->play(2);});
	input->setBtnPressCallback(BTN_R1, [](){ps->play(3);});
	input->setBtnPressCallback(BTN_R2, [](){ps->play(4);});

	input->setEncoderMovedCallback(ENC_L1, [](int8_t val){
		uint8_t index = 4;
		int temp = intensities[index] + val*17;
		temp = min(temp, 255);
		temp = max(temp, 0);
		intensities[index] = temp;
		Serial.println(temp);
		editSlot->setEffect(EffectData::Type::VOLUME, intensities[index]);
	});

	input->setEncoderMovedCallback(ENC_L2, [](int8_t val){
		uint8_t index = 4;
		int temp = speed + val*17;
		temp = min(temp, 255);
		temp = max(temp, 0);
		speed = temp;
		Serial.println(temp);
		editSlot->setSpeed(speed);
	});

	input->setEncoderMovedCallback(ENC_L3, [](int8_t val){
		uint8_t index = 3;
		int temp = intensities[index] + val*17;
		temp = min(temp, 255);
		temp = max(temp, 0);
		intensities[index] = temp;
		Serial.println(temp);
		editSlot->setEffect(EffectData::Type::BITCRUSHER, intensities[index]);
	});
}

void loop(){
	LoopManager::loop();
}