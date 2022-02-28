#ifndef SYNTHIA_FIRMWARE_VISUALIZER_HPP
#define SYNTHIA_FIRMWARE_VISUALIZER_HPP

#include <Arduino.h>
#include <CircuitOS.h>
#include <Loop/LoopManager.h>

template<typename T>
class Visualizer {
public:
	bool isStarted() const{
		return started;
	}
	void visualize(T){
		start();
	}
	void revisualize(){
		visualize(currentProp);
	}
	void setMain(){
		mainVis = this;
	}
	void loop(){
		if((!started) || (mainVis == nullptr) || (mainVis == this)){
			LoopManager::removeListener(this);
			return;
		}
		if(millis() - updateTime > timeout){
			revisualize();
		}
	}

protected:
	virtual void onStart(){};
	virtual void onStop(){};
	virtual void propChanged(T) = 0;

private:
	static Visualizer<T>* current;
	static T currentProp;
	bool started;
	static Visualizer<T>* currentVis;
	static Visualizer<T>* mainVis;

	static uint32_t updateTime;
	const uint32_t timeout = 2000;
	void start(){
		if(isStarted()){
			return;
		}
		current->stop();
		current = this;
		onStart();
		if(current != mainVis){
			updateTime = millis();
			LoopManager::addListener(this);
		}
	}

	void stop(){
		if(!isStarted()){
			return;
		}
		current = nullptr;
		onStop();
	}

};

#endif
