#include "SampleSpeedVisualizer.h"
#include "LEDStrip.h"

void SampleSpeedVisualizer::visualize(){
	//TODO - start speed anim on TrackMatrix
	auto val = getProp();
	LEDStrip.setLeftFromCenter((int8_t)((int)(val) - 128));
}

void SampleSpeedVisualizer::onStop(){
	//TODO - stop anim
}
