#include "RGBController.h"
#include <Devices/Matrix/Matrix.h>
#include <Loop/LoopManager.h>

RGBController RGBTrack;
RGBController RGBSlot;

RGBController::RGBController(){

}

void RGBController::begin(Matrix* matrix){
	this->matrix = matrix;
	LoopManager::addListener(this);
}

void RGBController::setColor(uint8_t slot, MatrixPixel color){
	slotColors[slot] = color;
	if(state == Anim){
		matrix->stopAnimation();
		state = Single;
		matrix->drawPixel(slot, color);
		matrix->push();
	}else if(state == Single){
		matrix->drawPixel(slot, color);
		pushNeeded = true;
	}
	//sets the resting color after a blink is done
}

void RGBController::clear(){
	for(int i = 0; i < 5; ++i){
		setColor(i, {0, 0, 0, 0});
	}
}

void RGBController::blink(uint8_t slot, MatrixPixel color){
	if(state == Anim){
		matrix->stopAnimation();
		state = Single;
	}
	matrix->drawPixel(slot, color);
	matrix->push();
	slotState[slot] = Once;
	blinkMicros[slot] = 0;
	blinkState[slot] = true;
	blinkColors[slot] = color;
}

void RGBController::blinkTwice(uint8_t slot, MatrixPixel color){
	if(state == Anim){
		matrix->stopAnimation();
		state = Single;
	}
	matrix->drawPixel(slot, color);
	matrix->push();
	slotState[slot] = Twice;
	blinkMicros[slot] = 0;
	blinkState[slot] = true;
	blinkColors[slot] = color;
}

void RGBController::blinkAll(MatrixPixel color){
	for(int i = 0; i < 5; ++i){
		blink(i, color);
	}
}

void RGBController::blinkAllTwice(MatrixPixel color){
	for(int i = 0; i < 5; ++i){
		blinkTwice(i, color);
	}
}

void RGBController::blinkContinuous(uint8_t slot, MatrixPixel color){
	if(state == Anim){
		matrix->stopAnimation();
		state = Single;
	}
	matrix->drawPixel(slot, color);
	matrix->push();
	slotState[slot] = Continuous;
	blinkMicros[slot] = 0;
	blinkState[slot] = true;
	blinkColors[slot] = color;
}

void RGBController::loop(uint micros){
	if(state == Anim) return;

	bool blinkPushNeeded = false;

	for(int i = 0; i < 5; ++i){

		if(slotState[i] == Solid) continue;

		blinkMicros[i] += micros;
		if(blinkMicros[i] < blinkTime*1000) continue;

		blinkPushNeeded = true;
		blinkMicros[i] = 0;

		if(slotState[i] == Once){
			if(blinkState[i]){
				matrix->drawPixel(i, {0, 0, 0, 0});
				blinkState[i] = false;
			}else{
				slotState[i] = Solid;
				matrix->drawPixel(i, slotColors[i]);
			}
		}else if(slotState[i] == Twice){
			if(blinkState[i]){
				matrix->drawPixel(i, {0, 0, 0, 0});
				blinkState[i] = false;
			}else{
				blink(i, blinkColors[i]);
			}
		}else if(slotState[i] == Continuous){
			if(blinkState[i]){
				matrix->drawPixel(i, {0, 0, 0, 0});
				blinkState[i] = false;
			}else{
				matrix->drawPixel(i, blinkColors[i]);
				blinkState[i] = true;
			}
		}
	}

	if(blinkPushNeeded || pushNeeded){
		matrix->push();
		pushNeeded = false;
	}
}

void RGBController::playAnim(){
	state = Anim;
	// TODO: start GIF animation
}
