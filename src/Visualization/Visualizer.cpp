#include <Loop/LoopManager.h>
#include "Visualizer.h"

VisualizerManager VMan;

void VisualizerBase::start(){
	if(started) return;
	started = true;
	onStart();
}

void VisualizerBase::stop(){
	if(!started) return;
	started = false;
	onStop();
}

void VisualizerBase::setMain(){
	VMan.setMain(this);
}

void VisualizerBase::push(){
	VMan.pushing(this);
	visualize();
}

void VisualizerBase::onStart(){

}

void VisualizerBase::onStop(){

}

/** ### Manager ### */
void VisualizerManager::begin(){
	LoopManager::addListener(this);
}

void VisualizerManager::pushing(VisualizerBase* vis){
	if(vis != this->current){
		this->current->stop();
		printf("Pushing new\n");
	}

	this->current = vis;
	vis->start();

	pushTime = millis();
}

void VisualizerManager::loop(uint32_t time){
	if(pushTime == 0) return;
	if(main == nullptr || current == main) return;
	if(pushTime - millis() >= 2000){
		main->push();
		printf("restaring main\n");
	}
}

void VisualizerManager::setMain(VisualizerBase* main){
	this->main = main;
}