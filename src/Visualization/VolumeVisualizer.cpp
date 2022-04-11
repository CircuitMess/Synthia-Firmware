#include "VolumeVisualizer.h"
#include <Synthia.h>
#include "LEDStrip.h"

void VolumeVisualizer::visualize(){
	uint8_t value = getProp();

	//TODO - start volume anim on TrackMatrix
	LEDStrip.setRight(value);
	LEDStrip.setMidFill(value);
}
