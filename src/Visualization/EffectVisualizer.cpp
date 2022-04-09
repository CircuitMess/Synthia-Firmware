#include "EffectVisualizer.h"
#include "LEDStrip.h"
#include <Synthia.h>

void EffectVisualizer::visualize(){
	auto data = getProp();

	//TODO - lookup unordered map of animation enums and MatrixAnim pointers

	if(data.type == EffectData::Type::VOLUME){
		LEDStrip.setMidFillFromCenter((int8_t)((int)(data.intensity) - 128));
		LEDStrip.setMidFillFromCenter((int8_t)((int)(data.intensity) - 128));
	}else{
		LEDStrip.setMidFill(data.intensity);
		LEDStrip.setRight(data.intensity);
	}
}

void EffectVisualizer::onStart(){
	//TODO - create all animations on heap
}

void EffectVisualizer::onStop(){
	Synthia.TrackMatrix.stopAnimations();
}
