#include "VolumeVisualizer.h"
#include <Synthia.h>
#include "LEDStrip.h"

void VolumeVisualizer::visualize(){
	uint8_t value = getProp();

	uint32_t fill = map(value, 0, 255, 0, 255 * 16);
	for(int i = 0; i < 16; ++i){
		uint8_t pixelValue = min(fill, 255U);
		for(int j = 0; j < 5; ++j){
			Synthia.TrackMatrix.drawPixel(i, j, {255, 255, 255, pixelValue});
		}
		if(fill >= 255){
			fill -= 255;
		}else if(fill > 0){
			fill = 0;
		}
	}
	Synthia.TrackMatrix.push();

	LEDStrip.setMidFill(value);
}
