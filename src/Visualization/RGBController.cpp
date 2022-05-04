#include "RGBController.h"
#include <Devices/Matrix/Matrix.h>
#include <Loop/LoopManager.h>
#include <SPIFFS.h>
#include <FS/RamFile.h>
#include <Synthia.h>

RGBController RGBTrack;
RGBController RGBSlot;

RGBController::RGBController(){

}

void RGBController::begin(Matrix* matrix){
	this->matrix = matrix;
	LoopManager::addListener(this);

	anims = {
			new MatrixAnimGIF(RamFile::open(SPIFFS.open("/GIF/TrackEditState.gif")), matrix),
			new MatrixAnimGIF(RamFile::open(SPIFFS.open("/GIF/SampleEditState.gif")), matrix)
	};
}

void RGBController::setColor(uint8_t slot, MatrixPixel color){
	if(slot >= 5) return;

	slotColors[slot] = color;

	if(state == Single && slotStates[slot] == Solid){
		matrix->drawPixel(slot, color);
		matrix->push();
	}
}

void RGBController::clear(){
	if(state == Anim){
		matrix->stopAnimations();
		state = Single;
	}

	for(int i = 0; i < 5; ++i){
		slotStates[i] = Solid;
		blinkStates[i] = false;
		blinkColors[i] = MatrixPixel::Off;
		blinkStartTimes[i] = 0;

		slotColors[i] = MatrixPixel::Off;
		matrix->drawPixel(i, MatrixPixel::Off);
	}

	matrix->push();
}

void RGBController::setSolid(uint8_t slot, MatrixPixel color){
	if(slot >= 5) return;

	if(state == Anim) return;

	slotStates[slot] = Solid;

	setColor(slot, color);
}

void RGBController::blink(uint8_t slot, MatrixPixel color){
	if(slot >= 5) return;

	if(state == Anim) return;

	slotStates[slot] = Once;
	blinkColors[slot] = color;
	blinkStartTimes[slot] = millis();
	blinkStates[slot] = true;

	matrix->drawPixel(slot, color);
	matrix->push();
}

void RGBController::blinkTwice(uint8_t slot, MatrixPixel color){
	if(slot >= 5) return;

	if(state == Anim) return;

	blink(slot, color);
	slotStates[slot] = Twice;
}

void RGBController::blinkAll(MatrixPixel color){
	if(state == Anim){
		matrix->stopAnimations();
		state = Single;
	}

	for(int i = 0; i < 5; ++i){
		blink(i, color);
	}
}

void RGBController::blinkAllTwice(MatrixPixel color){
	if(state == Anim){
		matrix->stopAnimations();
		state = Single;
	}

	for(int i = 0; i < 5; ++i){
		blinkTwice(i, color);
	}
}

void RGBController::blinkContinuous(uint8_t slot, MatrixPixel color){
	if(state == Anim) return;

	blink(slot, color);
	slotStates[slot] = Continuous;
}

void RGBController::loop(uint micros){
	if(state == Anim) return;

	bool push = false;
	for(int i = 0; i < 5; i++){
		if(slotStates[i] == Solid) continue;
		if(millis() - blinkStartTimes[i] < blinkDuration) continue;

		if(slotStates[i] == Continuous){
			if(millis() - blinkStartTimes[i] < blinkContinuousDuration) continue;

			blinkStates[i] = !blinkStates[i];
			blinkStartTimes[i] = millis();
			matrix->drawPixel(i, blinkStates[i] ? blinkColors[i] : MatrixPixel::Off);
			push = true;
		}else if(slotStates[i] == Twice && blinkStates[i]){
			blinkStates[i] = false;
			blinkStartTimes[i] = millis();
			matrix->drawPixel(i, MatrixPixel::Off);
			push = true;
		}else if(slotStates[i] == Twice && !blinkStates[i]){
			blinkStates[i] = true;
			blinkStartTimes[i] = millis();
			slotStates[i] = Once;
			matrix->drawPixel(i, blinkColors[i]);
			push = true;
		}else if(slotStates[i] == Once){
			blinkStates[i] = false;
			blinkStartTimes[i] = 0;
			blinkColors[i] = MatrixPixel::Off;
			slotStates[i] = Solid;
			matrix->drawPixel(i, slotColors[i]);
			push = true;
		}
	}

	if(push){
		matrix->push();
	}
}

void RGBController::playAnim(enum Anim anim){
	int index = (int) anim;
	if(index >= anims.size()) return;

	state = Anim;
	anims[index]->reset();
	matrix->startAnimation(anims[index]);
}

void RGBController::stopAnim(){
	state = Single;
	matrix->stopAnimations();

	matrix->clear();
	matrix->push();
}
