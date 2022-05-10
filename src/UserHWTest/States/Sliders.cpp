#include <SPIFFS.h>
#include "Sliders.h"
#include <Synthia.h>
#include <Loop/LoopManager.h>
#include <Settings.h>

UserHWTest::Sliders::Sliders(UserHWTest::Test* test) : State(test){

}

void UserHWTest::Sliders::start(){
	play(SPIFFS.open("/HWTestSamples/sliders.aac"));
	talk();

	Settings.get().calibration = SlidersCalibration();
	Settings.store();

	showL.max = showL.min = ::Sliders.getLeftPotValue();
	lightSlider(0);

	showR.max = showR.min = ::Sliders.getRightPotValue();
	lightSlider(1);

	::Sliders.addListener(this);
	Synthia.getInput()->addListener(this);
	LoopManager::addListener(this);
}

void UserHWTest::Sliders::stop(){
	::Sliders.removeListener(this);
	Synthia.getInput()->removeListener(this);
	LoopManager::removeListener(this);

	sliderL.max -= 10;
	sliderL.min += 10;
	sliderR.max -= 10;
	sliderR.min += 10;

	auto& data = Settings.get().calibration;
	data.sliderLeftDown = sliderL.min;
	data.sliderLeftUp = sliderL.max;
	data.sliderRightDown = sliderR.min;
	data.sliderRightUp = sliderR.max;


	Synthia.SlidersMatrix.clear();
	Synthia.SlidersMatrix.push();
}

void UserHWTest::Sliders::buttonPressed(uint i){
	done();
}

void UserHWTest::Sliders::loop(uint micros){
	sliderSamplesL += analogRead(POT_L);
	sliderSamplesR += analogRead(POT_R);

	sampleCount++;
	if(sampleCount == totalSampleCount){
		sampleCount = 0;

		sliderSamplesL /= totalSampleCount;
		sliderSamplesR /= totalSampleCount;

		if(sliderL.max == -1 || sliderSamplesL > sliderL.max){
			sliderL.max = sliderSamplesL;
		}

		if(sliderL.min == -1 || sliderSamplesL < sliderL.min){
			sliderL.min = sliderSamplesL;
		}

		if(sliderR.max == -1 || sliderSamplesR > sliderR.max){
			sliderR.max = sliderSamplesR;
		}

		if(sliderR.min == -1 || sliderSamplesR < sliderR.min){
			sliderR.min = sliderSamplesR;
		}
	}

	if(isPlaying() || state != Talking) return;

	state = Waiting;
	showText("SLIDE SLIDERS");
}

void UserHWTest::Sliders::leftPotMove(uint8_t value){
	if(showL.max == -1 || value > showL.max){
		showL.max = value;
	}

	if(showL.min == -1 || value < showL.min){
		showL.min = value;
	}

	lightSlider(0);
}

void UserHWTest::Sliders::rightPotMove(uint8_t value){
	if(showR.max == -1 || value > showR.max){
		showR.max = value;
	}

	if(showR.min == -1 || value < showR.min){
		showR.min = value;
	}

	lightSlider(1);
}

void UserHWTest::Sliders::lightSlider(uint8_t index){
	auto val = index == 0 ? showL : showR;
	val.max *= Synthia.SlidersMatrix.getHeight();
	val.min *= Synthia.SlidersMatrix.getHeight();

	for(int i = 0; i < 8; i++){
		Synthia.SlidersMatrix.drawPixel(index, i, MatrixPixel::Off);
	}

	for(int i = 0, prog = 0; i < 8; i++, prog += 256){
		if(i == 7) printf("A %d %d %d\n", prog, val.max, val.min);
		if(prog + 256 <= val.min){
			if(i == 7) printf("A %d %d %d\n", prog, val.max, val.min);
			// skip
		}else if(prog <= val.min && val.min < prog + 256){
			uint8_t rest = prog + 250 - val.min;
			if(i == 7) printf("B %d %d %d %d\n", prog, val.max, val.min, rest);
			Synthia.SlidersMatrix.drawPixel(index, 7 - i, { 255, 255, 255, rest });
		}else if(prog + 256 <= val.max){
			if(i == 7) printf("C %d %d %d\n", prog, val.max, val.min);
			Synthia.SlidersMatrix.drawPixel(index, 7 - i, MatrixPixel::White);
		}else if(prog <= val.max && val.max <= prog + 256){
			uint8_t rest = val.max - prog;
			if(i == 7) printf("D %d %d %d %d\n", prog, val.max, val.min, rest);
			Synthia.SlidersMatrix.drawPixel(index, 7 - i, { 255, 255, 255, rest });
		}else if(prog > val.max){
			if(i == 7) printf("E %d %d %d\n", prog, val.max, val.min);
			// skip
		}
	}

	Synthia.SlidersMatrix.push();
}
