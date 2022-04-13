#include "TempoVisualizer.h"
#include <Synthia.h>
#include "TempoAnim.h"
#include "LEDStrip.h"

char text[4];

TempoVisualizer::TempoVisualizer() : anim(&Synthia.CursorMatrix){}

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

	//TODO - add MetronomeAnim on TrackMatrix
	track.clear();
	track.setFont(Matrix::SMALL);
	sprintf(text, "%3d", value);
	track.drawString(5, 5, text);
	track.push();

	LEDStrip.setLeft(map(value, 60, 220, 0, 255));
}
