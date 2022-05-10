#include <SPIFFS.h>
#include <FS/RamFile.h>
#include <Synthia.h>
#include "SampleSpeedVisualizer.h"
#include "LEDStrip.h"

SampleSpeedVisualizer::SampleSpeedVisualizer(){
	File f = SPIFFS.open("/GIF/Pitch.gif");
	if(!f){
		ESP_LOGE("SampleVis", "Can't open /GIF/Pitch.gif");
		return;
	}
	f = RamFile::open(f);

	anim.reset(new MatrixAnimGIF(f));
	anim->setMatrix(&Synthia.TrackMatrix);
}

void SampleSpeedVisualizer::visualize(){
	auto val = getProp();
	LEDStrip.setLeftFromCenter((int8_t)((int)(val) - 127));
	LEDStrip.setMidFillFromCenter((int8_t)((int)(val) - 127));
}

void SampleSpeedVisualizer::onStart(){
	anim->start();
}

void SampleSpeedVisualizer::onStop(){
	anim->stop();
	LEDStrip.setMidFill(0);
}
