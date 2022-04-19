#include "TempoVisualizer.h"
#include <Synthia.h>
#include "TempoAnim.h"
#include "LEDStrip.h"

char text[4];

TempoVisualizer::TempoVisualizer() : anim(&Synthia.CursorMatrix), metronome(&Synthia.TrackMatrix){}

void TempoVisualizer::visualize(){
	uint8_t value = getProp();
	Matrix& track = Synthia.TrackMatrix;
	Matrix& cursor = Synthia.CursorMatrix;

	if(!anim.isStarted()){
		anim.start();
	}
	if(anim.getTempo() != value){
		anim.setTempo(value);
	}

	if(!metronome.isStarted()){
		metronome.start();
	}
	if(metronome.getTempo() != value){
		metronome.setTempo(value);
	}

	for(int i = 5; i < track.getWidth(); ++i){
		for(int j = 0; j < track.getHeight(); ++j){
			track.drawPixel(i, j, MatrixPixel::Off);
		}
	}

	track.setFont(Matrix::SMALL);
	sprintf(text, "%3d", value);
	track.drawString(5, 5, text);
	track.push();

	LEDStrip.setLeft(map(value, 60, 220, 0, 255));
}

void TempoVisualizer::onStop(){
	anim.stop();
	metronome.stop();
}
