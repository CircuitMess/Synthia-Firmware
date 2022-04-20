#include "LEDStrip.h"
#include "Synthia.h"

LEDStripImpl LEDStrip;

void LEDStripImpl::setLeft(uint8_t value){
	setStrip(value, 0);
}

void LEDStripImpl::setRight(uint8_t value){
	setStrip(value, 1);
}

void LEDStripImpl::setLeftSingle(uint8_t value){
	setStripSingle(value, 0);
}

void LEDStripImpl::setRightSingle(uint8_t value){
	setStripSingle(value, 1);
}

void LEDStripImpl::setMidFill(uint8_t value){
	Synthia.CursorMatrix.stopAnimations();
	uint32_t fill = map(value, 0, 255, 0, 255 * 16);
	for(int i = 0; i < 16; ++i){
		uint8_t pixelValue = min(fill, 255U);
		Synthia.CursorMatrix.drawPixel(i, {255, 255, 255, pixelValue});
		if(fill >= 255){
			fill -= 255;
		}else if(fill > 0){
			fill = 0;
		}
	}
	Synthia.CursorMatrix.push();
}

void LEDStripImpl::setMidSingle(uint8_t index){
	Synthia.CursorMatrix.stopAnimations();
	Synthia.CursorMatrix.clear();
	Synthia.CursorMatrix.drawPixel(index, {255, 255, 255, 100});
	Synthia.CursorMatrix.push();
}

void LEDStripImpl::setMidSelection(uint8_t value){
	Synthia.CursorMatrix.stopAnimations();
	if(value > 1) return;
	Synthia.CursorMatrix.clear();

	for(int i = 0; i < 8; ++i){
		Synthia.CursorMatrix.drawPixel(i + value * 8, {255, 255, 255, 255});
	}
	Synthia.CursorMatrix.push();
}

void LEDStripImpl::setStrip(uint8_t value, uint8_t x){
	uint32_t fill = map(value, 0, 255, 0, 255 * 8);
	for(int i = 7; i >= 0; --i){
		uint8_t pixelValue = min(fill, 255U);
		Synthia.SlidersMatrix.drawPixel(x, i, {255, 255, 255, pixelValue});
		if(fill >= 255){
			fill -= 255;
		}else if(fill > 0){
			fill = 0;
		}
	}
	Synthia.SlidersMatrix.push();
}

void LEDStripImpl::setStripSingle(uint8_t value, uint8_t x){
	int32_t fill = map(value, 0, 255, 0, 255 * 7);

	for(int i = 7; i >= 0; --i){
		uint8_t pixelValue = 0;
		if(fill < (i - 1) * 255 || fill >= (i + 1) * 255){
			pixelValue = 0;
		}else{
			if(fill > i * 255){
				pixelValue = (i + 1) * 255 - fill;
			}else if(fill <= i * 255){
				pixelValue = fill - (i - 1) * 255;
			}
		}
		Synthia.SlidersMatrix.drawPixel(x, 7 - i, { 255, 255, 255, pixelValue });
	}
	Synthia.SlidersMatrix.push();
}
