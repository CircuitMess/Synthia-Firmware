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
		Synthia.CursorMatrix.drawPixel(i, { 255, 255, 255, pixelValue });
		if(fill >= 255){
			fill -= 255;
		}else if(fill > 0){
			fill = 0;
		}
	}
	Synthia.CursorMatrix.push();
}

void LEDStripImpl::setMidSingle(uint8_t index){
	Synthia.CursorMatrix.clear();
	Synthia.CursorMatrix.drawPixel(index, { 255, 255, 255, 255 });
	Synthia.CursorMatrix.push();
}

void LEDStripImpl::setMidSelection(uint8_t value){
	if(value > 1) return;
	Synthia.CursorMatrix.clear();

	for(int i = 0; i < 8; ++i){
		Synthia.CursorMatrix.drawPixel(i + value * 8, { 255, 255, 255, 255 });
	}
	Synthia.CursorMatrix.push();
}

void LEDStripImpl::setStrip(uint8_t value, uint8_t x){
	int32_t fill = map(value, 0, 255, 0, 255 * 8);
	for(int i = 0; i < 8; ++i){
		uint8_t pixelValue = min(max(fill - (7 - i) * 255, 0), 255);
		Synthia.SlidersMatrix.drawPixel(x, i, { 255, 255, 255, pixelValue });
	}
	Synthia.SlidersMatrix.push();
}

void LEDStripImpl::setLeftFromCenter(int8_t value){
	setStripFromCenter(value, 0);
}

void LEDStripImpl::setRightFromCenter(int8_t value){
	setStripFromCenter(value, 1);
}

void LEDStripImpl::setStripFromCenter(int8_t value, uint8_t x){
	int32_t fill = map(value, -128, 127, -128 * 8, 127 * 8);
	for(int i = 0; i < 8; ++i){
		uint8_t pixelValue = 0;

		if((i > 3 && value > 0) || (i <= 3 && value < 0)){ 		//on opposite side of the center, should all be zeroed
			pixelValue = 0;
		}else{
			if(fill > 0){
				pixelValue = min(max(fill - (3 - i) * 255, 0), 255);
			}else if(fill < 0){
				pixelValue = min(max(abs(fill) - (i - 4) * 255, 0), 255);
			}
		}
		Synthia.SlidersMatrix.drawPixel(x, i, { 255, 255, 255, pixelValue });
	}
	Synthia.SlidersMatrix.push();
}

void LEDStripImpl::setMidFillFromCenter(int8_t value){
	value *= -1; //inverted alignment compared to slider strips
	int32_t fill = map(value, -128, 127, -128 * 16, 127 * 16);
	for(int i = 0; i < 16; ++i){
		uint8_t pixelValue = 0;

		if((i > 7 && value > 0) || (i <= 7 && value < 0)){ 		//on opposite side of the center, should all be zeroed
			pixelValue = 0;
		}else{
			if(fill > 0){
				pixelValue = min(max(fill - (7 - i) * 255, 0), 255);
			}else if(fill < 0){
				pixelValue = min(max(abs(fill) - (i - 8) * 255, 0), 255);
			}
		}
		Synthia.CursorMatrix.drawPixel(i, { 255, 255, 255, pixelValue });
	}
	Synthia.CursorMatrix.push();
}
