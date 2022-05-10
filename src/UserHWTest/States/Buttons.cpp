#include <SPIFFS.h>
#include <Loop/LoopManager.h>
#include <Synthia.h>
#include "Buttons.h"

UserHWTest::Buttons::Buttons(UserHWTest::Test* test) : State(test){

}

void UserHWTest::Buttons::start(){
	play(SPIFFS.open("/HWTestSamples/buttons.aac"));
	talk();

	LoopManager::addListener(this);
	Synthia.getInput()->addListener(this);
}

void UserHWTest::Buttons::stop(){
	LoopManager::removeListener(this);
	Synthia.getInput()->removeListener(this);

	Synthia.SlotRGB.clear();
	Synthia.SlotRGB.push();

	Synthia.SlidersMatrix.clear();
	Synthia.SlidersMatrix.push();
}

void UserHWTest::Buttons::loop(uint micros){
	if(state != Talking){
		LoopManager::removeListener(this);
		return;
	}

	if(isPlaying()) return;

	state = Waiting;
	LoopManager::removeListener(this);
	showText("CLICK BUTTONS");
}

void UserHWTest::Buttons::buttonPressed(uint btn){
	int i;
	if(btn == BTN_ENC_L){
		i = 5;
		Synthia.SlidersMatrix.drawPixel(0, 7, MatrixPixel::White);
		Synthia.SlidersMatrix.push();
	}else if(btn == BTN_ENC_R){
		i = 6;
		Synthia.SlidersMatrix.drawPixel(1, 7, MatrixPixel::White);
		Synthia.SlidersMatrix.push();
	}else{
		i = Synthia.btnToSlot(btn);
		Synthia.SlotRGB.drawPixel(i, MatrixPixel::Green);
		Synthia.SlotRGB.push();
	}

	if(clicked[i]) return;
	clicked[i] = true;
	clickCount++;

	if(clickCount == 7){
		done();
	}
}
