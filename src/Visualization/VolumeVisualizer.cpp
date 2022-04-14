#include "VolumeVisualizer.h"
#include <Synthia.h>
#include <SPIFFS.h>
#include "LEDStrip.h"

VolumeVisualizer::VolumeVisualizer() : anim(SPIFFS.open("/GIF/Volume.gif")){
	anim.setMatrix(&Synthia.TrackMatrix);
}

void VolumeVisualizer::visualize(){
	uint8_t value = getProp();

	//TODO - start volume anim on TrackMatrix
	LEDStrip.setRight(value);
	LEDStrip.setMidFill(value);
}

void VolumeVisualizer::onStart(){
	anim.start();
}

void VolumeVisualizer::onStop(){
	anim.stop();
}
