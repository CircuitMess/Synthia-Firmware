#include "SampleVisualizer.h"
#include <Synthia.h>
#include <SPIFFS.h>

const char* SampleVisualizer::Anims[] = {
	"/GIF/Samples/Kick.gif",
	"/GIF/Samples/Snare.gif",
	"/GIF/Samples/Clap.gif",
	"/GIF/Samples/HatClos.gif",
	"/GIF/Samples/HatOpen.gif",
	"/GIF/Samples/RecOff.gif"
};

SampleVisualizer::SampleVisualizer(){
	for(int i = 0; i < 6; i++){
		File f = SPIFFS.open(Anims[i]);
		if(!f){
			ESP_LOGE("SampleVis", "Can't open %s", Anims[i]);
			anims.clear();
			return;
		}

		anims.emplace_back(new MatrixAnimGIF(f));
		anims.back()->setMatrix(&Synthia.TrackMatrix);
	}
}

void SampleVisualizer::visualize(){
	if(anims.empty()) return;

	auto &matrix = Synthia.TrackMatrix;
	matrix.stopAnimations();

	uint8_t sample = (uint8_t) getProp();
	anims[sample]->reset();
	anims[sample]->start();
}

void SampleVisualizer::onStart(){

}

void SampleVisualizer::onStop(){
	Synthia.TrackMatrix.stopAnimations();
}
