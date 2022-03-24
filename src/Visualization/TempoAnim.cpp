#include <Loop/LoopManager.h>
#include "TempoAnim.h"

TempoAnim::TempoAnim(Matrix* matrix) : MatrixAnim(matrix), width(16){

}

void TempoAnim::loop(uint micros){
	if(!isStarted()){
		LoopManager::removeListener(this);
		return;
	}

	uint32_t currentStep = (int) floor((float) (millis() - startTime) / stepTime) % (width * 2);
	if(step != currentStep){
		step = currentStep;
		pushPixel();
	}
}

void TempoAnim::reset(){
	step = 0;
}

void TempoAnim::onStart(){
	LoopManager::addListener(this);
	pushPixel();
	startTime = millis();
}

void TempoAnim::onStop(){
	LoopManager::removeListener(this);
}

void TempoAnim::setTempo(uint8_t tempo){
	this->tempo = tempo;

	float beatTime = 60000.0f / (float) tempo;
	stepTime = beatTime / (float) (width * 2);

	if(isStarted()){
		startTime = millis() - (float) step * stepTime;
	}
}

void TempoAnim::pushPixel(){
	Matrix* matrix = getMatrix();
	matrix->clear();
	matrix->drawPixel(step < width ? step : (width*2) - step - 1, { 255, 255, 255, 255 });
	matrix->push();
}

uint8_t TempoAnim::getTempo() const{
	return tempo;
}
