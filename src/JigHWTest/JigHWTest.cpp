#include <SPIFFS.h>
#include <Settings.h>
#include "JigHWTest.h"
#include "SPIFFSChecksum.hpp"
#include <Synthia.h>
#include "../AudioSystem/PlaybackSystem.h"
#include <Wire.h>
#include <Devices/Matrix/MatrixAnimGIF.h>
#include "../Visualization/TempoAnim.h"
#include <Loop/LoopManager.h>
#include <Util/HWRevision.h>

static const i2s_config_t i2s_config = {
		.mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX),
		.sample_rate = SAMPLE_RATE,
		.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
		.channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
		.communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
		.intr_alloc_flags = 0,
		.dma_buf_count = 2,
		.dma_buf_len = 512,
		.use_apll = false,
		.tx_desc_auto_clear = true,
		.fixed_mclk = 0
};

JigHWTest* JigHWTest::test = nullptr;

JigHWTest::JigHWTest(){
	test = this;

	tests.push_back({ JigHWTest::PSRAMTest, "PSRAM", [](){}});
	tests.push_back({ JigHWTest::IS31Test, "Charlie", [](){}});
	tests.push_back({ JigHWTest::AWTest, "AW9523", [](){}});
	tests.push_back({ JigHWTest::XL9555Test, "XL9555", [](){}});
	tests.push_back({ JigHWTest::SPIFFSTest, "SPIFFS", [](){}});
	tests.push_back({ JigHWTest::MicTest, "Mic", [](){}});
	tests.push_back({ JigHWTest::hwRevision, "HW rev", [](){}});
}

void JigHWTest::start(){
	Serial.println();
	Serial.printf("TEST:begin:%llx\n", ESP.getEfuseMac());

	Synthia.TrackMatrix.setFont(Matrix::SMALL);

	const auto progress = [&](uint8_t test){
		const float ratio = (float) (test + 1) / (float) tests.size();
		Synthia.CursorMatrix.clear();

		for(int i = 0; i < Synthia.CursorMatrix.getWidth(); i++){
			const float pixel = (float) (i + 1) / (float) Synthia.CursorMatrix.getWidth();
			if(pixel <= ratio){
				Synthia.CursorMatrix.drawPixel(i, MatrixPixel::White);
			}
		}

		Synthia.CursorMatrix.push();
	};

	bool pass = true;
	for(int i = 0; i < tests.size(); i++){
		const Test& singleTest = tests[i];
		currentTest = singleTest.name;

		Serial.printf("TEST:startTest:%s\n", currentTest);

		progress(i);
		bool result = singleTest.test();

		Serial.printf("TEST:endTest:%s\n", result ? "pass" : "fail");

		if(!(pass &= result)){
			if(singleTest.onFail){
				singleTest.onFail();
			}

			break;
		}
	}

	Synthia.clearMatrices();

	if(pass){
		Serial.println("TEST:passall");
		postTestPass();
	}else{
		Serial.printf("TEST:fail:%s\n", currentTest);
		postTestFail();
	}
}

void JigHWTest::postTestFail(){
	Synthia.TrackMatrix.clear();
	Synthia.TrackMatrix.drawString(0, 5, "FAIL");
	Synthia.TrackMatrix.push();

	uint32_t flashTime = 0;
	bool state = true;
	for(;;){
		if(millis() - flashTime < 500) continue;
		flashTime = millis();
		Synthia.SlotRGB.clear(state ? MatrixPixel::Red : MatrixPixel::Off);
		Synthia.TrackRGB.clear(state ? MatrixPixel::Red : MatrixPixel::Off);
		Synthia.SlotRGB.push();
		Synthia.TrackRGB.push();
		state = !state;
	}
}

void JigHWTest::postTestPass(){
	// Audio test
	Playback.begin();
	Playback.setVolume(200);

	// Matrix test
	Synthia.getCharlie().setBrightness(80);

	Synthia.TrackMatrix.startAnimation(new MatrixAnimGIF(SPIFFS.open("/GIF/Swipe.gif")));
	Synthia.SlidersMatrix.startAnimation(new MatrixAnimGIF(SPIFFS.open("/GIF/Intro/Sliders.gif")));

	auto tempo = new TempoAnim(&Synthia.CursorMatrix);
	tempo->setTempo(20);
	Synthia.CursorMatrix.startAnimation(tempo);

	uint32_t blinkTime = 0;
	uint8_t blinkIndex = 0;
	std::array<MatrixPixel, 3> blinkColors = { MatrixPixel::Red, MatrixPixel::Green, MatrixPixel::Blue };

	for(;;){
		LoopManager::loop();

		if(millis() - blinkTime < 500) continue;
		blinkTime = millis();

		blinkIndex = (blinkIndex + 1) % blinkColors.size();

		Synthia.TrackRGB.clear(blinkColors[blinkIndex]);
		Synthia.TrackRGB.push();

		Synthia.SlotRGB.clear(blinkColors[blinkIndex]);
		Synthia.SlotRGB.push();
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
		test->log("response", false);
		return false;
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

bool JigHWTest::hwRevision(){
	const auto rev = HWRevision::get();
	if(rev != 0){
		return rev == CurrentVersion;
	}

	HWRevision::write(CurrentVersion);
	HWRevision::commit();

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

bool JigHWTest::MicTest(){

	esp_err_t err = i2s_driver_install(I2S_NUM_0, &i2s_config, 0, nullptr);
	if(err != ESP_OK){
		test->log("Failed installing I2S driver", (uint32_t) err);
		return false;
	}

	err = i2s_set_pin(I2S_NUM_0, &i2s_pin_config);
	if(err != ESP_OK){
		test->log("Failed setting I2S pins", (uint32_t) err);
		return false;
	}

	int numSamples = 128;
	size_t i2sBufferSize = numSamples * 2 * sizeof(int16_t);
	char* i2sBuffer = static_cast<char*>(malloc(i2sBufferSize));
	int16_t* buffer = static_cast<int16_t*>(malloc(i2sBufferSize / 2));
	memset(buffer, 0, i2sBufferSize / 2);

	size_t readSize;
	// Pre-record
	for(int i = 0; i < 100; i++){
		i2s_read(I2S_NUM_0, (char*) i2sBuffer, i2sBufferSize, &readSize, portMAX_DELAY);
	}

	// Record
	for(int j = 0; j < i2sBufferSize; j += 4){
		int16_t sample = *(int16_t*) (&i2sBuffer[j + 2]);
		buffer[j / 4] = sample;
	}


	err = i2s_driver_uninstall(I2S_NUM_0);
	delay(500);

	if(err != ESP_OK){
		test->log("Failed uninstalling I2S driver", (uint32_t) err);
		return false;
	}
	bool noiseBelowThreshold = true; //Threshold of +/-10 samples
	for(int i = 0; i < numSamples; i++){
		if(buffer[i] < -10 || buffer[i] > 10){
			noiseBelowThreshold = false;
		}

		printf("%d\n", buffer[i]);
		if(buffer[i] == INT16_MIN || buffer[i] == INT16_MAX) return false;
	}

	if(noiseBelowThreshold){
		test->log("No change in mic signal, possible defect", (uint32_t) 0);
		return false;
	}

	return true;
}

bool JigHWTest::AWTest(){
	Wire.begin(Pins.get(Pin::I2C_Sda), Pins.get(Pin::I2C_Scl), 400000);
	Wire1.begin(Pins.get(Pin::I2C2_Sda), Pins.get(Pin::I2C2_Scl), 400000);

	Wire.beginTransmission(0x5B);
	if(Wire.endTransmission() != 0){
		test->log("AW9523 I2C0 response", false);
		return false;
	}

	Wire1.beginTransmission(0x5B);
	if(Wire1.endTransmission() != 0){
		test->log("AW9523 I2C1 response", false);
		return false;
	}

	return true;
}

bool JigHWTest::XL9555Test(){
	Wire1.begin(Pins.get(Pin::I2C2_Sda), Pins.get(Pin::I2C2_Scl), 400000);

	Wire1.beginTransmission(0b0100000);
	if(Wire1.endTransmission() != 0){
		test->log("XL9555 I2C1 response", false);
		return false;
	}

	return true;
}


