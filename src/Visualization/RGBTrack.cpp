#include "RGBTrack.h"
#include <Synthia.h>
#include <Devices/Matrix/MatrixAnimGIF.h>

void RGBTrack::setColor(uint8_t slot, MatrixPixel color){
	slotColors[slot] = color;
	if(state == Anim){
		TrackRGB.stopAnimation();
		state = Single;
		TrackRGB.drawPixel(slot, color);
		TrackRGB.push();
	}
	//sets the resting color after a blink is done
}

void RGBTrack::clear(){
	for(int i = 0; i < 5; ++i){
		setColor(i, {0, 0, 0, 0});
	}
}

void RGBTrack::blink(uint8_t slot, MatrixPixel color){
	if(state == Anim){
		TrackRGB.stopAnimation();
		state = Single;
	}
	TrackRGB.drawPixel(slot, color);
	TrackRGB.push();
	slotState[slot] = Once;
	blinkMicros[slot] = 0;
	blinkState[slot] = true;
	blinkColors[slot] = color;
}

void RGBTrack::blinkTwice(uint8_t slot, MatrixPixel color){
	if(state == Anim){
		TrackRGB.stopAnimation();
		state = Single;
	}
	TrackRGB.drawPixel(slot, color);
	TrackRGB.push();
	slotState[slot] = Twice;
	blinkMicros[slot] = 0;
	blinkState[slot] = true;
	blinkColors[slot] = color;
}

void RGBTrack::blinkAll(MatrixPixel color){
	for(int i = 0; i < 5; ++i){
		blink(i, color);
	}
}

void RGBTrack::blinkAllTwice(MatrixPixel color){
	for(int i = 0; i < 5; ++i){
		blinkTwice(i, color);
	}
}

void RGBTrack::loop(uint micros){
	if(state == Anim) return;

	bool pushNeeded = false;

	for(int i = 0; i < 5; ++i){

		if(slotState[i] == Solid) continue;

		blinkMicros[i] += micros;
		if(blinkMicros[i] < blinkTime*1000) continue;

		pushNeeded = true;
		if(slotState[i] == Once){
			if(blinkState[i]){
				TrackRGB.drawPixel(i, {0, 0, 0, 0});
				blinkState[i] = false;
			}else{
				slotState[i] = Solid;
				TrackRGB.drawPixel(i, slotColors[i]);
			}
		}else if(slotState[i] == Twice){
			if(blinkState[i]){
				TrackRGB.drawPixel(i, {0, 0, 0, 0});
				blinkState[i] = false;
			}else{
				blink(i, blinkColors[i]);
			}
		}
	}

	if(pushNeeded){
		TrackRGB.push();
	}
}

void RGBTrack::playRunning(MatrixPixel color){
	state = Anim;
	//TODO - start animation from file
}
