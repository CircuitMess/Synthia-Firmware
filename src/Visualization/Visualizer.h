#ifndef SYNTHIA_FIRMWARE_VISUALIZER_H
#define SYNTHIA_FIRMWARE_VISUALIZER_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Loop/LoopListener.h>

class VisualizerBase;

class VisualizerManager : public LoopListener {
friend VisualizerBase;
public:
	void begin();
	void loop(uint32_t time) override;
	void clearMain();

private:
	const uint32_t mainRestartTimeout = 3500; // [ms]

	VisualizerBase* main = nullptr;
	VisualizerBase* current = nullptr;

	void setMain(VisualizerBase* main);
	void pushing(VisualizerBase* vis);

	uint32_t pushTime = 0;

};

extern VisualizerManager VMan;

class VisualizerBase {
friend VisualizerManager;
public:
	virtual ~VisualizerBase(){
		free(prop);
	}

	void push();

	void setMain();

	bool isStarted();

protected:
	template<typename T>
	void push(const T& value){
		if(propSize == 0){
			propSize = sizeof(T);
			prop = malloc(propSize);
		}else if(propSize != sizeof(T)){
			return;
		}

		memcpy(prop, &value, propSize);

		push();
	}

	template<typename T>
	const T& getProp(){
		return *static_cast<T*>(prop);
	}

	virtual void visualize() = 0;

	virtual void onStart();
	virtual void onStop();

private:
	void* prop = nullptr;
	size_t propSize = 0;

	bool started = false;

	void start();
	void stop();
};

template<typename T>
class Visualizer : public VisualizerBase {
public:
	void push(const T& value){
		VisualizerBase::push(value);
	}

protected:
	const T& getProp(){
		return VisualizerBase::getProp<T>();
	}

};

#endif
