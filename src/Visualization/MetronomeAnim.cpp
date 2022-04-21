#include "MetronomeAnim.h"
#include <Loop/LoopManager.h>

MetronomeAnim::MetronomeAnim(Matrix* matrix) : MatrixAnim(matrix){
	setWidth(5);
	setHeight(5);
	steps = getWidth()*2 - 2;
	startPoint.first = getWidth()/2;
	startPoint.second = getY() + getHeight() - 1;

	Serial.printf("startPoint x: %d, y: %d\n", startPoint.first, startPoint.second);
}

void MetronomeAnim::loop(uint micros){
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

void MetronomeAnim::onStart(){
	LoopManager::addListener(this);
	startTime = millis();
	push();
}

void MetronomeAnim::onStop(){
	LoopManager::removeListener(this);
}

void MetronomeAnim::setTempo(uint8_t tempo){
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

void MetronomeAnim::push(){
	clear();

	int x0 = startPoint.first;
	int y0 = startPoint.second;
	int x1 = step < getWidth() ? step : steps - step;
	int y1 = y0 - sqrt(pow(getHeight(), 2) - pow(x1 - x0, 2));

	int dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2; /* error value e_xy */

	for (;;){  /* loop */
		drawPixel(x0,y0, MatrixPixel::White);
		if (x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
		if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
	}

	pushMatrix();
}

void MetronomeAnim::reset(){
	step = 0;
}

uint8_t MetronomeAnim::getTempo() const{
	return tempo;
}

