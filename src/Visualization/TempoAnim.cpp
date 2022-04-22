#include <Loop/LoopManager.h>
#include "TempoAnim.h"

TempoAnim::TempoAnim(Matrix* matrix) : MatrixAnim(matrix){
	setWidth(16);
	steps = getWidth()*2 - 2;
	setTempo(120);
}

void TempoAnim::loop(uint micros){
	if(!isStarted()){
		LoopManager::removeListener(this);
		return;
	}

	uint32_t currentStep = (int) floor((float) (millis() - startTime) / stepTime) % steps;
	if(currentStep != step){
		step = currentStep;
		push();
	}
}

void TempoAnim::reset(){
	step = 0;
}

void TempoAnim::onStart(){
	LoopManager::addListener(this);
	startTime = millis();
	push();
}

void TempoAnim::onStop(){
	LoopManager::removeListener(this);
}

void TempoAnim::setTempo(uint8_t tempo){
	tempo = max((uint8_t) 1, tempo);

	auto oldTempo = this->tempo;
	auto oldStepTime = this->stepTime;

	this->tempo = tempo;
	float beatTime = 60000.0f / (float) tempo;
	stepTime = beatTime / (float) steps;

	// initial state
	if(oldTempo == 0 || oldStepTime == 0){
		startTime = millis();
		return;
	}

	uint32_t currentStepTime = round((float) step * oldStepTime);
	uint32_t currentLoopTime = (millis() - startTime) % (int) round((float) steps * oldStepTime);
	uint32_t diff = currentLoopTime - currentStepTime;

	startTime = round((float) millis() - (float) step * stepTime - (float) diff);
}

void TempoAnim::push(){
	clear();
	drawPixel(step < getWidth() ? step : steps - step, { 255, 255, 255, 255 });
	pushMatrix();
}

uint8_t TempoAnim::getTempo() const{
	return tempo;
}