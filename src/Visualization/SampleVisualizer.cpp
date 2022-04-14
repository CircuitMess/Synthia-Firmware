#include "SampleVisualizer.h"
#include "LEDStrip.h"
#include <Synthia.h>
#include <SPIFFS.h>
#include <FS/RamFile.h>

const char* SampleVisualizer::Anims[] = {
	"/GIF/Samples/Kick.gif",
	"/GIF/Samples/Snare.gif",
	"/GIF/Samples/Clap.gif",
	"/GIF/Samples/HatClos.gif",
	"/GIF/Samples/HatOpen.gif",
	"/GIF/Samples/RecOff.gif"
};

SampleVisualizer::SampleVisualizer(){
	for(int i = 0; i < (size_t) Sample::Type::SIZE; i++){
		File f = SPIFFS.open(Anims[i]);
		if(!f){
			ESP_LOGE("SampleVis", "Can't open %s", Anims[i]);
			anims.clear();
			return;
		}
		f = RamFile::open(f);

		anims.emplace_back(new MatrixAnimGIF(f));
		anims.back()->setMatrix(&Synthia.TrackMatrix);
	}
}

void SampleVisualizer::visualize(){
	if(anims.empty()) return;

	uint8_t sample = (uint8_t) getProp();
	for(int i = 0; i < anims.size(); i++){
		if(anims[i]->isStarted()){
			if(i == sample) return;
			anims[i]->stop();
			break;
		}
	}

	anims[sample]->reset();
	anims[sample]->start();
}

void SampleVisualizer::onStart(){

}

void SampleVisualizer::onStop(){
	Synthia.TrackMatrix.stopAnimations();
}
