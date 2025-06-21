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
	tests.push_back({ JigHWTest::SPIFFSTest, "SPIFFS", [](){}});
	tests.push_back({ JigHWTest::ButtonsTest, "Buttons", [](){}});
	tests.push_back({ JigHWTest::SlidersTest, "Sliders", [](){}});
	tests.push_back({ JigHWTest::EncodersTest, "Encoders", [](){}});
	// tests.push_back({ JigHWTest::MicTest, "Mic", [](){}}); // TODO: uncomment once the test is done
}

void JigHWTest::start(){
	Serial.println();
	Serial.printf("TEST:begin:%llx\n", ESP.getEfuseMac());

	Synthia.TrackMatrix.setFont(Matrix::SMALL);

	bool pass = true;
	for(const Test &singleTest : tests){
		currentTest = singleTest.name;

		Serial.printf("TEST:startTest:%s\n", currentTest);

		bool result = singleTest.test();

		Serial.printf("TEST:endTest:%s\n", result ? "pass" : "fail");

		if(!(pass &= result)){
			if(singleTest.onFail){
				singleTest.onFail();
			}

			break;
		}
	}

	if(!pass){
		Serial.printf("TEST:fail:%s\n", currentTest);
		for(;;);
	}

	Serial.println("TEST:passall");

	postTest();
}

void JigHWTest::postTest(){
	// Disable input
	LoopManager::removeListener(Synthia.getInput());
	LoopManager::removeListener(&Sliders);
	LoopManager::removeListener(&Encoders);

	// Audio test
	/*Playback.begin();
	Playback.setVolume(200);

	Playback.set(0, SPIFFS.open("/Samples/clap.wav"), SlotConfig{});
	Playback.play(0);

	Synthia.TrackMatrix.setBrightness(150);
	Synthia.CursorMatrix.setBrightness(150);
	Synthia.SlidersMatrix.setBrightness(150);*/

	// Matrix test
	Synthia.TrackMatrix.startAnimation(new MatrixAnimGIF(SPIFFS.open("/GIF/Swipe.gif")));
	Synthia.SlidersMatrix.startAnimation(new MatrixAnimGIF(SPIFFS.open("/GIF/Intro/Sliders.gif")));
	Synthia.CursorMatrix.startAnimation(new TempoAnim(&Synthia.CursorMatrix));

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

bool JigHWTest::ButtonsTest(){
	auto input = Synthia.getInput();

	const std::string string = "BUTTONS";
	int32_t textScrollCursor = -Synthia.TrackMatrix.getWidth();
	uint32_t textScrollTime = 0;

	std::vector<bool> pressed(ButtonCount, false);
	std::vector<bool> released(ButtonCount, false);
	uint8_t pressCount = 0;
	uint8_t releaseCount = 0;
	for(;;){
		LoopManager::loop();

		for(int i = 0; i < ButtonCount; i++){
			if(input->getButtonState((int) ((int) Pins.get(Pin::Btn1) + i)) && !pressed[i]){
				printf("Press %d\n\r", i);
				pressed[i] = true;
				pressCount++;
			}else if(!input->getButtonState((int) ((int) Pins.get(Pin::Btn1) + i)) && pressed[i] && !released[i]){
				printf("Release %d\n\r", i);
				released[i] = true;
				releaseCount++;
			}
		}

		if(pressCount == ButtonCount && releaseCount == ButtonCount) break;

		if(millis() - textScrollTime > 100){
			textScrollTime = millis();
			textScrollCursor++;
			if(textScrollCursor >= (int16_t) (string.size() * (3 + 1) - 1)){
				textScrollCursor = -Synthia.TrackMatrix.getWidth();
			}

			Synthia.TrackMatrix.clear();
			Synthia.TrackMatrix.drawString(-textScrollCursor, 5, string.c_str());
			Synthia.TrackMatrix.push();
		}

		Synthia.SlotRGB.clear();
		for(int i = 0; i < 5; i++){
			if(input->getButtonState((int) ((int) Pins.get(Pin::Btn1) + i))){
				Synthia.SlotRGB.drawPixel(i, MatrixPixel::Yellow);
			}else if(pressed[i] && released[i]){
				Synthia.SlotRGB.drawPixel(i, MatrixPixel::Blue);
			}else{
				Synthia.SlotRGB.drawPixel(i, { 10, 10, 10, 255 });
			}
		}

		Synthia.SlidersMatrix.clear();
		for(int i = 0; i < 2; i++){
			if(pressed[5+i]){
				Synthia.SlidersMatrix.drawPixel(i, 6, MatrixPixel::White);
				Synthia.SlidersMatrix.drawPixel(i, 7, MatrixPixel::White);
			}
		}

		Synthia.SlotRGB.push();
		Synthia.SlidersMatrix.push();
	}

	Synthia.clearMatrices();
	return pressCount == ButtonCount && releaseCount == ButtonCount;
}

bool JigHWTest::SlidersTest(){
	const std::string string = "SLIDERS";
	int32_t textScrollCursor = -Synthia.TrackMatrix.getWidth();
	uint32_t textScrollTime = 0;

	uint8_t leftMin, leftMax, rightMin, rightMax;
	leftMin = leftMax = Sliders.getLeftPotValue();
	rightMin = rightMax = Sliders.getRightPotValue();

	while(leftMin > 0 || leftMax < 255 || rightMin > 0 || rightMax < 255){
		LoopManager::loop();

		if(millis() - textScrollTime > 100){
			textScrollTime = millis();
			textScrollCursor++;
			if(textScrollCursor >= (int16_t) (string.size() * (3 + 1) - 1)){
				textScrollCursor = -Synthia.TrackMatrix.getWidth();
			}

			Synthia.TrackMatrix.clear();
			Synthia.TrackMatrix.drawString(-textScrollCursor, 5, string.c_str());
			Synthia.TrackMatrix.push();
		}

		leftMin = min(leftMin, Sliders.getLeftPotValue());
		leftMax = max(leftMax, Sliders.getLeftPotValue());
		rightMin = min(rightMin, Sliders.getRightPotValue());
		rightMax = max(rightMax, Sliders.getRightPotValue());

		uint8_t leftStart = map(255 - leftMax, 0, 255, 0, 7);
		uint8_t leftEnd = map(255 - leftMin, 0, 255, 0, 7);
		uint8_t rightStart = map(255 - rightMax, 0, 255, 0, 7);
		uint8_t rightEnd = map(255 - rightMin, 0, 255, 0, 7);
		if(leftStart == 7 && leftMin > 0){
			leftStart = 6;
		}
		if(leftEnd == 0 && leftMax < 255){
			leftEnd = 1;
		}
		if(rightStart == 7 && rightMin > 0){
			rightStart = 6;
		}
		if(rightEnd == 0 && rightMax < 255){
			rightEnd = 1;
		}

		Synthia.SlidersMatrix.clear();
		for(int i = leftStart; i <= leftEnd; i++){
			Synthia.SlidersMatrix.drawPixel(0, i, MatrixPixel::White);
		}
		for(int i = rightStart; i <= rightEnd; i++){
			Synthia.SlidersMatrix.drawPixel(1, i, MatrixPixel::White);
		}
		Synthia.SlidersMatrix.push();
	}

	Synthia.clearMatrices();
	return true;
}

bool JigHWTest::EncodersTest(){
	const std::string string = "ENCODERS";
	int32_t textScrollCursor = -Synthia.TrackMatrix.getWidth();
	uint32_t textScrollTime = 0;

	static bool lLeft, lRight, rLeft, rRight;
	lLeft = lRight = rLeft = rRight = false;

	Encoders.setLeftEncCallback([](int8_t val){
		if(val == -1){
			lLeft = true;
		}else if(val == 1){
			lRight = true;
		}
	});

	Encoders.setRightEncCallback([](int8_t val){
		if(val == -1){
			rLeft = true;
		}else if(val == 1){
			rRight = true;
		}
	});

	while(!lLeft || !lRight || !rLeft || !rRight){
		LoopManager::loop();

		if(millis() - textScrollTime > 100){
			textScrollTime = millis();
			textScrollCursor++;
			if(textScrollCursor >= (int16_t) (string.size() * (3 + 1) - 1)){
				textScrollCursor = -Synthia.TrackMatrix.getWidth();
			}

			Synthia.TrackMatrix.clear();
			Synthia.TrackMatrix.drawString(-textScrollCursor, 5, string.c_str());
			Synthia.TrackMatrix.push();
		}

		Synthia.SlidersMatrix.clear();
		for(int x = 0; x < 2; x++){
			for(int y = 0; y < 8; y++){
				if((x == 0 && y < 4 && lLeft) || (x == 1 && y < 4 && rLeft)){
					Synthia.SlidersMatrix.drawPixel(x, y, MatrixPixel::White);
				}else if((x == 0 && y >= 4 && lRight) || (x == 1 && y >= 4 && rRight)){
					Synthia.SlidersMatrix.drawPixel(x, y, MatrixPixel::White);
				}else{
					Synthia.SlidersMatrix.drawPixel(x, y, MatrixPixel::Off);
				}
			}
		}
		Synthia.SlidersMatrix.push();
	}

	Encoders.setLeftEncCallback(nullptr);
	Encoders.setRightEncCallback(nullptr);
	Synthia.clearMatrices();

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
		test->log("Failed installing I2S driver", (uint32_t)err);
		return false;
	}

	err = i2s_set_pin(I2S_NUM_0, &i2s_pin_config);
	if(err != ESP_OK){
		test->log("Failed setting I2S pins", (uint32_t)err);
		return false;
	}

	Task waveTask("Wave", [](Task* task){
		size_t bufferSize;
		int16_t* buffer = nullptr;
		float currentFreq;

		auto generate = [&bufferSize, &buffer](float freq){
			int numSamples = ceil(16000.0f / freq);
			float omega = 2.0f * M_PI * freq;
			float timeDelta = (1.0f / freq) / (float)numSamples;
			float amp = 2000; // amplitude (max is 2^15 - 1)

			bufferSize = numSamples * 2 * sizeof(int16_t);
			buffer = static_cast<int16_t*>(realloc(buffer, bufferSize));
			for(int i = 0; i < numSamples; i++){
				buffer[i * 2 + 1] = sin(omega * timeDelta * (float)i) * amp;
			}
		};

		size_t bytesWritten;

		while(task->running){
			float freq = *static_cast<float*>(task->arg);
			if(freq == 0) continue;
			if(freq != currentFreq){
				generate(freq);
				currentFreq = freq;
			}

			i2s_write(I2S_NUM_0, buffer, bufferSize, &bytesWritten, portMAX_DELAY);
		}

		free(buffer);
	});

	float currentFreq = 0;
	waveTask.arg = &currentFreq;

	int numSamples = 128;
	size_t i2sBufferSize = numSamples * 2 * sizeof(int16_t);
	char* i2sBuffer = static_cast<char*>(malloc(i2sBufferSize));
	int16_t* buffer = static_cast<int16_t*>(malloc(i2sBufferSize / 2));
	memset(buffer, 0, i2sBufferSize / 2);

	struct cmplx {
		float re;
		float im;
	};
	float W = 2.0f * M_PI / (float)numSamples;
	cmplx* X = static_cast<cmplx*>(malloc(numSamples * sizeof(cmplx)));

	auto recordFreq = [&currentFreq, buffer, i2sBuffer, i2sBufferSize](float freq){
		currentFreq = freq;

		size_t readSize;
		// Pre-record
		for(int i = 0; i < 100; i++){
			i2s_read(I2S_NUM_0, (char*)i2sBuffer, i2sBufferSize, &readSize, portMAX_DELAY);
		}

		// Record
		for(int j = 0; j < i2sBufferSize; j += 4){
			int16_t sample = *(int16_t*)(&i2sBuffer[j + 2]) + 3705;
			buffer[j / 4] = sample;
		}
	};

	auto testFreq = [recordFreq, buffer, numSamples, W, X](float freq) -> bool{
		recordFreq(freq);

		// DFT
		for(int i = 0; i < numSamples; i++){
			X[i].re = X[i].im = 0;

			for(int j = 0; j < numSamples; j++){
				X[i].re += (float)buffer[j] * cos(W * (float)i * (float)j);
				X[i].im -= (float)buffer[j] * sin(W * (float)i * (float)j);
			}
		}

		auto f = [X](size_t i) -> float{
			return sqrt(pow(X[i].re, 2) + pow(X[i].im, 2));
		};

		int target = round((float)numSamples * freq / 16000.0f);

		float targetFreq = f(target);

		test->log("amp", targetFreq);

		return targetFreq > 50000
			   && targetFreq > 3.0f * f(target - 3)
			   && targetFreq > 3.0f * f(target - 2)
			   && targetFreq > 3.0f * f(target + 2)
			   && targetFreq > 3.0f * f(target + 3);
	};

	waveTask.start(2, 0);
	delay(500);


	float freqTests[] = {500, 750, 1250};
	bool freqTest = true;

	for(float & frequency : freqTests){
		bool freq = testFreq(frequency);
		freqTest = freqTest && freq;
		if(!freq){
			test->log("Failed on frequency", frequency);
		}
	}

	err = i2s_driver_uninstall(I2S_NUM_0);
	waveTask.stop();
	delay(500);

	if(err != ESP_OK){
		test->log("Failed uninstalling I2S driver", (uint32_t)err);
		return false;
	}

	return freqTest;
}


