#include "LEDStrip.h"
#include "Synthia.h"

LEDStripImpl LEDStrip;

void LEDStripImpl::setLeft(uint8_t value){
	setStrip(value, 0);
}

void LEDStripImpl::setRight(uint8_t value){
	setStrip(value, 1);
}

void LEDStripImpl::setMidFill(uint8_t value){
	uint32_t fill = map(value, 0, 255, 0, 255 * 16);
	for(int i = 0; i < 16; ++i){
		uint8_t pixelValue = min(fill, 255U);
		CursorMatrix.drawPixel(i, {pixelValue, pixelValue, pixelValue, pixelValue});
		if(fill >= 255){
			fill -= 255;
		}else if(fill > 0){
			fill = 0;
		}
	}
}

void LEDStripImpl::setMidSingle(uint8_t index){
	CursorMatrix.drawPixel(index, {255, 255, 255, 255});
}

void LEDStripImpl::setMidSelection(uint8_t value){
	if(value > 1) return;

	for(int i = 0; i < 8; ++i){
		CursorMatrix.drawPixel(i + value * 8, {255, 255, 255, 255});
	}
}

void LEDStripImpl::setStrip(uint8_t value, uint8_t x){
	uint32_t fill = map(value, 0, 255, 0, 255 * 8);
	for(int i = 7; i >= 0; --i){
		uint8_t pixelValue = min(fill, 255U);
		SlidersMatrix.drawPixel(x, i, {pixelValue, pixelValue, pixelValue, pixelValue});
		if(fill >= 255){
			fill -= 255;
		}else if(fill > 0){
			fill = 0;
		}
	}
}
