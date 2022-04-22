#include "UserHWTest.h"
#include "../Visualization/RGBController.h"
#include "../Visualization/LEDStrip.h"
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

UserHWTest::UserHWTest(void (* callback)()) : talking(SPIFFS.open("/talk.gif"), &Synthia.TrackMatrix), doneCallback(callback){
	playback.begin();
	playback.setVolume(255);
}

void UserHWTest::loop(uint micros){
	if(!playback.isPlaying()){
		if(state == TestState::Intro){
			stopState(TestState::Intro);
			startState(TestState::Buttons);
		}else if(state == TestState::Outro){
			stopState(TestState::Outro);
		}
	}

	Matrix& track = Synthia.TrackMatrix;
	if(state == TestState::Buttons || state == TestState::Sliders){
		textScrollTimer += micros;
		if(textScrollTimer >= 200000){
			textScrollTimer = 0;
			textCursor--;
			if(textCursor <= -((int)(strlen(scrollingText)) * (3 + 1) - 1)){
				textCursor = track.getWidth();
			}
			track.clear();
			track.setFont(Matrix::SMALL);
			track.drawString(textCursor, 5, scrollingText);
			track.push();
		}
	}
}

void UserHWTest::onStart(){
	Sliders.addListener(this);
	Encoders.addListener(this);
	Synthia.getInput()->addListener(this);
	LoopManager::addListener(this);

	startState(TestState::Intro);

}

void UserHWTest::onStop(){
	Sliders.removeListener(this);
	Encoders.removeListener(this);
	Synthia.getInput()->removeListener(this);
	LoopManager::removeListener(this);

	Synthia.TrackMatrix.stopAnimations();
	Synthia.SlotRGB.clear();
}

void UserHWTest::buttonPressed(uint i){
	switch(state){
		case TestState::Buttons:
			if(buttonsPressed[btnToIndex(i)])
				return;

			switch(i){
				case BTN_ENC_L:
					Synthia.SlidersMatrix.drawPixel(0, Synthia.SlidersMatrix.getHeight() - 1, MatrixPixel::White);
					Synthia.SlidersMatrix.push();
					break;
				case BTN_ENC_R:
					Synthia.SlidersMatrix.drawPixel(1, Synthia.SlidersMatrix.getHeight() - 1, MatrixPixel::White);
					Synthia.SlidersMatrix.push();
					break;
				default:
					RGBSlot.setColor(Synthia.btnToSlot(i), MatrixPixel::Green);
					break;
			}

			pressedCount++;
			buttonsPressed[btnToIndex(i)] = true;
			if(pressedCount == 7){
				stopState(state);
				startState(TestState::Sliders);
			}

			break;

		case TestState::Sliders:
			if(abs((int)(calibration.sliderLeftDown) - (int)(calibration.sliderLeftUp)) <= 150 ||
			   abs((int)(calibration.sliderRightDown) - (int)(calibration.sliderRightUp)) <= 150)
				return;

			stopState(state);
			startState(TestState::Encoders);
			break;

		case TestState::Encoders:
			stopState(state);
			startState(TestState::Outro);
			break;

		default:
			break;
	}
}

void UserHWTest::leftEncMove(int8_t amount){
	if(state != TestState::Encoders) return;

	Synthia.TrackMatrix.drawPixel(knobX, knobY, MatrixPixel::Off);
	if(knobX == 0 && amount < 0){
		knobX = Synthia.TrackMatrix.getWidth() - 1;
	}else{
		knobX = (knobX + amount) % Synthia.TrackMatrix.getWidth();
	}

	Synthia.TrackMatrix.drawPixel(knobX, knobY, MatrixPixel::White);
	Synthia.TrackMatrix.push();
}

void UserHWTest::rightEncMove(int8_t amount){
	if(state != TestState::Encoders) return;

	Synthia.TrackMatrix.drawPixel(knobX, knobY, MatrixPixel::Off);
	if(knobY == 0 && amount < 0){
		knobY = Synthia.TrackMatrix.getHeight() - 1;
	}else{
		knobY = (knobY + amount) % Synthia.TrackMatrix.getHeight();
	}

	Synthia.TrackMatrix.drawPixel(knobX, knobY, MatrixPixel::White);
	Synthia.TrackMatrix.push();

}

void UserHWTest::leftPotMove(uint8_t value){
	if(state != TestState::Sliders) return;

	uint16_t read = analogRead(POT_L);
	if(read < calibration.sliderLeftDown){
		calibration.sliderLeftDown = read;
	}else if(read > calibration.sliderLeftUp){
		calibration.sliderLeftUp = read;
	}

	LEDStrip.setLeftSingle(Sliders.getLeftPotValue());
}

void UserHWTest::rightPotMove(uint8_t value){
	if(state != TestState::Sliders) return;

	uint16_t read = analogRead(POT_R);
	if(read < calibration.sliderRightDown){
		calibration.sliderRightDown = read;
	}else if(read > calibration.sliderRightUp){
		calibration.sliderRightUp = read;
	}

	LEDStrip.setRightSingle(Sliders.getRightPotValue());
}

void UserHWTest::startState(UserHWTest::TestState state){
	this->state = state;
	switch(state){
		case TestState::Intro:
			playFile("/HWTestSamples/intro.aac");
			talking.start();
			break;

		case TestState::Buttons:
			textCursor = 0;
			textScrollTimer = 0;
			Synthia.TrackMatrix.setFont(Matrix::SMALL);
			Synthia.TrackMatrix.clear();
			Synthia.TrackMatrix.drawString(0, 5, "CLICK BUTTONS");
			Synthia.TrackMatrix.push();
			scrollingText = "CLICK BUTTONS";

			pressedCount = 0;
			playFile("/HWTestSamples/buttons.aac");
			break;

		case TestState::Sliders:
			textCursor = 0;
			textScrollTimer = 0;

			playFile("/HWTestSamples/sliders.aac");
			calibration.sliderLeftDown = calibration.sliderLeftUp = analogRead(POT_L);
			calibration.sliderRightDown = calibration.sliderRightUp = analogRead(POT_L);

			LEDStrip.setLeftSingle(Sliders.getLeftPotValue());
			LEDStrip.setRightSingle(Sliders.getRightPotValue());

			Synthia.TrackMatrix.setFont(Matrix::SMALL);
			Synthia.TrackMatrix.clear();
			Synthia.TrackMatrix.drawString(0, 5, "SLIDERS");
			Synthia.TrackMatrix.push();
			scrollingText = "SLIDERS";

			break;

		case TestState::Encoders:
			playFile("/HWTestSamples/knobs.aac");
			knobX = Synthia.TrackMatrix.getWidth() / 2;
			knobY = Synthia.TrackMatrix.getHeight() / 2;
			Synthia.TrackMatrix.drawPixel(knobX, knobY, MatrixPixel::White);
			Synthia.TrackMatrix.push();
			break;

		case TestState::Outro:
			talking.start();
			playFile("/HWTestSamples/outro.aac");
			break;
	}
}

void UserHWTest::stopState(UserHWTest::TestState state){
	switch(state){
		case TestState::Intro:
			Synthia.TrackMatrix.stopAnimations();
			Synthia.TrackMatrix.clear();
			Synthia.TrackMatrix.push();
			playback.block(0);
			break;

		case TestState::Buttons:
			playback.block(0);
			RGBSlot.clear();
			Synthia.SlidersMatrix.clear();
			Synthia.SlidersMatrix.push();
			Synthia.TrackMatrix.clear();
			break;

		case TestState::Sliders:
			playback.block(0);
			Synthia.SlidersMatrix.clear();
			Synthia.SlidersMatrix.push();
			Synthia.TrackMatrix.clear();
			Synthia.TrackMatrix.push();

			Settings.get().calibration = calibration;
			Settings.store();
			break;

		case TestState::Encoders:
			playback.block(0);
			Synthia.TrackMatrix.clear();
			Synthia.TrackMatrix.push();
			break;


		case TestState::Outro:
			playback.block(0);
			Synthia.TrackMatrix.stopAnimations();
			Synthia.TrackMatrix.clear();
			Synthia.TrackMatrix.push();

			if(doneCallback){
				stop();
				volatile auto callback = doneCallback;
				delete this;
				callback();
			}else{
				pop();
			}

			break;
	}
}

void UserHWTest::playFile(const char* path){
	playback.set(0, SPIFFS.open(path), SlotConfig{});
	playback.play(0);
	while(!playback.isPlaying()) delay(1);
}

uint8_t UserHWTest::btnToIndex(uint32_t i){
	if(i == BTN_ENC_L){
		return 5;
	}else if(i == BTN_ENC_R){
		return 6;
	}else{
		return Synthia.btnToSlot(i);
	}
}
