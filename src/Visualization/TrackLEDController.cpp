#include "TrackLEDController.h"
#include <Devices/Matrix/MatrixAnimGIF.h>
#include <Devices/Matrix/Matrix.h>

TrackLEDController::TrackLEDController(Matrix& matrix) : matrix(matrix){

}

void TrackLEDController::setColor(uint8_t slot, MatrixPixel color){
	slotColors[slot] = color;
	if(state == Anim){
		matrix.stopAnimation();
		state = Single;
		drawPixel(slot, color);
		matrix.push();
	}
	//sets the resting color after a blink is done
}

void TrackLEDController::clear(){
	for(int i = 0; i < 5; ++i){
		setColor(i, {0, 0, 0, 0});
	}
}

void TrackLEDController::blink(uint8_t slot, MatrixPixel color){
	if(state == Anim){
		matrix.stopAnimation();
		state = Single;
	}
	drawPixel(slot, color);
	matrix.push();
	slotState[slot] = Once;
	blinkMicros[slot] = 0;
	blinkState[slot] = true;
	blinkColors[slot] = color;
}

void TrackLEDController::blinkTwice(uint8_t slot, MatrixPixel color){
	if(state == Anim){
		matrix.stopAnimation();
		state = Single;
	}
	drawPixel(slot, color);
	matrix.push();
	slotState[slot] = Twice;
	blinkMicros[slot] = 0;
	blinkState[slot] = true;
	blinkColors[slot] = color;
}

void TrackLEDController::blinkAll(MatrixPixel color){
	for(int i = 0; i < 5; ++i){
		blink(i, color);
	}
}

void TrackLEDController::blinkAllTwice(MatrixPixel color){
	for(int i = 0; i < 5; ++i){
		blinkTwice(i, color);
	}
}

void TrackLEDController::loop(uint micros){
	if(state == Anim) return;

	bool pushNeeded = false;

	for(int i = 0; i < 5; ++i){

		if(slotState[i] == Solid) continue;

		blinkMicros[i] += micros;
		if(blinkMicros[i] < blinkTime*1000) continue;

		pushNeeded = true;
		if(slotState[i] == Once){
			if(blinkState[i]){
				drawPixel(i, {0, 0, 0, 0});
				blinkState[i] = false;
			}else{
				slotState[i] = Solid;
				drawPixel(i, slotColors[i]);
			}
		}else if(slotState[i] == Twice){
			if(blinkState[i]){
				drawPixel(i, {0, 0, 0, 0});
				blinkState[i] = false;
			}else{
				blink(i, blinkColors[i]);
			}
		}
	}

	if(pushNeeded){
		matrix.push();
	}
}

void TrackLEDController::playAnim(){
	state = Anim;
	playSpecificAnim();
}

