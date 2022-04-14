#include "EffectVisualizer.h"
#include "LEDStrip.h"
#include <Synthia.h>
#include <SPIFFS.h>
#include <FS/RamFile.h>

const char* EffectVisualizer::Anims[] = {
		"/GIF/Effects/LowPass.gif",
		"/GIF/Effects/HighPass.gif",
		"/GIF/Effects/Reverb.gif",
		"/GIF/Effects/BitCrush.gif",
		"/GIF/Volume.gif",
};

EffectVisualizer::EffectVisualizer(){
	for(int i = 0; i < (size_t) EffectData::Type::COUNT; i++){
		File f = SPIFFS.open(Anims[i]);
		if(!f){
			ESP_LOGE("EffectVis", "Can't open %s", Anims[i]);
			anims.clear();
			return;
		}
		f = RamFile::open(f);

		anims.emplace_back(new MatrixAnimGIF(f));
		anims.back()->setMatrix(&Synthia.TrackMatrix);
	}
}

void EffectVisualizer::visualize(){
	uint8_t effect = (uint8_t) getProp().type;

	LEDStrip.setRight(getProp().intensity);
	LEDStrip.setMidFill(getProp().intensity);

	if(anims.empty()) return;

	for(int i = 0; i < anims.size(); i++){
		if(anims[i]->isStarted()){
			if(i == effect) return;
			anims[i]->stop();
			break;
		}
	}

	anims[effect]->reset();
	anims[effect]->start();
}

void EffectVisualizer::onStart(){

}

void EffectVisualizer::onStop(){
	Synthia.TrackMatrix.stopAnimations();
	LEDStrip.setMidFill(0);
}
