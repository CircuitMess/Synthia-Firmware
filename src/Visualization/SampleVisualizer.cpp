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
	"/GIF/Samples/RecWait.gif",
	"/GIF/Samples/RecIn.gif",
	"/GIF/Samples/RecOut.gif"
};

SampleVisualizer::SampleVisualizer(){
	for(auto animPath : Anims){
		File f = SPIFFS.open(animPath);
		if(!f){
			ESP_LOGE("SampleVis", "Can't open %s", animPath);
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

	auto data = getProp();
	uint8_t sample = (uint8_t) data.type;
	if(data.type == Sample::Type::RECORDING){
		if(data.recState == SampleVisData::Recording){
			sample += 1;
		}else if(data.recState == SampleVisData::Recorded){
			sample += 2;
		}
	}

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
