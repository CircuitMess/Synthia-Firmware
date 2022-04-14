#include <SPIFFS.h>
#include <Settings.h>
#include "JigHWTest.h"
#include "SPIFFSChecksum.hpp"
#include <Pins.hpp>
#include "../AudioSystem/PlaybackSystem.h"
#include <Wire.h>

JigHWTest *JigHWTest::test = nullptr;

JigHWTest::JigHWTest(){
	test = this;

	tests.push_back({ JigHWTest::PSRAMTest, "LoRa", [](){}});
	tests.push_back({ JigHWTest::IS31Test, "Battery", [](){}});
	tests.push_back({JigHWTest::SPIFFSTest, "SPIFFS", [](){ }});
}

void JigHWTest::start(){
	Serial.println();
	Serial.printf("TEST:begin:%llx\n", ESP.getEfuseMac());

	bool pass = true;
	for(const Test &test : tests){
		currentTest = test.name;

		Serial.printf("TEST:startTest:%s\n", currentTest);

		bool result = test.test();

		Serial.printf("TEST:endTest:%s\n", result ? "pass" : "fail");

		if(!(pass &= result)){
			if(test.onFail){
				test.onFail();
			}

			break;
		}
	}

	if(!pass){
		Serial.printf("TEST:fail:%s\n", currentTest);
		for(;;);
	}

	Serial.println("TEST:passall");

	Playback.set(0, SPIFFS.open("/Samples/intro.wav"), SlotConfig{});
	Playback.play(0);

	for(;;){
/*		LED.setRGB(LEDColor::RED);
		delay(500);
		LED.setRGB(LEDColor::GREEN);
		delay(500);
		LED.setRGB(LEDColor::BLUE);
		delay(500);*/
	}
}

void JigHWTest::log(const char* property, const char* value){
	Serial.printf("%s:%s:%s\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, float value){
	Serial.printf("%s:%s:%f\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, double value){
	Serial.printf("%s:%s:%lf\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, bool value){
	Serial.printf("%s:%s:%s\n", currentTest, property, value ? "TRUE" : "FALSE");
}

void JigHWTest::log(const char* property, uint32_t value){
	Serial.printf("%s:%s:%lu\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, const String& value){
	Serial.printf("%s:%s:%s\n", currentTest, property, value.c_str());
}

bool JigHWTest::PSRAMTest(){
	if(!(psramFound() && psramInit())) return false;

	uint32_t free = ESP.getFreePsram();

	if(free != PSRAMSize){
		test->log("free", free);
		return false;
	}

	return true;
}

bool JigHWTest::IS31Test(){
	Wire.beginTransmission(0x74);

	if(Wire.endTransmission() != 0){
		test->log("responding", false);
		return;
	}
	return true;
}

bool JigHWTest::SPIFFSTest(){
	for(const auto& f : SPIFFSChecksums){
		if(!SPIFFS.exists(f.name)){
			test->log("missing", f.name);
			return false;
		}

		fs::File file = SPIFFS.open(f.name, "r");
		uint32_t sum = calcChecksum(file);
		file.close();

		if(sum != f.sum){
			test->log("file", f.name);
			test->log("expected", (uint32_t) f.sum);
			test->log("got", (uint32_t) sum);

			return false;
		}
	}

	return true;
}

uint32_t JigHWTest::calcChecksum(File& file){
	if(!file) return 0;

#define READ_SIZE 512

	uint32_t sum = 0;
	uint8_t b[READ_SIZE];
	size_t read = 0;
	while(read = file.read(b, READ_SIZE)){
		for(int i = 0; i < read; i++){
			sum += b[i];
		}
	}

	return sum;
}


