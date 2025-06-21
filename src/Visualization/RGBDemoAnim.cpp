#include <Loop/LoopManager.h>
#include "RGBDemoAnim.h"

RGBDemoAnim::RGBDemoAnim(Matrix& matrix) : MatrixAnim(&matrix){

}

void RGBDemoAnim::onStart(){
	LoopManager::addListener(this);
	lastTime = 0;
	push();
}

void RGBDemoAnim::onStop(){
	LoopManager::removeListener(this);
}

void RGBDemoAnim::reset(){
	lastTime = 0;
	push();
}

void RGBDemoAnim::push(){
	if(millis() - lastTime < UpdatePeriod) return;
	lastTime = millis();

	for(int i = 0; i < Count; i++){
		const auto cI = rand() % 3;
		MatrixPixel color = {
				.r = (uint8_t) ((cI == 0) * 255),
				.g = (uint8_t) ((cI == 1) * 255),
				.b = (uint8_t) ((cI == 2) * 255),
				.i = 255
		};
		drawPixel(i, color);
	}

	pushMatrix();
}

void RGBDemoAnim::loop(uint micros){
	if(!isStarted()){
		stop();
		LoopManager::removeListener(this);
		return;
	}

	push();
}
